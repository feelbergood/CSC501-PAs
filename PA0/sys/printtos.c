#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

unsigned long *esp;
unsigned long *ebp;

void printtos() {
    kprintf("\n======printtos======");
    asm("movl %esp, esp");
    asm("movl %ebp, ebp");
    kprintf("\ncurrent: 0x%x\nbefore entry: 0x%x\nafter entry: 0x%x", ebp, ebp+2, esp);
    kprintf("\n------Contents of upto four stack locations below the top of the stack------");
    int i;
    for (i=0; i<4; i++) {
        kprintf("\nlocation %d, address: 0x%x, content: 0x%x", i, ebp-i, *(ebp-i));
    }
}