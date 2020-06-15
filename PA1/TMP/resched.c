/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lab1.h>
// #include <time.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);

int globalschedclass = -1;
int epoch = 0;

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
		int isnewepoch = 0;
		int newepochval = 0;
		int currmaxgoodness = 0;
		int nextproctorun = 0;

		epoch = epoch - (proctab[currpid].counter - preempt);
		if (epoch <= 0 || currpid == NULLPROC) {
			isnewepoch = 1;
		}
		proctab[currpid].counter = preempt;

		struct pentry* procp = &proctab[0];
		int i;
		for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
			if ((procp+i)->pstate != PRFREE) {
				if (isnewepoch == 1 || (procp+i)->isnew == 0) {
					if (isnewepoch == 1) {
						// New epoch
						if ((procp+i)->quantum == (procp+i)->counter) {
							(procp+i)->quantum = (procp+i)->pprio;
						} else {
							(procp+i)->quantum = ((procp+i)->counter)/2 + (procp+i)->pprio;
						}
						(procp+i)->counter = (procp+i)->quantum;
						(procp+i)->goodness = (procp+i)->counter + (procp+i)->pprio;
						(procp+i)->isnew = 0;
						(procp+i)->oldprio = (procp+i)->pprio;
						newepochval += (procp+i)->quantum;
					} else {
						if ((procp+i)->counter == 0) {
							(procp+i)->goodness = 0;
						} else {
							(procp+i)->goodness = (procp+i)->counter + (procp+i)->pprio;
						}	
					}
					if ((procp+i)->goodness > 0 &&
						(procp+i)->goodness > currmaxgoodness &&
						((procp+i)->pstate == PRCURR || (procp+i)->pstate == PRREADY)) {
							currmaxgoodness = (procp+i)->goodness;
							nextproctorun = i;
						}
				}
			}
		}

		if (isnewepoch == 1) {
			epoch = newepochval;
		}

		optr= &proctab[currpid];
		// Force context switch
		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}
		dequeue(nextproctorun);
		nptr = &proctab[currpid = nextproctorun];
		nptr->pstate = PRCURR;		/* mark it currently running	*/
	#ifdef	RTCLOCK
		if (nextproctorun == NULLPROC) {
			preempt = QUANTUM;		/* reset preemption counter	*/
		} else {
			preempt = nptr->counter;
		}
	#endif
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
	struct pentry* procp = &proctab[0];
	int sumpprio = 0;
	int i;
	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ((procp+i)->pstate == PRCURR || (procp+i)->pstate == PRREADY) {
			sumpprio += (procp+i)->pprio;
		}
	}

	// srand(time(NULL));
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
