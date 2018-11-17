#OBA=b $(A) 延时变量
#A:=b 立即变量
#A?=b 延时变量  不覆盖
#A+=b  A=a A+=b A==a b 
TARGET		:=arm
START_OBJ	:=start.o
OBJS		:=clock.o ddr.o nand.o boot.o dm9000.o led.o main.o lib.o uart.o  cmd_tftp.o net.o tftp.o unpack.o pack.o irq.o go.o
BIN		:=$(TARGET).bin
LD_ADDR		:=0x57000000

#CROSS_COMPILE	:=arm-linux-gnueabi-
CROSS_COMPILE	:=arm-none-eabi-
Q		:=@

CC		:=$(Q)$(CROSS_COMPILE)gcc
AS		:=$(Q)$(CROSS_COMPILE)as
LD		:=$(Q)$(CROSS_COMPILE)ld
OBJCOPY		:=$(Q)$(CROSS_COMPILE)objcopy
NM		:=$(Q)$(CROSS_COMPILE)nm

ECHO		:=$(Q)echo

RM		:=$(Q)rm -rf
CP		:=$(Q)/bin/cp -rf
#make prefix=/tftpboot
prefix		?=/tftproot

CFLAGS		+=-I./include -Wall -O2 -fno-stack-protector
#################################
#GNUmakefile makefile Makefile
#make -f a
#目标  依赖  规则
#$<第一个依赖  自动变量
#$^所有依赖  自动变量
#$@目标 自动变量
#arm=[ELF][arm.bin]

.PHONY:all
all:$(TARGET)
	$(OBJCOPY) -O binary $< $(BIN)
	$(ECHO) OBJCOPY		$<
	$(NM) $<	>system.map
	$(ECHO) NM	$<

$(TARGET):$(START_OBJ) $(OBJS)
	$(LD) $^ -o $@ -Ttext $(LD_ADDR)
	$(ECHO) LD	$@
%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)
	$(ECHO) CC	$@
%.o:%.s
	$(AS) -o $@ $<
	$(ECHO) AS	$@

.PHONY:install
install:
	$(CP) $(BIN) $(prefix)
	$(ECHO) INSTALL	$(BIN) $(prefix) 
.PHONY:clean
clean:
	$(RM) *.o $(TARGET) $(BIN)
	$(ECHO) RM	./





