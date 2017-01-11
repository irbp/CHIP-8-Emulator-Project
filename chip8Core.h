//35 opcodes, which are all two bytes long
unsigned short opcode;

//8-CHIP system has 4Kb memory in total (4096 bytes)
unsigned char memory[4096];

//16 8-bit data registers
unsigned char V[16];

//16-bit register (for memory access)
unsigned short I;

//program counter
unsigned short pc;

//graphic memory (64 x 32 pixels - 2048 pixels)
unsigned char gfx[64*32];

//two timer registers (60 Hz)
unsigned char delay_timer;
unsigned char sound_timer;

//stack of 16 levels (return to the caller routine)
unsigned char stack[16];

//stack pointer
unsigned char sp;

//8-CHIP keypad
unsigned char keys[16];
