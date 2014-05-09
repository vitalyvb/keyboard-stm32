/* Copyright (c) 2014, Vitaly Bursov <vitaly<AT>bursov.com>
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

#include "csp_incl.h"
#include "csp_defs.h"
#include "csp_api.h"

#include "lexer.h"
#include "parser.tab.h"
#include "gen.h"

int csp_errno;
int csp_errline;
#if CSP_STRING_ERRORS
char csp_errstr[CSP_ERRSTR_SIZE];
#endif

static const struct csp_names *env_functions;

static void _csp_clear_error(void)
{
    csp_errno = 0;
    csp_errline = 0;
#if CSP_STRING_ERRORS
    csp_errstr[0] = 0;
#endif
}

void CSP_EXTERNAL csp_init(void)
{
    _csp_clear_error();
}

void CSP_EXTERNAL csp_free(void)
{

}

void CSP_EXTERNAL csp_set_environ(const struct csp_names *functions)
{
    env_functions = functions;
}

int CSP_EXTERNAL csp_parse(uint8_t *prog_buffer, int prog_size)
{
    char lex_buf[CSP_LEX_BUFSIZE];

    struct csp_names global, functions, local;
    struct ragel_lexer_t lex;
    int res;

    _csp_clear_error();
    gen_init(prog_buffer, prog_size);

    scanner_init(&lex, lex_buf, CSP_LEX_BUFSIZE);

    global.idx = 0;
    global.tail = 0;
    global.size = CSP_NS_GLOBAL_NAMES_SIZE;
    global.buf = alloca(global.size);
    global.buf[0] = 0;

    functions.idx = env_functions->idx + MACRO_OP_BUILTIN_COUNT;
    functions.tail = env_functions->tail;
    functions.size = functions.tail + CSP_NS_FUNCTIONS_NAMES_SIZE;
    functions.buf = alloca(functions.size);
    memcpy(functions.buf, env_functions->buf, functions.tail+1); /* copy '\0' char too */

    local.idx = 0;
    local.tail = 0;
    local.size = CSP_NS_LOCAL_NAMES_SIZE;
    local.buf = alloca(local.size);
    local.buf[0] = 0;

    gen_set_namespaces(&global, &functions, &local);

    res = yyparse(&lex);

    if (res == 0)
	program_finish();

    return res;
}

int CSP_EXTERNAL csp_get_program_size(uint8_t *prog_buffer, int size)
{
    if (size < PROGRAM_HEADER_SIZE)
	return -1;

    return prog_buffer[0] | (prog_buffer[1] << 8);
}


void _csp_set_error(int err, int line, const char *msg, ...)
{
    va_list va;

    /* XXX register first error only ? */
    if (csp_errno && csp_errline)
	return;

    csp_errno = -err;
    csp_errline = line;

    if (msg) {
	va_start(va, msg);
	vsnprintf(csp_errstr, CSP_ERRSTR_SIZE, msg, va);
	va_end(va);
    }
}


