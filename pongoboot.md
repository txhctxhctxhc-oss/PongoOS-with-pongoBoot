# PongoBoot - UI/UX Design Document

## Design Philosophy

PongoBoot UI emphasizes **technical clarity** over visual complexity. The interface is:
- **Minimalist**: Text-based, high-contrast, minimal decorative elements
- **Terminal-like**: Monospace fonts, box-drawing characters, grid-based layout
- **Accessible**: High contrast ratios, clear visual hierarchy
- **Functional**: Every pixel serves a purpose; no wasted space

---

## Color Palette

```
Background:        #0a0e27  (Dark navy blue)
Primary Text:      #e8e8e8  (Light gray)
Highlight:         #00d4ff  (Bright cyan) — Selected items, active focus
Accent:            #00ff88  (Bright green) — Status indicators, success states
Border/Divider:    #444444  (Gray) — Boxes, rules, separators
Warning:           #ffaa00  (Orange) — Cautions, non-critical issues
Error:             #ff3333  (Bright red) — Errors, failures
Success:           #33ff33  (Bright green) — Confirmation states
Disabled:          #666666  (Dark gray) — Unavailable options
```

---

## Typography

- **Font**: Monospace (8x8 bitmap from existing framework)
- **Scale**: 2x default (16x16 effective pixels per character)
- **Line Height**: 1.5× character height (40px per line with 26px font)
- **Font Weights**: Regular only (no bold needed; use color/borders for emphasis)

---

## Layout Structure

### Screen Zones

```
┌─────────────────────────────────────────────────────────────────────────────┐
│ HEADER: Title + Status Bar (80px)                                          │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│ MENU AREA: Boot Targets (280px, 7 items max)                              │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│ DESCRIPTION AREA: Selection Details (120px)                               │
├─────────────────────────────────────────────────────────────────────────────┤
│ OPTIONS AREA: Boot Configuration (200px)                                   │
├─────────────────────────────────────────────────────────────────────────────┤
│ FOOTER: Help Text + Control Button (60px)                                 │
└─────────────────────────────────────────────────────────────────────────────┘

Total Height: ~740px (typical device)
Usable Width: 1024px (typical iPad/device)
Margin: 20px all sides
Content Width: 984px
```

---

## Screen 1: Main Boot Menu

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                                                             │
/**
 *                                   _                 _   
 *     _ __   ___  _ __   __ _  ___ | |__   ___   ___ | |_ 
 *    | '_ \ / _ \| '_ \ / _` |/ _ \| '_ \ / _ \ / _ \| __|
 *    | |_) | (_) | | | | (_| | (_) | |_) | (_) | (_) | |_ 
 *    | .__/ \___/|_| |_|\__, |\___/|_.__/ \___/ \___/ \__|
 *    |_|                |___/                             
 *                                                         
 *                                                         
 *                                                         
 *                                                         
 *                                                         
 *                                                         
 */   
                                                                                                                      
                                                                                                                          │
│                                                                             │
│   v2.1.0  │  Boot Menu  │  Device: iPhone12,1  │  Status: Ready            │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐  │
│  │                                                                     │  │
│  │  STANDARD BOOT OPTIONS                                             │  │
│  │                                                                     │  │
│  │  ► Boot pongoOS (Default)                                          │  │
│  │                                                                     │  │
│  │    Boot iOS (16.2)                                                 │  │
│  │                                                                     │  │
│  │    Recovery Mode                                                   │  │
│  │                                                                     │  │
│  │  ──────────────────────────────────────────────────────────────    │  │
│  │                                                                     │  │
│  │  UTILITIES & TOOLS                                                 │  │
│  │                                                                     │  │
│  │    System Information                                              │  │
│  │                                                                     │  │
│  │  ○ Linux Kernel (Not Available)                                    │  │
│  │                                                                     │  │
│  └─────────────────────────────────────────────────────────────────────┘  │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  SELECTED: Boot pongoOS (Default)                                           │
│                                                                             │
│  Continue to pongoOS with default kernel parameters. Loads custom           │
│  kernel, ramdisk, and boot modules. Recommended for general use.            │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌─ Boot Options ─────────────────────────────────────────────────────┐  │
│  │                                                                     │  │
│  │  Kernel Arguments:  -v debug=0x14e                                │  │
│  │  Verbose Boot:      ☐                                             │  │
│  │  Safe Mode:         ☐                                             │  │
│  │  Auto-Boot In:      5 seconds                                     │  │
│  │                                                                     │  │
│  └─────────────────────────────────────────────────────────────────────┘  │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ▲ VOLUP  ▼ VOLDOWN  ⊙ Select  ⌂ Back  ┌────────────┐  Timeout: 5s        │
│                                        │   Boot   ▶  │                    │
│                                        └────────────┘                      │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Menu Item Formats

