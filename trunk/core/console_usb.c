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
#include <stdarg.h>
#include "string.h"
#include "printf.h"
#include "stm32conf.h"
#include "hal_usb.h"

#include "console_usb.h"

#include "usb_conf.h"

#define DEBUG_RX 0
#define DEBUG_TX 1

struct debug_io {
    char *buffer;
    uint8_t buffer_size;
    volatile uint8_t swptr;
    volatile uint8_t hwptr;
};


static void _intern_tx_pending();


static struct debug_io debug_io[2];

static char debug_tx_buffer[DEBUG_SIZE_DATA*2]; /* tx and rx ring buffers */
static char debug_rx_buffer[DEBUG_SIZE_DATA*2];

static __IO uint32_t debug_recv_enable;
static __IO uint32_t debug_sending;

/****************************************************************************/
/****************************************************************************/

static size_t get_buffer_free_space(size_t size, size_t swptr, size_t hwptr)
{
    if (swptr <= hwptr){
	return size - (hwptr - swptr) - 1;
    } else {
	return swptr - hwptr - 1;
    }
}

void EP6_OUT_Callback(void)
{
    struct debug_io *p = &debug_io[DEBUG_RX];
    uint8_t buffer[DEBUG_SIZE_DATA];
    uint32_t datasize;
    uint32_t newhwptr, bptr, swptr;
    uint32_t free_space;

    datasize = hal_usb_ep_read(EP6_OUT, buffer, DEBUG_SIZE_DATA);

    __disable_irq();

    bptr = p->hwptr;
    swptr = p->swptr;

    free_space = get_buffer_free_space(p->buffer_size, swptr, bptr);

    __enable_irq();

    /* avoid overflow, discard extra data, nothing can be done */
    if (datasize > free_space)
	datasize = free_space;

    newhwptr = bptr + datasize;
    if (newhwptr >= p->buffer_size)
	newhwptr -= p->buffer_size;

    free_space -= datasize;

    if (bptr < newhwptr){
	memcpy(&p->buffer[bptr], buffer, datasize);
    } else {
	/* split buffer */
	uint32_t til_end = p->buffer_size - bptr;
	memcpy(&p->buffer[bptr], buffer, til_end);
	memcpy(p->buffer, &buffer[til_end], datasize-til_end);
    }

    p->hwptr = newhwptr;

    if (free_space >= DEBUG_SIZE_DATA){
	hal_usb_ep_set_status(EP6_OUT, EP_STATE_VALID);
    } else {
	debug_recv_enable = 0;
    }
}


void EP6_IN_Callback(void)
{
    struct debug_io *p = &debug_io[DEBUG_TX];
    int again;

    /* in case we get an interrupt and its handler does debug output */
    __disable_irq();
    again = (p->swptr != p->hwptr);
    if (!again){
	debug_sending = 0;
    }
    __enable_irq();

    if (again) {
	_intern_tx_pending();
    }
}


static void _intern_tx_pending()
{
    struct debug_io *p = &debug_io[DEBUG_TX];
    uint32_t newhwptr;
    uint8_t to_send;
    uint8_t send_ptr;

    __disable_irq();

    send_ptr = p->hwptr;
    if (p->swptr == send_ptr){
	// err
	debug_sending = 0;
	__enable_irq();
	return;
    } else
    if (p->swptr > send_ptr){
	to_send = p->swptr - send_ptr;
    } else {
	to_send = p->buffer_size - send_ptr + p->swptr;
    }

    __enable_irq();

    if (to_send > DEBUG_SIZE_DATA)
	to_send = DEBUG_SIZE_DATA;

    newhwptr = send_ptr + to_send;
    if (newhwptr >= p->buffer_size)
	newhwptr -= p->buffer_size;



    if (to_send == 0){
	// ???
	debug_sending = 0;
	return;
    }

    if (newhwptr < send_ptr){
	/* buffer wrapped */
	uint8_t til_end = p->buffer_size - send_ptr;

//	needs aligned data and even lengths
//
//	UserToPMABufferCopy((void*)&p->buffer[send_ptr], GetEPTxAddr(EP2_IN & 0x7F), til_end);
//	if ((to_send - til_end) > 0)
//	    UserToPMABufferCopy((void*)&p->buffer[0], GetEPTxAddr(EP2_IN & 0x7F)+til_end, to_send-til_end);
//	SetEPTxCount((EP2_IN & 0x7F), to_send);

	uint8_t bounce[to_send];

	if (til_end > 0);
	    memcpy(&bounce[0], &p->buffer[send_ptr], til_end);
	if (to_send-til_end > 0)
	    memcpy(&bounce[til_end], &p->buffer[0], to_send-til_end);

	hal_usb_ep_write(EP6_IN, bounce, to_send);
    } else {
	hal_usb_ep_write(EP6_IN, (void*)&p->buffer[send_ptr], to_send);
    }

    hal_usb_ep_set_status(EP6_IN, EP_STATE_VALID);

    p->hwptr = newhwptr;
}

