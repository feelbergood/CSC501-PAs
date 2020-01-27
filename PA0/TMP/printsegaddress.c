#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include "lab0.h"

extern int etext, edata, *end;

void printsegaddress() {
    kprintf("======printsegaddress======\n");
    // end of text segment
    kprintf("End of text segment address: 0x%08x\n", &etext-2);
    // end of data segment
    kprintf("End of data segment address: 0x%08x\n", &edata-2);
    // end of bss segment
    kprintf("End of bss segment address: 0x%08x\n", &end-2);
    // preceding end of text segment
    kprintf("Preceding end of text segment content: 0x%08x\n", *(&etext-1));
    // preceding end of data segment
    kprintf("Preceding end of data segment content: 0x%08x\n", *(&edata-1));
    // preceding end of bss segment
    kprintf("Preceding end of bss segment content: 0x%08x\n", *(&end-1));
    // after end of text segment
    kprintf("After end of text segment content: 0x%08x\n", etext);
    // after end of data segment
    kprintf("After end of data segment content: 0x%08x\n", edata);
    // after end of bss segment
    kprintf("After end of bss segment content: 0x%08x\n", end);
}
