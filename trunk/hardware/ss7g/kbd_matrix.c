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
#include "matrix.h"
#include "hwdefs.h"

/*******************************************************************/

const struct portpin column_portpins[] = {
    {.gpio = GPIOA, .pinmask = GPIO_Pin_14 },
    {.gpio = GPIOA, .pinmask = GPIO_Pin_15 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_0 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_1 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_2 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_3 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_4 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_5 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_8 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_9 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_10 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_11 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_12 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_13 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_14 },
    {.gpio = GPIOB, .pinmask = GPIO_Pin_15 },
};

__IO const struct portpin *prev_column_portpin;

/*******************************************************************/

void matrix_init_all()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    int i;

    /* Cols - Open-drain output
     * Rows - Input with weak pull-up
     */

    /* configure PA0-PA7 as ROW input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
				  GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* configure COLs as outputs, set level Hi */
    for (i=0;i<MATRIX_COLS;i++){
	GPIO_InitStructure.GPIO_Pin = column_portpins[i].pinmask;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(column_portpins[i].gpio, &GPIO_InitStructure);
	GPIO_SetBits(column_portpins[i].gpio, column_portpins[i].pinmask);
    }

    prev_column_portpin = &column_portpins[0];

    /* configure EXTI on ROWs to handle wakeup and
     * resume from keypress
     */

    for (i=0;i<8;i++){
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, i);
    }

}

/* may be called from interrupt */
void matrix_select_col(uint32_t col)
{
    const struct portpin *col_port;

    /* unselect previous row, open-drain output, hi */
    GPIO_SetBits(prev_column_portpin->gpio, prev_column_portpin->pinmask);

    /* select row, open-drain output, low */
    col_port = &column_portpins[col];
    GPIO_ResetBits(col_port->gpio, col_port->pinmask);

    prev_column_portpin = col_port;
}

void matrix_unselect_cols()
{
    /* unselect previous row, open-drain output, hi */
    GPIO_SetBits(prev_column_portpin->gpio, prev_column_portpin->pinmask);
    prev_column_portpin = &column_portpins[0];
}

/* may be called from interrupt */
uint32_t matrix_read_rows()
{
    return (~GPIO_ReadInputData(GPIOA)) & ((1<<MATRIX_ROWS)-1);
}


void matrix_idle_wakeup_event_enter()
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    int i;
    const int ints_list[] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn,
			EXTI4_IRQn, EXTI9_5_IRQn};

    /* Configure EXTIn lines */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line3|
				   EXTI_Line4|EXTI_Line5|EXTI_Line6|EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* set COLs Low */
    for (i=0;i<MATRIX_COLS;i++){
	GPIO_ResetBits(column_portpins[i].gpio, column_portpins[i].pinmask);
    }

    EXTI_ClearFlag(EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line3|
		   EXTI_Line4|EXTI_Line5|EXTI_Line6|EXTI_Line7);

    /* Enable and set Interrupts to the lowest priority */
    for (i=0;i<sizeof(ints_list)/sizeof(ints_list[0]);i++){
	/* If controller is STOPped from the interrupt (most likely it is,
	 * from USB low priority interrupt) then wakeup EXTI interrupts
	 * must have higher priority to trigger wakeup.
	 * Assign highest possible priority here.
	 */
	NVIC_InitStructure.NVIC_IRQChannel = ints_list[i];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    }
}

void matrix_idle_wakeup_event_leave()
{
    EXTI_InitTypeDef EXTI_InitStructure;
    int i;

    /* set COLs Hi */
    for (i=0;i<MATRIX_COLS;i++){
	GPIO_SetBits(column_portpins[i].gpio, column_portpins[i].pinmask);
    }

    /* Configure EXTIn lines */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line3|
				   EXTI_Line4|EXTI_Line5|EXTI_Line6|EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_DisableIRQ(EXTI0_IRQn);
    NVIC_DisableIRQ(EXTI1_IRQn);
    NVIC_DisableIRQ(EXTI2_IRQn);
    NVIC_DisableIRQ(EXTI3_IRQn);
    NVIC_DisableIRQ(EXTI4_IRQn);
    NVIC_DisableIRQ(EXTI9_5_IRQn);

    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    NVIC_ClearPendingIRQ(EXTI1_IRQn);
    NVIC_ClearPendingIRQ(EXTI2_IRQn);
    NVIC_ClearPendingIRQ(EXTI3_IRQn);
    NVIC_ClearPendingIRQ(EXTI4_IRQn);
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
}
