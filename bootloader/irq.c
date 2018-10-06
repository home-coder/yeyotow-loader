#include <common.h>
#include <irq.h>

void clear_irq(void)
{
	VIC0ADDRESS = 0;
	VIC1ADDRESS = 0;
}

int irq_request(int no, void (*handle) (void))
{
	if (no >= 0 && no <= 31) {
		VIC0VECTADDR[no] = (int)handle;
		VIC0INTSELECT &= ~(1 << no);
		VIC0INTENABLE |= 1 << no;
	} else if (no >= 32 && no <= 63) {
		no -= 32;
		VIC1INTSELECT &= ~(1 << no);
		VIC1VECTADDR[no] = (int)handle;
		VIC1INTENABLE |= 1 << no;
	} else {
		return -1;
	}

	return 0;
}

void irq_enable(void)
{
	__asm__ __volatile__("mrs r0, cpsr\n" "bic r0, r0, #0x80\n" "msr cpsr, r0\n":::"r0");
}

void irq_disable(void)
{
	__asm__ __volatile__("mrs r0, cpsr\n" "orr r0, r0, #0x80\n" "msr cpsr, r0\n":::"r0");
}
