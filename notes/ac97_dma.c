#include "common.h"
#include "regs.h"

struct{
	unsigned int sour_addr;
	unsigned int dest_addr;
	unsigned int next;
	unsigned int control0;
	unsigned int control1;
}next_dma;

void delay(int n);

void ac97_init(void)
{
	GPDCON = 0x44444;

	AC_GLBCTRL |= 1;
	delay(1);
	AC_GLBCTRL &= ~1;

	AC_GLBCTRL |= 1 << 1;
	delay(1);
	AC_GLBCTRL &= ~(1 << 1);

	AC_GLBCTRL |= 1 << 2;
	delay(1);
	AC_GLBCTRL |= 1 << 3;

	AC_CODEC_CMD |= 1 << 23;
	delay(10);
	if((AC_GLBSTAT & 0x7) == 3){
		printf("AC97 is active...\n");
	}else{
		printf("errno is %d\n", AC_GLBSTAT & 0x7);
	}
}

void wm9714_write(unsigned int reg, unsigned int val)
{
	AC_CODEC_CMD = (reg << 16) | (val);
	delay(1);
}

void wm9714_init(void)
{
	wm9714_write(0x0c, (0xf << 8) | (0xf));
	wm9714_write(0x5c, 1 << 7);
	wm9714_write(0x2a, 1);
	wm9714_write(0x2c , 0xac44);
	wm9714_write(0x1c, (2 << 4) | (2 << 6) | (3 << 8) | (3 << 11));
	wm9714_write(0x04, 0);
	wm9714_write(0x02, 0);
	wm9714_write(0x24, 1 << 4);
	wm9714_write(0x5a, 0 << 6);

	wm9714_write(0x26, 0);
	wm9714_write(0x3c, 0);
	wm9714_write(0x3e, 0);
}

void ac97_dma_init(unsigned int addr, unsigned int len)
{
	next_dma.sour_addr = addr;
	next_dma.dest_addr = (unsigned int)(&AC_PCMDATA);
	next_dma.next = (unsigned int)(&next_dma);
	next_dma.control0 = (1 << 25) | (1 << 26) | (2 << 21) | (2 << 18);
	next_dma.control1 = len >> 2;

	DMACC0LLI = (unsigned int)(&next_dma);
	/*选择用何种DMA，参考S3C6410手册P158*/
	SDMA_SEL |= 1 << 22;

	/*设置DMA源地址参考S3C6410手册P400*/
	DMACC0SrcAddr = addr;
	/*设置DMA目的地址参考S3C6410手册P400*/
	DMACC0DestAddr = (unsigned int)(&AC_PCMDATA);
	/*设置DMA的数据宽度等，参考S3C6410手册P401*/
	DMACC0Control0 = (1 << 25) | (1 << 26) | (2 << 21) | (2 << 18);
	/*设置DMA取数据的长度*/
	DMACC0Control1 = len >> 2;
	/*选则DMA目的外设，参考S3C6410手册P405*/
	DMACC0Configuration = (1 << 11) | (6 << 6) | 1;
}

void play_music(unsigned int addr, unsigned int len)
{
	ac97_dma_init(addr, len);

	AC_GLBCTRL |= 2 << 12;//设置为DMA方式
	DMACConfiguration = 1;//使能DMA
}

void delay(int n)
{
	int i, j;
	
	for(i = 0; i < n; i++){
		for(j = 0; j < 10000; j++){
			;
		}
	}
}
