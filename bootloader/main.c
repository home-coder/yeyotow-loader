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
		bootm(atoi(value[0]));
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
		} else if (!strcmp(key, DDRTEST)) {
			/*
			* 测试ddr的128M的访问是否合法,0x50000000
			*/
			//TODO write 0x128M 5a

			/*
			* 测试在一个地址上如0x50008000写入0x5a5a5a5a四个字节  0x5a5a5a三个字节 0x5a5a两个字节 0x5a一个字节
			* 是否都可以正常读取出来
			*/
			//TODO

			/*
			* 测试写入一个地址范围全部为0x5A，然后从某一个固定地址读取出这个数据如 (*(unsigned int *)0x50000001)
			的值应该就是0x5A
			*/
			//TODO
			/*
			* ddr的复制功能测试，如将某个范围的内存1完全复制到另一个范围2内，然后将内存2的数据完全打印出来和内存1中的
			进行一个一个字节的比较，看看是否一致，直到所有的数据完全比较完毕
			*/
			//TODO

		} else {
			return -1;
		}
	} else {
		return -1;
	}

	return 0;
}

int main_boot(void)
{
	unsigned int register pc asm("pc");
	char cmd_buf[1024];
	char *pbuf, *key;
	char *value[32];
	unsigned int valsz;
	int ret = 0;

	uart_init();
	uprintf("PC:%x\n", pc);

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
