TOPDIR=../../
include $(TOPDIR)config.mk

CFLAGS+=-c

OBJS := misc.o \
	stm32f10x_adc.o \
	stm32f10x_bkp.o \
	stm32f10x_can.o \
	stm32f10x_cec.o \
	stm32f10x_crc.o \
	stm32f10x_dac.o \
	stm32f10x_dbgmcu.o \
	stm32f10x_dma.o \
	stm32f10x_exti.o \
	stm32f10x_flash.o \
	stm32f10x_fsmc.o \
	stm32f10x_gpio.o \
	stm32f10x_i2c.o \
	stm32f10x_iwdg.o \
	stm32f10x_pwr.o \
	stm32f10x_rcc.o \
	stm32f10x_rtc.o \
	stm32f10x_sdio.o \
	stm32f10x_spi.o \
	stm32f10x_tim.o \
	stm32f10x_usart.o \
	stm32f10x_wwdg.o

LIB := libstm32_lib.a

all: $(LIB)

libstm32_lib.a: $(OBJS)

.PHONY: clean

clean:
	rm -f 	$(OBJS) $(LIB)
