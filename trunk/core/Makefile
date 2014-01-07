TOPDIR=../
include $(TOPDIR)config.mk

BIN=core
STARTUP=$(TOPDIR)device_support/gcc/startup_stm32f10x_md.s

CFLAGS+=-c
CFLAGS+=-DVECT_TAB_OFFSET=0x3000
CFLAGS+=-DERASE_HEAP
CFLAGS+=-fwhole-program

LDFLAGS+=-Wl,-Map,$(BIN).map
LDFLAGS+=-Wl,-T,$(TOPDIR)device_support/gcc/stm32f103_64k_20k_dfu.ld

OBJS := whole_program.o startup.o

all: ldall $(BIN).lst $(BIN).hex $(BIN).bin
	$(SIZE) -B $(BIN).elf

-include $(OBJS:.o=.d)

whole_program.o: whole_program.c
	$(CC) $(CFLAGS) -o $@ whole_program.c
	$(CC) -MM $(CFLAGS) whole_program.c > whole_program.d

startup.o: $(STARTUP)
	$(AS) $(ASFLAGS) $(STARTUP) -o $@

$(BIN).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

.PHONY: clean load ldall

clean:
	rm -f 	$(OBJS) \
		whole_program.d \
		$(BIN).map \
		$(BIN).lst \
		$(BIN).elf

flash: $(BIN).hex
	$(DFUUTIL) -s 0x08003000 -D core.bin
