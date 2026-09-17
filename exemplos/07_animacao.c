#include "Clic.h"

int main(void) {
    int largura = Clic_getScreenWidth();
    int coluna = 2;
    int passo = 1;

    if (largura < 60 || Clic_getScreenHeight() < 18) {
        fprintf(stderr, "Use um terminal com pelo menos 60 colunas e 18 linhas.\n");
        return 1;
    }
    if (largura > 70) {
        largura = 70;
    }

    Clic_clearScreen();
    printf("Animacao com leitura nao bloqueante. Pressione q para sair.\n");
    Clic_hideCursor();

    for (;;) {
        int tecla = Clic_keyCaptureNonBlocking();
        if (tecla == Key_Q) {
            break;
        }
        if (tecla == EOF) {
            /* EAGAIN/ausencia de entrada pode marcar erro ou EOF em stdin. */
            clearerr(stdin);
        }

        Clic_move(5, 1);
        Clic_clearLine();
        Clic_move(5, coluna);
        Clic_setBackgroundColor(Color_BLUE);
        Clic_printHBlockLine(4);
        Clic_resetColor();
        Clic_pause(0.06f);  /* Segundos; tambem descarrega stdout. */

        coluna += passo;
        if (coluna >= largura - 4 || coluna <= 2) {
            passo = -passo;
        }
    }

    Clic_resetColor();
    Clic_showCursor();
    Clic_move(8, 1);
    printf("Animacao encerrada.\n");
    return 0;
}
