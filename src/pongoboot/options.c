#include "options.h"
#include <string.h>
#include <stdio.h>

void pongoboot_options_init(boot_options_t* opts)
{
    if (!opts) return;
    
    memset(opts, 0, sizeof(boot_options_t));
    opts->timeout_seconds = 5;
    opts->boot_graphics = 1;  /* Framebuffer mode by default */
    
    /* Default kernel arguments */
    snprintf(opts->kernel_args, sizeof(opts->kernel_args), "-v debug=0x14e");
}

void pongoboot_options_set_verbose(boot_options_t* opts, int enabled)
{
    if (!opts) return;
    
    opts->verbose_boot = enabled ? 1 : 0;
    
    /* Update kernel args */
    if (enabled && strstr(opts->kernel_args, "-v") == NULL) {
        char temp[512];
        snprintf(temp, sizeof(temp), "-v %s", opts->kernel_args);
        snprintf(opts->kernel_args, sizeof(opts->kernel_args), "%s", temp);
    }
}

void pongoboot_options_set_safe_mode(boot_options_t* opts, int enabled)
{
    if (!opts) return;
    
    opts->safe_mode = enabled ? 1 : 0;
}

void pongoboot_options_set_debug(boot_options_t* opts, uint32_t debug_level)
{
    if (!opts) return;
    
    opts->debug_flags = debug_level;
}

int pongoboot_options_parse_args(boot_options_t* opts, const char* arg_string)
{
    if (!opts || !arg_string) return -1;
    
    /* Parse kernel argument string */
    if (strlen(arg_string) >= sizeof(opts->kernel_args)) {
        return -1;  /* Argument too long */
    }
    
    snprintf(opts->kernel_args, sizeof(opts->kernel_args), "%s", arg_string);
    
    /* Parse flags from arguments */
    if (strstr(arg_string, "-v")) opts->verbose_boot = 1;
    if (strstr(arg_string, "-x")) opts->safe_mode = 1;
    if (strstr(arg_string, "-s")) opts->safe_mode = 1;
    
    return 0;
}

void pongoboot_options_dump(boot_options_t* opts)
{
    if (!opts) return;
    
    printf("[PongoBoot] Boot Options:\n");
    printf("  Kernel Args: %s\n", opts->kernel_args);
    printf("  Verbose Boot: %d\n", opts->verbose_boot);
    printf("  Safe Mode: %d\n", opts->safe_mode);
    printf("  Debug Flags: 0x%x\n", opts->debug_flags);
    printf("  Timeout: %u seconds\n", opts->timeout_seconds);
    printf("  Graphics Mode: %u\n", opts->boot_graphics);
}
