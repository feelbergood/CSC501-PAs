/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lab1.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);

int globalschedclass = -1;

LOCAL int getrandompid();

void setschedclass(int sched_class) 
{
	globalschedclass = sched_class;
}

int getschedclass()
{
	return globalschedclass;
}

/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	int schedclass = getschedclass();

	if (schedclass == RANDOMSCHED) 
	{
		optr= &proctab[currpid];
	
		/* force context switch */

		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}

		/* remove highest priority process at end of ready list */
		int randompid = getrandompid();
		nptr = &proctab[currpid = randompid];
		nptr->pstate = PRCURR;		/* mark it currently running	*/
	#ifdef	RTCLOCK
		preempt = QUANTUM;		/* reset preemption counter	*/
	#endif
	}
	else if (schedclass == LINUXSCHED)
	{
		
	}
	else 
	{
		/* no switch needed if current process priority higher than next*/

		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
		(lastkey(rdytail)<optr->pprio)) {
			return(OK);
		}
		
		/* force context switch */

		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}

		/* remove highest priority process at end of ready list */

		nptr = &proctab[ (currpid = getlast(rdytail)) ];
		nptr->pstate = PRCURR;		/* mark it currently running	*/
	#ifdef	RTCLOCK
		preempt = QUANTUM;		/* reset preemption counter	*/
	#endif
	}
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}

/*------------------------------------------------------------------------
 * getrandompid  --  obtain a random process id with random scheduler
 *------------------------------------------------------------------------
 */
LOCAL int getrandompid()
{
	int	pid;			/* process id to return		*/
	struct pentry* procp = &proctab[0];
	int sumpprio = 0;
	int i;
	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ((procp+i)->pstate == PRCURR || (procp+i)->pstate == PRREADY) {
			sumpprio += (procp+i)->pprio;
		}
	}

	srand(time(NULL));
	int randnum = rand();
	if (sumpprio != 0) {
		randnum %= sumpprio;
	} else {
		randnum = 0;
	}
	int proc;
	proc = q[rdytail].qprev;
	while (q[proc].qkey < randnum)
	{
		randnum -= q[proc].qkey;
		proc = q[proc].qprev;
	}
	if (proc<NPROC) {
		return dequeue(proc);
	} else {
		return EMPTY;
	}
}
