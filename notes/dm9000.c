#include "common.h"
#include "regs.h"

void dm9000_write(unsigned char reg, unsigned char val);
unsigned char dm9000_read(unsigned char reg);

void eth_init(void)
{
	/*把srom的数据位宽设置为16位*/
	/*参考S3C6410手册P209*/
	SROM_BW |= 1 << 4;

	/*给PHY芯片上电，参考网卡应用手册P22*/
	dm9000_write(0x1f, 0x00);
	/*使网卡软复位，即回复网卡内寄存器的默认值*/
	dm9000_write(0x00, 0x01);
	/*清除软复位*/
	dm9000_write(0x00, 0x00);
	/*网卡应用手册P22之第三步与软复位合为一步*/
	/*使能发送和接收中断，并打开指针归位功能*/
	dm9000_write(0xff, (1 << 7) | (1 << 0) | (1 << 1));
	/*写入MAC地址*/
	dm9000_write(0x10, 0x11);
	dm9000_write(0x11, 0x22);
	dm9000_write(0x12, 0x33);
	dm9000_write(0x13, 0x44);
	dm9000_write(0x14, 0x55);
	dm9000_write(0x15, 0x66);	
	/*清除发送完成标记位*/
	dm9000_write(0x01, (1 << 2) | (1 << 3) | (1 <<5));
	/*使能发送*/
	dm9000_write(0x05, 0x01);
}

int eth_recv(char *buf)
{	
	unsigned short status;
	unsigned short lenth;
	int i;
	
	/*判断是否收到数据，参考网卡应用手册P30*/
	/*如果没收到数据则直接返回-1报错*/
	if(!(dm9000_read(0xfe) & 0x01)){
		return -1;	
	}
	/*根据手册假读一次，不然可能出错*/
	dm9000_read(0xf0);
	/*读以下网卡的SRAM，判断是否有包收到，没有收到则返回-1报错*/
	/*参考网卡芯片手册P30的数据格式（图）和P31*/
	if((dm9000_read(0xf0) & 0x03) != 0x01){
		return -1;
	}
	/*判断网卡工作在16位还是8位模式下，如果是8位模式返回报错*/
	if(dm9000_read(0xfe) & 0x80){
		return -1;
	}
	
	/*把0xf2索引发给网卡，让网卡得知要读数据，且读数据后指针要增加*/
	/*注意：SRAM的地址我们直接操作不到*/
	*((volatile unsigned char *)DM9000_ADDR) = 0xf2;
	/*读取收到的数据的长度和状态*/
	status = *((volatile unsigned short *)DM9000_DATA);
	lenth = *((volatile unsigned short *)DM9000_DATA);

	/*把收到的数据放到buf中，这个数据是网线上传送的数据，带协议头的*/
	for(i = 0; i < ((lenth + 1) >> 1); i++){
		((volatile unsigned short *)buf)[i] = *((volatile unsigned short *)DM9000_DATA);
	}
	/*清除接收标记位*/
	dm9000_write(0xfe, 0x01);

	/*返回数据的长度*/
	return lenth;
}

void dm9000_write(unsigned char reg, unsigned char val)
{
	/*把索引发到网卡*/
	*((volatile unsigned char *)DM9000_ADDR) = reg;
	/*把数据发到上一次索引指向的寄存器*/
	/*只要不发送索引去覆盖上一次的索引，则上一次的索引一直有效，直到被覆盖*/
	*((volatile unsigned char *)DM9000_DATA) = val;
}

unsigned char dm9000_read(unsigned char reg)
{
	/*把索引发到网卡*/
	*((volatile unsigned char *)DM9000_ADDR) = reg;
	/*把读到的数据返回*/
	return *((volatile unsigned char *)DM9000_DATA);
}
