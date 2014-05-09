TOPDIR=../../
include $(TOPDIR)config.mk

CFLAGS+=-c

OBJS := memcmp.o \
	memmove.o \
	memset.o \
	ctype.o \
	strcpy.o \
	strncpy.o \
	strcat.o \
	strcmp.o \
	strncmp.o \
	strlen.o \
	strtol.o \
	strtoul.o

LIB := liblibc.a

all: $(LIB)

liblibc.a: $(OBJS)

.PHONY: clean

clean:
	rm -f 	$(OBJS) $(LIB)
