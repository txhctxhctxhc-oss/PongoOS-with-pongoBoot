#include "ui.h"
#include "input.h"
#include "render.h"
#include <stdio.h>
#include <string.h>

static int pongoboot_target_selectable(boot_registry_t* registry, uint32_t index)
{
    if (!registry || index >= registry->count) return 0;
    return registry->targets[index].type != BOOT_SECTION_DIVIDER &&
           registry->targets[index].is_available;
}

static uint32_t pongoboot_find_default(boot_registry_t* registry)
{
    if (!registry) return 0;
    for (uint32_t i = 0; i < registry->count; i++) {
        if (registry->targets[i].is_default && pongoboot_target_selectable(registry, i)) {
            return i;
        }
    }
    for (uint32_t i = 0; i < registry->count; i++) {
        if (pongoboot_target_selectable(registry, i)) {
            return i;
        }
    }
    return 0;
}

static void pongoboot_select_next(ui_context_t* ctx, boot_registry_t* registry)
{
    if (!ctx || !registry || !registry->count) return;
    uint32_t index = ctx->selection_index;
    for (uint32_t tries = 0; tries < registry->count; tries++) {
        index = (index + 1) % registry->count;
        if (pongoboot_target_selectable(registry, index)) {
            ctx->selection_index = index;
            return;
        }
    }
}

static void pongoboot_select_prev(ui_context_t* ctx, boot_registry_t* registry)
{
    if (!ctx || !registry || !registry->count) return;
    uint32_t index = ctx->selection_index;
    for (uint32_t tries = 0; tries < registry->count; tries++) {
        index = index ? index - 1 : registry->count - 1;
        if (pongoboot_target_selectable(registry, index)) {
            ctx->selection_index = index;
            return;
        }
    }
}

void pongoboot_ui_init(ui_context_t* ctx, boot_registry_t* registry)
{
    if (!ctx || !registry) return;
    
    memset(ctx, 0, sizeof(ui_context_t));
    
    ctx->current_state = UI_STATE_MENU;
    ctx->previous_state = UI_STATE_MENU;
    ctx->input_mode = INPUT_MODE_NAVIGATION;
    
    ctx->selection_index = pongoboot_find_default(registry);
    ctx->scroll_offset = 0;
    ctx->input_field_cursor = 0;
    
    ctx->timeout_remaining = 5;  /* 5 second default timeout */
    ctx->timeout_ms_remaining = ctx->timeout_remaining * 1000;
    ctx->boot_armed = 0;
    
    ctx->error_code = 0;
    ctx->error_message = NULL;
    pongoboot_options_init(&ctx->pending_options);
    
    printf("[PongoBoot] UI initialized\n");
}

void pongoboot_ui_state_transition(ui_context_t* ctx, ui_state_t new_state)
{
    if (!ctx) return;
    
    printf("[PongoBoot] State transition: %d -> %d\n", ctx->current_state, new_state);
    
    ctx->previous_state = ctx->current_state;
    ctx->current_state = new_state;
    
    /* Reset state-specific data */
    switch (new_state) {
        case UI_STATE_MENU:
            ctx->input_mode = INPUT_MODE_NAVIGATION;
            ctx->timeout_remaining = 5;
            ctx->timeout_ms_remaining = ctx->timeout_remaining * 1000;
            ctx->boot_armed = 0;
            break;
            
        case UI_STATE_OPTIONS:
            ctx->input_mode = INPUT_MODE_EDIT_TEXT;
            ctx->input_field_cursor = 0;
            break;
            
        case UI_STATE_CONFIRM:
            ctx->input_mode = INPUT_MODE_CONFIRM;
            ctx->boot_armed = 1;
            ctx->timeout_remaining = 5;
            ctx->timeout_ms_remaining = ctx->timeout_remaining * 1000;
            break;
            
        case UI_STATE_BOOTING:
            ctx->input_mode = INPUT_MODE_NAVIGATION;
            break;
            
        case UI_STATE_ERROR:
            ctx->input_mode = INPUT_MODE_NAVIGATION;
            ctx->timeout_remaining = 10;
            ctx->timeout_ms_remaining = ctx->timeout_remaining * 1000;
            break;
            
        case UI_STATE_INFO:
            ctx->input_mode = INPUT_MODE_NAVIGATION;
            break;
    }
}

