#ifndef PONGOBOOT_TARGETS_H
#define PONGOBOOT_TARGETS_H

#include <stdint.h>
#include "options.h"

/* Boot Target Types */
typedef enum {
    BOOT_PONGOBOOT      = 0x01,     /* Continue to pongoOS kernel */
    BOOT_XNU            = 0x02,     /* XNU kernel (iOS/macOS) */
    BOOT_RECOVERY       = 0x03,     /* Recovery/DFU mode */
    BOOT_UTILITIES      = 0x04,     /* Utility tools within pongoOS */
    BOOT_SECTION_DIVIDER = 0x05,    /* UI section divider */
    BOOT_LINUX          = 0x06,     /* Linux kernel (if available) */
} boot_target_type_t;

/* Boot callback function type */
typedef int (*boot_callback_t)(boot_options_t* opts);

/* Boot Target Registry Entry */
typedef struct {
    const char* name;               /* e.g., "Boot pongoOS", "Boot iOS" */
    const char* version;            /* Version string (e.g., "16.2", NULL if N/A) */
    const char* description;        /* User-friendly description */
    uint32_t id;                    /* Unique identifier */
    boot_target_type_t type;        /* Target type enum */
    boot_callback_t boot_fn;        /* Function to execute boot */
    int is_available;               /* Can this target be booted? */
    int is_default;                 /* Auto-select if timeout expires */
    uint32_t display_order;         /* Menu ordering (lower = higher priority) */
} boot_target_t;

/* Boot Target Registry */
typedef struct {
    boot_target_t targets[10];      /* Fixed array of targets */
    uint32_t count;                 /* Number of targets registered */
    uint32_t current_selection;     /* Currently selected target index */
} boot_registry_t;

/* Target Registry Functions */
int pongoboot_targets_init(boot_registry_t* registry);
int pongoboot_targets_register(boot_registry_t* registry, boot_target_t* target);
void pongoboot_targets_dump(boot_registry_t* registry);

#endif /* PONGOBOOT_TARGETS_H */
