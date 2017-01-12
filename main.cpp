#include <stdio.h>
#include "chip8Core.h"

chip8Core myChip8;

int main() {
    if (myChip8.loadRom("TETRIS.c8")) {
        printf ("ok\n");
    }
    else {
        printf ("deu ruim\n");
    }

    myChip8.emulateCycle();

    return 0;
}