// NCSU VCL linux environment is 64-bit, and push/pop with 32-bit registers like ebp/esp/eax is not supported
// https://stackoverflow.com/questions/43435764/64-bit-mode-do-not-support-push-and-pop-instructions
// Therefore we do not put zfunction in the Makefile in this lab
.global zfunction
zfunction:
    push %ebp
    mov %esp, %ebp

    mov %eax, [%ebp + 8]
    and 0xff803fff, %eax
    shl 0x4, %eax
    
    pop %ebp

    ret