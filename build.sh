nasm -f bin boot/loader.s -o bin/loader.bin &&
i686-elf-gcc -c kernel/kernel.c -o bin/kernel.o -ffreestanding -m32 -I kernel/lib&&
i686-elf-ld -T boot/linker.ld -o bin/kernel.bin bin/kernel.o &&
cat bin/loader.bin bin/kernel.bin > bin/os.bin &&
cp bin/kernel.bin iso/boot/kernel.bin &&
grub-mkrescue -o bin/os.iso iso &&
echo "build succsess"
