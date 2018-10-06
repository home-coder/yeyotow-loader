#include <common.h>
#include <cmd.h>
#include <uart.h>
#include <setup.h>
#include <irq.h>
#include <lib.h>

void bootm(unsigned int addr)
{
	void (*kernel) (unsigned int zero, unsigned int type, unsigned int list) = (void *)addr;
	unsigned int *list = (void *)0x50000100;
	char *bootargs = "root=/dev/nfs mem=128M nfsroot=192.168.1.111:/nfsroot ip=192.168.1.20 console=ttySAC0,115200";	//page==4K---struct page

	uprintf("kernel is booting..........\n");

	*list = 2;
	*(list + 1) = ATAG_CORE;

	list += *list;
	*list = ((strlen(bootargs) + 1 + 3) >> 2) + 2;
	*(list + 1) = ATAG_CMDLINE;
	strcpy((void *)(list + 2), bootargs);

	list += *list;
	*list = 0;
	*(list + 1) = ATAG_NONE;

	//irq mmu cache dma
	irq_disable();
	//r0=0 r1=1626 r2=0x50000100
	//bl kernel
	kernel(0, 1626, 0x50000100);	//call kernel
}
