#include "render.h"
#include "theme.h"
#include <pongo.h>
#include <font8x8_basic.h>
#include <stdio.h>
#include <string.h>

extern const char* hal_platform_name(void);

#define PB_FONT_SCALE 2U
#define PB_CHAR_W (8U * PB_FONT_SCALE)
#define PB_CHAR_H (8U * PB_FONT_SCALE)

static void pb_flush(void)
{
    if (gFramebuffer) {
        cache_clean(gFramebuffer, gHeight * gRowPixels * 4);
    }
}

static void pb_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (!gFramebuffer || x >= gWidth || y >= gHeight) return;
    gFramebuffer[x + y * gRowPixels] = color;
}

static void pb_fill_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    if (!gFramebuffer || x >= gWidth || y >= gHeight) return;
    if (x + width > gWidth) width = gWidth - x;
    if (y + height > gHeight) height = gHeight - y;

    for (uint32_t yy = 0; yy < height; yy++) {
        for (uint32_t xx = 0; xx < width; xx++) {
            gFramebuffer[(x + xx) + ((y + yy) * gRowPixels)] = color;
        }
    }
}

static void pb_draw_char(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg)
{
    if (!gFramebuffer) return;

    const uint8_t* glyph = (const uint8_t*)font8x8_basic[(uint8_t)c & 0x7f];
    for (uint32_t gy = 0; gy < 8; gy++) {
        for (uint32_t gx = 0; gx < 8; gx++) {
            uint32_t color = (glyph[gy] & (1U << gx)) ? fg : bg;
            for (uint32_t sy = 0; sy < PB_FONT_SCALE; sy++) {
                for (uint32_t sx = 0; sx < PB_FONT_SCALE; sx++) {
                    pb_pixel(x + (gx * PB_FONT_SCALE) + sx, y + (gy * PB_FONT_SCALE) + sy, color);
                }
            }
        }
    }
}

static void pb_draw_text_bg(const char* text, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg)
{
    if (!text) return;
    while (*text && x + PB_CHAR_W < gWidth) {
        pb_draw_char(*text++, x, y, fg, bg);
        x += PB_CHAR_W;
    }
}

static void pb_draw_text(const char* text, uint32_t x, uint32_t y, uint32_t fg)
{
    pb_draw_text_bg(text, x, y, fg, COLOR_BG);
}

static void pb_draw_border(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    if (width < 2 || height < 2) return;
    pb_fill_rect(x, y, width, 2, color);
    pb_fill_rect(x, y + height - 2, width, 2, color);
    pb_fill_rect(x, y, 2, height, color);
    pb_fill_rect(x + width - 2, y, 2, height, color);
}

static uint32_t pb_content_width(void)
{
    if (gWidth < 80) return gWidth;
    return gWidth - 40;
}

void pongoboot_render_clear_screen(void)
{
    screen_fill(COLOR_BG);
}

void pongoboot_render_header(const char* title, const char* subtitle)
{
    pb_fill_rect(0, 0, gWidth, HEADER_HEIGHT, COLOR_BG);
    pb_draw_text(title ? title : "PongoBoot", 24, 22, COLOR_ACCENT);
    if (subtitle && *subtitle) {
        pb_draw_text(subtitle, 24, 48, COLOR_TEXT);
    }
    pb_fill_rect(20, HEADER_HEIGHT - 4, pb_content_width(), 2, COLOR_BORDER);
}

void pongoboot_render_footer(const char* help_text)
{
    uint32_t y = gHeight > FOOTER_HEIGHT ? gHeight - FOOTER_HEIGHT : 0;
    pb_fill_rect(0, y, gWidth, FOOTER_HEIGHT, COLOR_BG);
    pb_fill_rect(20, y, pb_content_width(), 2, COLOR_BORDER);
    pb_draw_text(help_text ? help_text : "W/K: up  S/J: down  Enter/B: select  Esc/Q: back", 24, y + 18, COLOR_TEXT);
}

