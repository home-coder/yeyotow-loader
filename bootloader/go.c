#include "go.h"

void go(unsigned int addr)
{

	__asm__ __volatile__("mov lr,pc\n" "mov pc,%0\n"::"r"(addr)
			     :"lr", "pc");
}
