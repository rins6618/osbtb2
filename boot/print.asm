print:
    pusha

    mov ah, 0x0e
    .loop:
        mov al, [bx]
        cmp al, 0
        je .end

    .then:
        int 10h
        inc bx
        jmp .loop

    .end:

    popa
    ret
__print_ret: