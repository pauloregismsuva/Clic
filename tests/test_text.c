#include "Clic.h"
#include "test_locale.h"

int main(void) {
    Test_useUtf8Locale();

    assert(Clic_textWidth("") == 0);
    assert(Clic_textWidth("Ana") == 3);
    assert(Clic_textWidth("Jo\u00e3o") == 4);
    assert(Clic_textWidth("A\u00e7\u00e3o") == 4);
    assert(Clic_textWidth("\u754c") == 2);
    assert(Clic_textWidth("e\u0301") == 1);
    assert(Clic_textWidth("\u2026") == 1);
    assert(Clic_textWidth(NULL) == -1);
    assert(Clic_textWidth("\xc3") == -1);
    assert(Clic_textWidth("\xc3(") == -1);
    assert(Clic_textWidth("\xc0\xaf") == -1);
    assert(Clic_textWidth("a\tb") == -1);
    assert(Clic_textWidth("a\nb") == -1);
    assert(Clic_textWidth("\033[31mred") == -1);

    assert(setlocale(LC_CTYPE, "C") != NULL);
    assert(Clic_textWidth("ASCII") == 5);
    assert(Clic_textWidth("\u00e9") == -1);
    Test_useUtf8Locale();
    assert(Clic_textWidth("Jo\u00e3o") == 4);

    puts("text width: OK");
    return 0;
}
