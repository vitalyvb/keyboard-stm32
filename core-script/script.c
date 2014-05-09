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
#include "alloca.h"

#include "csp_api.h"
#include "csp_vm.h"

#include "util.h"
#include "hwdefs.h"
#include "vt100.h"
#include "kbd.h"

const unsigned char *constkeynames_kp;
const unsigned char *constkeynames;

struct script_state {
    int valid;
};


static uint8_t program_buffer[500];
static int program_size;
static struct script_state script_state;

static int load_script_eof;

int wait_for_program_end()
{
    int cnt=0;
    int c;

    while (cnt < 1024){
	c = console_usb_getchar();

	if (c == KEY_EOT)
	    return 0;

	if (c >= 0)
	    cnt++;
	else
	    app_yield();
    }
    return -1;
}

int csp_text_input_callback (char *buf, int max_size)
{
    int c, cnt=0;

    if (load_script_eof)
	return -1;

    while (max_size > 0){
	c = console_usb_getchar();

	if (c == KEY_EOT) {
	    /* ^D ends transmission */
	    load_script_eof = 1;
	    break;
	} else if (c >= 0) {
	    console_usb_putchar(c);
	    if (c == '\r'){
		console_usb_putchar('\n');
		c = '\n';
	    }

	    buf[cnt++] = c;
	    max_size--;
	} else {
	    if (cnt > 0) {
		/* return received data */
		break;
	    } else {
		/* idle */
		app_yield();
	    }
	}
    }

    console_usb_flush(0);
    return cnt;
}

void load_script_from_console()
{
    int res;

    load_script_eof = 0;
    res = csp_parse(program_buffer, sizeof(program_buffer));

    if (res) {
	do {
	    /* repeat message periodically */
	    Cprintf("\r\nError %d on line %d: %s\r\n", csp_errno, csp_errline, csp_errstr);
	} while (wait_for_program_end());

	return;
    }

    program_size = csp_get_program_size(program_buffer, sizeof(program_buffer));

    Cprintf("\r\nSuccess! Bytecode size %d\r\n", program_size);

    res = csp_vm_load_program(program_buffer, program_size, 0);
    if (res){
	Cprintf("\r\nError: program load error: %d\r\n", res);
    }
}

static int search_const_val(const uint8_t *table, const char *name, int len, int *value)
{
    const uint8_t *p = table;
    int val, l;

    while (1) {
	val = p[0];
	l = p[1];

	if (l == 0)
	    return -1;

	p += 2;

	if (l == len && (strncmp((const char*)p, name, len) == 0))
	    break;

	p += l;
    }

    *value = val;
    return 0;
}

int csp_get_const_value(const char *name, int len, int *value)
{
#define STARTS_WITH(_n_) ( (len >= (sizeof((_n_))-1)) && (strncmp((_n_), name, (sizeof((_n_))-1)) == 0) )
    if (STARTS_WITH("SYS_")) {
	/* no yet */
    } else if (STARTS_WITH("KP_")) {
	return search_const_val(constkeynames_kp, name+3, len-3, value);
    } else {
	return search_const_val(constkeynames, name, len, value);
    }

    return -1;
}

int csp_vm_api_call_callback(int num, int argc, int *argv, int *res)
{
    return -1;
}

const struct csp_names my_functions = {
    .buf = (uint8_t*)"",
    .idx = 0,
    .tail = 0,
};

void script_init(uint8_t *vmbuf, int vm_size)
{
    csp_init();
    csp_set_environ(&my_functions);

    memset(&script_state, 0, sizeof(struct script_state));


    csp_vm_init(vmbuf, vm_size);
}

