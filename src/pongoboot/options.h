#ifndef PONGOBOOT_OPTIONS_H
#define PONGOBOOT_OPTIONS_H

#include <stdint.h>

/* Boot Options Configuration */
typedef struct {
    char kernel_args[512];          /* Kernel command-line arguments */
    uint32_t verbose_boot;          /* -v flag */
    uint32_t safe_mode;             /* Safe mode flags */
    uint32_t debug_flags;           /* Debug level (0x14e, etc.) */
    uint32_t timeout_seconds;       /* Auto-boot delay */
    uint32_t boot_graphics;         /* Graphics mode (text/framebuffer) */
} boot_options_t;

/* Preset Boot Options */
typedef struct {
    const char* name;
    const char* description;
    const char* kernel_args;
    uint32_t flags;
} boot_preset_t;

/* Boot Options Functions */
void pongoboot_options_init(boot_options_t* opts);
void pongoboot_options_set_verbose(boot_options_t* opts, int enabled);
void pongoboot_options_set_safe_mode(boot_options_t* opts, int enabled);
void pongoboot_options_set_debug(boot_options_t* opts, uint32_t debug_level);
int pongoboot_options_parse_args(boot_options_t* opts, const char* arg_string);
void pongoboot_options_dump(boot_options_t* opts);

#endif /* PONGOBOOT_OPTIONS_H */
