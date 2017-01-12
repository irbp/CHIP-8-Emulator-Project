//
// Created by italo on 11/01/2017.
//

#include "chip8Core.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;


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

chip8Core::chip8Core() {

}

chip8Core::~chip8Core() {

}

//Debug stuff
void chip8Core::printa() {
    printf ("Hello, World");

    return;
}

//Initialize the system
void chip8Core::init() {
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
        memory[i] = 0;
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

void chip8Core::emulateCycle() {
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

        case 0x8000:
            switch (opcode & 0x000F)
            {
                case 0x0000: //(0x8XY0 - Sets VX to the value of VY)
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0001: //(0x8XY1 - Sets VX to VX or VY)
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0002: //(0x8XY2 - Sets VX to VX and VY)
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0003: //(0x8XY3 - Sets VX to VX xor VY)
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0004: //(0x8XY4 - Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't)
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) {
                        V[0xF] = 1;
                    }
                    else {
                        V[0xF] = 0;
                    }
                    pc += 2;
                break;
                
                case 0x0005: //(0x8XY5 - VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't)
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) {
                        V[0xF] = 0;
                    }
                    else {
                        V[0xF] = 1;
                    }
                    pc += 2;
                break;

                case 0x0006: //(0x8XY6 - Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.)
                   V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                   V[(opcode & 0x0F00) >> 8] >>= 1;
                   pc += 2;
                break;

                case 0x0007: //(0x8XY7 - Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't)
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    if (V[(opcode & 0x00F0) >> 4] < V[(opcode & 0x0F00) >> 8]) {
                        V[0xF] = 0;
                    }
                    else {
                        V[0xF] = 1;
                    }
                    pc += 2;
                break;

                case 0x000E: //(0x8XYE - Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift)
                    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                break;

                default:
                    printf ("Opcode error!\n");
            }
        break;

        case 0x9000: //(0x9XY0 - Skips the next instruction if VX doesn't equal VY)
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
                pc += 4;
            }
            else {
                pc += 2;
            }
        break;

        case 0xA000: //(0xANNN - Sets I to the address NNN)
            I = opcode & 0x0FFF;
            pc += 2;
        break;

        case 0xB000: //(0xBNNN - Jumps to the address NNN plus V0)
            pc = (opcode & 0x0FFF) + V[0x0];
        break;

        case 0xC000: //(0xCXNN - Sets VX to the result of a bitwise and operation on a random number and NN)
            V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
            pc += 2;
        break;

        case 0xD000: //(0xDXYN - Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels)
        {
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short N = opcode & 0x000F;
            unsigned short pixel;

            V[0xF] = 0;

            for (int i = 0; i < N; i++) {
                pixel = memory[I + i];
                for (int j = 0; j < 8; j++) {
                    if ((pixel & (0x80 >> j)) != 0) {
                        if (gfx[x + j + ((y + i) * 64)] == 1) {
                            V[0xF] = 1;
                        }
                        gfx[x + j + ((y + i) * 64)] ^= 1;
                    }
                }
            }

            drawFlag = true;
            pc += 2;
        }
        break;

        case 0xE000:
            switch (opcode & 0x000F)
            {
                case 0x000E: //(0xEX9E - Skips the next instruction if the key stored in VX is pressed)
                    if (keys[V[(opcode & 0x0F00) >> 8]] != 0) {
                        pc += 4;
                    }
                    else {
                        pc += 2;
                    }
                break;

                case 0x0001: //(0xEXA1 - Skips the next instruction if the key stored in VX isn't pressed)
                    if (keys[V[(opcode & 0x0F00) >> 8]] == 0) {
                        pc += 4;
                    }
                    else {
                        pc += 2;
                    }
                break;

                default:
                    printf ("Opcode error!\n");                
            }
        break;

        case 0xF000:
            switch (opcode & 0x00FF)
            {
                case 0x0007: //(0xFX07 - Sets VX to the value of the delay timer)
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                break;

                case 0x000A: //(0xFX0A - A key press is awaited, and then stored in VX)
                {
                    bool pressed = false;

                    for (int i = 0; i < 16; i++) {
                        if (keys[i] != 0) {
                            V[(opcode & 0x0F00) >> 8] = i;
                            pressed = true;
                            i = 16;
                        }
                    }

                    if (!pressed) {
                        return;
                    }

                    pc += 2;
                }
                break;

                case 0x0015: //(0xFX15 - Sets the delay timer to VX)
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;

                case 0x0018: //(0xFX18 - Sets the sound timer to VX)
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;

                case 0x001E: //(0xFX1E - Adds VX to I)
                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;

                case 0x0029: //(0xFX29 - Sets I to the location of the sprite for the character in VX)
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc += 2;
                break;

                case 0x0033: //(0xFX33)
                    memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I+1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I+2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
                    pc += 2;
                break;

                case 0x0055: //(0xFX55 - Stores V0 to VX in memory starting at address I)
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                        memory[I+i] = V[i];
                    }

                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                break;

                case 0x0065: //(0xFX65 - Fills V0 to VX with values from memory starting at address I)
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                        V[i] = memory[I+i];
                    }

                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                break;

                default:
                    printf ("Opcode error!\n");
            }
        break;

        default:
            printf ("Opcode error!\n");
    }

    //Timer update
    if (delay_timer > 0) {
        --delay_timer;
    }

    if (sound_timer > 0) {
        if (sound_timer == 1) {
            
        }
        --sound_timer;
    }
}

//Loading the game rom into the memory
bool chip8Core::loadRom(const char * filename) {
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

    return true;
}
