#ifndef PONGOBOOT_UI_H
#define PONGOBOOT_UI_H

#include <stdint.h>
#include "targets.h"
#include "options.h"

/* UI State Enum */
typedef enum {
    UI_STATE_MENU           = 0,    /* Boot target menu */
    UI_STATE_OPTIONS        = 1,    /* Boot options editor */
    UI_STATE_CONFIRM        = 2,    /* Confirmation screen */
    UI_STATE_BOOTING        = 3,    /* Boot sequence in progress */
    UI_STATE_ERROR          = 4,    /* Error display */
    UI_STATE_INFO           = 5,    /* System information */
} ui_state_t;

/* UI Input Modes */
typedef enum {
    INPUT_MODE_NAVIGATION   = 0,    /* Navigating menu */
    INPUT_MODE_EDIT_TEXT    = 1,    /* Editing text field */
    INPUT_MODE_CONFIRM      = 2,    /* Confirmation prompt */
} ui_input_mode_t;

/* UI Context - State Machine */
typedef struct {
    ui_state_t current_state;
    ui_state_t previous_state;
    ui_input_mode_t input_mode;
    
    /* Menu state */
    uint32_t selection_index;
    uint32_t scroll_offset;
    
    /* Text editing state */
    uint32_t input_field_cursor;
    char input_buffer[512];
    
    /* Boot options being edited */
    boot_options_t pending_options;
    
    /* Timeout tracking */
    uint32_t timeout_remaining;
    uint32_t timeout_ms_remaining;
    uint32_t boot_armed;            /* Ready to boot? */
    uint32_t requested_exit;
    
    /* Error tracking */
    uint32_t error_code;
    const char* error_message;
} ui_context_t;

/* UI Functions */
void pongoboot_ui_init(ui_context_t* ctx, boot_registry_t* registry);
void pongoboot_ui_state_transition(ui_context_t* ctx, ui_state_t new_state);
void pongoboot_ui_handle_input(ui_context_t* ctx, boot_registry_t* registry, int input_key);
void pongoboot_ui_update(ui_context_t* ctx, boot_registry_t* registry, uint32_t delta_ms);
void pongoboot_ui_render(ui_context_t* ctx, boot_registry_t* registry);

#endif /* PONGOBOOT_UI_H */
