/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

extern int flag;
extern int freq[3][NPROC];


/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL getpid()
{
	if (flag) {
		freq[0][currpid]++;
		kprintf("syscall getpid (called by process %s) %d times!\n", proctab[currpid].pname, freq[0][currpid]);
	}
	return(currpid);
}
