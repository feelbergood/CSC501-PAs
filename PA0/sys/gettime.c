/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <date.h>

extern int getutim(unsigned long *);

extern int flag;
extern int freq[3][NPROC];

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
    /* long	now; */

	/* FIXME -- no getutim */
    if (flag) {
		freq[1][currpid]++;
		kprintf("syscall gettime (called by process %s) %d times!\n", proctab[currpid].pname, freq[1][currpid]);
	}
    return OK;
}
