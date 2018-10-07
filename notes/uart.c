#include "common.h"
#include "regs.h"
#include "stdarg.h"

#define RECV_BUSY	while(!(UTRSTAT0 & 1))
#define TRAN_BUSY	while(!(UTRSTAT0 & 2))

void uart_init(void)
{
	/*配置GPA0-1为UART专用*/
	/*参考S3C6410 P312*/
	GPACON &= ~0xff;
	GPACON |= 0x22;

	/*配置UART的数据格式为8N1,波特率为115200*/
	/*参考S3C6410 P1091*/
	ULCON0 = 3;
	UCON0 |= (1 << 2) | (1 << 0);
	UFCON0 = 0;
	UMCON0 = 0;
 	UBRDIV0 = 34;
	UDIVSLOT0 = 0xdfdd;
}

void uputchar(char c)
{
	TRAN_BUSY;//检查上一次是否发完，如果发完则进行本次发送
	UTXH0 = c;//UTXH0对应原理图上灰色的条
}

char ugetchar(void)
{
	RECV_BUSY;//检查是否收到数据，如果收到则返回收到的数据
	return URXH0;
}

/*minicom识别的回车是'\r\n'
 /r代表回到行首，/n表示切换到下一行*/
void _uputs(char *buf)
{
	while(*buf != '\0'){
		uputchar(*buf);
		if(*buf == '\n'){
			uputchar('\r');
		}
		buf++;
	}
}

void uputs(char *buf)
{
	_uputs(buf);
	uputchar('\r');
	uputchar('\n');
}

char *ugets(char *buf)
{
	char *save = buf;
	char c;

	while((c = ugetchar()) != '\r'){
		*buf = c;
		uputchar(c);//回显
		buf++;
	}
	*buf = '\0';//注意未零
	uputchar('\r');
        uputchar('\n');

	return save;
}


/*把一个32位的整数转换为一个十进制显示的字符串*/
void itoa(unsigned int n, char *buf)
{
	int i;

	if(n < 10){
		buf[0] = n + '0';
		buf[1] = '\0';
		return;
	}
	itoa(n / 10, buf);
	for(i = 0; buf[i] != '\0'; i++){
		;
	}
	buf[i] = (n % 10) + '0';
	buf[i + 1] = '\0';
}

/*把一个32位整数转换为一个十六进制显示的字符串*/
void xtoa(unsigned int n, char *buf)
{
	int i;
	
	if(n < 16){
		if(n < 10){
			buf[0] = n + '0';
			buf[1] = '\0';
		}else{
			buf[0] = n - 10 + 'a';
			buf[1] = '\0';
		}
		return;
	}
	xtoa(n / 16, buf);
	for(i = 0; buf[i] != '\0'; i++){
		;
	}
	if((n % 16) < 10){
		buf[i] = (n % 16) + '0';
		buf[i + 1] = '\0';
	}else{
		buf[i] = (n % 16) - 10 + 'a';
                buf[i + 1] = '\0';
	}
}

/*应为stdarg.h中是用宏定义实现的以下几个方法，所以可以在裸板的情况下包含进来*/
/*valist 用来定义一个该宏所需要的变量*/
/*vastart 用来把fmt中的特殊符号(如%d中的d)和该特殊符号所对应的变量联系起来*/
/*va_arg 用来取出fmt中特殊符号对应的数据*/
/*注意用完后要用va_end来释放这些资源*/
int uprintf(const char *fmt, ...)
{
	va_list ap;
	int n;
	char *s;
	char buf[64];

	va_start(ap, fmt);
	while(*fmt != '\0'){
		if(*fmt == '%'){
			fmt++;
			switch(*fmt){
				case 'd':
					n = va_arg(ap, int);
					if(n < 0){
						uputchar('-');
						n =0 - n;
					}
					itoa(n, buf);
					_uputs(buf);
					break;
				case 's':
					s = va_arg(ap, char *);
					_uputs(s);
					break;
				case 'c':
					n = va_arg(ap, int);
					uputchar(n);
					break;
				case 'x':
					n = va_arg(ap, int);
					xtoa(n, buf);
					_uputs(buf);
					break;
				default:
					break;
			}
		}else{
			uputchar(*fmt);
			if(*fmt == '\n'){
				uputchar('\r');
			}
		}
		fmt++;
	}

	va_end(ap);

	return 0;
}


