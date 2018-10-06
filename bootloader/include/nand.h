#ifndef __NAND_H
#define __NAND_H

extern void nand_init(void);
//nand read 50008000 40000 300000
extern void nand_read(unsigned int ddr, unsigned int nand, unsigned int lenth);
//nand write 50008000 40000 300000
extern void nand_write(unsigned int ddr, unsigned int nand, unsigned int lenth);
//nand erase 0 40000
extern void nand_erase(unsigned int nand, unsigned int lenth);
extern void nand_id(char *s);

#define Base 0x70200000

#define GPOCON   (*(volatile unsigned int *)0x7F008140) 
#define GPPCON   (*(volatile unsigned int *)0x7F008160)
#define NFCONF   (*(volatile unsigned int *)(Base + 0x00))
#define NFCONT   (*(volatile unsigned int *)(Base + 0x04)) 
#define NFCMMD   (*(volatile unsigned int *)(Base + 0x08))
#define NFADDR   (*(volatile unsigned int *)(Base + 0x0c)) 
#define NFDATA   (*(volatile unsigned int *)(Base + 0x10)) 
#define NFSTAT   (*(volatile unsigned int *)(Base + 0x28))



#endif











