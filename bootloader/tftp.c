#include <common.h>
#include <net.h>
#include <dm9000.h>
#include <lib.h>
#include <uart.h>

static char *load_addr;
static char load_stat;
static short last_no;
static int last_len;
static int sign;
static int line;
static char *block = (char *)0x56000000;
//56000000->[] [1][1][1][1][0][][][][][].......
//50008000->   [x][x][y][x][x][][][][][].......

void store_data(char *data, unsigned int len, unsigned short no)
{
	//data--->[3 no [data]]
	//len ============
	//no = 1, 2, 3.........
	int i;

	if (block[no] == 0) {
		//load_addr = 0x500080000
		memcpy(load_addr + (no - 1) * 512, data, len);
		block[no] = 1;
		sign++;
		if (sign > 15) {
			uprintf("#");
			sign = 0;
			line++;
			if (line > 64) {
				uprintf("\n\t");
				line = 0;
			}
		}
	}
	//last data
	if (len < 512) {
		last_no = no;
		last_len = len;
	}

	if (last_no != 0) {
		for (i = last_no; i > 0; i--) {
			if (block[i] == 0)
				return;
		}
		load_stat = 0;	//last data && all
	}
#ifdef DEBUG
	uprintf("Save block# %d, len = %d Done.\n", no, len);
#endif
}

void send_ack(unsigned short port, unsigned short no)
{
	unsigned short *tftp = (unsigned short *)(send_buf + sizeof(eth_h) + sizeof(ip_h) + sizeof(udp_h));

#ifdef DEBUG
	uprintf("send ack... port = %d, block# = %d\n", htons(port), no);
#endif
	tftp[0] = htons(TFTP_ACK);	//4
	tftp[1] = htons(no);
	udp_pack(4, port);

#ifdef DEBUG
	uprintf("send ack... Done.\n");
#endif
}

void tftp_unpack(char *pkt, unsigned int len, unsigned short port)
{
	//first:pkt--->6   len = 4  port = 69
	//others:pkt--->3  len = 516 port = 69
	unsigned short no;

	switch (htons(((unsigned short *)pkt)[0])) {
	case TFTP_DATA:	//3
		no = htons(((unsigned short *)pkt)[1]);

		if ((no < 1) || (no > MAX_BLOCK) || (len < 4) || len > 516) {
			uprintf("Error: block#: %d, len: %d\n", no, len);
			return;
		}
		//pkt + 4 ---->[data] len - 4 <= 512data
		//no--->1, 2, 3.......
		store_data(pkt + 4, len - 4, no);

		send_ack(port, no);
		break;

	case TFTP_OACK:	//6
		//send_ack(port, 0);
		break;

	case TFTP_ERROR:	//5
		uprintf("Error: %s\n", pkt + 4);
		break;

	default:
		uprintf("Unkown Opcode.\n");
	}
}

void tftp_down(char *addr, char *filename)
{
	//addr = 0x50008000
	//filename = "zImage"
	unsigned int len;
	char *tftp;

	load_addr = addr;
	load_stat = 1;
	last_no = 0;
	sign = 0;
	line = 0;
	//block = 0x56000000 32K + 1
	memset(block, 0, MAX_BLOCK + 1);

/****************** tftp_pack() ******************************/

	tftp = (char *)(send_buf + sizeof(eth_h) + sizeof(ip_h) + sizeof(udp_h));
	len = 0;

	*(unsigned short *)tftp = htons(TFTP_RRQ);	//1
	len += 2;

	strcpy(tftp + len, filename);	//"zImage"
	len += strlen(filename) + 1;

	strcpy(tftp + len, "octet");
	len += 5 + 1;		//strlen("octet") + 1
#if 0
	strcpy(tftp + len, "timeout");
	len += 7 + 1;		//strlen("timeout") + 1

	*(tftp + len) = '5';	//5 Seconds
	*(tftp + len + 1) = 0;
	len += 2;
#endif
/************************************************/

	uprintf("Load address: 0x%x\n", load_addr);
	uprintf("Loading:\n\t");

	udp_pack(len, hport);	//Read Request
#if 0
	while (load_stat != 0) {
		unsigned int len;
		if ((len = eth_recv((void *)recv_buf)) > 14) {
			eth_unpack();
		}
	}
#else
	while (load_stat != 0)
		delay(100);
#endif
	uprintf("\nDone.\n");
	uprintf("Bytes transferred = %d\n", (last_no - 1) * 512 + last_len);
}
