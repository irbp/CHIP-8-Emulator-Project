#include <stdio.h>
#include "chip8Core.h"
#include <allegro.h>
#include <time.h>

#define ASPECT_RATIO 10

chip8Core chip8;

BITMAP *buffer;
RGB BLACK = {0, 0, 0};
RGB WHITE = {255, 255, 255};

volatile int exit_program = FALSE;
void exitEmulator() {
    exit_program = TRUE;
}
END_OF_FUNCTION(exitEmulator);

volatile int timer;
void timeIncrement() {
    timer++;
}
END_OF_FUNCTION(timeIncrement);

int mark;

void setupVideo(char * game) {
    allegro_init();
    install_timer();
    install_keyboard();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 64*ASPECT_RATIO, 32*ASPECT_RATIO, 0, 0);
    set_window_title(game);

    LOCK_VARIABLE(exit_program);
    LOCK_FUNCTION(exitEmulator);
    set_close_button_callback(exitEmulator);

    timer = 0;
    LOCK_FUNCTION(timeIncrement);
    LOCK_VARIABLE(timer);
    install_int_ex(timeIncrement, MSEC_TO_TIMER(1));

    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    clear(buffer);
}
END_OF_FUNCTION(setupVideo);

void drawSprite() {
    for (int i = 0; i < SCREEN_W; i++) {
        for (int j = 0; j < SCREEN_H; j++) {
            if (chip8.gfx[(i / ASPECT_RATIO) + ((j / ASPECT_RATIO) * 64)]) {
                putpixel(buffer, i, j, makecol(255, 255, 255));
            }
        }
    }

    mark = timer;
    draw_sprite(screen, buffer, 0, 0);
    clear(buffer);
    while (timer - mark <= 5);
    mark = timer;
}
END_OF_FUNCTION(drawSprite);

void verifyKey() {
    if (key[KEY_1]) {
        chip8.keys[0x1] = 1;
    }
    if (key[KEY_2]) {
        chip8.keys[0x1] = 1;
    }
    if (key[KEY_3]) {
        chip8.keys[0x2] = 1;
    }
    if (key[KEY_4]) {
        chip8.keys[0xC] = 1;
    }
    if (key[KEY_Q]) {
        chip8.keys[0x4] = 1;
    }
    if (key[KEY_W]) {
        chip8.keys[0x5] = 1;
    }
    if (key[KEY_E]) {
        chip8.keys[0x6] = 1;
    }
    if (key[KEY_R]) {
        chip8.keys[0xD] = 1;
    }
    if (key[KEY_A]) {
        chip8.keys[0x7] = 1;
    }
    if (key[KEY_S]) {
        chip8.keys[0x8] = 1;
    }
    if (key[KEY_D]) {
        chip8.keys[0x9] = 1;
    }
    if (key[KEY_F]) {
        chip8.keys[0xE] = 1;
    }
    if (key[KEY_Z]) {
        chip8.keys[0xA] = 1;
    }
    if (key[KEY_X]) {
        chip8.keys[0x0] = 1;
    }
    if (key[KEY_C]) {
        chip8.keys[0xB] = 1;
    }
    if (key[KEY_V]) {
        chip8.keys[0xF] = 1;
    }
}

void resetKeys() {
    for (int i = 0; i < 16; i++) {
        chip8.keys[i] = 0;
    }
}

int main(int argc, char **argv) {
    SAMPLE *beep = load_sample("beep.wav");
    
    setupVideo(argv[1]); //setup the video emulator

    //load the rom into the memory of CHIP-8
    if (!chip8.loadRom(argv[1])) {
        printf ("Error: Couldn't find %s!\n", argv[1]);
        return -1;
    }

    while(!exit_program) {
        chip8.emulateCycle(); //execute one cycle of the emulation
        if (chip8.drawFlag) {
            drawSprite();
            chip8.drawFlag = false;
        }

        if (chip8.beep) {
            play_sample(beep, 255, 128, 1000, FALSE);
            chip8.beep = false;
        }
        resetKeys();
        verifyKey();
    }

    destroy_bitmap(buffer);

    return 0;
}
END_OF_MAIN();