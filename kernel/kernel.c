#include "stdio.h"
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
    const char* message = "HabrOS 0.0.2                                                                    >>> ";
    int nextpos = printl(video_memory, message, 0);
    while(1) {
        unsigned char ch = getchar();
        unsigned char ch_descaned = transform(ch);
        printc(video_memory, ch_descaned, (char *)0x07, nextpos+1); 
        nextpos++;
    }
}
