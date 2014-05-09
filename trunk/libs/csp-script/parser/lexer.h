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

#ifndef LEXER_H
#define LEXER_H

#include "csp_defs.h"

struct ragel_lexer_t {

    char *te;		/* ragel */
    char *ts;		/* ragel */
    int cs;		/* ragel */
    int act;		/* ragel */

    char *buffer;
    uint16_t bufsize;

    /* state offsets */
    uint16_t p_offs;	/* ragel, offset from the buffer start */
    uint16_t pe_offs;	/* ragel, offset from the buffer start */

    /* other data */
    uint16_t lineno;
    uint8_t status;

    /* token information for a parser */
    char *token;
    uint16_t token_len;
};

#define LEX_STATUS_WIP		0
#define LEX_STATUS_EOF		1

#define LEX_STATUS_ERR		2 /* error states must be >= LEX_STATUS_ERR */
#define LEX_STATUS_OVERFLOW	3
#define LEX_STATUS_BAD_TOKEN	4

int scanner_init(struct ragel_lexer_t *YY, char *buffer, uint16_t size);

int yylex(void *yylval, void *lex_state);

#endif /* LEXER_H */
