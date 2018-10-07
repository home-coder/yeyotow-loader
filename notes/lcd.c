#include "common.h"
#include "regs.h"

/*定义三个初始化函数
	1.初始化GPI和GPJ为LCD作用
	2.初始化window0
		每个液晶屏支持五层的window*/
void gpio_init(void);
void common_init(void);
void window0_init(void);	

void lcd_init(void)
{
	gpio_init();
	common_init();
	window0_init();	
}

void gpio_init(void)
{
	GPICON = 0xaaaaaaaa;
	GPJCON = 0xaaaaaa;
	/*给LCD上电，把背光灯打开*/
	/*参考S3C6410手册和母版电路图*/
	GPFCON &= ~(3 << 28);
	GPFCON |= 1 << 28;
	GPFDAT |= 1 << 14;
	GPECON &= ~(0xf);
	GPECON |= 1;
	GPEDAT |= 1;
}

void common_init(void)
{
	/*配置方法参考S3C6410手册P491*/
	MIFPCON &= ~(1 << 3);
	SPCON &= ~3;
	SPCON |= 1;

	/*配置发送数据时钟，并打开DISPLAYC，参考S3C6410手册P495*/
	VIDCON0 = (14 << 6) | (1 << 4) | (3 << 0);
	/*配置时钟是否翻转，参考S3C6410手册P495*/
	/*参考S3C6410手册P485*/
	/*参考日立LCD手册P14*/
	VIDCON1 = (1 << 6) | (1 << 5);
	/*配置时序，参考S3C6410手册P497和P485，日立LCD手册P14*/
	VIDTCON0 = (1 << 16) | (1 << 8) | (9 << 0);
	VIDTCON1 = (1 << 16) | (1 << 8) | (40 << 0);
	/*配置LCD的分辨率大小，参考S3C6410手册P498*/
	VIDTCON2 = (271 << 11) | (479);
}

void window0_init(void)
{
#ifdef BPP565
	/*注意：这两个寄存器与图片格式相关*/
	/*配置输出数据格式和传送方法以并使能数据发送*/
	/*参考S3C6410手册P499*/
	WINCON0 = (1 << 16) | (5 << 2) | (1 << 0);
	/*设置一屏的数据量，参考S3C6410手册P506*/
	VIDOSD0C = (480 * 272) >> 1;
#else
	WINCON0 = (0xb << 2) | (1 << 0);
	VIDOSD0C = 480 * 272;
#endif
	/*设置LCD的最大和最小坐标，参考S3C6410手册P506*/
	VIDOSD0A = 0;
	VIDOSD0B = (479 << 11) | (271 << 0);

	/*设置frambuf的首地址*/
	VIDW00ADD0B0 = FRAM_BUFF;
}

/*画点函数：
 	在LCD上画一个点其实就是在这个坐标对应的frambuf内存空间中
	写入一个合适的值，x代表横坐标，y代表列坐标，后三个参数代表
	RGB三原色，可根据需要进行配置*/
void draw_point(int x, int y, int red, int green, int blue)
{
	/*注意：
 		565格式和888格式的区别：
			一个word的空间可以存放两个565格式的像素点数据
			一个word的空间可以存放一个888格式的像素点数据*/
#ifdef BPP565
	short *buf = (void *)FRAM_BUFF;
	*(buf + y * 480 + x) = (red << 11) | (green << 5) | (blue << 0);
#else
	int *buf = (void *)FRAM_BUFF;
	*(buf + y * 480 + x) = (red << 16) | (green << 8) | (blue << 0);
#endif
}