/****************************************************************************/
/****************************************************************************/

static void console_usb_enable_tx()
{
    /* are we're in progress already? */
    __disable_irq();
    if (debug_sending){
	__enable_irq();
	return;
    }
    debug_sending = 1;
    __enable_irq();

    _intern_tx_pending();
}


void console_usb_putchar(char c)
{
    struct debug_io *p = &debug_io[DEBUG_TX];
#if 0
    volatile int n=0;
#endif

    if (!HAL_USB_STATUS_CONFIGURED())
	return;

    __disable_irq();
    if (p->swptr == p->hwptr){
	/* buffer is empty */
    } else if (p->swptr == p->buffer_size-1){
	/* wait at least 1 char free in buffer */
	if (p->hwptr == 0){
	    __enable_irq();
	    console_usb_enable_tx();
#if 1
	    while (p->hwptr == 0);
#else
	    while (p->hwptr == 0 && n++<1000);
	    if (n>=1000){
		return;
	    }
#endif
	    __disable_irq();
	}
    } else {
	char tmp = p->swptr + 1;
	/* wait at least 1 char free in buffer */
	if (p->hwptr == tmp){
	    __enable_irq();
	    console_usb_enable_tx();
#if 1
	    while (p->hwptr == tmp);
#else
	    while (p->hwptr == tmp && n++<1000);
	    if (n>=1000){
		return;
	    }
#endif
	    __disable_irq();
	}
    }

    p->buffer[p->swptr++] = c;
    if (p->swptr >= p->buffer_size){
	p->swptr = 0;
    }
    __enable_irq();
}

int console_usb_getchar()
{
    struct debug_io *p = &debug_io[DEBUG_RX];
    uint32_t swptr;
    uint32_t free_space;
    uint8_t c;

    if (p->swptr == p->hwptr)
	return -1;

    __disable_irq(); // XXX is it really required here?
    swptr = p->swptr;

    c = p->buffer[swptr++];
    if (swptr >= p->buffer_size){
	swptr = 0;
    }

    p->swptr = swptr;

    if (debug_recv_enable){
	/* data receiving is already enabled */
	__enable_irq();
    } else {
	/* data receiving is disabled, check if free buffer space can handle usb packet */
	free_space = get_buffer_free_space(p->buffer_size, swptr, p->hwptr);
	__enable_irq();

	if (free_space >= DEBUG_SIZE_DATA){
	    hal_usb_ep_set_status(EP6_OUT, EP_STATE_VALID);
	    debug_recv_enable = 1;
	}
    }

    return c;
}


void console_usb_flush(int block)
{
    struct debug_io *p = &debug_io[DEBUG_TX];

    if (HAL_USB_STATUS_CONFIGURED()){
	if (p->hwptr != p->swptr)
	    console_usb_enable_tx();
	if (block)
	    while (p->hwptr != p->swptr);
    } else {
	/* not connected, erase buffers */
	__disable_irq();
	p->swptr = 0;
	p->hwptr = 0;
	__enable_irq();
    }
}

void console_usb_reset()
{
    debug_io[DEBUG_TX].swptr = 0;
    debug_io[DEBUG_TX].hwptr = 0;
    debug_io[DEBUG_RX].swptr = 0;
    debug_io[DEBUG_RX].hwptr = 0;

    hal_usb_ep_set_status(EP6_OUT, EP_STATE_VALID);
}

void console_usb_init()
{
    /* init buffers and io */
    debug_io[DEBUG_TX].buffer = debug_tx_buffer;
    debug_io[DEBUG_TX].buffer_size = sizeof(debug_tx_buffer);
    debug_io[DEBUG_TX].swptr = 0;
    debug_io[DEBUG_TX].hwptr = 0;

    debug_io[DEBUG_RX].buffer = debug_rx_buffer;
    debug_io[DEBUG_RX].buffer_size = sizeof(debug_rx_buffer);
    debug_io[DEBUG_RX].swptr = 0;
    debug_io[DEBUG_RX].hwptr = 0;
}

/****************************************************************************/
/****************************************************************************/

static void console_putc(void *p, char c)
{
    console_usb_putchar(c);
}

void Cprintf(char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    tfp_format(NULL, console_putc, fmt, va);
    va_end(va);
    console_usb_flush(0);
}

