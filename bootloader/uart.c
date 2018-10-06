#include <uart.h>
#include <common.h>
#include <stdarg.h>

#if 0
ä ¸²è¡Œã€å¹¶è¡ŒATA / IDE hda PCI
    SATA USB UART PIC - E IIC IIS CAN .....RS232 3 V - 12 V--->0 - 3 V-- - 12 V--->1
    TTL / CMOM 0 / 0-- -- -->0 5 V / 3.3 V-- -- -->1 115200 8 N1
#endif
void ready_to_recv(void)
{
	while (!(UTRSTAT0 & (1 << 0))) ;
}

void ready_to_tran(void)
{
	while (!(UTRSTAT0 & (1 << 1))) ;
}

void uart_init(void)
{
	GPACON &= ~0xff;
	GPACON |= 0x22;
	//8N1
	ULCON0 = 3;
	UCON0 |= (1 << 2) | 1;
	UFCON0 = 0;		//close fifo
	UMCON0 = 0;

	//115200
	UBRDIV0 = 34;
	UDIVSLOT0 = 0xdfdd;
}

void uputchar(char c)
{
	UTXH0 = c;
	ready_to_tran();
}

char ugetchar(void)
{
	ready_to_recv();
	return URXH0;
}

void _uputs(char *s)
{
	while (*s) {
		uputchar(*s);
		if (*s == '\n')
			uputchar('\r');
		s++;
	}
}

void uputs(char *s)
{
	_uputs(s);
	uputchar('\r');
	uputchar('\n');
}

char *ugets(char *s)
{
	char *save = s;
	char c;

	while ((c = ugetchar()) != '\r') {
		*s = c;
		uputchar(c);
		s++;
	}

	*s = '\0';
	uputchar('\r');
	uputchar('\n');

	return save;
}

//123
//n = 123       buf[1][2][3][0]
//n = 12        buf[1][2][0]
//n = 1         buf[1][0]
void itoa(int n, char *buf)
{
	int i;

	if (n < 10) {
		buf[0] = n + '0';
		buf[1] = '\0';
		return;
	}
	itoa(n / 10, buf);
	for (i = 0; buf[i] != '\0'; i++) ;
	buf[i] = n % 10 + '0';
	buf[i + 1] = '\0';
}

void xtoa(int n, char *buf)
{
	int i;

	if (n < 16) {
		if (n < 10)
			buf[0] = n + '0';
		else
			buf[0] = n + 'a' - 10;
		buf[1] = '\0';
		return;
	}
	xtoa(n / 16, buf);
	for (i = 0; buf[i] != '\0'; i++) ;
	if ((n % 16) < 10)
		buf[i] = (n % 16) + '0';
	else
		buf[i] = (n % 16) + 'a' - 10;
	buf[i + 1] = '\0';
}

#if 0
va_list va_start va_end va_arg
#endif
//printf("nihaodfsdf %d %c %s %x\n", 100, 'a', "nohao", 0x100);
int uprintf(const char *fmt, ...)
{
	va_list ap;
	char c;
	int d;
	char *s;
	char buf[32];

	va_start(ap, fmt);
	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
			case 'c':
				c = va_arg(ap, int);
				uputchar(c);
				break;
			case 's':
				s = va_arg(ap, char *);
				_uputs(s);
				break;
			case 'd':
				d = va_arg(ap, int);
				if (d < 0) {
					uputchar('-');
					d = -d;
				}
				itoa(d, buf);
				_uputs(buf);
				break;
			case 'x':
				d = va_arg(ap, int);
				xtoa(d, buf);
				_uputs(buf);
				break;
			default:
				break;
			}
		} else {
			uputchar(*fmt);
			if (*fmt == '\n')
				uputchar('\r');
		}
		fmt++;
	}

	va_end(ap);

	return 0;
}
