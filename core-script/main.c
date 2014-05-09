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

#include "hw_config.h"
//#include "usb_lib.h"
//#include "usb_pwr.h"

#include "util.h"
#include "hwdefs.h"
#include "hal_usb.h"
#include "init.h"
#include "main.h"
#include "keyproc.h"
#include "vt100.h"
#include "kbd.h"
#include "script.h"

static __IO uint8_t do_usb_resume;


void app_usb_prepare_suspend()
{
    matrix_scan_stop();

    kbd_process_suspend();

    matrix_unselect_cols();

    if (hal_usb_remote_wakeup_enabled()) {
	matrix_idle_wakeup_event_enter();
    }
}

void app_usb_early_resume()
{
    /* restart clocks*/
    SystemInit();

    matrix_idle_wakeup_event_leave();
    kbd_process_resume();
    matrix_scan_start();
}

int app_usb_get_serial_num(uint8_t *dest)
{
    return Get_SerialNum(dest);
}

void app_usb_cable_pullup(int enable)
{
    // no-op, reserved
}

void kbd_handle_exti_interrupt()
{
    do_usb_resume = 1;
    /* do not suspend here: high priority interrupt,
     * will be impossible to wakeup
     */
}

void app_usb_reset(void)
{
    kbd_process_reset();
    vt100_reset();
}

void app_yield()
{
    /* XXX: what processes should be run here
     * this func is called from vt100, so
     * don't process vt100, only keys
     */

    /* XXX handle suspend and resume? */ 
    kbd_process_send_events();

    __WFI(); /* Wait for interrupt */
}

void main(void)
{
    uint8_t vmbuf[1000];

    hwinit();

    usb_cable_reset();

    matrix_init_all();
    matrix_scan_init();
    led_init_all();
    hal_usb_init();
    vt100_init();
    script_init(vmbuf, sizeof(vmbuf));

    kbd_process_reset();

    matrix_scan_start();

    /* Main loop */
    while (1){

	kbd_process_leds();

	if (unlikely(do_usb_resume)){
	    /* Check if the remote wakeup feature is enabled (it could be disabled
	     * by the host through ClearFeature request)
	     */
	    if (!HAL_USB_STATUS_SUSPENDED()) {
		/* do nothing */
		do_usb_resume = 0;
	    } else
	    if (hal_usb_remote_wakeup_enabled()) {
		hal_usb_do_wakeup();
		do_usb_resume = 0;
	    } else {
		/* resume disabled, disable kbd-triggered wakeup and go to the suspend state */
		matrix_idle_wakeup_event_leave();
		/* clear flag before suspend
		 * because the flag can be set again on resume
		 */
		do_usb_resume = 0; 
		app_usb_suspend();
	    }
	    continue;
	}

	if (!HAL_USB_STATUS_CONFIGURED()){
	    __WFI(); /* Wait for interrupt */
	    continue;
	}

	vt100_poll();

	kbd_process_send_events();

	__WFI(); /* Wait for interrupt */
    }
}

