#include "Clic.h"

int main(void) {
    if (Clic_getScreenWidth() < 60 || Clic_getScreenHeight() < 18) {
        fprintf(stderr, "Use um terminal com pelo menos 60 colunas e 18 linhas.\n");
        return 1;
    }

    Clic_clearScreen();
    Clic_move(2, 3);  /* Primeiro a linha; depois a coluna. */
    Clic_setFontColor(Color_BLUE);
    Clic_printBox(46, 7);
    Clic_resetColor();

    Clic_move(3, 5);
    printf("Minha primeira interface com Clic");
    Clic_move(5, 5);
    Clic_setBackgroundColor(Color_GREEN);
    Clic_printHBlockLine(18);  /* Espacos preenchidos pela cor de fundo. */
    Clic_resetColor();
    Clic_move(7, 5);
    Clic_printSymbol(Symbol_ARROW);
    printf(" Desenho em coordenadas do terminal");

    Clic_move(11, 1);
    Clic_showCursor();
    return 0;
}
