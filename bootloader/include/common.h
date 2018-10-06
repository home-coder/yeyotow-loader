#ifndef LZY_COMMON_H
#define LZY_COMMON_H
 
#define INT_EINT1 	1	//中断源
#define TFTP_RRQ        1	//读请求
#define TFTP_WRQ        2	//写请求
#define TFTP_DATA       3	//数据标志
#define TFTP_ACK        4	//应答标志
#define TFTP_ERROR      5	//错误标志
#define TFTP_OACK       6	//请求应答
#define MAX_BLOCK       (2048 * 16)	//限定的最大数据
#define NULL ((void *)0)	
//#define printf(...) (((int (*)(const char *, ...))0xc7e11d70)(__VA_ARGS__))

//#define DEBUG

#endif

