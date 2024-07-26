[org 0x7c00] ; bootloader offset
KERNEL_OFFSET equ 0x1000
[global _start]
_start:

    mov [BOOT_DRIVE], dl
    mov bp, 0x9000 ; set the stack
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print ; This will be written after the BIOS messages


    call load_kernel
    call switch_to_pm
    jmp $ ; this will actually never be executed

%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/gdt.asm"
%include "boot/disk.asm"
%include "boot/print_pm.asm"
%include "boot/32bit-switch.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 32                      ; sectors to load for the kernel code
                                    ; every sector is 512bytes
                                    ; increment this number for every time you change this
                                    ; > > > inc = 4 
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM: ; after the switch we will get here
    mov ebx, MSG_LOAD
    call print_pm ; Note that this will be written at the top left corner
    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE: db 0
MSG_REAL_MODE: db "Started in 16-bit real mode", 13, 10, 0
MSG_LOAD: db "T <<< this should be X", 0


; bootsector
times 510-($-$$) db 0
dw 0xaa55
