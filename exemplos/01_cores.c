#include "Clic.h"

int main(void) {
    Clic_setFontColor(Color_GREEN);
    printf("Operacao concluida.\n");

    Clic_setFontColor(Color_BLACK);
    Clic_setBackgroundColor(Color_YELLOW);
    printf(" Atencao: confira os dados. ");
    Clic_resetColor();
    Clic_breakLine();

    printf("Texto com as cores padrao do terminal.\n");
    return 0;
}
