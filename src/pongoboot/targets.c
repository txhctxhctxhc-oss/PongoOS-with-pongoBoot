#include "targets.h"
#include <string.h>
#include <stdio.h>

int pongoboot_targets_init(boot_registry_t* registry)
{
    if (!registry) return -1;
    
    memset(registry, 0, sizeof(boot_registry_t));
    registry->count = 0;
    registry->current_selection = 0;
    
    return 0;
}

int pongoboot_targets_register(boot_registry_t* registry, boot_target_t* target)
{
    if (!registry || !target) return -1;
    if (registry->count >= 10) return -1;  /* Max 10 targets */
    
    memcpy(&registry->targets[registry->count], target, sizeof(boot_target_t));
    registry->count++;
    
    return 0;
}

void pongoboot_targets_dump(boot_registry_t* registry)
{
    if (!registry) return;
    
    printf("[PongoBoot] Boot Target Registry (%u targets):\n", registry->count);
    for (uint32_t i = 0; i < registry->count; i++) {
        boot_target_t* target = &registry->targets[i];
        printf("  [%u] %s", i, target->name);
        if (target->version) {
            printf(" (%s)", target->version);
        }
        printf(" - %s\n", target->description);
        printf("      Type: 0x%02x, Available: %d, Default: %d, Order: %u\n",
               target->type, target->is_available, target->is_default, target->display_order);
    }
}
