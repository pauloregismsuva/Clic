#include "Clic.h"
#include "test_locale.h"

int main(int argc, char **argv) {
    assert(argc == 2);
    Test_useUtf8Locale();

    if (strcmp(argv[1], "center") == 0) {
        assert(Clic_printCenter("Jo\u00e3o") == 9);
        return 0;
    }
    if (strcmp(argv[1], "center_long") == 0) {
        assert(Clic_printCenter("12345678901234567890\u00e9") == 1);
        return 0;
    }
    if (strcmp(argv[1], "center_invalid") == 0) {
        assert(Clic_printCenter("\xc3") == 0);
        assert(Clic_printCenter(NULL) == 0);
        return 0;
    }

    Table *table = NULL;
    int width = 0;
    if (strcmp(argv[1], "accents") == 0) {
        table = Table_create("%-s %s");
        assert(table != NULL);
        width = 22;
        Table_addRow(table, "Ana", "A\u00e7\u00e3o");
        Table_addRow(table, "Jo\u00e3o", "Sa\u00fade");
    } else if (strcmp(argv[1], "truncate") == 0) {
        table = Table_create("%-s %s");
        assert(table != NULL);
        width = 14;
        Table_addRow(table, "Jo\u00e3o Alves", "\u754c\u754c\u754c");
        Table_addRow(table, "e\u0301e\u0301e\u0301e\u0301e\u0301e\u0301", "12345");
    } else if (strcmp(argv[1], "wide_gap") == 0) {
        table = Table_create("%-s %s");
        assert(table != NULL);
        width = 12;
        Table_addRow(table, "\u754c\u754c\u754c", "\u754c\u754c\u754c");
    } else if (strcmp(argv[1], "custom") == 0) {
        table = Table_create("%12s %-s");
        assert(table != NULL);
        width = 24;
        Table_addRow(table, "Jo\u00e3o", "Sa\u00fade");
        assert(strcmp(table->firstRow->cell[0].content, "Jo\u00e3o") == 0);
    } else if (strcmp(argv[1], "all_custom") == 0) {
        table = Table_create("%-4s %4s");
        assert(table != NULL);
        width = 20;
        Table_addRow(table, "Jo\u00e3o", "\u00e9");
    } else if (strcmp(argv[1], "narrow_custom") == 0) {
        table = Table_create("%-12s %-12s");
        assert(table != NULL);
        width = 9;
        Table_addRow(table, "A\u00e7\u00e3o longa", "ignorado");
    } else if (strcmp(argv[1], "too_narrow") == 0) {
        table = Table_create("%s %s");
        assert(table != NULL);
        width = 3;
        Table_addRow(table, "a", "b");
    } else if (strcmp(argv[1], "long") == 0) {
        char longText[801];
        for (int i = 0; i < 400; i++) memcpy(longText + 2 * i, "\u00e9", 2);
        longText[800] = '\0';
        table = Table_create("%-s");
        assert(table != NULL);
        width = 650;
        Table_addRow(table, longText);
    } else {
        assert(strcmp(argv[1], "one") == 0 || strcmp(argv[1], "zero") == 0 ||
               strcmp(argv[1], "ascii_locale") == 0);
        table = Table_create("%-s");
        assert(table != NULL);
        width = strcmp(argv[1], "zero") == 0 ? 3 : 4;
        if (strcmp(argv[1], "ascii_locale") == 0) {
            assert(setlocale(LC_CTYPE, "C") != NULL);
            Table_addRow(table, "long");
        } else {
            Table_addRow(table, "\u00e9\u00e9");
        }
    }

    /* A fixed width makes rendering reproducible even with redirected stdout. */
    table->width = width;
    Table_print(table);
    Table_free(table);
    return 0;
}
