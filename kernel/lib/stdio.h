int crspos = 0;
char* vm = (char*)0xB8000;

void move_screen() {
    for (int i = 0; i != 80*24*2; i++) {
        vm[i] = vm[i+80];
    }
    crspos = crspos - 80;
}

void move_cursor_inputing(int w) {
    switch (w){
        case 1:
            if ((crspos / 80 * 80 + 4) < crspos) {crspos--;}
            else {return;}
        case 2:
            if (vm[crspos*2] != '\0') {crspos++;}
            else {return;}
    }
}

void printc(char symb, char* color) {
    if (symb == '\a') {beep();}
    else if (symb == '\n') {
        crspos = crspos+80;
    }
    else if (symb == '\r') {
        crspos = crspos / 80 * 80;
    }
    else if (symb == '\t') {
        crspos = crspos + 4;
    }
    else if (symb == (char)0x08) {
        crspos--;
        printc(' ', (char*)0x07);
        crspos--;
    }
    else {
        vm[crspos * 2] = symb;
        vm[crspos * 2 + 1] = color;
        crspos++;
    }
    if (crspos > 80*25) {
        move_screen();
    }
}
void print(const char* text, char* color) {
    for (int i = 0;; i++) {
        if (text[i] == '\0') {break;}
        printc(text[i], color);
    }
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
    unsigned char codes[] = {
    (unsigned char)0x01, (unsigned char)0x02, (unsigned char)0x03,
    (unsigned char)0x04, (unsigned char)0x05, (unsigned char)0x06, 
    (unsigned char)0x07, (unsigned char)0x08, (unsigned char)0x09,
    (unsigned char)0x0A, (unsigned char)0x0B, (unsigned char)0x0C,
    (unsigned char)0x0D, (unsigned char)0x0E, (unsigned char)0x0F,
    (unsigned char)0x10, (unsigned char)0x11, (unsigned char)0x12,
    (unsigned char)0x13, (unsigned char)0x14, (unsigned char)0x15,
    (unsigned char)0x16, (unsigned char)0x17, (unsigned char)0x18,
    (unsigned char)0x19, (unsigned char)0x1A, (unsigned char)0x1B,
    (unsigned char)0x1C, (unsigned char)0x1D, (unsigned char)0x1E,
    (unsigned char)0x1F, (unsigned char)0x20, (unsigned char)0x21,
    (unsigned char)0x22, (unsigned char)0x23, (unsigned char)0x24,
    (unsigned char)0x25, (unsigned char)0x26, (unsigned char)0x27,
    (unsigned char)0x28, (unsigned char)0x29, (unsigned char)0x2A,
    (unsigned char)0x2B, (unsigned char)0x2C, (unsigned char)0x2D,
    (unsigned char)0x2E, (unsigned char)0x2F, (unsigned char)0x30,
    (unsigned char)0x31, (unsigned char)0x32, (unsigned char)0x33,
    (unsigned char)0x34, (unsigned char)0x35, (unsigned char)0x36,
    (unsigned char)0x37, (unsigned char)0x38, (unsigned char)0x39,
    (unsigned char)0x3A, (unsigned char)0x3B, (unsigned char)0x3C,
    (unsigned char)0x3D, (unsigned char)0x3E, (unsigned char)0x3F,
    (unsigned char)0x40, (unsigned char)0x41, (unsigned char)0x42,
    (unsigned char)0x43, (unsigned char)0x44, (unsigned char)0x45,
    (unsigned char)0x46, (unsigned char)0x47, (unsigned char)0x48,
    (unsigned char)0x49, (unsigned char)0x4A, (unsigned char)0x4B,
    (unsigned char)0x4C, (unsigned char)0x4D, (unsigned char)0x4E,
    (unsigned char)0x4F, (unsigned char)0x50, (unsigned char)0x51,
    (unsigned char)0x52, (unsigned char)0x53, (unsigned char)0x57,
    (unsigned char)0x58
    };
    
    unsigned char symbols_standart[] = {
    (unsigned char)0x00, '1', '2', // 0x00 - null (not bind)
    '3', '4', '5',
    '6', '7', '8',
    '9', '0', '-',
    '=', (unsigned char)0x08, (unsigned char)0x09,
    'q', 'w', 'e',
    'r', 't', 'y',
    'u', 'i', 'o',
    'p', '[', ']',
    (unsigned char)0x0A, (unsigned char)0x01, 'a', //0x01 - l ctrl
    's', 'd', 'f',
    'g', 'h', 'j',
    'k', 'l', ';',
    '\'', '`', (unsigned char)0x02, //0x02 - l shift
    '\\', 'z', 'x',
    'c', 'v', 'b',
    'n', 'm', ',',
    '.', '/', (unsigned char)0x00,
    '*', (unsigned char)0x00, ' ',
    (unsigned char)0x03, (unsigned char)0x00, (unsigned char)0x00, //0x03 - caps
    (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
    (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
    (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
    (unsigned char)0x00, '7', '8',
    '9', '-', '4',
    '5', '6', '+',
    '1', '2', '3',
    '0', '.', (unsigned char)0x00,
    (unsigned char)0x00
    };
    int status = 0;
    for (int i = 0; i != (sizeof(codes) / sizeof(char)); i++) {
        if (ch == codes[i]) {
            status = 1;
            return symbols_standart[i];
        }
    }
    if (status == 0) {
        return ch;
    }
}

void write_logo() {
    print("\n\n\n\n\n\n\n\n\r        /$$   /$$           /$$                  /$$$$$$   /$$$$$$\n\r", (char*)0x03);
    print("\t\t| $$  | $$          | $$                 /$$__  $$ /$$__  $$\n\r", (char*)0x03);
    print("\t\t| $$  | $$  /$$$$$$ | $$$$$$$   /$$$$$$ | $$  \\ $$| $$  \\__/\n\r", (char*)0x03);
    print("\t\t| $$$$$$$$ |____  $$| $$__  $$ /$$__  $$| $$  | $$|  $$$$$$\n\r", (char*)0x03);
    print("\t\t| $$__  $$  /$$$$$$$| $$  \\ $$| $$  \\__/| $$  | $$ \\____  $$\n\r", (char*)0x03);
    print("\t\t| $$  | $$ /$$__  $$| $$  | $$| $$      | $$  | $$ /$$  \\ $$\n\r", (char*)0x03);
    print("\t\t| $$  | $$|  $$$$$$$| $$$$$$$/| $$      |  $$$$$$/|  $$$$$$/\n\r", (char*)0x03);
    print("\t\t|__/  |__/ \\_______/|_______/ |__/       \\______/  \\______/\n\n\r", (char*)0x03);
    print("\t\t\t\t\t  ", (char*)0x07);
    print(version, (char*)0x07);
    print(" loading...", (char*)0x07);
}

void clear() {
    for (int i = 0; i != 80*25*2; i++) {
        vm[i] = '\0';
        i++;
        vm[i] = (char*)0x07;
    }
    crspos = 0;
}
