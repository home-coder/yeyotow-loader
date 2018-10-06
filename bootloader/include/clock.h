#ifndef __CLOCK_H
#define __CLOCK_H

extern void clock_init(void);

#define APLL_LOCK 	(*(volatile unsigned int *)0x7E00F000) 
#define MPLL_LOCK 	(*(volatile unsigned int *)0x7E00F004) 
#define EPLL_LOCK 	(*(volatile unsigned int *)0x7E00F008)
#define APLL_CON 	(*(volatile unsigned int *)0x7E00F00C) 
#define MPLL_CON 	(*(volatile unsigned int *)0x7E00F010) 
#define EPLL_CON0 	(*(volatile unsigned int *)0x7E00F014) 
#define EPLL_CON1 	(*(volatile unsigned int *)0x7E00F018) 
#define CLK_SRC 	(*(volatile unsigned int *)0x7E00F01C) 
#define CLK_DIV0 	(*(volatile unsigned int *)0x7E00F020) 
#define HCLK_GATE 	(*(volatile unsigned int *)0x7E00F030) 
#define PCLK_GATE 	(*(volatile unsigned int *)0x7E00F034) 
#define SCLK_GATE 	(*(volatile unsigned int *)0x7E00F038) 
#define MISC_CON 	(*(volatile unsigned int *)0x7E00F838) 
#define OTHERS 		(*(volatile unsigned int *)0x7E00F900) 

#endif





