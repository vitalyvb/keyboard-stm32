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

#include "led.h"
#include "matrix.h"
#include "main_dfukbd.h"
#include "kbd_config.h"

void rcc_init()
{
    /* diable jtag */
    /* moved to system_stm32f10x.c */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | HW_ENABLE_RCC_APB2_PERIPF, ENABLE);
}

void xdelay(int c)
{
    volatile int i;
    for (i=0;i<c;i++);
}

uint16_t kbd_read_row(int col)
{
    uint16_t val;
    matrix_select_col(col);
    xdelay(1000);
    val = matrix_read_rows();
    matrix_unselect_cols();
    return val;
}

int kbd_dfu_pressed()
{
    return kbd_read_row(MAGIC_KEY_ROW) == MAGIC_KEY_COLS;
}

int is_valid_app(void *address)
{
    uint32_t *sp_ptr = address;

    return ((sp_ptr[0]) & 0x2FFE0000) == 0x20000000;
}

void __attribute__((noinline, noreturn)) start_app(void *address)
{
    register void (*proc)(void);

    proc = *(void(**)())(address + sizeof(void*));

    /* Initialize application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) address);

    proc();
    while (1);
}


int main(void)
{
    rcc_init();
    matrix_init_all();

    if (!kbd_dfu_pressed()){
	/* magic keys are not pressed, try to run the app */
	if (is_valid_app(&_application_start)){
	    start_app(&_application_start);
	}
    }

    dfu_main();
    return 0;
}

