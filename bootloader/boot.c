#include <common.h>
#include <clock.h>
#include <ddr.h>
#include <nand.h>
#include <led.h>
#include <dm9000.h>

void armboot(void)
{
	clock_init();
	led_init();
	led_on(0);

	ddr_init();
	nand_init();
	eth_init();

	nand_read(0x57000000, 0x0, 0x40000);

	__asm__ __volatile__(
			"mov sp, #0x58000000\n"
		   	"ldr pc, =main_boot\n"
	);
}
