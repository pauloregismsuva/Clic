CC ?= cc
CPPFLAGS ?=
CFLAGS ?= -std=gnu11 -Wall -Wextra
LDFLAGS ?=
LDLIBS ?=
BUILD_DIR ?= build

.PHONY: test clean

test: $(BUILD_DIR)/test_text
	./$(BUILD_DIR)/test_text

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/test_text: tests/test_text.c tests/test_locale.h Clic.c Clic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. tests/test_text.c Clic.c $(LDFLAGS) $(LDLIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)
