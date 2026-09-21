#include "Clic.h"

int main(void) {
    char *opcoes[] = {"Iniciar", "Instrucoes", "Sair"};
    setvbuf(stdout, NULL, _IONBF, 0);  /* Atualiza cada redesenho imediatamente. */

    if (Clic_getScreenWidth() < 60 || Clic_getScreenHeight() < 18) {
        fprintf(stderr, "Use um terminal com pelo menos 60 colunas e 18 linhas.\n");
        return 1;
    }

    Table *menu = Table_create("%-s");
    if (menu == NULL) {
        return 1;
    }
    Table_setWidth(menu, 40);
    for (int i = 0; i < 3; i++) {
        Table_addRow(menu, opcoes[i]);
    }
    menu->highlightColor = Color_CIAN;
    menu->highlightedIndex = 1;

    Clic_clearScreen();
    printf("Use cima/baixo e Enter. Backspace pode enviar codigo 127.\n");
    Clic_hideCursor();
    /* Table_select desenha e espera a tecla internamente. */
    fflush(stdout);
    Clic_move(3, 1);
    int escolha = Table_select(menu);

    Clic_resetColor();
    Clic_showCursor();
    Clic_move(11, 1);
    if (escolha >= 1 && escolha <= menu->nRows) {
        printf("Opcao escolhida: %s\n", opcoes[escolha - 1]);
    } else if (escolha < 0) {
        printf("Solicitacao de exclusao da linha %d. Nada foi removido.\n", -escolha);
    } else {
        printf("Nenhuma linha valida foi selecionada.\n");
    }

    /* Libera a tabela, as linhas, os formatos e os textos das celulas. */
    Table_free(menu);
    return 0;
}
