#include <ddr.h>
#include <common.h>

#if 0
SRAM DRAM SDRAM == SYNC DRAM MSDDR == MOBILE SYNC DDR DRAM K4X5xxx sam � ��路图引脚链接
#endif
    unsigned int ns_to_clk(unsigned int ns) {
	return ns / (1000000000 / 133000000);
}

void ddr_init(void)
{
	MEM_SYS_CFG &= ~(1 << 7);

	//1、进入配置模式
	P1MEMCCMD = 4;
	//2、配置属性（位宽，片数， 类型...）
	P1MEMCFG = (1 << 21) | (2 << 15) | (2 << 3) | (2);
	P1MEMCFG2 |= (1 << 11) | (3 << 8) | (1 << 6) | (3);
	//3、配置时序
	P1REFRESH = ns_to_clk(7800);
	P1CASLAT = 3 << 1;
	P1T_DQSS = 1;
	P1T_MRD = 2;
	P1T_RAS = ns_to_clk(50);
	P1T_RC = ns_to_clk(70);
	P1T_RCD = ns_to_clk(25) | ((ns_to_clk(25) - 3) << 3);
	P1T_RFC = ns_to_clk(80) | ((ns_to_clk(80) - 3) << 5);
	P1T_RP = ns_to_clk(25) | ((ns_to_clk(25) - 3) << 3);
	P1T_RRD = ns_to_clk(25);
	P1T_WR = ns_to_clk(25);
	P1T_WTR = 1;
	P1T_XP = 2;
	P1T_XSR = ns_to_clk(120);
	//P1T_ESR
	//4、发送命令--->ddr_reg
	P1DIRECTCMD = 2 << 18;
	P1DIRECTCMD = 0 << 18;
	P1DIRECTCMD = 3 << 18;
	P1DIRECTCMD = 3 << 18;
	P1DIRECTCMD = (2 << 18) | (2 << 16);
	P1DIRECTCMD = (2 << 18) | (0 << 16) | (3 << 4) | 2;
	//5、进入运行模式
	P1MEMCCMD = 0;
	//6、检测运行是否正确
	while ((P1MEMSTAT & 3) != 1) ;
}
