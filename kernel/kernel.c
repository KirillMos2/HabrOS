#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "types.h"

#define MULTIBOOT_MAGIC 0x1BADB002
#define MULTIBOOT_FLAGS 0

typedef struct {
    u32 magic;
    u32 flags;
    u32 checksum;
} __attribute__((packed)) multiboot_header_t;

multiboot_header_t multiboot_header = {
    MULTIBOOT_MAGIC,
    MULTIBOOT_FLAGS,
    -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)
};

unsigned char stack[4096] __attribute__((aligned(16)));
unsigned char* stack_top = stack + sizeof(stack);

void _start() {
    __asm__ volatile ("mov %0, %%esp" : : "r" (stack_top));

    char* video_memory = (char*)0xB8000;
    const char* message = "HabrOS 0.0.2         \n";
    int nextpos = printl(video_memory, message, 0);
    unsigned char command[70];
    unsigned char ch;
    unsigned char ch_dec;
    int i = 0;
    while(1) {
        nextpos = printl(video_memory, ">>> ", nextpos);
        do {
            ch = getchar();
            ch_dec = transform(ch);
            if (ch_dec == (unsigned char)0x00) {continue;}
            if (ch_dec == (unsigned char)0x0A) {
                nextpos = (nextpos/80+1)*80-1;
                printc(video_memory, ' ', (char*)0x07, nextpos);
                break;
            }
            printc(video_memory, ch_dec, (char*)0x07, nextpos++);
            command[i] = ch_dec;
            i++;
        } while (ch_dec != (unsigned char)0x0A);
        command[i+1] = '\0';
        if (strequ((const char*)command, (const char*)"11")) {
            shutdown();
        }
        if (strequ((const char*)command, (const char*)"12")) {
            nextpos = printl(video_memory, version, nextpos++);
        }
        i = 0;
    }
}
