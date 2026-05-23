# PongoBoot Makefile
# Multi-boot bootloader for pongoOS

SRCDIR := .
OBJDIR ?= build
DEPDIR := $(OBJDIR)/.deps

# Source files
SOURCES := $(wildcard $(SRCDIR)/*.c)
HEADERS := $(wildcard $(SRCDIR)/*.h)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
DEPS := $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SOURCES))

# Output
PONGOBOOT_LIB := $(OBJDIR)/libpongoboot.a
PONGOBOOT_OBJ := $(OBJDIR)/pongoboot.o

# Compiler flags
CC ?= clang
CFLAGS ?= -O2 -Wall -Wextra -fPIC
INCLUDES := -I$(SRCDIR) -I../../include -I../kernel -I../drivers -I../lib -I../../apple-include
DEFINES := -DPONGOBOOT_BUILD -DPONGO_PRIVATE=1

# Default target
.PHONY: all
all: $(PONGOBOOT_LIB)

# Create directories
$(OBJDIR) $(DEPDIR):
	@mkdir -p $@

# Compile C source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR) $(DEPDIR)
	@echo "  CC  $<"
	@$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -MMD -MF $(DEPDIR)/$*.d -c $< -o $@

# Create static library
$(PONGOBOOT_LIB): $(OBJECTS)
	@echo "  AR  $@"
	@ar rcs $@ $^

# Link object file (for linking into kernel)
$(PONGOBOOT_OBJ): $(PONGOBOOT_LIB)
	@echo "  LD  $@"
	@ld -r -o $@ --whole-archive $< --no-whole-archive

# Dependencies
-include $(DEPS)

# Clean
.PHONY: clean
clean:
	@echo "  CLEAN pongoboot"
	@rm -rf $(OBJDIR)

# Verbose output
.PHONY: verbose
verbose: CFLAGS += -v
verbose: all

# Dump symbols
.PHONY: symbols
symbols: $(PONGOBOOT_LIB)
	@echo "  NM  $<"
	@nm $<

.PHONY: help
help:
	@echo "PongoBoot Makefile targets:"
	@echo "  all       - Build libpongoboot.a (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  verbose   - Build with verbose output"
	@echo "  symbols   - List library symbols"
	@echo "  help      - Show this help message"
