#ifndef __BUTTON_H
#define __BUTTON_H

#define GPNCON (*(volatile unsigned int *)0x7F008830)
#define EINT0CON0  (*(volatile unsigned int *)0x7F008900) 
#define EINT0MASK  (*(volatile unsigned int *)0x7F008920) 
#define EINT0PEND  (*(volatile unsigned int *)0x7F008924) 

#endif
