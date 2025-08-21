void shutdown() {
    unsigned char reset_value = 0x06;
    __asm__ __volatile__ (
       "outb %0, %1"
       :
       : "a" (reset_value), "d" (0xCF9)
       : "memory"
    );
}

void wait(int ms)
{
    volatile int count;
    while (ms--)
    {
        count = 100000;
        while (count--)
        {
            __asm__("nop");
        }
    }
}
