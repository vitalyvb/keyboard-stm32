TOPDIR=../../
include $(TOPDIR)config.mk

CFLAGS+=-c

OBJS := memcmp.o \
	memmove.o \
	memset.o \
	memcpy.o

LIB := liblibc.a

all: $(LIB)

liblibc.a: $(OBJS)

.PHONY: clean

clean:
	rm -f 	$(OBJS) $(LIB)
