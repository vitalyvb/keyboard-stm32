#include "stm32conf.h"
#include "core_cm3.c"

#include "libc.c"

#include "stm32f10x_rcc.c"
#include "stm32f10x_gpio.c"
#include "stm32f10x_exti.c"
#include "stm32f10x_tim.c"
#include "stm32f10x_flash.c"

#include "misc.c"

#include "usb_int.c"
#include "usb_init.c"
#include "usb_core.c"
#include "usb_mem.c"
#include "usb_regs.c"
#include "usb_sil.c"


#include "dfu_mal.c"
#include "flash_if.c"
#include "hw_config.c"
#include "main.c"
#include "stm32_it.c"
#include "system_stm32f10x.c"
#include "usb_istr.c"
#include "usb_prop.c"
#include "usb_pwr.c"
#include "usb_endp.c"

#include "keymap_boot.c"
#include "kbd_matrix.c"
#include "kbd_led.c"
#include "hwboard.c"

#include "main_dfukbd.c"
#include "matrix.c"

#define EXTERNAL __attribute__((externally_visible))

void EXTERNAL SystemInit (void);
void EXTERNAL NMI_Handler(void);
void EXTERNAL HardFault_Handler(void);
void EXTERNAL MemManage_Handler(void);
void EXTERNAL BusFault_Handler(void);
void EXTERNAL UsageFault_Handler(void);
void EXTERNAL SVC_Handler(void);
void EXTERNAL DebugMon_Handler(void);
void EXTERNAL PendSV_Handler(void);
void EXTERNAL SysTick_Handler(void);
void EXTERNAL USB_LP_IRQHandler(void);
void EXTERNAL USB_LP_CAN1_RX0_IRQHandler(void);
void EXTERNAL EXTI0_IRQHandler(void);
void EXTERNAL EXTI2_TS_IRQHandler(void);
void EXTERNAL EXTI15_10_IRQHandler(void);
void EXTERNAL USB_FS_WKUP_IRQHandler(void);
void EXTERNAL USBWakeUp_IRQHandler(void);
void EXTERNAL TIM2_IRQHandler(void);
