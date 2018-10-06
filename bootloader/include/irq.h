#ifndef __IRQ_H
#define __IRQ_H

#define VIC0_BASE 0x71200000 
#define VIC1_BASE 0x71300000

#define VIC0IRQSTATUS 	(*(volatile unsigned int *)(VIC0_BASE + 0x000)) 
#define VIC0FIQSTATUS 	(*(volatile unsigned int *)(VIC0_BASE + 0x004)) 
#define VIC0RAWINTR 	(*(volatile unsigned int *)(VIC0_BASE + 0x008)) 
#define VIC0INTSELECT 	(*(volatile unsigned int *)(VIC0_BASE + 0x00C)) 
#define VIC0INTENABLE 	(*(volatile unsigned int *)(VIC0_BASE + 0x010)) 
#define VIC0INTENCLEAR 	(*(volatile unsigned int *)(VIC0_BASE + 0x014))  
#define VIC0SOFTINT 	(*(volatile unsigned int *)(VIC0_BASE + 0x018))  
#define VIC0SOFTINTCLEAR (*(volatile unsigned int *)(VIC0_BASE + 0x01C)) 
#define VIC0PROTECTION 	 (*(volatile unsigned int *)(VIC0_BASE + 0x020))  
#define VIC0SWPRIORITYMASK 	(*(volatile unsigned int *)(VIC0_BASE + 0x024)) #define VIC0PRIORITYDAISY 	(*(volatile unsigned int *)(VIC0_BASE + 0x028)) 
#define VIC0VECTADDR ((volatile unsigned int *)(VIC0_BASE + 0x100))
#define VIC0ADDRESS 	   (*(volatile unsigned int *)(VIC0_BASE + 0xF00))

#define VIC1IRQSTATUS 	(*(volatile unsigned int *)(VIC1_BASE + 0x000)) 
#define VIC1FIQSTATUS 	(*(volatile unsigned int *)(VIC1_BASE + 0x004)) 
#define VIC1RAWINTR 	(*(volatile unsigned int *)(VIC1_BASE + 0x008)) 
#define VIC1INTSELECT 	(*(volatile unsigned int *)(VIC1_BASE + 0x00C)) 
#define VIC1INTENABLE 	(*(volatile unsigned int *)(VIC1_BASE + 0x010)) 
#define VIC1INTENCLEAR 	(*(volatile unsigned int *)(VIC1_BASE + 0x014))  
#define VIC1SOFTINT 	(*(volatile unsigned int *)(VIC1_BASE + 0x018))  
#define VIC1SOFTINTCLEAR (*(volatile unsigned int *)(VIC1_BASE + 0x01C)) 
#define VIC1PROTECTION 	 (*(volatile unsigned int *)(VIC1_BASE + 0x020))  
#define VIC1SWPRIORITYMASK 	(*(volatile unsigned int *)(VIC1_BASE + 0x024)) 
#define VIC1PRIORITYDAISY 	(*(volatile unsigned int *)(VIC1_BASE + 0x028)) 
#define VIC1VECTADDR ((volatile unsigned int *)(VIC1_BASE + 0x100))
#define VIC1ADDRESS 	   (*(volatile unsigned int *)(VIC1_BASE + 0xF00))

extern void clear_irq(void);
extern int irq_request(int no, void (*handle)(void));
extern void irq_enable(void);
extern void irq_disable(void);

#endif









