print_hex:
    pusha
    
    xor eax, eax
    mov cx, 0xf000
    mov ax, 2
    .while_hex:
        push cx
        and cx, dx
        call _get_char
        mov [HEX_OUT + eax], cl
        inc ax
        pop cx
        shr cx, 4
        cmp cx, 0
        je .endw_hex
        jmp .while_hex
    .endw_hex:
    mov bx, HEX_OUT
    call print

    popa
    ret

    HEX_OUT: db '0x0000', 13, 10, 0

    ; cx - num
    ; cl - out
    _get_char:
        cmp cx, 0x0010
        jc .endw
        .while:
            shr cx, 4
            cmp cx, 0x0010
            jl .endw
            jmp .while
        .endw:
        cmp cx, 0x000a
        jge .alpha
        mov ch, '0'
        add cl, ch
        xor ch, ch
        ret
        .alpha:
            mov ch, 'a'
            sub cl, 0xa
            add cl, ch
            xor ch, ch
            ret
__print_hex_ret: