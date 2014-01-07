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

#ifndef HAL_USB_H
#define HAL_USB_H

#include "stddef.h"
#include "stdint.h"

/**
 * USB controller detection
 */

#define USB_FS 1
#define USB_FS_L 2
#define USB_FS_F3 3

#define USB_OTG 10


#if defined(STM32F10X_LD) || defined(STM32F10X_MD) || defined (STM32F10X_HD) || defined (STM32F10X_XL) \
    /* excludes F1 connectivity line - STM32F105/7 */
    #define USB_MODEL USB_FS
#elif defined(STM32L1XX_MD) || defined(STM32L1XX_HD) || defined(STM32L1XX_MD_PLUS)
    #define USB_MODEL USB_FS_L
#elif defined(STM32F30X) || defined(STM32F37X)
    #define USB_MODEL USB_FS_F3
#elif defined(STM32F10X_CL) || defined(STM32F2XX) || defined(STM32F4XX)
    #define USB_MODEL USB_OTG
#else
    #error unknown mcu
#endif


#ifndef STM32F10X_MD
    #warning only STM32F10X_MD family is tested
#endif

#if (USB_MODEL != USB_FS)
    #warning USB_MODEL of USB_FS is supported only
#endif

/**
 * Common defines
 */

/* Endpoint Addresses (w/direction) */
#define EP0_OUT     ((uint8_t)0x00)
#define EP0_IN      ((uint8_t)0x80)
#define EP1_OUT     ((uint8_t)0x01)
#define EP1_IN      ((uint8_t)0x81)
#define EP2_OUT     ((uint8_t)0x02)
#define EP2_IN      ((uint8_t)0x82)
#define EP3_OUT     ((uint8_t)0x03)
#define EP3_IN      ((uint8_t)0x83)
#define EP4_OUT     ((uint8_t)0x04)
#define EP4_IN      ((uint8_t)0x84)
#define EP5_OUT     ((uint8_t)0x05)
#define EP5_IN      ((uint8_t)0x85)
#define EP6_OUT     ((uint8_t)0x06)
#define EP6_IN      ((uint8_t)0x86)
#define EP7_OUT     ((uint8_t)0x07)
#define EP7_IN      ((uint8_t)0x87)

/* endpoints enumeration */
#define ENDP0       ((uint8_t)0)
#define ENDP1       ((uint8_t)1)
#define ENDP2       ((uint8_t)2)
#define ENDP3       ((uint8_t)3)
#define ENDP4       ((uint8_t)4)
#define ENDP5       ((uint8_t)5)
#define ENDP6       ((uint8_t)6)
#define ENDP7       ((uint8_t)7)

/* endpoints states */
#define EP_STATE_VALID		0
#define EP_STATE_DIS		1
#define EP_STATE_STALL		2
#define EP_STATE_NAK		3

/**
 * Called from application regarding USB hardware
 */

/* initialization and hardware */
int hal_usb_init();
void hal_usb_interrupt_lp();
void hal_usb_interrupt_hp();

/* usb status */
#define HAL_USB_DEVICE_UNCONNECTED	(0)
#define HAL_USB_DEVICE_ATTACHED		(1)
#define HAL_USB_DEVICE_SUSPENDED	(3)
#define HAL_USB_DEVICE_ADDRESSED	(4)
#define HAL_USB_DEVICE_CONFIGURED	(5)

int hal_usb_get_device_state();

#define HAL_USB_STATUS_CONFIGURED() (hal_usb_get_device_state() == HAL_USB_DEVICE_CONFIGURED)
#define HAL_USB_STATUS_SUSPENDED() (hal_usb_get_device_state() == HAL_USB_DEVICE_SUSPENDED)

/* power management */
int hal_usb_remote_wakeup_enabled();
void hal_usb_do_wakeup();

/* high-level hid interface */
int hal_usb_can_send_report(uint32_t ep_num);
int hal_usb_send_report(uint32_t ep_num, const uint8_t *buf, size_t size);


/**
 * Called from USB driver as an application callbacks
 */

/* initialization and hardware */
void app_usb_reset();
void app_usb_cable_pullup(int enable);
void app_usb_enable_interrupts();
void app_usb_start_clock();

/* power management */
void app_usb_prepare_suspend();
void app_usb_suspend();
void app_usb_early_resume();

/* high-level hid interface */
int app_usb_get_kbd_report(uint8_t *dst_ptr);
void app_usb_set_kbd_report(uint8_t *src_ptr, size_t size);
int app_usb_set_kbd_idle(int report_id, uint32_t idle);

/* misc */
int app_usb_get_serial_num(uint8_t *dest);


/**
 * Service functions
 */

size_t hal_usb_ep_read(uint8_t ep_addr, uint8_t *buf, size_t size);
void hal_usb_ep_write(uint8_t ep_addr, const uint8_t *buf, size_t size);
void hal_usb_ep_set_status(uint8_t ep_addr, uint32_t state);

#endif /* HAL_USB_H */
