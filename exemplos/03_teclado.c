#include "Clic.h"

int main(void) {
    if (Clic_getScreenWidth() < 60 || Clic_getScreenHeight() < 18) {
        fprintf(stderr, "Use um terminal com pelo menos 60 colunas e 18 linhas.\n");
        return 1;
    }

    Clic_clearScreen();
    printf("Pressione as setas. Use q minusculo para sair.\n");
    Clic_hideCursor();

    for (;;) {
        fflush(stdout);  /* Exibe a interface antes de esperar uma tecla. */
        int tecla = Clic_keyCapture();
        if (tecla == Key_Q || tecla == EOF) {
            break;
        }

        Clic_move(3, 1);
        Clic_clearLine();
        switch (tecla) {
            case Key_UP:    printf("Direcao: cima"); break;
            case Key_DOWN:  printf("Direcao: baixo"); break;
            case Key_LEFT:  printf("Direcao: esquerda"); break;
            case Key_RIGHT: printf("Direcao: direita"); break;
            default:       printf("Codigo recebido: %d", tecla); break;
        }
    }

    Clic_resetColor();
    Clic_showCursor();
    Clic_move(5, 1);
    return 0;
}
