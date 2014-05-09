TOPDIR=../../../
include $(TOPDIR)config.mk

CFLAGS+=-c -fwhole-program
CFLAGS+=-I$(TOPDIR)include
LDLIBS=

all: libparser.a


parser.o: allcode.c gen.h lexer.rl parser.tab.c parser.y \
	  csp_api.c gen.c lexer.h lexer.tab.c parser.tab.h
	$(CC) $(CFLAGS) -o $@ allcode.c

libparser.a: parser.o

lexer.tab.c: lexer.rl parser.y
	ragel -T1 -e -o lexer.tab.c lexer.rl

parser.tab.c parser.tab.h: parser.y
	bison -v -d -b parser parser.y

clean:
	rm -f libparser.a parser.o parser.output
	rm -f parser-arm.lst parser-arm.o

cleanall: clean
	rm -f lexer.tab.c parser.tab.c parser.tab.h


arm: parser.tab.c lexer.tab.c
	arm-none-eabi-gcc-4.8.1 -Os -mcpu=cortex-m3 -mtune=cortex-m3 -mfix-cortex-m3-ldrd -mthumb -D__thumb2__=1 \
		-I$(TOPDIR)include \
		-msoft-float -mno-sched-prolog -mapcs-frame -Os -fno-strict-aliasing -fwhole-program -c -o parser-arm.o allcode.c
	arm-none-eabi-objdump -h -S parser-arm.o > parser-arm.lst
	size parser-arm.o
