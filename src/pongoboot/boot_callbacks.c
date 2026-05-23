#include "boot_callbacks.h"
#include <pongo.h>
#include <stdio.h>
#include <string.h>

extern int iprintf(const char* fmt, ...);

static char* xnu_command_line(void)
{
    if (!gBootArgs) return NULL;
    return (char*)((int64_t)gBootArgs->iOS13.CommandLine - 0x800000000 + kCacheableView);
}

static void apply_xnu_args(boot_options_t* opts)
{
    char* cmdline = xnu_command_line();
    if (!cmdline || !opts) return;
    snprintf(cmdline, BOOT_LINE_LENGTH_iOS13, "%s", opts->kernel_args);
}

int boot_pongoOS(boot_options_t* opts)
{
    iprintf("[PongoBoot] Continuing to pongoOS shell...\n");
    
    if (opts) {
        iprintf("[PongoBoot] Session kernel arguments: %s\n", opts->kernel_args);
    }

    return 0;
}

int boot_xnu(boot_options_t* opts)
{
    iprintf("[PongoBoot] Booting XNU kernel...\n");
    
    if (opts) {
        iprintf("[PongoBoot] Kernel arguments: %s\n", opts->kernel_args);
        apply_xnu_args(opts);
    }

    gBootFlag = BOOT_FLAG_HARD;
    task_yield();
    return 0;
}

int boot_recovery(boot_options_t* opts)
{
    (void)opts;
    iprintf("[PongoBoot] Recovery/DFU boot is not implemented in this pongoOS build\n");
    
    return -1;
}

int boot_system_info(boot_options_t* opts)
{
    (void)opts;
    return 0;
}

int boot_linux(boot_options_t* opts)
{
    (void)opts;
    if (!linux_can_boot()) {
        iprintf("[PongoBoot] Linux boot is not prepared\n");
        return -1;
    }

    iprintf("[PongoBoot] Booting Linux...\n");
    gBootFlag = BOOT_FLAG_LINUX;
    task_yield();
    return 0;
}