void pongoboot_render_menu(ui_context_t* ctx, boot_registry_t* registry)
{
    if (!ctx || !registry) return;

    pongoboot_render_clear_screen();
    pongoboot_render_header("PongoBoot", "Select a boot target");

    uint32_t y = MENU_START_Y;
    for (uint32_t i = 0; i < registry->count; i++) {
        boot_target_t* target = &registry->targets[i];
        if (target->type == BOOT_SECTION_DIVIDER) {
            pb_fill_rect(40, y + (MENU_ITEM_HEIGHT / 2), pb_content_width() - 40, 2, COLOR_BORDER);
            y += MENU_ITEM_HEIGHT;
            continue;
        }

        int selected = (i == ctx->selection_index);
        uint32_t bg = selected ? COLOR_HIGHLIGHT : COLOR_BG;
        uint32_t fg = target->is_available ? (selected ? COLOR_BG : COLOR_TEXT) : COLOR_DISABLED;
        char line[160];

        snprintf(line, sizeof(line), "%s%s%s%s",
                 target->is_default ? "* " : "  ",
                 target->name ? target->name : "(unnamed)",
                 target->version ? " " : "",
                 target->version ? target->version : "");

        if (selected) {
            pb_fill_rect(28, y - 8, pb_content_width() - 16, MENU_ITEM_HEIGHT - 4, bg);
        }
        pb_draw_text_bg(line, 44, y, fg, bg);
        y += MENU_ITEM_HEIGHT;
    }

    if (ctx->selection_index < registry->count) {
        boot_target_t* target = &registry->targets[ctx->selection_index];
        if (target->description) {
            pb_draw_text(target->description, 32, DESCRIPTION_Y, COLOR_TEXT);
        }
    }

    char timeout[80];
    snprintf(timeout, sizeof(timeout), "Auto-boot in %u seconds", ctx->timeout_remaining);
    pb_draw_text(timeout, 32, DESCRIPTION_Y + 32, COLOR_WARNING);
    pongoboot_render_footer("W/K: up  S/J: down  Enter/B: boot  Q/Esc: shell");
    pb_flush();
}

void pongoboot_render_options(ui_context_t* ctx, boot_registry_t* registry)
{
    (void)registry;
    if (!ctx) return;

    pongoboot_render_clear_screen();
    pongoboot_render_header("Boot Options", "Session-only settings");
    pb_draw_text("Kernel arguments:", 32, 120, COLOR_TEXT);
    pb_draw_border(32, 150, pb_content_width() - 24, 44, COLOR_BORDER);
    pb_draw_text(ctx->pending_options.kernel_args, 44, 164, COLOR_ACCENT);
    pb_draw_text(ctx->pending_options.verbose_boot ? "[x] Verbose boot" : "[ ] Verbose boot", 32, 220, COLOR_TEXT);
    pb_draw_text(ctx->pending_options.safe_mode ? "[x] Safe mode" : "[ ] Safe mode", 32, 252, COLOR_TEXT);
    pongoboot_render_footer("Enter/B: accept  Q/Esc: cancel");
    pb_flush();
}

void pongoboot_render_confirm(ui_context_t* ctx, boot_registry_t* registry)
{
    if (!ctx || !registry || ctx->selection_index >= registry->count) return;

    boot_target_t* target = &registry->targets[ctx->selection_index];
    pongoboot_render_clear_screen();
    pongoboot_render_header("Confirm Boot", target->name);
    pb_draw_text("Target:", 32, 126, COLOR_TEXT);
    pb_draw_text(target->name, 144, 126, COLOR_ACCENT);
    pb_draw_text("Kernel args:", 32, 166, COLOR_TEXT);
    pb_draw_text(ctx->pending_options.kernel_args, 32, 198, COLOR_TEXT);

    char timeout[80];
    snprintf(timeout, sizeof(timeout), "Booting in %u seconds", ctx->timeout_remaining);
    pb_draw_text(timeout, 32, 250, COLOR_WARNING);
    pongoboot_render_footer("Enter/B: boot now  Q/Esc: cancel");
    pb_flush();
}

