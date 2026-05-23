#ifndef PONGOBOOT_DISCOVERY_H
#define PONGOBOOT_DISCOVERY_H

#include <stdint.h>
#include "targets.h"

/* Boot Target Discovery */
int pongoboot_discover_targets(boot_registry_t* registry);

/* Device Detection */
typedef enum {
    DEVICE_TYPE_UNKNOWN = 0,
    DEVICE_TYPE_IPHONE  = 1,
    DEVICE_TYPE_IPAD    = 2,
    DEVICE_TYPE_MAC     = 3,
} device_type_t;

device_type_t pongoboot_detect_device(void);
const char* pongoboot_get_device_model(void);

/* Target Availability Checks */
int pongoboot_can_boot_ios(char* version_out, int version_len);
int pongoboot_can_boot_macos(char* version_out, int version_len);
int pongoboot_can_boot_recovery(void);

#endif /* PONGOBOOT_DISCOVERY_H */
