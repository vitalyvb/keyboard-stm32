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

#include "matrix.h"

/*******************************************************************/
#define DEBOUNCE_TIMER_PRESS (2)
#define DEBOUNCE_TIMER_RELEASE (8)

struct debounce_status {
    uint8_t  key_down:1;
    uint8_t  counter:7;
};

static int selected_col;
static struct debounce_status debouncer[MATRIX_COLS][MATRIX_ROWS];

/*******************************************************************/

void matrix_scan_init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef base_timer;

    TIM_TimeBaseStructInit(&base_timer);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // XXX assume 72MHz clock
    base_timer.TIM_Prescaler = (72) - 1;
    base_timer.TIM_Period = 1000/MATRIX_COLS; // should check all cols in 1ms (72M/72/(1000/MATRIX_COLS))
                                              // tick event generated after full scan - every 1ms
    TIM_TimeBaseInit(TIM2, &base_timer);


    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    /* Enable the Timer Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void matrix_scan_start()
{
    TIM_SetCounter(TIM2, 0);
    TIM_Cmd(TIM2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void matrix_scan_stop()
{
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
}


static void process_key(int col, int row, int down)
{
    struct debounce_status *status = &debouncer[col][row];

    if (!down){
	/* KEY RELEASED */

	if (likely(status->counter == 0)){
	    /* key is released, do nothing */
	    return;
	} else if (status->counter == 1){
	    if (status->key_down)
		event_keyup(col, row);
	    status->key_down = 0;
	    status->counter--;
	} else if (status->counter > 0){
	    status->counter--;
	}

    } else {
	/* KEY DOWN */

	if (likely(status->counter >= DEBOUNCE_TIMER_PRESS)){
	    /* key is still pressed, do nothing */
	    return;
	} else if (status->counter < DEBOUNCE_TIMER_PRESS-1){
	    status->counter++;
	} else if (status->counter == DEBOUNCE_TIMER_PRESS-1){
	    if (!status->key_down)
		event_keydown(col, row);
	    status->key_down = 1;
	    status->counter = DEBOUNCE_TIMER_RELEASE;
	    //status->counter++;
	}

    }
}

void matrix_timer_interrupt()
{
    uint32_t rows;
    int i;

    rows = matrix_read_rows();

    for (i=0;i<MATRIX_ROWS;i++){
	process_key(selected_col, i, (rows & (1<<i)) != 0);
    }

    selected_col++;
    if (selected_col >= MATRIX_COLS){
	selected_col = 0;
	event_tick();
    }
    matrix_select_col(selected_col);
}
