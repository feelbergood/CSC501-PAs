#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

static unsigned long    *esp;

void printprocstks(int priority) {
    struct pentry   *p;
    int i;
    kprintf("======printprocstks======\n");
    for (i=0; i<NPROC; i++) {
        p = &proctab[i];
        if (p->pprio > priority) {
            // print the stack base, stack size, stacklimit, and stack pointer. 
            if (p->pstate == PRCURR) {
                asm("movl %esp, esp");
                kprintf("---currently executing process---\n");
                kprintf("stack pointer: 0x%x\n", esp);
                kprintf("stack pointer: 0x%d\n", esp - p->pbase);
            } else {
                kprintf("---saved process---\n");
                kprintf("stack pointer: 0x%x\n", p->pesp);
                kprintf("stack size: 0x%d\n", p->pstklen);
            }
            kprintf("stack base: 0x%x\n", p->pbase);
            kprintf("stack limit: 0x%x\n", p->plimit);
            // Also, for each process, include the process name, the process id and the process priority.
            kprintf("process name: %s\n", p->pname);
            kprintf("process id: %d\n", currpid);
            kprintf("process priority: %d\n\n", p->pprio);
        }
    }
}
