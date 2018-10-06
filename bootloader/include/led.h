#ifndef __LED_H
#define __LED_H

extern void led_init(void);
extern void led_on(int n);
extern void led_off(int n);

#define GPMCON (*(volatile unsigned int *)0x7F008820) 
#define GPMDAT (*(volatile unsigned int *)0x7F008824) 
#define GPMPUD (*(volatile unsigned int *)0x7F008828) 

#endif
