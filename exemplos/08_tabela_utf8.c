#include "Clic.h"
#include <locale.h>

int main(void) {
    if (!setlocale(LC_CTYPE, "") || Clic_textWidth("Ação") != 4) {
        fprintf(stderr, "Configure uma localidade UTF-8 antes de executar.\n");
        return 1;
    }
    if (Clic_getScreenWidth() < 60 || Clic_getScreenHeight() < 18) {
        fprintf(stderr, "Use um terminal com pelo menos 60 colunas e 18 linhas.\n");
        return 1;
    }

    Table *table = Table_create("%-14s %-s %6.2f");
    if (!table) return 1;

    Table_setWidth(table, 58);
    Table_addRow(table, "João", "Programação", 8.50);
    Table_addRow(table, "Márcia", "Saúde e educação", 9.00);
    Table_addRow(table, "Érica", "Ação comunitária e participação social", 9.25);
    Table_addRow(table, "Lívia", "e\u0301: acento combinante", 7.50);
    Table_addRow(table, "界界", "Caracteres de duas colunas", 8.00);

    Clic_clearScreen();
    Clic_printCenter("Tabela com acentuação e UTF-8");
    Clic_move(2, 1);
    printf("Nome | Atividade | Nota");
    Clic_move(4, 1);
    Table_print(table);
    Clic_move(16, 1);
    printf("O texto completo é preservado; o corte ocorre na exibição.\n");

    Table_free(table);
    Clic_resetColor();
    Clic_showCursor();
    return 0;
}
