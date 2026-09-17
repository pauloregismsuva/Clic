#include "Clic.h"

int main(void) {
    if (Clic_getScreenWidth() < 60 || Clic_getScreenHeight() < 18) {
        fprintf(stderr, "Use um terminal com pelo menos 60 colunas e 18 linhas.\n");
        return 1;
    }

    /* As duas primeiras colunas ficam com largura automatica. */
    Table *tabela = Table_create("%-s %d %8.2f");
    if (tabela == NULL) {
        return 1;
    }
    Table_setWidth(tabela, 56);
    Table_addRow(tabela, "Caneta", 3, 2.50);
    Table_addRow(tabela, "Caderno", 2, 18.90);
    Table_addRow(tabela, "Borracha", 1, 1.75);

    Clic_clearScreen();
    printf("Produto | Quantidade | Valor unitario (R$)\n");
    Clic_move(3, 1);
    Table_print(tabela);
    Clic_move(11, 1);

    /* Veja na documentacao a limitacao de Table_free nesta versao. */
    Table_free(tabela);
    return 0;
}
