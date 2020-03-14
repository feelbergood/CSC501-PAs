/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
int main()
{
	kprintf("\n\nHello World, Xinu lives\n\n");
	kprintf("Zfunction(0xaabbccdd) = 0x%lX\n",zfunction(0xaabbccdd));
	printsegaddress();
	printtos();
	return 0;
}
