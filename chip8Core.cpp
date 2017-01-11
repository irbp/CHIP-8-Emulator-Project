//
// Created by italo on 11/01/2017.
//

#include "chip8Core.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned char chip8_fontSet[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

chip8::chip8() {

}

chip8::~chip8() {

}

//Initialize the system
void chip8::init() {
    pc = 0x200; //Start program counter address
    opcode = 0; //Reset opcode
    I = 0; //Reset index register
    sp = 0; //Reset stack pointer

    //Clear the graphic memory
    for (int i = 0; i < 2048; i++) {
        gfx[i] = 0;
    }

    //Clear the stack
    for (int i = 0; i < 16; i++) {
        stack[i] = 0;
    }

    //Clear the memory
    for (int i = 0; i < 4096; i++) {
        stack[i] = 0;
    }

    //Clear the registers
    for (int i = 0; i < 16; i++) {
        V[i] = 0;
    }

    //Load the fontset to the memory
    for (int i = 0; i < 80; i++) {
        memory[i] = chip8_fontSet[i];
    }

    //Reset timers counters
    delay_timer = 0;
    sound_timer = 0;

    drawFlag = true;

    srand(time(NULL));
}

void chip8::emulateCycle() {
    //Fetch opcode
    opcode  = memory[pc] << 8 | memory[pc + 1];

    //Decode opcode
    //All opcodes are from the oficial specs of the CHIP-8
    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x000F)
            {
                case 0x0000: //(0x00E0 - Clears the screen)
                    for (int i = 0; i < 2048; i++) {
                        gfx[i] = 0x0;
                    }
                    drawFlag = true;
                    pc += 2;
                break;

                case 0x000E: //(0x00EE - Returns from a subroutine)
                    --sp;
                    pc = stack[sp];
                    pc += 2;
                break;

                default:
                    printf ("Opcode error!\n");
            }
        break;

        case 0x1000: //(0x1NNN - Jumps to address NNN)
            pc = opcode & 0x0FFF;
        break;

        case 0x2000: //(0x2NNN - Calls subroutine at NNN)
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
        break;

        case 0x3000: //(0x3XNN - Skips the next instruction if VX equals NN)
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc += 4;
            }
            else {
                pc += 2;
            }
        break;

        case 0x4000: //(0x4XNN - Skips the next instruction if VX doesn't equal NN)
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                pc += 4;
            }
            else {
                pc += 2;
            }
        break;

        case 0x5000: //(0x5XY0 - Skips the next instruction if VX equals VY)
            if (V[(opcode & 0x0F00) >> 8] == V[opcode & 0x00F0 >> 4]) {
                pc += 4;
            }
            else {
                pc += 2;
            }
        break;

        case 0x6000: //(0x6XNN - Sets VX to NN)
            V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            pc += 2;
        break;

        case 0x7000: //(0x7XNN - Adds NN to NX)
            V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            pc += 2;
        break;

    }

    //Execute opcode
}

//Loading the game rom into de memory
bool chip8::loadRom(const char * filename) {
    init();

    FILE * pFile;

    //Open file
    pFile = fopen(filename, "rb");

    //Verifying error
    if (pFile == NULL) {
        printf ("File error!\n");
        return false;
    }

    //Check file size
    fseek(pFile, 0, SEEK_END);
    long fSize = ftell(pFile);
    rewind(pFile);
    printf ("Filesize: %d\n", (int)fSize);

    //Allocate memory (buffer)
    char * buffer  = (char*)malloc(sizeof(char) * fSize);
    if (buffer == NULL) {
        printf ("Memory allocation error!\n");
        return false;
    }

    //Copy the file into the buffer
    size_t result = fread (buffer, 1, fSize, pFile);
    if (result != fSize) {
        printf ("Reading error!\n");
        return false;
    }

    //Copy buffer into CHIP-8 memory
    if ((4096 - 512) > fSize) {
        for (int i = 0; i < fSize; i++) {
            memory[i + 512] = buffer[i];
        }
    }
    else {
        printf ("Error: The selected ROM is too big for CHIP-8 memory!\n");
    }

    fclose(pFile);
    free(buffer);

}