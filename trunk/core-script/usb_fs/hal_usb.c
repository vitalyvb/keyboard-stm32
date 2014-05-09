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

#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "hal_usb.h"

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "usb_istr.h"
#include "usb_endp.h"

/******************************************************************************/

#define HAL_USB_DEVICE_UNCONNECTED	(0)
#define HAL_USB_DEVICE_ATTACHED		(1)
#define HAL_USB_DEVICE_SUSPENDED	(3)
#define HAL_USB_DEVICE_ADDRESSED	(4)
#define HAL_USB_DEVICE_CONFIGURED	(5)

int hal_usb_get_device_state()
{
    switch (bDeviceState) {
	case UNCONNECTED:
	    return HAL_USB_DEVICE_UNCONNECTED;
	case ATTACHED:
	    return HAL_USB_DEVICE_ATTACHED;
	case SUSPENDED:
	    return HAL_USB_DEVICE_SUSPENDED;
	case ADDRESSED:
	    return HAL_USB_DEVICE_ADDRESSED;
	case CONFIGURED:
	    return HAL_USB_DEVICE_CONFIGURED;
    }
    return -1;
}

void hal_usb_do_wakeup()
{
    Resume(RESUME_INTERNAL);
}

int hal_usb_remote_wakeup_enabled()
{
    return !!(pInformation->Current_Feature & 0x20);
}

int hal_usb_init()
{
    app_usb_start_clock();
    USB_Init();
    return 0;
}

void hal_usb_interrupt_lp()
{
    USB_Istr();
}

/******************************************************************************/

size_t hal_usb_ep_read(uint8_t ep_addr, uint8_t *buf, size_t size)
{
    /* XXX todo check size */
    return USB_SIL_Read(ep_addr, buf);
}

void hal_usb_ep_write(uint8_t ep_addr, const uint8_t *buf, size_t size)
{
    USB_SIL_Write(ep_addr, (uint8_t*)buf, size);
}

void hal_usb_ep_set_status(uint8_t ep_addr, uint32_t state)
{
    uint8_t epn = ep_addr & 0x7f;
    const uint16_t txstates[] = {
	[EP_STATE_VALID]  = EP_TX_VALID,
	[EP_STATE_DIS]    = EP_TX_DIS,
	[EP_STATE_STALL]  = EP_TX_STALL,
	[EP_STATE_NAK]    = EP_TX_NAK,
    };
    const uint16_t rxstates[] = {
	[EP_STATE_VALID]  = EP_RX_VALID,
	[EP_STATE_DIS]    = EP_RX_DIS,
	[EP_STATE_STALL]  = EP_RX_STALL,
	[EP_STATE_NAK]    = EP_RX_NAK,
    };

    if (ep_addr & 0x80){
	/* IN - send */
	SetEPTxStatus(epn, txstates[state]);
    } else {
	/* OUT - recv */
	SetEPRxStatus(epn, rxstates[state]);
    }
}

/******************************************************************************/

int hal_usb_can_send_report(uint32_t ep_num)
{
    if (ep_num < NUM_ENDPOINTS)
	return hid_ep_status[ep_num] == HID_EP_IDLE;
    return 0;
}

int hal_usb_send_report(uint32_t ep_num, const uint8_t *buf, size_t size)
{
    uint8_t ep_addr = 0x80 | ep_num; /* convert to IN ep_addr */

    if (ep_num >= NUM_ENDPOINTS)
	return -1;

    hal_usb_ep_write(ep_addr, buf, size);

    hid_ep_status[ep_num] = HID_EP_SENDING;
//    SetEPTxStatus(ep_num, EP_TX_VALID);
    hal_usb_ep_set_status(ep_addr, EP_STATE_VALID);
    return size;
}

/******************************************************************************/

void USB_Cable_Config(FunctionalState NewState)
{
    app_usb_cable_pullup(NewState != DISABLE);
}

void Suspend_Low(void)
{
    app_usb_suspend();
}
