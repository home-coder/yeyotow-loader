#include <common.h>
#include <led.h>

void led_init(void)
{
	//gpm0-3 out
	GPMCON &= ~0xffff;
	GPMCON |= 0x1111;
	//gpm0-3 1 off
	GPMDAT |= 0xf;
}

//0-3
void led_on(int n)
{
	if (n > 3 || n < 0)
		return;
	GPMDAT &= ~(1 << n);
}

//0-3
void led_off(int n)
{
	if (n > 3 || n < 0)
		return;
	GPMDAT |= 1 << n;
}
