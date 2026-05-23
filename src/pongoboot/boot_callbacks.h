#ifndef PONGOBOOT_CALLBACKS_H
#define PONGOBOOT_CALLBACKS_H

#include "options.h"

/* Boot callback implementations */
int boot_pongoOS(boot_options_t* opts);
int boot_xnu(boot_options_t* opts);
int boot_recovery(boot_options_t* opts);
int boot_system_info(boot_options_t* opts);
int boot_linux(boot_options_t* opts);

#endif /* PONGOBOOT_CALLBACKS_H */