void pongoboot_ui_handle_input(ui_context_t* ctx, boot_registry_t* registry, int input_key)
{
    if (!ctx) return;
    
    switch (ctx->current_state) {
        case UI_STATE_MENU:
            switch (input_key) {
                case INPUT_VOLUP:
                    pongoboot_select_prev(ctx, registry);
                    ctx->timeout_remaining = 5;  /* Reset timeout on input */
                    ctx->timeout_ms_remaining = ctx->timeout_remaining * 1000;
                    break;
                case INPUT_VOLDOWN:
                    pongoboot_select_next(ctx, registry);
                    ctx->timeout_remaining = 5;  /* Reset timeout on input */
                    ctx->timeout_ms_remaining = ctx->timeout_remaining * 1000;
                    break;
                case INPUT_HOME:
                    /* Enter options or confirm boot */
                    if (pongoboot_target_selectable(registry, ctx->selection_index)) {
                        if (registry->targets[ctx->selection_index].type == BOOT_UTILITIES) {
                            pongoboot_ui_state_transition(ctx, UI_STATE_INFO);
                        } else {
                            pongoboot_ui_state_transition(ctx, UI_STATE_CONFIRM);
                        }
                    }
                    break;
                case INPUT_SIDE:
                    /* Escape from the menu means continue to the shell. */
                    ctx->requested_exit = 1;
                    break;
            }
            break;
            
        case UI_STATE_OPTIONS:
            switch (input_key) {
                case INPUT_VOLUP:
                    if (ctx->input_field_cursor > 0) ctx->input_field_cursor--;
                    break;
                case INPUT_VOLDOWN:
                    if (ctx->input_field_cursor < strlen(ctx->input_buffer)) {
                        ctx->input_field_cursor++;
                    }
                    break;
                case INPUT_HOME:
                    /* Accept edit */
                    pongoboot_ui_state_transition(ctx, UI_STATE_MENU);
                    break;
                case INPUT_SIDE:
                    /* Cancel edit */
                    pongoboot_ui_state_transition(ctx, UI_STATE_MENU);
                    break;
            }
            break;
            
        case UI_STATE_CONFIRM:
            switch (input_key) {
                case INPUT_HOME:
                    /* Confirm boot */
                    pongoboot_ui_state_transition(ctx, UI_STATE_BOOTING);
                    break;
                case INPUT_SIDE:
                    /* Cancel boot */
                    pongoboot_ui_state_transition(ctx, UI_STATE_MENU);
                    break;
                case INPUT_VOLUP:
                case INPUT_VOLDOWN:
                    /* Reset timeout on input */
                    ctx->timeout_remaining = 5;
                    ctx->timeout_ms_remaining = ctx->timeout_remaining * 1000;
                    break;
            }
            break;
            
        case UI_STATE_ERROR:
            switch (input_key) {
                case INPUT_SIDE:
                case INPUT_HOME:
                case INPUT_POWER:
                    /* Return to menu on any input */
                    pongoboot_ui_state_transition(ctx, UI_STATE_MENU);
                    break;
            }
            break;
            
        case UI_STATE_INFO:
            switch (input_key) {
                case INPUT_SIDE:
                case INPUT_HOME:
                    /* Return to menu */
                    pongoboot_ui_state_transition(ctx, UI_STATE_MENU);
                    break;
            }
            break;
            
        default:
            break;
    }
}

void pongoboot_ui_update(ui_context_t* ctx, boot_registry_t* registry, uint32_t delta_ms)
{
    if (!ctx) return;
    
    /* Update timeout */
    if (ctx->timeout_ms_remaining > 0) {
        if (delta_ms >= ctx->timeout_ms_remaining) {
            ctx->timeout_ms_remaining = 0;
            ctx->timeout_remaining = 0;
            
            /* Auto-boot on timeout */
            if (ctx->current_state == UI_STATE_MENU || ctx->current_state == UI_STATE_CONFIRM) {
                printf("[PongoBoot] Timeout - auto-booting default target\n");
                ctx->selection_index = pongoboot_find_default(registry);
                pongoboot_ui_state_transition(ctx, UI_STATE_BOOTING);
            } else if (ctx->current_state == UI_STATE_ERROR) {
                pongoboot_ui_state_transition(ctx, UI_STATE_MENU);
            }
        } else {
            ctx->timeout_ms_remaining -= delta_ms;
            ctx->timeout_remaining = (ctx->timeout_ms_remaining + 999) / 1000;
        }
    }
}

void pongoboot_ui_render(ui_context_t* ctx, boot_registry_t* registry)
{
    if (!ctx || !registry) return;
    
    switch (ctx->current_state) {
        case UI_STATE_MENU:
            pongoboot_render_menu(ctx, registry);
            break;
        case UI_STATE_OPTIONS:
            pongoboot_render_options(ctx, registry);
            break;
        case UI_STATE_CONFIRM:
            pongoboot_render_confirm(ctx, registry);
            break;
        case UI_STATE_BOOTING:
            pongoboot_render_booting(ctx);
            break;
        case UI_STATE_ERROR:
            pongoboot_render_error(ctx);
            break;
        case UI_STATE_INFO:
            pongoboot_render_info();
            break;
        default:
            break;
    }
}
