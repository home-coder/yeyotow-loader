#include "common.h"
#include "regs.h"

/*对于开发板上的NAND Flash是256M的，这个NAND Flash中有2048个块
  每个块由64个页组成，每个页的大小是2048个字节*/

/*用宏定义实现三个命令：
 	DISABLE_NAND 	在不使用NAND芯片的时候关掉片选，防止误操作
	ENABLE_NAND 	在使用NAND芯片的时候打开片选
	WAIT_NAND	检测NAND芯片是否准备好
*/
#define DISABLE_NAND	NFCONT |= 1 << 1
#define ENABLE_NAND	NFCONT &= ~(1 << 1)
#define WAIT_NAND	while(!(NFSTAT & 1))

void nand_init(void)
{
	/*配置GPO和GPP的引脚为NAND所用，参考原理图P7和P3*/
	/*参考S3C6410手册P341*/
	/*注意：在给一个寄存器的某些位赋值时一定要先清除这些位*/
	GPOCON &= ~(0xf);	
	GPOCON |= 0xa;

	GPPCON &= ~(0xffc0);
	GPPCON |= 0xaa80;

	/*设置NAND控制器发出的时序，时序图参考S3C6410手册P247*/
	/*参考NAND芯片作序要的时序，时序图参考K9F2G08手册P20*/
	NFCONF = (0 << 12) | (2 << 8) | (0 << 4) | (1 << 2);

	/*关掉ECC校验，使能NAND控制器，禁止NAND芯片的片选，在使用的时候再打开*/
	NFCONT = (1 << 6) | (1 << 7) | (1 << 1) | (1 << 0);

	/*使NAND芯片重新启动一次，发重启命令即可*/
	/*NAND芯片支持的其他命令参考K9F2G08手册P10*/
	ENABLE_NAND;
	NFCMMD = 0xff;
	WAIT_NAND;
	DISABLE_NAND;
}

/*在读写NAND芯片的时候发送的要读些的地址*/
/*把该地址写到NFADDR寄存器中即可*/
/*分五个周期发给NAND芯片，参考K9F2G08手册P9*/
void send_addr(unsigned int addr)
{	
	unsigned int col_addr = addr & 0x7ff;
	unsigned int raw_addr = (addr >> 11) & 0x1ffff;

	NFADDR = col_addr & 0xff;
	NFADDR = (col_addr >> 8) & 0x7;
	NFADDR = raw_addr & 0xff;
	NFADDR = (raw_addr >> 8) & 0xff;
	NFADDR = (raw_addr >> 16) & 0x1;
}

/*在擦除NAND芯片时发送的要擦除的块的页地址*/
/*发一个页地址过去，NAND芯片就会自动把该页所在块全部擦掉*/
void send_page_addr(unsigned int addr)
{
	unsigned int raw_addr = (addr >> 11) & 0x1ffff;

	NFADDR = raw_addr & 0xff;
        NFADDR = (raw_addr >> 8) & 0xff;
        NFADDR = (raw_addr >> 16) & 0x1;
}

/*NAND芯片的擦除操作，每次擦除一个块*/
/*擦除流程图参考K9F2G08手册P17*/
void nand_erase(unsigned int nand, unsigned int len)
{
	int addr = nand;
	
	ENABLE_NAND;
	for(addr = nand; addr < nand + len; addr += 64 * 2048){
		NFCMMD = 0x60;
		send_page_addr(addr);
		NFCMMD = 0xd0;
		WAIT_NAND;
	}
	DISABLE_NAND;
}

/*NAND芯片的写操作，在这里每次写一个页大小*/
/*写NAND流程图参考K9F2G08手册P16*/
/*把要写的数据写到NFDATA中即可*/
void nand_write(unsigned int ddr, unsigned int nand, unsigned int len)
{
	int addr = nand;
	int i;
	
	ENABLE_NAND;
	for(addr = nand; addr < nand + len; addr += 2048){
		NFCMMD = 0x80;
		send_addr(addr);
		for(i = 0; i < 2048; i += 4, ddr += 4){
			NFDATA = *((volatile unsigned int *)ddr);
		}
		NFCMMD = 0x10;
		WAIT_NAND;
	}
	DISABLE_NAND;
}

/*NAND芯片的读操作，在这里每次读取一个页大小*/
/*读NAND流程图参考K9F2G08手册P16*/
/*发完读命令后直接从NFDATA读取数据即可*/
void nand_read(unsigned int ddr, unsigned int nand, unsigned int len)
{
	int addr = nand;
	int i;

	ENABLE_NAND;
	for(addr = nand; addr < nand + len; addr += 2048){
		NFCMMD = 0x00;
		send_addr(addr);
		NFCMMD = 0x30;
		WAIT_NAND;
		for(i = 0; i < 2048; i += 4, ddr += 4){
			*((volatile unsigned int *)ddr) = NFDATA;
		}
	}	
}

void nand_read_id(unsigned int ddr)
{
	int i;

	ENABLE_NAND;
	NFCMMD = 0x90;
	NFCMMD = 0x00;
	
	for(i = 0; i < 6; i++, ddr += 4){
		*((volatile unsigned int *)ddr) = NFDATA;
	}
}










