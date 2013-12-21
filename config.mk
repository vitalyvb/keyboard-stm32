BOARD=

######################################################################3

TOOLS_PATH=
TOOLS_PREFIX=arm-none-eabi-
TOOLS_VERSION_SUFFIX=

CC = $(TOOLS_PREFIX)gcc$(TOOLS_VERSION_SUFFIX)
AS = $(TOOLS_PREFIX)as
AR = $(TOOLS_PREFIX)ar
SIZE = $(TOOLS_PREFIX)size
OBJDUMP = $(TOOLS_PREFIX)objdump
OBJCOPY = $(TOOLS_PREFIX)objcopy
STM32FLASH = stm32flash

-include $(TOPDIR)config-local.mk

######################################################################3

ASFLAGS=-mcpu=cortex-m3 -I$(TOPDIR)libs/cmsis -I$(TOPDIR)libs/stm32_lib -gdwarf-2


CFLAGS=-mcpu=cortex-m3 -mtune=cortex-m3 -mfix-cortex-m3-ldrd
CFLAGS+=-DSTM32F10X_MD -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER
CFLAGS+=-mthumb -D__thumb2__=1 -msoft-float
CFLAGS+=-mno-sched-prolog -mapcs-frame
CFLAGS+=-Os -fno-strict-aliasing -fno-hosted
CFLAGS+=-Wall -Wno-unused-function 
#-Wno-implicit-function-declaration
CFLAGS+=-gdwarf-2 -g
# -nostdinc
CFLAGS+=-ffunction-sections -fdata-sections

CFLAGS+=-I$(TOPDIR)libs/cmsis -I$(TOPDIR)libs/stm32_lib -I$(TOPDIR)libs/stm32_usb-fs
CFLAGS+=-I$(TOPDIR)libs/tinyprintf -I$(TOPDIR)libs/libc
CFLAGS+=-I$(TOPDIR)hardware/$(BOARD)
CFLAGS+=-I$(TOPDIR)include
CFLAGS+=-I.


LDFLAGS=-static -mcpu=cortex-m3 -mthumb -mthumb-interwork -Wl,-O2
LDFLAGS+=-nostartfiles -nodefaultlibs -nostdlib

LDLIBS_PATH=-L$(TOPDIR)libs/cmsis -L$(TOPDIR)libs/stm32_lib -L$(TOPDIR)libs/stm32_usb-fs
LDLIBS_PATH+=-L$(TOPDIR)libs/tinyprintf -L$(TOPDIR)libs/libc
LDLIBS_PATH+=-L$(TOPDIR)hardware/$(BOARD)

######################################################################3

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@
%.hex: %.elf
	$(OBJCOPY) -j .isr_vector -j .text -j .data -O ihex $< $@
%.bin: %.elf
	$(OBJCOPY) -j .isr_vector -j .text -j .data -O binary $< $@

%.a:
	$(AR) r  $@ $^

