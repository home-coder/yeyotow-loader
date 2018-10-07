#ifndef LZY_COMMON_H
#define LZY_COMMON_H

/*这里注意：
 	每次编译的u-boot会对应不同的地址
	编译完u-boot后会出现文件System.map
	在该文件中有u-boot提供的函数接口的地址*/
#define printf(...) (((int (*)(const char *, ...))0xc7e11d70)(__VA_ARGS__))

#endif
