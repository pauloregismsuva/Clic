CC ?= cc
CPPFLAGS ?=
CFLAGS ?= -std=gnu11 -Wall -Wextra
LDFLAGS ?=
LDLIBS ?=
BUILD_DIR ?= build

.PHONY: test test-allocations clean

test: $(BUILD_DIR)/test_text $(BUILD_DIR)/test_rows
	./$(BUILD_DIR)/test_text
	./$(BUILD_DIR)/test_rows

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/test_text: tests/test_text.c tests/test_locale.h Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. tests/test_text.c Clic.c $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/test_rows: tests/test_rows.c tests/test_locale.h Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. tests/test_rows.c Clic.c $(LDFLAGS) $(LDLIBS) -o $@

test-allocations: $(BUILD_DIR)/test_allocations
	./$(BUILD_DIR)/test_allocations

$(BUILD_DIR)/test_allocations: tests/test_rows.c tests/track_allocations.c tests/test_locale.h Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCLIC_TEST_ALLOCATIONS -I. tests/test_rows.c tests/track_allocations.c Clic.c $(LDFLAGS) -Wl,--wrap=malloc,--wrap=calloc,--wrap=free $(LDLIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)
