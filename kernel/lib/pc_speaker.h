void play_sound(unsigned int nFrequence) {
	unsigned int Div;
	unsigned char tmp;

	Div = 1193180 / nFrequence;
	outb((unsigned short)0x43, 0xb6);
	outb((unsigned short)0x42, (unsigned char) (Div) );
	outb((unsigned short)0x42, (unsigned char) (Div >> 8));

	tmp = inb(0x61);
 	if (tmp != (tmp | 3)) {
		outb(0x61, tmp | 3);
	}
}
 
void nosound() {
 	unsigned char tmp = inb((unsigned short)0x61) & 0xFC;
     
 	outb((unsigned short)0x61, tmp);
}

void beep() {
 	 play_sound(1000);
 	 wait(10);
 	 nosound();
}
