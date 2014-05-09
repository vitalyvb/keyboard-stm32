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
#include "stm32conf.h"
#include "stdlib.h"
#include "string.h"

#include "hal_usb.h"
#include "printf.h"
#include "console_usb.h"
#include "vt100.h"
#include "microrl.h"
#include "shell.h"

static char microrl_cmdline[_COMMAND_LINE_LEN];
static ring_history_t microrl_history;
static microrl_t microrl;
static vt100_handler vt100_redirect_handler;

/****************************************************************************/
/****************************************************************************/

// print to stream callback
static void rlprint (const char * str)
{
    int i;
    for (i=0;str[i];i++)
	console_usb_putchar(str[i]);
    console_usb_flush(0);
}

// execute callback
static int rlexecute (int argc, const char * const * argv)
{
    const struct shell_command *cmd;
    int i, found=0;

    if (argc < 1)
	return 0;

    for (i=0;i<shell_commands_count;i++){
	/* TODO: sort shell_commands and use a binary search */

	cmd = &shell_commands[i];

	if (strcmp(argv[0], cmd->name) == 0){
	    found = 1;
	    cmd->handler(argc-1, &argv[1]);
	    break;
	}

    }

    if (!found){
	Cprintf("Unknown command: %s\nType 'help' for help\n", argv[0]);
    }

    return 0;
}

// ctrl+c callback
static void rlsigint (void)
{
    // XXX todo: clear command line
    rlprint("\nOuch! CTRL+C !\n");
}

/****************************************************************************/
/****************************************************************************/

void vt100_poll()
{
    int c;

    c = console_usb_getchar();
    if (c >= 0){
	if (unlikely(vt100_redirect_handler))
	    vt100_redirect_handler(c);
	else
	    microrl_insert_char(&microrl, c);
    }
}

vt100_handler vt100_redirect(vt100_handler new_handler)
{
    vt100_handler old = vt100_redirect_handler;
    vt100_redirect_handler = new_handler;
    return old;
}

void vt100_reset()
{
    // XXX todo: clear command line
    console_usb_reset();
}

void vt100_init()
{
    console_usb_init();

    /* init microrl */

    microrl_init (&microrl, microrl_cmdline, &microrl_history, rlprint);
    microrl_set_execute_callback (&microrl, rlexecute);
//    microrl_set_complete_callback (&microrl, complet);
    microrl_set_sigint_callback(&microrl, rlsigint);
}

