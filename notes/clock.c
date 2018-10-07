#include "common.h"
#include "regs.h"

/*配置ARMCLK = 533MHZ
      PCLK = 66MHZ
      HCLK = 133MHZ
*/
/*参考S3C6410 P125的图*/
void clock_init(void)
{
	/*把波形锁在PLL中倍频，等频率稳定后输出*/
	APLL_LOCK = 0xffff;//参考S3C6410 P141
	/*配置APLL的倍频系数*/
	APLL_CON = (1 << 31) | (266 << 16) | (3 << 8) | (1 << 0);//参考S3C6410 P141

	OTHERS |= 1 << 6;//参考S3C6410 P169
	MISC_CON &= ~(1 << 19);//参考S3C6410 P168
	CLK_DIV0 = (0 << 0) | (1 << 9) | (1 << 8) | (3 << 12);////参考S3C6410 P147
	HCLK_GATE = ~0; //参考S3C6410 P150
	PCLK_GATE = ~0;//参考S3C6410 P150

	CLK_SRC |= 1;//参考S3C6410 P145
}
