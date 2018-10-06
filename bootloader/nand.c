#include <common.h>
#include <nand.h>

#if 0
flash-- nand-- nor
    nand-- > device--->256 M
    device = 2 K blocks
    block = 64 pages
    page = 2 K bytes
    � ��写一次最大一个页擦除必须以块为单位[p1][oob 64 bytes][p2][oob 64 bytes][p3][oob 64 bytes][p4][oob 64 bytes]

    buf == sizeof(page)

    ctrl-- -- -- -- -- -- -- -->nand xxxCMD xxxADDR xxxDATA � ��路图：引脚链接
#endif
void nand_select(int select)
{
	if (select)
		NFCONT &= ~(1 << 1);
	else
		NFCONT |= 1 << 1;
}

void nand_if_busy(void)
{
	while (!(NFSTAT & 0x1)) ;
}

void nand_init(void)
{
	//gpio gpo0  gpp3-7
	GPOCON &= ~3;
	GPOCON |= 2;
	GPPCON &= ~(0x3ff << 6);
	GPPCON |= 0xaa80;
	//属性
	NFCONF = (0 << 12) | (2 << 8) | (1 << 4) | (1 << 2);
	NFCONT = 1 | (1 << 1);
	//reset
	nand_select(1);
	NFCMMD = 0xff;
	nand_if_busy();
	nand_select(0);
}

void send_block_addr(unsigned int addr)
{
	unsigned int raw_addr = (addr >> 11) & 0x1ffff;	//列一共17个一

	NFADDR = raw_addr & 0xff;
	NFADDR = (raw_addr >> 8) & 0xff;
	NFADDR = (raw_addr >> 16) & 1;
}

void send_addr(unsigned int addr)
{
	unsigned int col_addr = addr & 0x7ff;	//行一共11个一
	unsigned int raw_addr = (addr >> 11) & 0x1ffff;	//列一共17个一

	NFADDR = col_addr & 0xff;	//看nand的手册 有发送规则
	NFADDR = (col_addr >> 8) & 0x7;
	NFADDR = raw_addr & 0xff;
	NFADDR = (raw_addr >> 8) & 0xff;
	NFADDR = (raw_addr >> 16) & 1;
}

//read as page
//linux block driver sector(page)
void nand_read(unsigned int ddr, unsigned int nand, unsigned int lenth)
{
	unsigned int addr;
	int i;

	nand_select(1);
	for (addr = nand; addr < nand + lenth; addr += 2048) {
		NFCMMD = 0x00;
		send_addr(addr);
		NFCMMD = 0x30;
		nand_if_busy();
		for (i = 0; i < 512; i++, ddr += 4)
			*(volatile unsigned int *)ddr = NFDATA;
	}
	nand_select(0);
}

void nand_write(unsigned int ddr, unsigned int nand, unsigned int lenth)
{
	unsigned int addr;
	int i;

	nand_select(1);
	for (addr = nand; addr < nand + lenth; addr += 2048) {
		NFCMMD = 0x80;
		send_addr(addr);
		for (i = 0; i < 512; i++, ddr += 4)
			NFDATA = *(volatile unsigned int *)ddr;
		NFCMMD = 0x10;
		nand_if_busy();
	}
	nand_select(0);
}

void nand_erase(unsigned int nand, unsigned int lenth)
{
	unsigned int addr;

	nand_select(1);
	for (addr = nand; addr < nand + lenth; addr += 64 * 2048) {
		NFCMMD = 0x60;
		send_block_addr(addr);
		NFCMMD = 0xd0;
		nand_if_busy();
	}
	nand_select(0);
}

void nand_id(char *s)
{
	int i;

	nand_select(1);
	NFCMMD = 0x90;
	NFADDR = 0x00;
	nand_if_busy();

#define ID_DATA (*(volatile char *)0x70200010)

	for (i = 0; i < 5; i++)
		s[i] = ID_DATA;

	nand_select(0);
}
