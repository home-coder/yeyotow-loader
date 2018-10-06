#ifndef __DM9000_H
#define __DM9000_H

extern void eth_init(void);
extern void dm9000_write(unsigned char reg, unsigned char val);
extern unsigned char dm9000_read(unsigned char reg);
extern int eth_recv(char *buf);
extern int eth_send(char *buf, unsigned short len);

#define DM9000_ADDR	0x18000000
#define DM9000_DATA	0x18000004
#define SROM_BW         (*(volatile unsigned int *)0x70000000) 


#endif
