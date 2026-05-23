#ifndef PONGOBOOT_H
#define PONGOBOOT_H

#include <stdint.h>
#include "targets.h"
#include "options.h"
#include "ui.h"
#include "input.h"
#include "render.h"
#include "discovery.h"

#define PONGOBOOT_VERSION_MAJOR 2
#define PONGOBOOT_VERSION_MINOR 1
#define PONGOBOOT_VERSION_PATCH 0

/* Main PongoBoot Context */
typedef struct {
    boot_registry_t registry;
    boot_options_t default_options;
    boot_options_t selected_options;
    ui_context_t ui_ctx;
    
    uint32_t boot_ticks;            /* Milliseconds elapsed */
    int should_exit;
} pongoboot_context_t;

/* Main entry point and loop */
int pongoboot_main(void);
void pongoboot_init(pongoboot_context_t* ctx);
void pongoboot_run(pongoboot_context_t* ctx);
void pongoboot_cleanup(pongoboot_context_t* ctx);

#endif /* PONGOBOOT_H */
