#include "Clic.h"
#include "test_locale.h"

#ifdef CLIC_TEST_ALLOCATIONS
extern size_t Test_outstandingAllocations;
#endif

int main(void) {
    Test_useUtf8Locale();

    char longText[803];
    memset(longText, 'a', 298);
    memcpy(longText + 298, "\u00e7", 2);
    memset(longText + 300, 'b', 500);
    memcpy(longText + 800, "\u00e3", 3);

    Table *table = Table_create("%-s %d %8.2f");
    assert(table != NULL);
    Table_addRow(table, longText, 42, 3.5);
    assert(table->nRows == 1);
    assert(strcmp(table->firstRow->cell[0].content, longText) == 0);
    assert(strlen(table->firstRow->cell[0].content) == 802);
    assert(Clic_textWidth(table->firstRow->cell[0].content) == 800);
    assert(strcmp(table->firstRow->cell[1].content, "42") == 0);
    assert(strcmp(table->firstRow->cell[2].content, "    3.50") == 0);
    longText[0] = 'x';
    assert(table->firstRow->cell[0].content[0] == 'a');
    Table_free(table);

    table = Table_create("%s %s");
    assert(table != NULL);
    Table_addRow(table, "valid", "\xc3");
    assert(table->nRows == 0 && table->firstRow == NULL);
    Table_addRow(table, "valid", (char *)NULL);
    assert(table->nRows == 0);
    Table_addRow(table, "valid", "contains\ttab");
    assert(table->nRows == 0);
    Table_addRow(table, "Jo\u00e3o", "A\u00e7\u00e3o");
    assert(table->nRows == 1 && table->firstRow->index == 1);
    Table_free(table);

    /* printf string precision counts bytes: reject an incomplete character. */
    table = Table_create("%.1s %s");
    assert(table != NULL);
    Table_addRow(table, "\u00e9", "unused");
    assert(table->nRows == 0);
    Table_free(table);

    const char *unsupported[] = {"%ld", "%u", "%*s", "%ls"};
    for (size_t i = 0; i < sizeof unsupported / sizeof unsupported[0]; i++) {
        table = Table_create((char *)unsupported[i]);
        assert(table != NULL);
        /* Rejection must happen before trying to consume a mismatched value. */
        Table_addRow(table);
        assert(table->nRows == 0);
        Table_free(table);
    }

    for (int i = 0; i < 100; i++) {
        table = Table_create("%s");
        assert(table != NULL);
        Table_addRow(table, "Sa\u00fade");
        assert(table->nRows == 1);
        Table_free(table);
    }
    Table_free(NULL);

#ifdef CLIC_TEST_ALLOCATIONS
    assert(Test_outstandingAllocations == 0);
#endif

    puts("cell storage: OK (invalid-input diagnostics above are expected)");
    return 0;
}
