CC ?= cc
PYTHON ?= python3
CPPFLAGS ?=
CFLAGS ?= -std=gnu11 -Wall -Wextra
LDFLAGS ?=
LDLIBS ?=
BUILD_DIR ?= build
EXAMPLE_SOURCES := $(wildcard exemplos/*.c)
EXAMPLE_BINARIES := $(patsubst exemplos/%.c,$(BUILD_DIR)/%,$(EXAMPLE_SOURCES))

.PHONY: test test-allocations examples clean

test: $(BUILD_DIR)/test_text $(BUILD_DIR)/test_rows $(BUILD_DIR)/render_fixture
	./$(BUILD_DIR)/test_text
	./$(BUILD_DIR)/test_rows
	$(PYTHON) tests/test_render.py ./$(BUILD_DIR)/render_fixture

$(BUILD_DIR):
	mkdir -p $@

examples: $(EXAMPLE_BINARIES)

$(BUILD_DIR)/%: exemplos/%.c Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. $< Clic.c $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/test_text: tests/test_text.c tests/test_locale.h Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. tests/test_text.c Clic.c $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/test_rows: tests/test_rows.c tests/test_locale.h Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. tests/test_rows.c Clic.c $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/render_fixture: tests/render_fixture.c tests/test_locale.h Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. tests/render_fixture.c Clic.c $(LDFLAGS) $(LDLIBS) -o $@

test-allocations: $(BUILD_DIR)/test_allocations
	./$(BUILD_DIR)/test_allocations

$(BUILD_DIR)/test_allocations: tests/test_rows.c tests/track_allocations.c tests/test_locale.h Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCLIC_TEST_ALLOCATIONS -I. tests/test_rows.c tests/track_allocations.c Clic.c $(LDFLAGS) -Wl,--wrap=malloc,--wrap=calloc,--wrap=free $(LDLIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)
