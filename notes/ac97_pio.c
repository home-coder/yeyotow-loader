#include "common.h"
#include "regs.h"

#define AC97_INT	36

void delay(int n);

unsigned int music_addr;
unsigned int music_len;
unsigned int music_offset;

void ac97_init(void)
{
	/*把端口GPD设置为wm9714专用*/
	/*参考S3C6410手册P318*/
	GPDCON = 0x44444;

	/*使AC97控制器冷启动，即把控制器和身卡的寄存器复位*/
	/*参考S3C6410手册P1152*/
	AC_GLBCTRL |= 1;
	delay(1);
	AC_GLBCTRL &= ~1;

	/*使AC97控制器热启动，即相当于给wm9714上电唤醒*/
	AC_GLBCTRL |= 1 << 1;
	delay(1);
	AC_GLBCTRL &= ~(1 << 1);

	/*发出AC—LINK信号*/
	AC_GLBCTRL |= 1 << 2;
	delay(1);
	/*使能AC-LINK信号*/
	AC_GLBCTRL |= 1 << 3;

	/*在此一定要读一次声卡，控制器才能进入ACTIVE模式*/
	AC_CODEC_CMD |= 1 << 23;
	delay(10);//这个延时一定要有
	/*判断是否达到了ACTIVE状态*/
	if((AC_GLBSTAT & 0x7) == 3){
		printf("AC97 is active...\n");
	}else{
		printf("errno is %d\n", AC_GLBSTAT & 0x7);
	}
}

/*写wm9714函数，参考S3C6410手册P1153*/
void wm9714_write(unsigned int reg, unsigned int val)
{
	AC_CODEC_CMD = (reg << 16) | (val);
	delay(1);//这个延时不能小于1/48KHZ
}

void wm9714_init(void)
{
	/*打开耳机和扬声器，并设置音量，参考wm9714手册P38*/
	wm9714_write(0x0c, (0xf << 8) | (0xf));
	/*打开 零输入时停止DA转换 功能*/
	wm9714_write(0x5c, 1 << 7);
	/*设置采样频率，使声卡能够正确的回复音频数据，参考wm9714手册P43*/
	wm9714_write(0x2a, 1);
	wm9714_write(0x2c , 0xac44);
	/*耳机选择耳机的数据源，扬声器选择扬声器的数据源*/
	wm9714_write(0x1c, (2 << 4) | (2 << 6) | (3 << 8) | (3 << 11));
	/*单独调节耳机的音量，参考wm9714手册P55*/
	wm9714_write(0x04, 0);
	/*单独调节扬声器的音量，参考wm9714手册P57*/
	wm9714_write(0x02, 0);
	/*设置检测耳机插入，即耳机插入后扬声器不响，参考wm9714手册P60*/
	wm9714_write(0x24, 1 << 4);
	wm9714_write(0x5a, 0 << 6);

	/*带开电源，参考wm9714手册P76*/
	wm9714_write(0x26, 0);
	wm9714_write(0x3c, 0);
	wm9714_write(0x3e, 0);
}

void do_ac97(void)
{
	/*判断是否是数据缓存为空的中断来了*/
	if(AC_GLBSTAT & (1 << 21)){
		AC_GLBCTRL |= 1 << 29;//清除中断
		AC_PCMDATA = *((unsigned int *)(music_addr + music_offset));
		music_offset += 4;//取下一个数据
		/*实现回放*/
		if(music_offset > music_len){
			music_offset = 0;
		}
	}
}

void play_music(unsigned int addr, unsigned int len)
{
	music_addr = addr;
	music_len = len;
	music_offset = 0;

	/*选择PIO方式，并使能数据缓存为空时候的中断*/
	AC_GLBCTRL |= (1 << 21) | (1 << 12);
	/*申请AC97中断*/
	irq_request(AC97_INT, do_ac97);
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