void pongoboot_render_booting(ui_context_t* ctx)
{
    (void)ctx;
    pongoboot_render_clear_screen();
    pongoboot_render_header("PongoBoot", "Booting selected target");
    pongoboot_render_progress_bar(65, 160);
    pb_draw_text("Transferring control...", 32, 210, COLOR_TEXT);
    pb_flush();
}

void pongoboot_render_error(ui_context_t* ctx)
{
    if (!ctx) return;

    char code[80];
    pongoboot_render_clear_screen();
    pongoboot_render_header("Boot Failed", "The selected target returned an error");
    snprintf(code, sizeof(code), "Error code: 0x%08x", ctx->error_code);
    pb_draw_text(code, 32, 126, COLOR_ERROR);
    if (ctx->error_message) {
        pb_draw_text(ctx->error_message, 32, 166, COLOR_TEXT);
    }
    snprintf(code, sizeof(code), "Returning to menu in %u seconds", ctx->timeout_remaining);
    pb_draw_text(code, 32, 220, COLOR_WARNING);
    pongoboot_render_footer("Enter/B/Q/Esc: return to menu");
    pb_flush();
}

void pongoboot_render_info(void)
{
    char line[128];

    pongoboot_render_clear_screen();
    pongoboot_render_header("System Information", "pongoOS runtime");
    snprintf(line, sizeof(line), "Platform: %s", hal_platform_name());
    pb_draw_text(line, 32, 120, COLOR_TEXT);
    snprintf(line, sizeof(line), "Device: %s", gDevType ? gDevType : "unknown");
    pb_draw_text(line, 32, 152, COLOR_TEXT);
    snprintf(line, sizeof(line), "SoC: %s / 0x%x", soc_name, socnum);
    pb_draw_text(line, 32, 184, COLOR_TEXT);
    snprintf(line, sizeof(line), "Framebuffer: %ux%u", gWidth, gHeight);
    pb_draw_text(line, 32, 216, COLOR_TEXT);
    pongoboot_render_footer("Enter/B/Q/Esc: return");
    pb_flush();
}

void pongoboot_render_box(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    pb_draw_border(x, y, width, height, color);
    pb_flush();
}

void pongoboot_render_text(const char* text, uint32_t x, uint32_t y, uint32_t color)
{
    pb_draw_text(text, x, y, color);
    pb_flush();
}

void pongoboot_render_menu_item(const char* text, int is_selected, int is_available, uint32_t y)
{
    uint32_t bg = is_selected ? COLOR_HIGHLIGHT : COLOR_BG;
    uint32_t fg = is_available ? (is_selected ? COLOR_BG : COLOR_TEXT) : COLOR_DISABLED;
    if (is_selected) {
        pb_fill_rect(28, y - 8, pb_content_width() - 16, MENU_ITEM_HEIGHT - 4, bg);
    }
    pb_draw_text_bg(text, 44, y, fg, bg);
    pb_flush();
}

void pongoboot_render_progress_bar(uint32_t progress_percent, uint32_t y)
{
    uint32_t width = pb_content_width() - 24;
    if (progress_percent > 100) progress_percent = 100;
    pb_draw_border(32, y, width, 28, COLOR_BORDER);
    pb_fill_rect(36, y + 4, ((width - 8) * progress_percent) / 100, 20, COLOR_ACCENT);
}

void pongoboot_render_countdown(uint32_t seconds_remaining, uint32_t y)
{
    char line[64];
    snprintf(line, sizeof(line), "%u seconds", seconds_remaining);
    pb_draw_text(line, 32, y, COLOR_WARNING);
    pb_flush();
}
