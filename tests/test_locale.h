#ifndef CLIC_TEST_LOCALE_H
#define CLIC_TEST_LOCALE_H

#include <locale.h>

static void Test_useUtf8Locale(void) {
    const char *locales[] = {"", "C.UTF-8", "en_US.UTF-8", "pt_BR.UTF-8", "UTF-8"};

    for (size_t i = 0; i < sizeof locales / sizeof locales[0]; i++) {
        if (setlocale(LC_CTYPE, locales[i]) && Clic_textWidth("\u00e9") == 1)
            return;
    }

    fprintf(stderr, "The tests require an installed UTF-8 locale.\n");
    exit(1);
}

#endif
