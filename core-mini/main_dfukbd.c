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

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"

#include "main_dfukbd.h"
#include "kbd.h"
#include "keymap.h"

__IO uint32_t tick_counter;
__IO uint8_t do_usb_resume;

#define REPORT_STATUS_NO 0
#define REPORT_STATUS_PROC 1
#define REPORT_STATUS_DONE 2

__IO uint8_t kbd_layer;
__IO uint8_t kbd_report_status;
__IO uint8_t kbd_report_scanning[KBD_SIZE];
__IO uint8_t kbd_report_fullscan[KBD_SIZE];
__IO uint32_t usb_report_sent;
__IO uint32_t usb_report_idle;
__IO uint32_t usb_report_idle_timer;

#define LEDS_OVERRIDE_NONE (0)
#define LEDS_OVERRIDE_STARTUP (1)
__IO uint32_t leds_ticks;
__IO uint8_t leds_override;
__IO uint8_t leds_real_status;

#define FN1_LAYER (1)

void set_usb_kbd_leds(uint8_t leds)
{
    int i;

    leds_real_status = leds;

    if (leds_override != LEDS_OVERRIDE_NONE)
	return;

    for (i=0;i<LED_COUNT;i++){
	led_set_status(i, leds & (1<<i));
    }
}

void set_usb_kbd_idle(uint32_t idle)
{
    __disable_irq();

    usb_report_idle = idle;
    usb_report_idle_timer = idle;

    __enable_irq();
}

void kbd_suspend()
{
    int i;

    matrix_scan_stop();

    /* shutdown LEDs */
    for (i=0;i<LED_COUNT;i++){
	led_set_status(i, 0);
    }

    matrix_unselect_cols();

    if (usb_remote_wakeup_enabled()) {
	matrix_idle_wakeup_event_enter();
    }
}

void kbd_resume()
{
    matrix_idle_wakeup_event_leave();
    set_usb_kbd_leds(leds_real_status);
    matrix_scan_start();
}

void kbd_handle_exti_interrupt()
{
    do_usb_resume = 1;
    /* do not suspend here: high priority interrupt,
     * will be impossible to wakeup
     */
}

void handle_leds_override()
{
    if (likely(leds_ticks>0))
	return;

    switch (leds_override){
    case LEDS_OVERRIDE_STARTUP:
	led_set_status(LED_NUM_LOCK, 1);
	led_set_status(LED_CAPS_LOCK, 1);
	led_set_status(LED_SCROLL_LOCK, 1);
	leds_override++;
	leds_ticks = 500;
	break;
    case LEDS_OVERRIDE_STARTUP+1:
	leds_override = LEDS_OVERRIDE_NONE;
	set_usb_kbd_leds(leds_real_status);
	break;
    }
}

void kbd_release_keys(void)
{
    __disable_irq();

    memset((void*)kbd_report_scanning, 0, KBD_SIZE);
    memset((void*)kbd_report_fullscan, 0, KBD_SIZE);

    __enable_irq();
}

void dfu_usb_reset(void)
{
    __disable_irq();

    kbd_layer = 0;
    kbd_report_status = 0;
    tick_counter = 0;
    usb_report_sent = 1;
    memset((void*)kbd_report_scanning, 0, KBD_SIZE);
    memset((void*)kbd_report_fullscan, 0, KBD_SIZE);

    leds_ticks = 0;
    leds_override = LEDS_OVERRIDE_STARTUP;
    leds_real_status = 0;

    __enable_irq();
}

