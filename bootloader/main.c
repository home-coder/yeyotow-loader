#include <common.h>
#include <lib.h>
#include <uart.h>
#include <clock.h>
#include <cmd.h>
#include <led.h>
#include <nand.h>
#include <go.h>

#define SHELL "[yeyotow@do]# "

#define LED            "led"
#define BOOTM          "bootm"
#define TFTP           "tftp"
#define GO             "go"
#define NAND           "nand"
#define DDRTEST        "ddrtest"

typedef enum { false, true } bool;

/*
led on/off 1
bootm 57000000
tftp 58000000 arm.bin
go 58000000
nand erase 0 40000
nand write 5000000 0 40000
nand read 50000000 0 40000
*/

int parse_cmdbuf(char *getbuf, char **key, char **value, unsigned int *valsz)
{
	char *pbuf = getbuf;
	char *delim = " ";
	unsigned int i = 0;

	if (!pbuf) {
		uprintf("error input\n");
		return -1;
	}

	*key = strsep(&pbuf, delim);
	while (pbuf) {
		value[i] = strsep(&pbuf, delim);
		i++;
	}

	*valsz = i + 1;

	return 0;
}

int do_command(char *key, char **value, unsigned int valsz)
{
 	char *pnum;

	uprintf("key is %s\n", key);
	if (!strcmp(key, LED)) {
		if (valsz != 3) {
			return -1;
		} else {
			pnum = value[1];
			if (!strcmp(value[0], "on")) {
				led_on(atoi(pnum));
			} else if (!strcmp(value[0], "off")) {
				led_off(atoi(pnum));
			}
		}
	} else if (!strcmp(key, BOOTM)) {
		if (valsz != 2) {
			return -1;
		}
		//bootm(atoi(value[0]));
	} else if (!strcmp(key, TFTP)) {
		if (valsz != 3) {
			return -1;
		}
		tftp(atoi(value[0]), value[1]);
	} else if (!strcmp(key, GO)) {
		if (valsz != 2) {
			return -1;
		}
		go(atoi(value[0]));
	} else if (!strcmp(key, NAND)) {
		if (!strcmp(value[0], "erase")) {
			uprintf("%s %s %s %s\n", key, value[0], value[1], value[2]);
			nand_erase(atoi(value[1]), atoi(value[2]));
		} else if (!strcmp(value[0], "write")) {
			uprintf("%s %s %s %s %s\n", key, value[0], value[1], value[2], value[3]);
			nand_write(atoi(value[1]), atoi(value[2]), atoi(value[3]));
		} else if (!strcmp(value[0], "read")) {
			uprintf("%s %s %s %s %s\n", key, value[0], value[1], value[2], value[3]);
			nand_read(atoi(value[1]), atoi(value[2]), atoi(value[3]));
		} else {
			return -1;
		}
	} else if (!strcmp(key, DDRTEST)) {
			/*
			* 测试ddr的128M的访问是否合法,0x50000000～0x58000000
			* 测试在一个地址上如0x50008000写入0x5a5a5a5a四个字节  0x5a5a5a三个字节 0x5a5a两个字节 0x5a一个字节
			* 是否都可以正常读取出来
			*/
			#define SDRAM_MIN    (*(volatile unsigned int *)0x50000000)
			#define SDRAM_MAX    (*(volatile unsigned int *)0x60000000)

			uprintf("addr: 0x%x\n", &SDRAM_MAX);
			SDRAM_MAX = 0x5a5a5a5a;
			uprintf("SDRAM_MAX: 0x%x\n", SDRAM_MAX);
			SDRAM_MAX = 0x5a5a5a;
			uprintf("SDRAM_MAX: 0x%x\n", SDRAM_MAX);
			SDRAM_MAX = 0x5a5a;
			uprintf("SDRAM_MAX: 0x%x\n", SDRAM_MAX);
			SDRAM_MAX = 0x5a5a;
			uprintf("SDRAM_MAX: 0x%x\n", SDRAM_MAX);
			SDRAM_MAX = 0x5A;
			uprintf("SDRAM_MAX: 0x%x\n", SDRAM_MAX);

			/*
			* 测试最高位是0  即 00 00 00 5a存储；
			*/
			unsigned int *pint = (unsigned int *)0x50000000;
			*pint = 0x5a;
			uprintf("0x%x\n", (unsigned int)(long)*pint);

			/*
			* 同上面的问题类似，求大小端问题，比如是50000003的值为5a 还是500000000的值是5a
			*/
			//TODO

			/*
			* 57000000~58000000是arm.bin从nand复制到内存的空间，不可以操作，否则程序被篡改
			*/
			unsigned int *bstart = (unsigned int *)0x57000000;
			unsigned int *bend   = (unsigned int *)0x58000000;

			unsigned int *pchange;
			for (pchange = bstart; pchange < bend; pchange++) {
				*pchange = 0x5a;
			}

			/*
			* 测试写入一个地址范围全部为0x5A，然后从某一个固定地址读取出这个数据如 (*(unsigned int *)0x50000001)
			的值应该就是0x5A
			*/
			volatile unsigned int *from_addr;
			unsigned int sub = 0x60000000 - 0x50000000;
			uprintf("0x%x\n", sub);
			uprintf("0x%x, 0x%x, 0x%x\n", (unsigned int)(long)&SDRAM_MIN,  \
					(unsigned int)(long)&SDRAM_MAX, (unsigned int)(long)&SDRAM_MAX \
							- (unsigned int)(long)&SDRAM_MIN + 1);
			unsigned char *pm = (unsigned char *)0x50000001;
			//memset((void *)pm, 0xff, (0x60000000 - 0x50000000) + 1);
			int pmi = 0x7000000;
			while (pmi--) {
				*pm++ = 0x5a;
			}

			uprintf("0x%x\n", (unsigned int)(long)*(volatile unsigned int *)0x50008000);
			uprintf("--------------------------\n");
			for (from_addr = (volatile unsigned int *)0x50000000; from_addr < (volatile unsigned int *)0x57000000; from_addr++) {
				*from_addr = 0x5a5b5c5d;
			}
			uprintf("0x%x\n", *(volatile unsigned int *)0x50008000);
			/*
			* ddr的复制功能测试，如将某个范围的内存1完全复制到另一个范围2内，然后将内存2的数据完全打印出来和内存1中的
			进行一个一个字节的比较，看看是否一致，直到所有的数据完全比较完毕
			*/
			volatile unsigned int *from = &SDRAM_MIN;
			volatile unsigned int *to   = (volatile unsigned int *)0x58000000;
			for (; from < &SDRAM_MIN + 1024; from++) {
				*to++ = *from++;
			}
			uprintf("0x%x\n", *(volatile unsigned int *)0x58000399);

	} else {
		return -1;
	}

	return 0;
}

int main_boot(void)
{
//	unsigned int register pc asm("pc");
	char cmd_buf[1024];
	char *pbuf, *key;
	char *value[32];
	unsigned int valsz;
	int ret = 0;

	uart_init();
	uprintf("PC:%x\n", "pc");

	while (1) {
		uprintf(SHELL);
		pbuf = ugets(cmd_buf);
		ret = parse_cmdbuf(pbuf, &key, value, &valsz);
		if (ret < 0) {
			uprintf("cmd buffer error\n");
			continue;
		}
		ret = do_command(key, value, valsz);
		if (ret < 0) {
			uprintf("command is not supported\n");
			continue;
		}
	}

	return 0;
}
