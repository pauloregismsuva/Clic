CC ?= cc
CPPFLAGS ?=
CFLAGS ?= -std=gnu11 -Wall -Wextra
LDFLAGS ?=
LDLIBS ?=
BUILD_DIR ?= build
EXAMPLE_SOURCES := $(wildcard exemplos/*.c)
EXAMPLE_BINARIES := $(patsubst exemplos/%.c,$(BUILD_DIR)/%,$(EXAMPLE_SOURCES))

.PHONY: examples clean

examples: $(EXAMPLE_BINARIES)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%: exemplos/%.c Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. $< Clic.c $(LDFLAGS) $(LDLIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)
