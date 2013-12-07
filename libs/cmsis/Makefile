TOPDIR=../../
include $(TOPDIR)config.mk

CFLAGS+=-c

OBJS := core_cm3.o system_stm32f10x.o
LIB := libcmsis.a

all: $(LIB)

libcmsis.a: $(OBJS)

.PHONY: clean

clean:
	rm -f 	$(OBJS) $(LIB)
