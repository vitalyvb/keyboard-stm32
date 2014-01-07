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

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "console_usb.h"
#include "shell.h"

static void cmd_info(int argc, const char * const * argv)
{
    Cprintf("hi\r\n");
}

static void cmd_dumpmem(int argc, const char * const * argv)
{
    uint8_t *ptr;
    char *eptr;
    uint32_t addr;
    uint32_t size;
    int idx;

    if (argc != 2)
	goto syntax_err;

    addr = strtoul(argv[0], &eptr, 0);
    if (!eptr || *eptr != 0){
	goto syntax_err;
    }
    size = strtoul(argv[1], &eptr, 0);
    if (!eptr || *eptr != 0 || size<=0){
	goto syntax_err;
    }

    if (size > 0x10000)
	size = 0x10000;

    ptr = (void*)addr;

    idx = 0;
    while (size > 0){
	if (idx == 0)
	    Cprintf("\r\n%08x: ", ptr);

	Cprintf("%02x ", *ptr++);

	idx = (idx + 1) & 0xf;
	size--;
    }

    Cprintf("\r\n");

    return;

syntax_err:
    Cprintf("Usage: dumpmem <addr> <count>\r\n");
}

static void cmd_help(int argc, const char * const * argv)
{
    const struct shell_command *cmd;
    int i;

    Cprintf("Commands:\r\n");
    for (i=0;i<shell_commands_count;i++){
	cmd = &shell_commands[i];
	Cprintf("    %-16s - %s\r\n", cmd->name, cmd->descr);
    }
}

/******************************************************************************/
/******************************************************************************/

const struct shell_command shell_commands[] = {
    {
	.name = "dumpmem",
	.descr = "Dump memory region",
	.handler = cmd_dumpmem,
    },
    {
	.name = "info",
	.descr = "Show generic information",
	.handler = cmd_info,
    },
    {
	.name = "help",
	.descr = "Show help",
	.handler = cmd_help,
    },
};

const int shell_commands_count = (sizeof(shell_commands)/sizeof(shell_commands[0]));
