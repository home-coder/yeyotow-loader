#ifndef __DDR_H
#define __DDR_H

extern void ddr_init(void);

#define MEM_SYS_CFG     (*(volatile unsigned int *)0x7E00F120) 
#define P1MEMSTAT       (*(volatile unsigned int *)0x7E001000) 
#define P1MEMCCMD       (*(volatile unsigned int *)0x7E001004)
#define P1MEMCFG        (*(volatile unsigned int *)0x7E00100C)
#define P1MEMCFG2       (*(volatile unsigned int *)0x7E00104C) 
#define P1REFRESH       (*(volatile unsigned int *)0x7E001010) 
#define P1CASLAT        (*(volatile unsigned int *)0x7E001014) 
#define P1T_DQSS        (*(volatile unsigned int *)0x7E001018) 
#define P1T_MRD         (*(volatile unsigned int *)0x7E00101C)
#define P1T_RAS         (*(volatile unsigned int *)0x7E001020) 
#define P1T_RC  (*(volatile unsigned int *)0x7E001024) 
#define P1T_RCD (*(volatile unsigned int *)0x7E001028) 
#define P1T_RFC (*(volatile unsigned int *)0x7E00102C) 
#define P1T_RP  (*(volatile unsigned int *)0x7E001030) 
#define P1T_RRD (*(volatile unsigned int *)0x7E001034) 
#define P1T_WR  (*(volatile unsigned int *)0x7E001038) 
#define P1T_WTR (*(volatile unsigned int *)0x7E00103C) 
#define P1T_XP  (*(volatile unsigned int *)0x7E001040) 
#define P1T_XSR (*(volatile unsigned int *)0x7E001044) 
#define P1DIRECTCMD (*(volatile unsigned int *)0x7E001008)

#endif
