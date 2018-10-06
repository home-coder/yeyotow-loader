#include <dm9000.h>
#include <common.h>

void eth_init(void)
{
	SROM_BW |= 1 << 4;

	dm9000_write(0x1f, 0x00);
	dm9000_write(0x00, 0x01);
	dm9000_write(0x00, 0x00);
	dm9000_write(0xff, (1 << 7) | 1);

	dm9000_write(0x10, 0x11);
	dm9000_write(0x11, 0x22);
	dm9000_write(0x12, 0x33);
	dm9000_write(0x13, 0x44);
	dm9000_write(0x14, 0x55);
	dm9000_write(0x15, 0x66);

	dm9000_write(0x01, (1 << 2) | (1 << 3) | (1 << 5));
	dm9000_write(0xfe, 0x03);
	dm9000_write(0x05, (1 << 4) | (1 << 5) | 1);
}

void dm9000_write(unsigned char reg, unsigned char val)
{
	*(volatile unsigned char *)DM9000_ADDR = reg;
	*(volatile unsigned char *)DM9000_DATA = val;
}

unsigned char dm9000_read(unsigned char reg)
{
	*(volatile unsigned char *)DM9000_ADDR = reg;
	return *(volatile unsigned char *)DM9000_DATA;
}

int eth_recv(char *buf)
{
	unsigned int i;
	unsigned short len = 0, status = 0;

	if (!(dm9000_read(0xfe) & 0x01)) {
		return -1;
	}

	dm9000_read(0xf0);

	if ((dm9000_read(0xf0) & 0x03) != 0x01) {
		goto end;
	}
	if ((dm9000_read(0xfe) & 0x80)) {
		goto end;
	}

	*(volatile unsigned char *)DM9000_ADDR = 0xf2;
	status = *(volatile unsigned short *)DM9000_DATA;
	len = *(volatile unsigned short *)DM9000_DATA;

	for (i = 0; i < ((len + 1) >> 1); i++) {
		((volatile unsigned short *)buf)[i] = *(volatile unsigned short *)DM9000_DATA;
	}
      end:
	dm9000_write(0xfe, 0x01);

	return len;
}

int eth_send(char *buf, unsigned short len)
{
	int i;

	dm9000_write(0xff, (1 << 7));
	if ((dm9000_read(0xfe) & 0x80)) {
		return -1;
	}
	*(volatile unsigned char *)DM9000_ADDR = 0xf8;

	for (i = 0; i < ((len + 1) >> 1); i++) {
		*(volatile unsigned short *)DM9000_DATA = ((volatile unsigned short *)buf)[i];
	}

	dm9000_write(0xfc, len & 0xff);
	dm9000_write(0xfd, (len >> 8) & 0xff);

	dm9000_write(0x02, 0x01);

	while (!(dm9000_read(0xfe) & 0x02)) ;

	dm9000_write(0xff, (1 << 7) | 1);

	return len;
}
