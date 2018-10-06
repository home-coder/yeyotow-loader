#include <common.h>
#include <clock.h>

#if 0
ARMCLK == 533 M PCLK == 66 M HCLK == 133 M Ê ó∂ÈíüÊ∫ê-- -xM Ê ô∂ÊåØ-- -- -12 M OM0-- -- -0 S3C6410 P123 clk_get clk_enable
#endif
void clock_init(void)
{
	APLL_LOCK = 0xffff;
	APLL_CON = (1 << 31) | (266 << 16) | (3 << 8) | 1;
	OTHERS |= 1 << 6;
	MISC_CON &= ~(1 << 19);
	CLK_DIV0 = 0 | (1 << 9) | (1 << 8) | (3 << 12);

	HCLK_GATE = ~0;
	PCLK_GATE = ~0;

	CLK_SRC |= 1;
}
