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

#ifndef CSP_PARSER_API_H
#define CSP_PARSER_API_H

#include <stddef.h>
#include <stdint.h>
#include "csp_defs.h"

/* names are stored in the following format:
 *   byte 1 = var name len (N)
 *   bytes 2..(N+1) variable name without null-terminating char
 *   byte (N+2) = another variable len
 *   ...
 *   buffer ends or null char
 */
struct csp_names {
    uint8_t *buf;
    uint16_t tail;
    uint16_t size;
    uint8_t idx;
};

extern CSP_EXTERNAL int csp_errno;
extern CSP_EXTERNAL int csp_errline;
#if CSP_STRING_ERRORS
extern CSP_EXTERNAL char csp_errstr[CSP_ERRSTR_SIZE];
#else
#define csp_errstr ""
#endif

void CSP_EXTERNAL csp_init(void);
void CSP_EXTERNAL csp_free(void);
void CSP_EXTERNAL csp_set_environ(const struct csp_names *functions);
int CSP_EXTERNAL csp_parse(uint8_t *prog_buffer, int size);
int CSP_EXTERNAL csp_get_program_size(uint8_t *prog_buffer, int size);

/* callbacks that application should define */
int csp_text_input_callback (char *buf, int max_size);
int csp_get_const_value(const char *name, int len, int *value);

#endif /* CSP_PARSER_API_H */

