#ifndef PONGOBOOT_THEME_H
#define PONGOBOOT_THEME_H

#include <stdint.h>

/* Color Palette (ARGB8888) */
#define COLOR_BG            0xFF0a0e27   /* Dark navy blue background */
#define COLOR_TEXT          0xFFe8e8e8   /* Light gray text */
#define COLOR_HIGHLIGHT     0xFF00d4ff   /* Bright cyan - Selected items */
#define COLOR_ACCENT        0xFF00ff88   /* Bright green - Status indicators */
#define COLOR_BORDER        0xFF444444   /* Gray borders and dividers */
#define COLOR_WARNING       0xFFffaa00   /* Orange - Cautions */
#define COLOR_ERROR         0xFFff3333   /* Bright red - Errors */
#define COLOR_SUCCESS       0xFF33ff33   /* Bright green - Success states */
#define COLOR_DISABLED      0xFF666666   /* Dark gray - Unavailable options */

/* Screen Layout Dimensions (pixels) */
#define HEADER_HEIGHT       80
#define MENU_START_Y        100
#define MENU_ITEM_HEIGHT    40
#define DESCRIPTION_Y       350
#define OPTIONS_BOX_Y       400
#define FOOTER_HEIGHT       60
#define PONGOBOOT_LEFT_MARGIN  20
#define PONGOBOOT_RIGHT_MARGIN 20

/* Typography */
#define FONT_SCALE_FACTOR   2           /* Monospace font scale (2x default) */
#define CHAR_WIDTH          (8 * FONT_SCALE_FACTOR)
#define CHAR_HEIGHT         (8 * FONT_SCALE_FACTOR)

/* UI Element Width */
#define MAX_MENU_WIDTH      750
#define MAX_MENU_ITEMS      10

/* Timeout Configuration */
#define DEFAULT_BOOT_TIMEOUT_SEC    5
#define ERROR_TIMEOUT_SEC           10

#endif /* PONGOBOOT_THEME_H */
