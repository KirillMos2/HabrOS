void printc(char* video_memory, char symb, char* color, int charpos) {
    video_memory[charpos * 2] = symb;
    video_memory[charpos * 2 + 1] = color;
}
int printl(char* video_memory, const char* text, int startpos) {
    int nextpos;
    for (int i = 0;; i++) {
        if (text[i] == '\0') {break;}
        printc(video_memory, text[i], (char*)0x07, startpos);
        startpos++;
        nextpos = startpos;
    }
    return nextpos;
}

static inline unsigned char inb(unsigned short port) {
    unsigned char data;
    __asm__ volatile ("inb %1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

unsigned char getchar(void) {
    unsigned char scancode;
    do {
        scancode = inb(0x64);
        scancode &= 1;
    } while (!scancode);
    
    unsigned char scan = inb(0x60);
    if (scan & 0x80) {
        return 0;
    }
    return scan;
}

unsigned char transform(unsigned char ch) {
    unsigned char codes[] = {(unsigned char)0x01, (unsigned char)0x02, (unsigned char)0x03, (unsigned char)0x04, (unsigned char)0x05, (unsigned char)0x06, (unsigned char)0x07, (unsigned char)0x08, (unsigned char)0x09, (unsigned char)0x0A, (unsigned char)0x0B};
    unsigned char symbols[] = {'', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    int status = 0;
    for (int i = 0; i != (sizeof(codes) / sizeof(char)); i++) {
        if (ch == codes[i]) {
            status = 1;
            return symbols[i];
        }
    }
    if (status == 0) {
        return ch;
    }
}
