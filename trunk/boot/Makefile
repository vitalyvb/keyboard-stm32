TOPDIR=../
include $(TOPDIR)config.mk

BIN=boot
STARTUP=$(TOPDIR)device_support/gcc/startup_stm32f10x_md.s

CFLAGS+=-c
CFLAGS+=-DBOOTLOADER
CFLAGS+=-fwhole-program

LDFLAGS+=-Wl,-Map,$(BIN).map
LDFLAGS+=-Wl,-T,$(TOPDIR)device_support/gcc/stm32f103_64k_20k_boot.ld

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
	$(STM32FLASH) -b 38400 -w $(BIN).hex -v  /dev/ttyUSB1
