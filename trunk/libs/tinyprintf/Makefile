TOPDIR=../../
include $(TOPDIR)config.mk

CFLAGS+=-c

OBJS := printf.o
LIB := libprintf.a

all: $(LIB)

libprintf.a: $(OBJS)

.PHONY: clean

clean:
	rm -f 	$(OBJS) $(LIB)