void dfu_main(void)
{
    int send_report;
    uint8_t kbd_report_loc[KBD_SIZE];

    usb_cable_reset();

    Set_System();

    matrix_init_all();
    led_init_all();
    matrix_scan_init();

    leds_override = LEDS_OVERRIDE_STARTUP;
    usb_report_sent = 1;
    Set_USBClock();
    USB_Init();

    matrix_scan_start();

    /* Main loop */
    send_report = 0;
    while (1){

	if (unlikely(do_usb_resume)){
	    /* Check if the remote wakeup feature is enabled (it could be disabled
	     * by the host through ClearFeature request)
	     */
	    if (bDeviceState != SUSPENDED) {
		/* do nothing */
		do_usb_resume = 0;
	    } else
	    if (usb_remote_wakeup_enabled()) {
		Resume(RESUME_INTERNAL);
		do_usb_resume = 0;
	    } else {
		/* resume disabled, disable kbd-triggered wakeup and go to the suspend state */
		matrix_idle_wakeup_event_leave();
		/* clear flag before suspend
		 * because the flag can be set again on resume
		 */
		do_usb_resume = 0; 
		Suspend_Low();
	    }
	    continue;
	}

	if (bDeviceState != CONFIGURED){
	    __WFI(); /* Wait for interrupt */
	    continue;
	}

	if (unlikely(leds_override)){
	    handle_leds_override();
	}

	__disable_irq();
	if ((kbd_report_status & REPORT_STATUS_DONE) && usb_report_sent){
	    kbd_report_status &= ~REPORT_STATUS_DONE;
	    send_report = 1;
	    memcpy((void*)kbd_report_loc, (void*)kbd_report_fullscan, KBD_SIZE);
	}
	if (unlikely((usb_report_idle_timer == 0) && (usb_report_idle != 0))){
	    send_report = 1;
	    usb_report_idle_timer = usb_report_idle;
	}
	__enable_irq();

	if (send_report){
	    usb_report_sent = 0;
	    send_report = 0;
	    USB_SIL_Write(EP1_IN, kbd_report_loc, KBD_SIZE);
	    SetEPTxCount(ENDP1, KBD_SIZE);
	    SetEPTxValid(ENDP1);
	}


	__WFI(); /* Wait for interrupt */
    }
}

void event_keydown(int col, int row)
{
    __IO uint8_t *rep = kbd_report_scanning;
    uint8_t key;
    int i;

    key = keymaps[kbd_layer][col][row];

    if (IS_MOD(key)){
	switch (key){
	    case KC_LCTRL:
		rep[0] |= 0x01;
		break;
	    case KC_LSHIFT:
		rep[0] |= 0x02;
		break;
	    case KC_LALT:
		rep[0] |= 0x04;
		break;
	    case KC_LGUI:
		rep[0] |= 0x08;
		break;
	    case KC_RCTRL:
		rep[0] |= 0x10;
		break;
	    case KC_RSHIFT:
		rep[0] |= 0x20;
		break;
	    case KC_RALT:
		rep[0] |= 0x40;
		break;
	    case KC_RGUI:
		rep[0] |= 0x80;
		break;
	}
	kbd_report_status |= REPORT_STATUS_PROC;
    } else
    if (IS_KEY(key)){
	for (i=2;i<KBD_SIZE;i++){
	    if (rep[i] == 0){
		rep[i] = key;
		kbd_report_status |= REPORT_STATUS_PROC;
		break;
	    }
	}
    } else
    if (key == KC_FN1){
	kbd_release_keys();
	kbd_layer = FN1_LAYER;
	kbd_report_status |= REPORT_STATUS_PROC;
    }
}

void event_keyup(int col, int row)
{
    __IO uint8_t *rep = kbd_report_scanning;
    uint8_t key;
    int i, j;

    key = keymaps[kbd_layer][col][row];

    if (IS_MOD(key)){
	switch (key){
	    case KC_LCTRL:
		rep[0] &= ~0x01;
		break;
	    case KC_LSHIFT:
		rep[0] &= ~0x02;
		break;
	    case KC_LALT:
		rep[0] &= ~0x04;
		break;
	    case KC_LGUI:
		rep[0] &= ~0x08;
		break;
	    case KC_RCTRL:
		rep[0] &= ~0x10;
		break;
	    case KC_RSHIFT:
		rep[0] &= ~0x20;
		break;
	    case KC_RALT:
		rep[0] &= ~0x40;
		break;
	    case KC_RGUI:
		rep[0] &= ~0x80;
		break;
	}
	kbd_report_status |= REPORT_STATUS_PROC;
    } else
    if (IS_KEY(key)){
	for (i=2;i<KBD_SIZE;i++){
	    if (rep[i] == key){
		for (j=i+1;j<KBD_SIZE;j++){
		    rep[j-1] = rep[j];
		}
		rep[KBD_SIZE-1] = 0;
		kbd_report_status |= REPORT_STATUS_PROC;
		break;
	    }
	}
    } else
    if (key == KC_FN1){
	kbd_release_keys();
	kbd_layer = 0;
	kbd_report_status |= REPORT_STATUS_PROC;
    }
}

void event_tick()
{
    if (kbd_report_status & REPORT_STATUS_PROC){
	memcpy((void*)kbd_report_fullscan, (void*)kbd_report_scanning, KBD_SIZE);
	kbd_report_status = REPORT_STATUS_DONE;
    }
    tick_counter++;

    if (unlikely(leds_ticks>0)){
	leds_ticks--;
    }

    if (usb_report_idle_timer>0){
	usb_report_idle_timer--;
    }
}
