#include <stdio.h>
#include "chip8Core.h"

chip8Core myChip8;

int main() {
    if (myChip8.loadRom("TETRIS")) {
        printf ("ok\n");
    }
    else {
        printf ("deu ruim\n");
    }

    return 0;
}