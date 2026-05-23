# PongoBoot build configuration for integration with main pongoOS Makefile

PONGOBOOT_DIR := src/pongoboot
PONGOBOOT_OBJDIR := build/pongoboot
PONGOBOOT_LIB := $(PONGOBOOT_OBJDIR)/libpongoboot.a
PONGOBOOT_LINKED_OBJ := $(PONGOBOOT_OBJDIR)/pongoboot_linked.o

# Build PongoBoot library
.PHONY: pongoboot
pongoboot:
	@echo "[PongoBoot] Building..."
	@$(MAKE) -C $(PONGOBOOT_DIR) OBJDIR=$(abspath $(PONGOBOOT_OBJDIR))

# Clean PongoBoot build artifacts
.PHONY: pongoboot-clean
pongoboot-clean:
	@echo "[PongoBoot] Cleaning..."
	@$(MAKE) -C $(PONGOBOOT_DIR) clean

# Link PongoBoot library into kernel object file
$(PONGOBOOT_LINKED_OBJ): $(PONGOBOOT_LIB)
	@echo "[PongoBoot] Linking object file..."
	@mkdir -p $(dir $@)
	@ld -r -o $@ --whole-archive $< --no-whole-archive

# Include this in main kernel link line:
# $(LD) -o kernel.o $(KERNEL_OBJS) $(PONGOBOOT_LINKED_OBJ) ...

# Optionally add to main build target:
# kernel: pongoboot $(PONGOBOOT_LINKED_OBJ) ...
