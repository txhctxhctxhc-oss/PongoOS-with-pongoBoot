# PongoBoot - Multi-boot Bootloader for PongoOS

PongoBoot is an interactive bootloader UI that runs on top of PongoOS, allowing users to select and configure boot targets (pongoOS, iOS, macOS, Recovery Mode, Utilities) via a framebuffer-based menu system.

## Architecture

### Core Modules

- **main.c** — Entry point and main event loop
- **targets.c/h** — Boot target registry and management
- **options.c/h** — Boot options and kernel argument configuration
- **discovery.c/h** — Dynamic boot target detection
- **ui.c/h** — UI state machine and event handling
- **input.c/h** — Hardware input handling (buttons, keyboard, touchscreen)
- **render.c/h** — Framebuffer rendering and display output
- **boot_callbacks.c/h** — Boot target execution callbacks
- **theme.h** — Color palette, layout dimensions, typography constants

## Building

### Standalone
```bash
cd src/pongoboot
make
```

Output: `build/pongoboot/libpongoboot.a`

### Integrated with PongoOS
The main pongoOS Makefile should include PongoBoot:
```bash
make -C src/pongoboot
PONGOBOOT_LIB = src/pongoboot/build/pongoboot/libpongoboot.a
```

Link into kernel:
```bash
$(LD) -r -o kernel.o ... $(PONGOBOOT_LIB)
```

## Integration Points

### 1. Boot Flow Integration
PongoBoot runs after stage3 and before the pongoOS kernel:

```
Stage2/Checkra1n
    ↓
Stage3 Loader
    ↓
pongoboot_main() ← Call from src/boot/main.c or src/kernel/entry.c
    ↓
User selects boot target
    ↓
Boot callback executes (boot_xnu, boot_pongoOS, etc.)
    ↓
Transfer control to kernel or alternative system
```

### 2. Framebuffer Integration
PongoBoot uses existing framebuffer driver (`src/drivers/framebuffer/fb.c`):

**Required functions:**
- `screen_putc(char c)` — Output single character
- `screen_puts(const char* str)` — Output string
- `screen_fill(uint32_t color)` — Fill screen with color
- `gFramebuffer` — Global framebuffer pointer (uint32_t*)
- `gWidth`, `gHeight` — Framebuffer dimensions
- `gRowPixels` — Pixels per scanline

**Enhancement TODO:** Add box-drawing primitives (rectangles, lines) for improved UI rendering.

### 3. Input System Integration
PongoBoot defines input interface (`input.h`):

```c
typedef enum {
    INPUT_VOLUP,    /* Volume up button */
    INPUT_VOLDOWN,  /* Volume down button */
    INPUT_HOME,     /* Home button */
    INPUT_SIDE,     /* Side button */
    INPUT_POWER,    /* Power button */
} input_key_t;
```

**Hardware integration points:**
- Volume buttons → GPIO
- Home button → GPIO or USB
- Side button → GPIO
- Power button → PMU (power management unit)

See `input.c` for implementation hooks.

### 4. Boot Callbacks Integration
Each boot target has a callback function:

```c
int boot_pongoOS(boot_options_t* opts);    /* Continue to kernel */
int boot_xnu(boot_options_t* opts);        /* Boot iOS/macOS */
int boot_recovery(boot_options_t* opts);   /* Enter DFU/Recovery */
int boot_system_info(boot_options_t* opts);/* Display system info */
```

See `boot_callbacks.c` for implementation framework.

## Configuration

### Default Settings
See `theme.h` for customization:

- **Colors**: ARGB8888 format (modifiable constants)
- **Layout**: Screen zones, menu height, fonts
- **Timeout**: Default 5-second auto-boot (configurable in code)

### Boot Targets
Boot targets are discovered at runtime in `discovery.c`:

- **PongoOS** — Always available (default)
- **iOS/XNU** — iPhone/iPad only (detected at runtime)
- **macOS** — Mac only (detected at runtime)
- **Recovery** — Always available if hardware supports it
- **Utilities** — System information and tools

### Font & Typography
- Monospace bitmap font (8x8 from existing framework)
- 2x scaling = 16x16 effective pixel size
- Can be modified in `theme.h` and `render.c`

## Development Status

### Implemented ✓
- Core data structures and type definitions
- Boot target discovery framework
- UI state machine
- Input handling framework
- Renderer stubs
- Main event loop
- Build system (Makefile)

### TODO - Framebuffer Rendering
- [ ] Render box borders and frames
- [ ] Render text with color
- [ ] Render menu items with selection highlight
- [ ] Render progress bars
- [ ] Render countdown timer
- [ ] Optimize framebuffer updates (dirty region tracking)

### TODO - Boot Callbacks
- [ ] Implement `boot_pongoOS()` — Apply kernel args, transfer control
- [ ] Implement `boot_xnu()` — Load kernel, verify signature, boot
- [ ] Implement `boot_recovery()` — Enter DFU mode
- [ ] Implement `boot_system_info()` — Display device and partition info

### TODO - Hardware Integration
- [ ] Button input handling (interrupt-driven)
- [ ] USB keyboard input support (optional)
- [ ] Touchscreen input support (optional)
- [ ] NVRAM boot preferences (optional)

### TODO - Advanced Features
- [ ] Boot script support
- [ ] Multi-profile boot configurations
- [ ] Network boot capability
- [ ] Module loading before boot
- [ ] Graphical logo rendering

## Testing

### Build Test
```bash
make -C src/pongoboot
# Check for libpongoboot.a in build/pongoboot/
```

### Symbol Check
```bash
make -C src/pongoboot symbols
# Verify all symbols are present
```

### Integration Test
Compile into pongoOS kernel and boot on device to test:
1. Menu navigation (VOLUP/VOLDOWN)
2. Boot target selection (HOME)
3. Options editing
4. Timeout countdown
5. Boot execution

## Performance Targets

- **Menu navigation**: <16ms per frame (60 FPS)
- **Boot sequence**: ~500ms from menu to boot execution
- **Memory usage**: <50KB code, <10KB state data
- **Framebuffer overhead**: <300ms boot time impact

## References

- **Architecture Plan**: `/home/lasse/pongoOS/plan.md`
- **UI/UX Design**: `/home/lasse/pongoOS/pongoboot.md`
- **Framebuffer Driver**: `src/drivers/framebuffer/fb.c`
- **Boot Infrastructure**: `src/boot/main.c`, `src/kernel/entry.c`

## License

Same as pongoOS project

## Author

Development started: 2026-05-22
