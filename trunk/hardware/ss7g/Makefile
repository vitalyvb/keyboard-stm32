TOPDIR=../../
include $(TOPDIR)config.mk

CFLAGS+=-c

OBJS := kbd_led.o \
	kbd_matrix.o \
	keymap_boot.o \
	keymap.o \
	hwboard.o

LIB := libhw.a

all: $(LIB)

libhw.a: $(OBJS)

.PHONY: clean

clean:
	rm -f 	$(OBJS) $(LIB)
