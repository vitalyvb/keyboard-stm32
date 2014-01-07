TOPDIR=../../
include $(TOPDIR)config.mk

CFLAGS+=-c

OBJS := microrl.o

LIB := libmicrorl.a

all: $(LIB)

libmicrorl.a: $(OBJS)

.PHONY: clean

clean:
	rm -f 	$(OBJS) $(LIB)
