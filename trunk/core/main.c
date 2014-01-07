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
#include "kbd.h"
#include "keymap.h"
#include "vt100.h"

__IO uint32_t tick_counter;
__IO uint8_t do_usb_resume;

#define REPORT_STATUS_NO 0
#define REPORT_STATUS_PROC 1
#define REPORT_STATUS_DONE 2

__IO uint8_t kbd_layer;
__IO uint8_t kbd_report_status;
__IO uint8_t kbd_report_scanning[KBD_SIZE];
__IO uint8_t kbd_report_fullscan[KBD_SIZE];
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

void app_usb_set_kbd_report(uint8_t *src_ptr, size_t size)
{
    set_usb_kbd_leds(src_ptr[0]);
}

int app_usb_get_kbd_report(uint8_t *dst_ptr)
{
    atomic_memcpy(dst_ptr, (void*)kbd_report_fullscan, KBD_SIZE);
    return 0;
}

int app_usb_set_kbd_idle(int report_id, uint32_t idle)
{
    uint32_t irqstate;
    if (report_id != 0)
	return -1;

    irqstate = disable_irq();

    usb_report_idle = idle;
    usb_report_idle_timer = idle;

    restore_irq(irqstate);

    return 0;
}

void app_usb_prepare_suspend()
{
    int i;

    matrix_scan_stop();

    /* shutdown LEDs */
    for (i=0;i<LED_COUNT;i++){
	led_set_status(i, 0);
    }

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
    set_usb_kbd_leds(leds_real_status);
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
    uint32_t irqstate = disable_irq();

    memset((void*)kbd_report_scanning, 0, KBD_SIZE);
    memset((void*)kbd_report_fullscan, 0, KBD_SIZE);

    restore_irq(irqstate);
}

void app_usb_reset(void)
{
    uint32_t irqstate = disable_irq();

    kbd_layer = 0;
    kbd_report_status = 0;
    tick_counter = 0;
    memset((void*)kbd_report_scanning, 0, KBD_SIZE);
    memset((void*)kbd_report_fullscan, 0, KBD_SIZE);

    leds_ticks = 0;
    leds_override = LEDS_OVERRIDE_STARTUP;
    leds_real_status = 0;

    restore_irq(irqstate);

    vt100_reset();
}

void main(void)
{
    uint32_t irqstate;
    int send_report;
    uint8_t kbd_report_loc[KBD_SIZE];

    hwinit();

    usb_cable_reset();


    matrix_init_all();
    led_init_all();
    matrix_scan_init();

    leds_override = LEDS_OVERRIDE_STARTUP;

    hal_usb_init();

    vt100_init();

    matrix_scan_start();

    /* Main loop */
    send_report = 0;
    while (1){

	if (unlikely(leds_override)){
	    handle_leds_override();
	}

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


	irqstate = disable_irq();
	if ((kbd_report_status & REPORT_STATUS_DONE) && hal_usb_can_send_report(KBD_ENDPOINT)){
	    kbd_report_status &= ~REPORT_STATUS_DONE;
	    send_report = 1;
	    memcpy((void*)kbd_report_loc, (void*)kbd_report_fullscan, KBD_SIZE);
	}
	if (unlikely((usb_report_idle_timer == 0) && (usb_report_idle != 0))){
	    send_report = 1;
	    usb_report_idle_timer = usb_report_idle;
	}
	restore_irq(irqstate);

	if (send_report){
	    send_report = 0;
	    hal_usb_send_report(KBD_ENDPOINT, kbd_report_loc, KBD_SIZE);
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
