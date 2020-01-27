#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

// Only summarized 3 system calls for demo
int flag = 1;
int freq[3][NPROC];
char syscalls[3][10] = {
    "getpid",
    "gettime",
    "sleep"
};

void initfreq() {
	int i,j;
	for(i=0; i<3; i++)
        for(j = 0; j < NPROC; j++)
            freq[i][j] = 0;
}

void syscallsummary_start() {
	initfreq();
	flag = 1;
	kprintf("syscall summary start\n");
}

void syscallsummary_stop() {
    flag = 0;
    kprintf("syscall summary stop\n");
}

void show_freq(int pid) {
	int i;
	kprintf("------ Summary of process \"%s\" (pid:%d) ------\n",proctab[pid].pname, pid);
	for(i = 0; i < 3; i++){
		kprintf("process (%s) has been called \t%10d times\n", syscalls[i], freq[i][pid]);
	}
}

void printsyscalls(){
    int i;
	struct  pentry *p;
    for(i = 0; i < NPROC; i++) {
        p = &proctab[i];
        if(i >= 0 && i < NPROC && p->pstate != PRFREE) {
            show_freq(i);
        }
	}
}
