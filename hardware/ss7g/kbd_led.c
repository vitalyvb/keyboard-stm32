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

#include "stm32conf.h"
#include "hwdefs.h"
#include "led.h"

/*******************************************************************/

const struct portpin led_portpins[LED_COUNT] = {
    {.gpio = GPIOA, .pinmask = GPIO_Pin_9 },  /* NUM */
    {.gpio = GPIOA, .pinmask = GPIO_Pin_13 }, /* CAPS */
    {.gpio = GPIOA, .pinmask = GPIO_Pin_10 }, /* SCROLL */
    {}, /* COMPOSE */
    {}, /* KANA */
};

/*******************************************************************/

void led_init_all()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    int i;

    /* configure PA8 as LED+ output (TODO: PWM) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_8); // output +Vcc

    /* configure LED- as open-drain output */
    for (i=0;i<LED_COUNT;i++){
	if (led_portpins[i].gpio && led_portpins[i].pinmask){
	    GPIO_InitStructure.GPIO_Pin = led_portpins[i].pinmask;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	    GPIO_Init(led_portpins[i].gpio, &GPIO_InitStructure);

	    GPIO_SetBits(led_portpins[i].gpio, led_portpins[i].pinmask); // output +Vcc -> led OFF
	}
    }

}

void led_set_status(int led_id, int is_on)
{
    const struct portpin *pp;
    if (led_id < 0 || led_id >= LED_COUNT)
	return;

    pp = &led_portpins[led_id];

    if (pp->gpio && pp->pinmask){
	if (is_on)
	    GPIO_ResetBits(pp->gpio, pp->pinmask);
	else
	    GPIO_SetBits(pp->gpio, pp->pinmask);
    }
}
