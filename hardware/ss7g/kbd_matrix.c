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

    /* Cols - Hi-Z (Floating input)
     * Rows - Input with weak pull-down
     */

    /* configure PA0-PA7 as ROW input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
				  GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* configure COLs as inputs (not selected) */
    for (i=0;i<MATRIX_COLS;i++){
	GPIO_InitStructure.GPIO_Pin = column_portpins[i].pinmask;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(column_portpins[i].gpio, &GPIO_InitStructure);
    }

    prev_column_portpin = &column_portpins[0];
}

/* may be called from interrupt */
void matrix_select_col(uint32_t col)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    const struct portpin *col_port;

    /* unselect previous row, configure as floating input */
    GPIO_InitStructure.GPIO_Pin = prev_column_portpin->pinmask;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(prev_column_portpin->gpio, &GPIO_InitStructure);


    /* select row, configure as high output */
    col_port = &column_portpins[col];

    GPIO_InitStructure.GPIO_Pin = col_port->pinmask;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(col_port->gpio, &GPIO_InitStructure);

    GPIO_ResetBits(col_port->gpio, col_port->pinmask);

    prev_column_portpin = col_port;
}

void matrix_unselect_cols()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* unselect previous row, configure as floating input */
    GPIO_InitStructure.GPIO_Pin = prev_column_portpin->pinmask;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(prev_column_portpin->gpio, &GPIO_InitStructure);

    prev_column_portpin = &column_portpins[0];
}

/* may be called from interrupt */
uint32_t matrix_read_rows()
{
    return (~GPIO_ReadInputData(GPIOA)) & ((1<<MATRIX_ROWS)-1);
}

