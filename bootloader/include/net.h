#ifndef LZY_NET_H
#define LZY_NET_H

struct ethhdr{
	unsigned char	dest[6];	/* destination eth addr	*/
	unsigned char	source[6];	/* source ether addr	*/
	unsigned short	proto;		/* packet type ID field	*/
} __attribute__((packed));

typedef struct ethhdr eth_h;

struct iphdr{
    unsigned int 	ihl:4;
    unsigned int 	version:4;
    unsigned char 	tos;
    unsigned short 	tot_len;
    unsigned short 	id;
    unsigned short 	frag_off;
    unsigned char 	ttl;
    unsigned char 	protocol;
    unsigned short 	check;
    unsigned int 	saddr;
    unsigned int 	daddr;
} __attribute__((packed));

typedef struct iphdr ip_h;

struct udphdr{
  unsigned short source;
  unsigned short dest;
  unsigned short len;
  unsigned short check;
} __attribute__((packed));

typedef struct udphdr udp_h;

struct arphdr {
        unsigned short          ar_hrd;         /* format of hardware address   */
        unsigned short          ar_pro;         /* format of protocol address   */
        unsigned char   ar_hln;         /* length of hardware address   */
        unsigned char   ar_pln;         /* length of protocol address   */
        unsigned short          ar_op;          /* ARP opcode (command)         */

        unsigned char           ar_sha[6];       /* sender hardware address      */
        unsigned char           ar_sip[4];              /* sender IP address            */
        unsigned char           ar_tha[6];       /* target hardware address      */
        unsigned char           ar_tip[4];              /* target IP address            */
};

typedef struct arphdr arp_h;

//pack.c
extern int eth_pack(unsigned int, unsigned short);
extern int ip_pack(unsigned int, unsigned char);
extern int udp_pack(unsigned int, unsigned short);
extern int icmp_pack(int seq);
extern int arp_pack();
extern int arp_request_pack(void);

//unpack.c
extern void arp_unpack(void);
extern void eth_unpack(void);
extern void ip_unpack(void);
extern void udp_unpack(void);
extern void icmp_unpack(void);

//main.c
extern unsigned char heth[6];
extern unsigned char eeth[6];
extern unsigned int hip;
extern unsigned int eip;
extern unsigned short hport;
extern unsigned short eport;
extern unsigned char send_buf[2048];
extern unsigned char recv_buf[2048];
extern unsigned char sip[4];
extern unsigned char rip[4];
extern int arp_flag;

//tftp.c
extern void tftp_unpack(char *pkt, unsigned int length, unsigned short port);
extern void tftp_down(char *addr, char *filename);

extern unsigned int htonl(unsigned int host);
extern unsigned short htons(unsigned short host);
extern short cal_sum(unsigned short *buf, int len);

#endif
