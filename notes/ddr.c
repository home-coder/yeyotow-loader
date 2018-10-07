#include "common.h"
#include "regs.h"

void ddr_time(void);
void ddr_cmd(void);

void ddr_init(void)
{
	/*设置数据线data[26:16]为ddr所用，参考核心板电路图*/
	MEM_SYS_CFG &= ~(1 << 7);//该寄存器参考S3C6410手册160页

	/*设置ddr控制器进入配置模式*/
	P1MEMCCMD = 4;//S3C6410 P193
	/*配置ddr控制器的属性*/
	P1MEMCFG = (2 << 15) | (2 << 3) | (2 << 0);//S3C6410 P193
	P1MEMCFG2 = (1 << 11) | (3 << 8) | (1 << 6) | (1 << 1) | (1 << 0);//S3C6410 P201

	/*配置ddr控制器的时许，以满足ddr芯片*/
	ddr_time();
	/*由ddr控制器给ddr芯片发送命令，参考S3C6410 P192*/
	ddr_cmd();

	/*设置ddr控制器进入运行模式*/
	P1MEMCCMD = 0;

	/*检查ddr控制器配置是否成功*/
	while((P1MEMSTAT & 3) != 1);
}

/*把纳秒转换为HCLK下的时钟数*/
unsigned int ns_to_cycle(unsigned int ns)
{
	return ns / (1000000000 / 133000000);
}

/*配置ddr控制器的时许以满足ddr芯片*/
/*参考S3C6410 P196-200*/
/*参考ddr芯片手册P13*/
void ddr_time(void)
{
	P1REFRESH = ns_to_cycle(7800);//ddr芯片手册第一页
	P1CASLAT = 3 << 1;
	P1T_DQSS = 1;
	P1T_MRD = 2;
	P1T_RAS = ns_to_cycle(50);
	P1T_RC = ns_to_cycle(70);
	P1T_RCD = ns_to_cycle(25) | ((ns_to_cycle(25) - 3) << 3);
	P1T_RFC = ns_to_cycle(80) | ((ns_to_cycle(80) - 3) << 5);
	P1T_RP = ns_to_cycle(25) | ((ns_to_cycle(25) - 3) << 3);
	P1T_RRD = ns_to_cycle(20);
	P1T_WR = ns_to_cycle(20);
	P1T_WTR = 1;//对应tCDLR 
	P1T_XP = 2;//对应tPDEX 
	P1T_XSR = ns_to_cycle(120);
//	P1T_ESR = ns_to_cycle(120);//不存在
}	

/*由ddr控制器给ddr芯片发送命令*/
/*参考S3C6410 P192*/
void ddr_cmd(void)
{
	P1DIRECTCMD = 2 << 18;
	P1DIRECTCMD = 0 << 18;
	P1DIRECTCMD = 3 << 18;
	P1DIRECTCMD = 3 << 18;
	P1DIRECTCMD = (2 << 18) | (2 << 16);//再参考ddr芯片手册 P8
	P1DIRECTCMD = (2 << 18) | (3 << 4) | (2 << 0);//再参考ddr芯片手册 P6
} 
