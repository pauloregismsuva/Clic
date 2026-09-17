#include "Clic.h"

static void desenharMenu(const char *opcoes[], int quantidade, int selecionada) {
    for (int i = 0; i < quantidade; i++) {
        Clic_move(3 + i, 3);
        Clic_clearLine();
        if (i == selecionada) {
            Clic_setFontColor(Color_BLACK);
            Clic_setBackgroundColor(Color_YELLOW);
        }
        printf(" %-24s ", opcoes[i]);
        Clic_resetColor();
    }
    fflush(stdout);
}

int main(void) {
    const char *opcoes[] = {"Iniciar", "Instrucoes", "Sair"};
    int quantidade = (int)(sizeof opcoes / sizeof opcoes[0]);
    int selecionada = 0;
    bool confirmou = false;

    if (Clic_getScreenWidth() < 60 || Clic_getScreenHeight() < 18) {
        fprintf(stderr, "Use um terminal com pelo menos 60 colunas e 18 linhas.\n");
        return 1;
    }

    Clic_clearScreen();
    printf("Use cima/baixo e Enter. Pressione q para cancelar.\n");
    Clic_hideCursor();

    for (;;) {
        desenharMenu(opcoes, quantidade, selecionada);
        int tecla = Clic_keyCapture();
        if (tecla == Key_UP) {
            selecionada = (selecionada + quantidade - 1) % quantidade;
        } else if (tecla == Key_DOWN) {
            selecionada = (selecionada + 1) % quantidade;
        } else if (tecla == Key_ENTER) {
            confirmou = true;
            break;
        } else if (tecla == Key_Q || tecla == EOF) {
            break;
        }
    }

    Clic_resetColor();
    Clic_showCursor();
    Clic_move(8, 1);
    if (confirmou) {
        printf("Opcao escolhida: %s\n", opcoes[selecionada]);
    } else {
        printf("Menu cancelado.\n");
    }
    return 0;
}
