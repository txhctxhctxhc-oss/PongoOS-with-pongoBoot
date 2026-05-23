#ifndef PONGOBOOT_RENDER_H
#define PONGOBOOT_RENDER_H

#include <stdint.h>
#include "ui.h"
#include "targets.h"

/* Renderer Functions */
void pongoboot_render_clear_screen(void);
void pongoboot_render_header(const char* title, const char* subtitle);
void pongoboot_render_footer(const char* help_text);

/* State-specific renderers */
void pongoboot_render_menu(ui_context_t* ctx, boot_registry_t* registry);
void pongoboot_render_options(ui_context_t* ctx, boot_registry_t* registry);
void pongoboot_render_confirm(ui_context_t* ctx, boot_registry_t* registry);
void pongoboot_render_booting(ui_context_t* ctx);
void pongoboot_render_error(ui_context_t* ctx);
void pongoboot_render_info(void);

/* Utility renderers */
void pongoboot_render_box(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void pongoboot_render_text(const char* text, uint32_t x, uint32_t y, uint32_t color);
void pongoboot_render_menu_item(const char* text, int is_selected, int is_available, uint32_t y);
void pongoboot_render_progress_bar(uint32_t progress_percent, uint32_t y);
void pongoboot_render_countdown(uint32_t seconds_remaining, uint32_t y);

#endif /* PONGOBOOT_RENDER_H */
