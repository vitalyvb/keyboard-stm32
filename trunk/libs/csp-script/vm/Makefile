TOPDIR=../../../
include $(TOPDIR)config.mk

CFLAGS+=-I$(TOPDIR)libs/csp-script/parser

all: libvm.a

libvm.a: vm.o arrays.o

clean:
	rm -f vm.o arrays.o libvm.a
	rm -f vm-arm.S vm-arm.lst vm-arm.o


arm: 
	arm-none-eabi-gcc-4.8.1 -g -Os -mcpu=cortex-m3 -mtune=cortex-m3 -mfix-cortex-m3-ldrd -mthumb -D__thumb2__=1 \
		-msoft-float -mno-sched-prolog -mapcs-frame -fno-strict-aliasing \
		-I$(TOPDIR)parser -I$(TOPDIR)include -DBUILDING_CSP=1 \
		-fwhole-program -c -o vm-arm.o vm.c

	arm-none-eabi-gcc-4.8.1 -g -Os -mcpu=cortex-m3 -mtune=cortex-m3 -mfix-cortex-m3-ldrd -mthumb -D__thumb2__=1 \
		-msoft-float -mno-sched-prolog -mapcs-frame -fno-strict-aliasing \
		-I$(TOPDIR)parser -I$(TOPDIR)include -DBUILDING_CSP=1 \
		-fwhole-program -c -S -o vm-arm.S vm.c

	arm-none-eabi-objdump -h -S vm-arm.o > vm-arm.lst
	size vm-arm.o
