# PongoBoot Implementation Plan and Status

## Current Status

Checked: 2026-05-22

PongoBoot is now implemented as a v1 pre-shell framebuffer boot menu. It is integrated into the pongoOS startup path after hardware initialization and before the normal shell banner. The v1 goal is a usable menu over the boot flows already present in this tree, not a persistent storage-backed multi-boot manager.

Implemented:
- `src/pongoboot/` module with target registry, discovery, options, UI state, input, rendering, and boot callbacks.
- Top-level build inclusion for `src/pongoboot/*.c`.
- Startup integration through `pongoboot_main()` in `pongo_main_task()`.
- Framebuffer menu rendering using existing `gFramebuffer`, dimensions, cache flushing, and `font8x8_basic`.
- UART/USB key input through the existing stdin queue.
- Best-effort GPIO button input when a `buttons` device-tree node exposes interrupts.
- Boot targets for continuing to pongoOS shell, booting XNU through `BOOT_FLAG_HARD`, booting prepared Linux through `BOOT_FLAG_LINUX`, and viewing system info.
- In-memory boot options only.

Validated:
- `make -C src/pongoboot` completes and builds `src/pongoboot/build/libpongoboot.a`.
- Syntax-only checking of `src/pongoboot/*.c` no longer reports errors.

Known limitations:
- Full top-level build still depends on the repository's external/newlib setup.
- GPIO button ordering is best-effort: first interrupt maps to up, second to down, third to select, fourth to back.
- Linux is only available when the existing Linux loader state is already prepared.
- Recovery/DFU, raw image boot, persistent preferences, and storage scanning are not part of v1.

## Behavior

PongoBoot starts before the shell and displays a framebuffer menu. If the timeout expires, it selects the default target, `Boot pongoOS`, and continues to the normal shell. Pressing back from the main menu also exits to the shell.

Input mappings:
- `W`, `K`, or `+`: move up
- `S`, `J`, or `-`: move down
- Enter or `B`: select/boot
- Escape, Backspace, or `Q`: back/cancel
- GPIO buttons use the best-effort interrupt order described above

Boot behavior:
- `Boot pongoOS`: returns from PongoBoot and continues into the existing shell path.
- `Boot XNU`: applies session kernel arguments to the XNU command line, sets `gBootFlag = BOOT_FLAG_HARD`, and yields to the existing boot path.
- `Boot Linux`: only enabled when `linux_can_boot()` succeeds, then sets `gBootFlag = BOOT_FLAG_LINUX` and yields.
- `System Information`: renders current platform, device, SoC, and framebuffer details.

## Remaining Work

Priority 1: Hardware validation
- Test menu rendering on real framebuffer sizes and orientations.
- Verify UART/USB input before the shell starts.
- Verify GPIO button interrupts and adjust mapping if device-tree ordering differs by platform.

Priority 2: Boot-path validation
- Confirm XNU boot argument writes are correct on target iOS versions.
- Confirm XNU and Linux handoff paths leave the scheduler through the existing `gBootFlag` flow.
- Add error messages for unavailable or unprepared targets based on real device results.

Priority 3: Feature expansion
- Add raw image boot as an optional target using the existing `BOOT_FLAG_RAW` path.
- Add recovery/DFU only after identifying a safe existing reset/recovery API.
- Add persistent preferences only after a safe storage/NVRAM API is chosen.
- Add richer target discovery if storage probing becomes available in pongoOS.

## Acceptance Criteria

The v1 implementation is complete when:
- `make -C src/pongoboot` succeeds.
- The full pongoOS build compiles in a configured checkout.
- The menu appears before the shell on hardware.
- The default timeout reaches the shell.
- Keyboard navigation can boot/exit.
- XNU and prepared Linux selections enter their existing boot flows.
- GPIO buttons either work on validated devices or fail harmlessly with UART/USB input still usable.
