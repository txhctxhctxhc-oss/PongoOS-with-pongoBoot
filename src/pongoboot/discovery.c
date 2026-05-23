#include "discovery.h"
#include "boot_callbacks.h"
#include <pongo.h>
#include <string.h>
#include <stdio.h>

extern const char* hal_platform_name(void);

/* Device type detection - stub implementation */
static device_type_t g_device_type = DEVICE_TYPE_UNKNOWN;
static char g_device_model[32] = {0};

device_type_t pongoboot_detect_device(void)
{
    const char* model = gDevType ? gDevType : hal_platform_name();
    if (model && (strstr(model, "iPad") || strstr(model, "ipad"))) {
        g_device_type = DEVICE_TYPE_IPAD;
    } else if (model && (strstr(model, "Mac") || strstr(model, "mac"))) {
        g_device_type = DEVICE_TYPE_MAC;
    } else {
        g_device_type = DEVICE_TYPE_IPHONE;
    }
    return g_device_type;
}

const char* pongoboot_get_device_model(void)
{
    if (strlen(g_device_model) == 0) {
        snprintf(g_device_model, sizeof(g_device_model), "%s", gDevType ? gDevType : hal_platform_name());
    }
    return g_device_model;
}

int pongoboot_can_boot_ios(char* version_out, int version_len)
{
    if (version_out && version_len > 0) {
        snprintf(version_out, version_len, "current");
    }
    return 1;
}

int pongoboot_can_boot_macos(char* version_out, int version_len)
{
    /* macOS only available on Mac devices */
    device_type_t device = pongoboot_detect_device();
    
    if (device != DEVICE_TYPE_MAC) {
        return 0;  /* Not available on this device */
    }
    
    if (version_out && version_len > 0) {
        snprintf(version_out, version_len, "Ventura");
    }
    return 1;
}

int pongoboot_can_boot_recovery(void)
{
    return 0;
}

int pongoboot_discover_targets(boot_registry_t* registry)
{
    if (!registry) return -1;
    
    printf("[PongoBoot] Discovering boot targets...\n");
    
    /* Initialize registry */
    pongoboot_targets_init(registry);
    
    /* Detect device type */
    device_type_t device_type = pongoboot_detect_device();
    printf("[PongoBoot] Detected device type: %d\n", device_type);
    
    /* 1. PongoOS (always available) */
    boot_target_t pongoboot_target = {
        .name = "Boot pongoOS (Default)",
        .version = NULL,
        .description = "Continue to pongoOS with default kernel parameters",
        .id = 0x001,
        .type = BOOT_PONGOBOOT,
        .boot_fn = boot_pongoOS,
        .is_available = 1,
        .is_default = 1,
        .display_order = 0,
    };
    pongoboot_targets_register(registry, &pongoboot_target);
    
    /* 2. XNU (device-specific) */
    if (device_type == DEVICE_TYPE_IPHONE || device_type == DEVICE_TYPE_IPAD) {
        static char ios_version[32];
        memset(ios_version, 0, sizeof(ios_version));
        if (pongoboot_can_boot_ios(ios_version, sizeof(ios_version))) {
            boot_target_t ios_target = {
                .name = "Boot XNU",
                .version = ios_version,
                .description = "Boot the existing XNU image using pongoOS boot flow",
                .id = 0x002,
                .type = BOOT_XNU,
                .boot_fn = boot_xnu,
                .is_available = 1,
                .is_default = 0,
                .display_order = 1,
            };
            pongoboot_targets_register(registry, &ios_target);
        }
    }
    
    /* 3. macOS (Mac only) */
    if (device_type == DEVICE_TYPE_MAC) {
        static char macos_version[32];
        memset(macos_version, 0, sizeof(macos_version));
        if (pongoboot_can_boot_macos(macos_version, sizeof(macos_version))) {
            boot_target_t macos_target = {
                .name = "Boot macOS",
                .version = macos_version,
                .description = "Boot native macOS kernel",
                .id = 0x003,
                .type = BOOT_XNU,
                .boot_fn = boot_xnu,
                .is_available = 1,
                .is_default = 0,
                .display_order = 1,
            };
            pongoboot_targets_register(registry, &macos_target);
        }
    }
    
    /* 4. Linux if kernel/FDT preparation has already happened. */
    boot_target_t linux_target = {
        .name = "Boot Linux",
        .version = NULL,
        .description = linux_can_boot() ? "Boot prepared Linux kernel and FDT" : "Linux kernel/FDT not loaded",
        .id = 0x004,
        .type = BOOT_LINUX,
        .boot_fn = boot_linux,
        .is_available = linux_can_boot() ? 1 : 0,
        .is_default = 0,
        .display_order = 2,
    };
    pongoboot_targets_register(registry, &linux_target);

    /* 5. Recovery Mode (not supported in v1). */
    if (pongoboot_can_boot_recovery()) {
        boot_target_t recovery_target = {
            .name = "Recovery Mode",
            .version = NULL,
            .description = "Boot into Recovery/DFU mode",
            .id = 0x005,
            .type = BOOT_RECOVERY,
            .boot_fn = boot_recovery,
            .is_available = 1,
            .is_default = 0,
            .display_order = 3,
        };
        pongoboot_targets_register(registry, &recovery_target);
    }
    
    /* 6. Section Divider */
    boot_target_t divider_target = {
        .name = NULL,
        .version = NULL,
        .description = NULL,
        .id = 0x006,
        .type = BOOT_SECTION_DIVIDER,
        .boot_fn = NULL,
        .is_available = 0,
        .is_default = 0,
        .display_order = 4,
    };
    pongoboot_targets_register(registry, &divider_target);
    
    /* 7. System Information Utility */
    boot_target_t info_target = {
        .name = "System Information",
        .version = NULL,
        .description = "View device and boot configuration information",
        .id = 0x007,
        .type = BOOT_UTILITIES,
        .boot_fn = boot_system_info,
        .is_available = 1,
        .is_default = 0,
        .display_order = 5,
    };
    pongoboot_targets_register(registry, &info_target);
    
    printf("[PongoBoot] Discovered %u boot targets\n", registry->count);
    return registry->count;
}
