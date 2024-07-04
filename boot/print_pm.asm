;; THIS FILE SHOULD ONLY RUN IN PROTECTED MODE !
[bits 32]

;; consts
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_pm:
    pusha
    mov edx, VIDEO_MEMORY
    
    .print_pm_loop:
        mov al, [ebx]
        mov ah, WHITE_ON_BLACK

        cmp al, 0
        je .print_pm_end

        mov [edx], ax
        add ebx, 1
        add edx, 2
        jmp .print_pm_loop
        
    .print_pm_end:
    popa
    ret