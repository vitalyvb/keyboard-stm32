/* Copyright (c) 2013, Vitaly Bursov <vitaly<AT>bursov.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "string.h"
#include "stm32conf.h"

#include "hwdefs.h"
#include "hal_usb.h"
#include "init.h"

#include "kbd_config.h"

/******************************************************************************/

static void rcc_init()
{
    /* diable jtag */
    /* moved to system_stm32f10x.c */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | HW_ENABLE_RCC_APB2_PERIPF, ENABLE);
}

static void nvic_init()
{
    /* 2 bit for pre-emption priority, 2 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/**
 *  USB
 */

static void usb_init_cable()
{
    /* no cable */
    /* pullup is always enabled */
}


static void usb_init_hw()
{
    EXTI_InitTypeDef EXTI_InitStructure;

#if (USB_MODEL == USB_FS_L) || (USB_MODEL == USB_FS_F3)
    /* Enable the SYSCFG module clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
#endif

#if (USB_MODEL == USB_FS_F3)
    /*Set PA11,12 as IN - USB_DM,DP*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*SET PA11,12 for USB: USB_DM,DP*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_14);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_14);
#endif

    /* USB wakeup line */
    EXTI_ClearITPendingBit(EXTI_Line18);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line18; 
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}


static void usb_init()
{
    usb_init_cable();
    usb_init_hw();
}

void app_usb_enable_interrupts()
{
    NVIC_InitTypeDef NVIC_InitStructure;

#if (USB_MODEL == USB_FS_L)
    /* Enable the USB interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the USB Wake-up interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USB_FS_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#elif (USB_MODEL == USB_FS_F3)
    /* Enable the USB interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the USB Wake-up interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#elif (USB_MODEL == USB_FS)
    /* Enable the USB interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the USB Wake-up interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
}

void app_usb_start_clock()
{
#if (USB_MODEL == USB_FS_L)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#elif  (USB_MODEL == USB_FS)

    /* Select USBCLK source */
    if (SystemCoreClock == 72000000) {
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
    } else {
	/* assume 48MHz */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);
    }

    /* Enable the USB clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#endif
}

/******************************************************************************/

#ifdef ERASE_HEAP
extern char __bss_end__;
static void __attribute__((__noinline__)) clear_memory()
{
    register uint32_t *stack_pos = (void*)__get_MSP();
    register uint32_t *heap = (uint32_t*)((((long)&__bss_end__) + 3) & (~3));

    stack_pos -= 4; // just in case
    while (heap < stack_pos)
//	*(heap++) = 0x41414141;
	*(heap++) = 0;
}
#endif

void hwinit()
{
#if ERASE_HEAP
    clear_memory();
#endif

    rcc_init();
    nvic_init();

    usb_init();
}

/******************************************************************************/

void app_usb_suspend()
{
    uint32_t tmpreg = 0;
    uint32_t savePWR_CR=0;

    /*prepare entry in low power mode (STOP mode)*/
    /* Select the regulator state in STOP mode*/
    savePWR_CR = PWR->CR;
    tmpreg = PWR->CR;
    /* Clear PDDS and LPDS bits */
    tmpreg &= ((uint32_t)0xFFFFFFFC);
    /* Set LPDS bit according to PWR_Regulator value */
    tmpreg |= PWR_Regulator_LowPower;
    /* Store the new value */
    PWR->CR = tmpreg;
    /* Set SLEEPDEEP bit of Cortex System Control Register */

#if defined (STM32F30X) || defined (STM32F37X)
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
#else
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
#endif

    __WFI();

    /*restore sleep mode configuration */ 
    /* restore Power regulator config in sleep mode*/
    PWR->CR = savePWR_CR;

    /* Reset SLEEPDEEP bit of Cortex System Control Register */
#if defined (STM32F30X) || defined (STM32F37X)
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
#else
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);
#endif

}
