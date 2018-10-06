#include <common.h>
#include <net.h>
#include <eint.h>
#include <irq.h>
#include <dm9000.h>

/*以h开头的变量全部都是主机的信息*/
/*以e开头的变量全部都是开发板的信息*/
unsigned char heth[6] = { 0x00 };	//host
unsigned char eeth[6] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };

unsigned int hip = 192 << 24 | (168 << 16) | (1 << 8) | 111;	//192.168.1.10
unsigned int eip = 192 << 24 | (168 << 16) | (1 << 8) | 20;	//192.168.1.20
unsigned char sip[4] = { 192, 168, 1, 20 };
unsigned char rip[4] = { 192, 168, 1, 111 };

unsigned short hport;
unsigned short eport;

int arp_flag;
unsigned char send_buf[2048];
unsigned char recv_buf[2048];

void __attribute__ ((interrupt("IRQ"))) do_net(void)	//source 1
{
	unsigned int len;

	if (!(EINT0PEND & (1 << 7))) {
		goto end;
	}
	if ((len = eth_recv((void *)recv_buf)) > 14) {
		eth_unpack();
	}
      end:
	EINT0PEND = EINT0PEND;
	clear_irq();
}

int tftp(unsigned int addr, char *name)
{
	hport = htons(69);
	eport = htons(4321);

	eth_init();
#if 1
	irq_disable();

	//extern int 7
	GPNCON &= ~(3 << 14);
	GPNCON |= 2 << 14;
	EINT0CON0 &= ~(7 << 12);
	EINT0CON0 |= 1 << 12;
	EINT0MASK &= ~(1 << 7);

	EINT0PEND = EINT0PEND;

	irq_request(INT_EINT1, do_net);	//1

	irq_enable();
#endif
	arp_request_pack();
#ifdef DEBUG
	uprintf("send arp ok\n");
#endif
	tftp_down((void *)addr, name);

	return 0;
}
