; boot/loader.s
[org 0x7C00]
[bits 16]

KERNEL_OFFSET equ 0x1000 ; Адрес ядра в памяти (4 КБ после загрузчика)

start:
    cli
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    call load_kernel
    call enable_pm
    jmp 0x08:KERNEL_OFFSET ; 0x08 = селектор кода из GDT

; Загружаем ядро с диска (секторы 2-16)
load_kernel:
    mov bx, KERNEL_OFFSET
    mov ah, 0x02      ; BIOS: read sectors
    mov al, 15        ; Читаем 15 секторов (7.5 КБ)
    mov ch, 0         ; Cylinder 0
    mov cl, 2         ; Sector 2 (после загрузчика)
    mov dh, 0         ; Head 0
    int 0x13
    jc .error
    ret
.error:
    jmp $

; Переход в защищённый режим
enable_pm:
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    ret

; GDT (минимальная, без ошибок)
gdt:
    dq 0x0                         ; Null descriptor
code_segment:
    dw 0xFFFF                      ; Limit (low 16 bits)
    dw 0x0000                      ; Base (low 16 bits)
    db 0x00                        ; Base (middle 8 bits)
    db 10011010b                   ; Flags: code, 32-bit
    db 11001111b                   ; Flags: limit (high 4 bits), granularity
    db 0x00                        ; Base (high 8 bits)
data_segment:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b                   ; Flags: data
    db 11001111b
    db 0x00

gdt_descriptor:
    dw gdt_descriptor - gdt - 1    ; Size of GDT
    dd gdt                         ; Address of GDT

times 510 - ($ - $$) db 0
dw 0xAA55