**Selected (Highlighted)**
```
  ► Boot pongoOS (Default)
```
"►" = Selection indicator (cyan #00d4ff)
Item text uses primary text color (#e8e8e8)

**Available (Not selected)**
```
    Boot iOS (16.2)
```
Regular text color (#e8e8e8), no prefix

**Unavailable (Disabled)**
```
  ○ Linux Kernel (Not Available)
```
"○" = Unavailable indicator (gray #666666)
Text appears grayed out (#666666)
Cannot be selected, skipped by navigation

**Section Divider**
```
  ──────────────────────────────────────────────────────────────
```
Visual separator between Standard Boot Options and Utilities & Tools
Rendered in border color (#444444)

---

## Screen 2: Boot Options Editor

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                                                             │
│   PONGBOOT v2.1.0  │  Boot Options  │  Device: iPhone12,1  │  Editing...    │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  Target: Boot pongoOS (Default)                                             │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌─ Kernel Arguments ─────────────────────────────────────────────────┐  │
│  │                                                                     │  │
│  │  [-v debug=0x14e ml=debug verbose kcsuffix=d_______________________]  │
│  │                                                                     │  │
│  │  Cursor: 15/78 chars                                              │  │
│  │  Length: 42 bytes                                                 │  │
│  │                                                                     │  │
│  │  Navigation:  ◄ VOLDOWN  ► VOLUP                                  │  │
│  │  Editing:    DEL [BACK]  or add characters                        │  │
│  │                                                                     │  │
│  └─────────────────────────────────────────────────────────────────────┘  │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌─ Preset Options ───────────────────────────────────────────────────┐  │
│  │                                                                     │  │
│  │  [✓] Verbose Boot (-v)           [○] Safe Mode (-x)                │  │
│  │  [○] Single User (-s)            [○] No Cache (-xcache)            │  │
│  │  [○] Debug Mode (debug=0x14e)    [○] NVRAM Clear (-c)              │  │
│  │                                                                     │  │
│  └─────────────────────────────────────────────────────────────────────┘  │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  [✓] Enabled    [○] Disabled    [•] Preset                                │
│                                                                             │
│  ▲ VOLUP  ▼ VOLDOWN  ⊙ Toggle/Edit  ⌂ Cancel  ┌──────────────┐            │
│                                                 │   Continue  ▶ │            │
│                                                 └──────────────┘            │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Checkbox States

```
[✓]  Enabled (cyan checkmark)
[○]  Disabled (empty circle)
[•]  Preset/Default (filled circle)
```

---

## Screen 3: Boot Confirmation

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                                                             │
│   PONGBOOT v2.1.0  │  Ready to Boot  │  Device: iPhone12,1                 │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ╔══════════════════════════════════════════════════════════════════════╗  │
│  ║                                                                      ║  │
│  ║  BOOT CONFIRMATION                                                  ║  │
│  ║                                                                      ║  │
│  ║  Target:           Boot pongoOS (Default)                           ║  │
│  ║  Kernel Args:      -v debug=0x14e                                  ║  │
│  ║  Graphics Mode:    Framebuffer                                      ║  │
│  ║  Timeout Override: 5 seconds remaining                              ║  │
│  ║                                                                      ║  │
│  ║  STATUS: ARMED - Press ⊙ to boot now or wait for timeout            ║  │
│  ║                                                                      ║  │
│  ╚══════════════════════════════════════════════════════════════════════╝  │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ⊙ Confirm Boot                                                             │
│  ⌂ Cancel (Return to Menu)                                                 │
│                                                                             │
│                                                                             │
│  Auto-boot countdown:                                                      │
│  ████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  │
│  [====== 5 seconds ======]                                                │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Screen 4: Boot Progress

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                                                             │
│   PONGBOOT v2.1.0  │  Booting...  │  Device: iPhone12,1  │  Status: Active  │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ╭─────────────────────────────────────────────────────────────────────╮  │
│  │  ▶ [████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░]  60%   │  │
│  ╰─────────────────────────────────────────────────────────────────────╯  │
│                                                                             │
│  Initializing Boot Sequence...                                             │
│                                                                             │
│  ✓ Firmware pre-flight checks passed                                      │
│  ✓ Kernel image validated                                                 │
│  ✓ Ramdisk loaded into memory                                             │
│  ► Transferring to kernel...                                              │
│                                                                             │
│  Target:     Boot pongoOS (Default)                                        │
│  Mode:       Normal Boot                                                   │
│  Arguments:  -v debug=0x14e                                              │
│                                                                             │
│                                                                             │
│  Elapsed:  3.2s    Remaining: ~2s                                          │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Screen 5: Error Handling

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                                                             │
│   PONGBOOT v2.1.0  │  ERROR  │  Device: iPhone12,1  │  Status: Failed      │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ╔══════════════════════════════════════════════════════════════════════╗  │
│  ║  ⚠  BOOT FAILED                                                     ║  │
│  ╚══════════════════════════════════════════════════════════════════════╝  │
│                                                                             │
│  Error Code:        0xBEEF0001                                             │
│  Description:       Invalid kernel image signature                        │
│                                                                             │
│  Target:            Boot macOS                                             │
│  Last Action:       Loading kernel from storage                           │
│  Time of Failure:   14:35:42 UTC                                          │
│                                                                             │
│  ┌──────────────────────────────────────────────────────────────────┐    │
│  │  Log Output (Last 5 lines):                                      │    │
│  │                                                                  │    │
│  │  [14:35:40.123] Reading kernel from offset 0x8000000           │    │
│  │  [14:35:41.045] Image size verified: 0x400000 bytes            │    │
│  │  [14:35:41.234] Computing image digest...                      │    │
│  │  [14:35:42.001] ⚠  HASH MISMATCH: Expected 0xDEADBEEF         │    │
│  │  [14:35:42.002] Boot aborted.                                  │    │
│  │                                                                  │    │
│  └──────────────────────────────────────────────────────────────────┘    │
│                                                                             │
│  ⌂ Return to Menu  (or auto-return in 10 seconds)                          │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Error Display States

```
⚠  = Warning/Error indicator (red #ff3333)
✓ = Success checkmark (green #33ff33)
▶ = Progress indicator (cyan #00d4ff)
● = Status dot (varies by state)
```

---

## Screen 6: System Information

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                                                             │
│   PONGBOOT v2.1.0  │  System Information  │  Device: iPhone12,1            │
│                                                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌─ Device Information ──────────────────────────────────────────────┐   │
│  │  Model:              iPhone12,1 (A2176)                           │   │
│  │  Processor:          Apple A14 Bionic (ARMv8.3-A)                 │   │
│  │  Memory:             4.0 GB LPDDR4X                               │   │
│  │  NAND Storage:       128 GB                                       │   │
│  │  Bootloader:         PongoBoot v2.1.0                             │   │
│  │  Build:              24F5055c (dated 2026-05-22)                 │   │
│  └─────────────────────────────────────────────────────────────────────┘   │
│                                                                             │
│  ┌─ Boot Configuration ──────────────────────────────────────────────┐   │
│  │  Default Target:     Boot pongoOS (Default)                       │   │
│  │  Default Arguments:  -v debug=0x14e                              │   │
│  │  Auto-Boot Timeout:  5 seconds                                    │   │
│  │  Security Mode:      Enabled (Apple Secure Boot)                 │   │
│  └─────────────────────────────────────────────────────────────────────┘   │
│                                                                             │
│  ┌─ Storage Partitions ──────────────────────────────────────────────┐   │
│  │  System:             72.5 GB  [████████████████░░░░░░░]           │   │
│  │  Data:               42.8 GB  [██████████░░░░░░░░░░░░░░]          │   │
│  │  Recovery:           3.2 GB   [██░░░░░░░░░░░░░░░░░░░░░░]         │   │
│  │  Free:               9.5 GB                                       │   │
│  └─────────────────────────────────────────────────────────────────────┘   │
│                                                                             │
│  ⌂ Back to Menu                                                            │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Input/Control Reference

### Hardware Button Mapping

```
VOLUP               Move selection UP, or cursor LEFT in text
VOLDOWN             Move selection DOWN, or cursor RIGHT in text
HOME                Confirm selection, ENTER text edit mode
SIDE                Cancel/Go BACK
POWER (long press)  Force shutdown/reboot
```

### On-Screen Control Legend

```
▲ VOLUP      — Navigate up / Previous item
▼ VOLDOWN    — Navigate down / Next item
⊙ Select     — Confirm / Enter / Accept
⌂ Back       — Cancel / Go back / Escape
► [Button]   — Action button (cyan highlight)
```

---

## Visual Elements & Borders

### Box Drawing

```
Top-left corner:    ┌  │
Top-right corner:   ┐  │
Bottom-left corner: └  │
Bottom-right corner: ┘  │
Horizontal line:    ─
Vertical line:      │
T-junction:         ┬
Cross:              ┼
```

### Examples

```
┌─ Section Title ─────────────┐
│                             │
│  Content                    │
│                             │
└─────────────────────────────┘

╔════ Important ═════════════╗
║  Highlighted section       ║
╚════════════════════════════╝

├─────────────────────────────┤
Divider between sections
```

---

## Color Usage Guidelines

### Primary UI Elements
- **Cyan (#00d4ff)**: Selected items, active buttons, important indicators
- **Gray (#444444)**: Borders, dividers, structural elements
- **Light gray (#e8e8e8)**: Primary text content

### Status Indicators
- **Green (#33ff33)**: Success states, completed actions
- **Red (#ff3333)**: Errors, failures, warnings
- **Orange (#ffaa00)**: Cautions, non-critical issues
- **Gray (#666666)**: Disabled/unavailable options

### Background
- **Dark navy (#0a0e27)**: Main background (high contrast)

---

## Accessibility & Contrast

All text meets **WCAG AA contrast** standards:

```
Light gray text on navy background:   ~9:1 ratio ✓
Cyan highlights on navy background:   ~8:1 ratio ✓
Red errors on navy background:        ~6.5:1 ratio ✓
```

---

## Responsive Behavior

### For Smaller Screens (iPad Mini, ~768px wide)

```
Adjust margins to 10px
Reduce font scale from 2x to 1.5x (12px effective)
Menu items condensed to 5-6 visible at a time
Description area reduced or moved below
```

### For Larger Screens (iPad Pro, ~1280px wide)

```
Increase margins to 40px
Add additional sidebar (system status, recent logs)
Menu items expand with more padding
Options area laid out in columns
```

---

## Animation & Transitions

### Selection Highlight

```
When user navigates to new item:
  1. Current item loses cyan background
  2. New item gains cyan background
  3. Description text updates immediately (no fade)
  4. No animation delay (instant feedback)
```

### Progress Bars

```
Boot progress bar fills smoothly:
  - Update every 100ms
  - No interpolation (jumps to actual progress)
  - Green fill color when < 90%
  - Orange when 90-99%
  - Green confirmation when complete
```

### Timeout Countdown

```
Countdown timer updates every 1 second:
  - Visual progress bar depletes smoothly
  - Text shows remaining seconds
  - No animations (stark, technical feel)
```

---

## Error Message Hierarchy

### Critical (Block boot, requires user action)

```
╔══════════════════════════════════════╗
║  ⚠  CRITICAL ERROR                  ║
║  Description of what failed          ║
║  Error code: 0xXXXXXXXX              ║
║  Action required: Return to Menu     ║
╚══════════════════════════════════════╝
```

### Warning (Non-blocking, show info)

```
┌────────────────────────────────────┐
│  ⚠  WARNING                        │
│  Description of potential issue     │
│  [Continue] [Cancel]               │
└────────────────────────────────────┘
```

### Info (Informational only)

```
ℹ  Informational message shown in status bar or log
   No action required, continues normally
```

---

## Performance & Rendering

### Screen Update Strategy

- **Full refresh**: Only on major state changes (menu switch, boot start)
- **Partial update**: Selection highlight changes (just update two items)
- **Minimal update**: Countdown timer (just update one line)
- **No update**: During boot sequence (UART logging continues in background)

### Rendering Performance Target

- **Menu navigation**: <16ms per frame (60 FPS)
- **Text input**: <50ms character echo (20 FPS acceptable for text input)
- **Boot progress**: <100ms per update (10 FPS sufficient)

---

## Future UI Enhancements

1. **Touch Input Support**: Tap menu items, drag scroll menus
2. **Graphical Boot Logos**: Render Apple logos, custom artwork
3. **Dark/Light Themes**: User preference switching
4. **Localization**: Multi-language support (menus in different languages)
5. **Advanced Terminal**: Full POSIX shell in utility mode
6. **Network Boot**: Boot from network configurations
7. **Cloud Sync**: Sync preferences to iCloud (future enhancement)

