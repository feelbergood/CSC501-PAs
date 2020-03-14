.global zfunction
zfunction:
    push %ebp
    mov %esp, %ebp

    mov %eax, [%ebp + 8]
    and 0xff803fff, %eax
    shl 0x4, %eax
    
    pop %ebp

    ret