#include "pongoboot.h"
#include <pongo.h>
#include <stdio.h>
#include <string.h>

static pongoboot_context_t g_ctx = {0};

void pongoboot_init(pongoboot_context_t* ctx)
{
    if (!ctx) return;
    
    printf("[PongoBoot] v%u.%u.%u - Multi-boot Bootloader Initializing\n",
           PONGOBOOT_VERSION_MAJOR, PONGOBOOT_VERSION_MINOR, PONGOBOOT_VERSION_PATCH);
    
    memset(ctx, 0, sizeof(pongoboot_context_t));
    
    /* Initialize boot target discovery */
    printf("[PongoBoot] Discovering boot targets...\n");
    pongoboot_discover_targets(&ctx->registry);
    pongoboot_targets_dump(&ctx->registry);
    
    /* Initialize default boot options */
    pongoboot_options_init(&ctx->default_options);
    memcpy(&ctx->selected_options, &ctx->default_options, sizeof(boot_options_t));
    pongoboot_options_dump(&ctx->default_options);
    
    /* Initialize UI */
    pongoboot_ui_init(&ctx->ui_ctx, &ctx->registry);
    
    /* Initialize input system */
    pongoboot_input_init();
    
    ctx->boot_ticks = 0;
    ctx->should_exit = 0;
    
    printf("[PongoBoot] Initialization complete\n");
}

void pongoboot_run(pongoboot_context_t* ctx)
{
    if (!ctx) return;
    
    printf("[PongoBoot] Starting main loop\n");
    
    uint32_t frame_count = 0;
    const uint32_t FRAME_TIME_MS = 50;  /* 20 FPS */
    
    while (!ctx->should_exit) {
        /* Handle input */
        input_key_t key = pongoboot_input_get_nonblocking();
        if (key != INPUT_NONE) {
            printf("[PongoBoot] Input: 0x%02x\n", key);
            pongoboot_ui_handle_input(&ctx->ui_ctx, &ctx->registry, key);
            if (ctx->ui_ctx.requested_exit) {
                ctx->should_exit = 1;
            }
        }
        
        /* Update UI state */
        pongoboot_ui_update(&ctx->ui_ctx, &ctx->registry, FRAME_TIME_MS);
        
        /* Render current screen */
        pongoboot_ui_render(&ctx->ui_ctx, &ctx->registry);
        
        /* Handle state transitions */
        switch (ctx->ui_ctx.current_state) {
            case UI_STATE_BOOTING: {
                /* Execute boot callback for selected target */
                boot_target_t* target = &ctx->registry.targets[ctx->ui_ctx.selection_index];
                if (target->boot_fn && target->is_available) {
                    printf("[PongoBoot] Executing boot callback for: %s\n", target->name);
                    memcpy(&ctx->selected_options, &ctx->ui_ctx.pending_options, sizeof(boot_options_t));
                    int result = target->boot_fn(&ctx->selected_options);
                    
                    if (result != 0) {
                        /* Boot failed */
                        ctx->ui_ctx.error_code = result;
                        ctx->ui_ctx.error_message = "Boot target failed";
                        pongoboot_ui_state_transition(&ctx->ui_ctx, UI_STATE_ERROR);
                    } else {
                        /* Boot function completed cleanly; exit to allow shell or boot flow */
                        ctx->should_exit = 1;
                    }
                } else {
                    ctx->ui_ctx.error_code = 0xffffffff;
                    ctx->ui_ctx.error_message = "Boot target is not available";
                    pongoboot_ui_state_transition(&ctx->ui_ctx, UI_STATE_ERROR);
                }
                break;
            }
            
            case UI_STATE_CONFIRM:
                /* Check for auto-boot timeout */
                if (ctx->ui_ctx.timeout_remaining == 0 && ctx->ui_ctx.boot_armed) {
                    pongoboot_ui_state_transition(&ctx->ui_ctx, UI_STATE_BOOTING);
                }
                break;
                
            default:
                break;
        }
        
        ctx->boot_ticks += FRAME_TIME_MS;
        frame_count++;
        
        /* Periodically dump statistics */
        if (frame_count % 20 == 0) {
            printf("[PongoBoot] Frame %u, Ticks: %u ms\n", frame_count, ctx->boot_ticks);
        }
        
        usleep(FRAME_TIME_MS * 1000);
    }
    
    printf("[PongoBoot] Main loop exited\n");
}

void pongoboot_cleanup(pongoboot_context_t* ctx)
{
    if (!ctx) return;
    
    printf("[PongoBoot] Cleaning up\n");
    pongoboot_input_flush();
    
    memset(ctx, 0, sizeof(pongoboot_context_t));
}

int pongoboot_main(void)
{
    printf("[PongoBoot] Main entry point\n");
    
    pongoboot_init(&g_ctx);
    pongoboot_run(&g_ctx);
    pongoboot_cleanup(&g_ctx);
    
    printf("[PongoBoot] Exiting\n");
    return 0;
}
