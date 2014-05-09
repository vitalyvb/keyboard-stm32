#include "stm32conf.h"
#include "defs.h"
#include "core_cm3.c"

#include "libc.c"

#include "stm32f10x_rcc.c"
#include "stm32f10x_gpio.c"
#include "stm32f10x_exti.c"
#include "stm32f10x_tim.c"

#include "misc.c"

#include "hw_config.c"
#include "init.c"
#include "util.c"
#include "main.c"
#include "keyproc.c"
#include "stm32_it.c"
#include "system_stm32f10x.c"
#include "microrl.c"
#include "printf.c"

#include "keymap.c"
#include "kbd_matrix.c"
#include "kbd_led.c"
#include "hwboard.c"
#include "vt100.c"
#include "shell.c"
#include "console_usb.c"
#include "matrix.c"
#include "script.c"

#include "csp_api.c"
#include "gen.c"
#include "lexer.tab.c"
#include "parser.tab.c"
#include "vm.c"
#include "arrays.c"

#include "../tools/names_key.c"
#include "../tools/names_kp.c"

#include "hal_usb.h"
#if (USB_MODEL == USB_FS)
# include "usb_fs/hal_usb.c"
# include "usb_fs/usb_istr.c"
# include "usb_fs/usb_prop.c"
# include "usb_fs/usb_pwr.c"
# include "usb_fs/usb_endp.c"
# include "usb_int.c"
# include "usb_init.c"
# include "usb_core.c"
# include "usb_mem.c"
# include "usb_regs.c"
# include "usb_sil.c"
#endif

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
void EXTERNAL EXTI1_IRQHandler(void);
void EXTERNAL EXTI2_IRQHandler(void);
void EXTERNAL EXTI3_IRQHandler(void);
void EXTERNAL EXTI4_IRQHandler(void);
void EXTERNAL EXTI9_5_IRQHandler(void);
void EXTERNAL EXTI15_10_IRQHandler(void);
void EXTERNAL USB_FS_WKUP_IRQHandler(void);
void EXTERNAL USBWakeUp_IRQHandler(void);
void EXTERNAL TIM2_IRQHandler(void);
