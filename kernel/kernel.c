#include "types.h"
#include "asm_methods.h"
#include "syscalls.h"
#include "pc_speaker.h"
#include "stdio.h"
#include "string.h"
#define MULTIBOOT_MAGIC 0x1BADB002
#define MULTIBOOT_FLAGS 0

typedef struct {
    ui32 magic;
    ui32 flags;
    ui32 checksum;
} __attribute__((packed)) multiboot_header_t;

multiboot_header_t multiboot_header = {
    MULTIBOOT_MAGIC,
    MULTIBOOT_FLAGS,
    -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)
};

unsigned char stack[4096] __attribute__((aligned(16)));
unsigned char* stack_top = stack + sizeof(stack);
/*
unsigned char heap[8] = {
    (unsigned char*)stack_top+1, (unsigned char*)stack_top+2,
    (unsigned char*)stack_top+3, (unsigned char*)stack_top+4,
    (unsigned char*)stack_top+5, (unsigned char*)stack_top+6,
    (unsigned char*)stack_top+7, (unsigned char*)stack_top+8
};
*/

void _start() {
    clear();
    beep();
    write_logo();
    wait(5000);
    clear();
    __asm__ volatile ("mov %0, %%esp" : : "r" (stack_top));
    print(version, (char*)0x07);
    print("\n\rSource code you may see on Github: ", (char*)0x07);
    print("https://github.com/KirillMos2/HabrOS\n\r", (char*)0x01);
    print("For view all comamnds write ", (char*)0x07);
    print("help\n", (char*)0x02);
    unsigned char command[70];
    unsigned char ch;
    unsigned char ch_dec;
    const char* history[10];
    int history_point = 0;
    int i = 0;
    while(1) {
        print("\n\r", (char*)0x07);
        print(">>> ", (char*)0x07);
        do {
            ch = getchar();
            if (ch == (unsigned char)0x00) {continue;}
            ch_dec = transform(ch);
            if (ch_dec == (unsigned char)0x00) {continue;}
            if (ch_dec == (unsigned char)0x0A) {
                printc('\n', (char*)0x07);
                printc('\r', (char*)0x07);
                break;
            }
            printc(ch_dec, (char*)0x07);
            if (ch_dec == (unsigned char)0x08) {
                command[i-1] = '\0';
                i--;
            }
            else {
                command[i] = ch_dec;
                i++;
            } 
        } while (ch_dec != (unsigned char)0x0A);
        command[i] = '\0';
        if (strequ((const char*)command, "shutdown")) {
            shutdown();
        }
        else if (strequ((const char*)command, "version")) {
            print(version, (char*)0x07);
        }
        else if (strstart((const char*)command, "echo ")) {
            for (int j = 5; j != (sizeof(command)/sizeof(unsigned char)); j++) {
                printc((char)command[j], (char*)0x07);
            }
        }
        else if (strequ((const char*)command, "ascii")) {
            for (int h = 0; h != 256; h++) {
                printc((char)h, (char*)0x07);
            }
        }
        else if (strequ((const char*)command, "help")) {
            print("help - this message\n\rversion - view version\n\rascii - view all ASCII symbols\n\recho <text> - echo text\n\rshutdown - shutdown PC", (char*)0x07);
        }
        else if (command[0] == '\0') {
            i++;
            i--;
        }
        else {
            print("Uwnkown command ", (char*)0x07);
            print((const char*)command, (char*)0x07);
        }
        i = 0;
        for (int g = 0; g!=70; g++) {
            command[g] = '\0';
        }
        ch_dec = (unsigned char)0x00;
    }
}
