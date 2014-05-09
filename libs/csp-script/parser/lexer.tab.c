
#line 1 "lexer.rl"
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
#include "parser.tab.h"
#include "gen.h"
#include "lexer.h"

#define TOKEN_UNDEFINED -2

#define YY_PREPARE_TOK do { saved_tok_char = *YY->te; *YY->te = 0; } while (0)
#define YY_RELEASE_TOK do { *YY->te = saved_tok_char; } while (0)


#line 108 "lexer.rl"



#line 47 "lexer.tab.c"
static const short _clex_key_offsets[] = {
	0, 0, 5, 11, 50, 51, 58, 59, 
	60, 61, 64, 66, 72, 73, 74, 75, 
	83, 91, 99, 107, 115, 123, 131, 139, 
	147, 155, 163, 171, 179, 187, 195, 204, 
	212, 220, 228, 236, 244, 252, 260, 268, 
	276, 284, 292, 300, 308, 316, 324, 332, 
	340, 348, 356, 364, 372, 380, 388, 396
};

static const unsigned char _clex_trans_keys[] = {
	95u, 65u, 90u, 97u, 122u, 48u, 57u, 65u, 
	70u, 97u, 102u, 10u, 32u, 33u, 36u, 37u, 
	38u, 47u, 48u, 59u, 60u, 61u, 62u, 91u, 
	95u, 98u, 99u, 100u, 101u, 102u, 103u, 105u, 
	109u, 114u, 119u, 124u, 9u, 13u, 40u, 45u, 
	49u, 57u, 65u, 90u, 93u, 94u, 97u, 122u, 
	123u, 126u, 61u, 95u, 48u, 57u, 65u, 90u, 
	97u, 122u, 38u, 47u, 10u, 120u, 48u, 57u, 
	48u, 57u, 48u, 57u, 65u, 70u, 97u, 102u, 
	61u, 61u, 61u, 95u, 114u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 101u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 97u, 48u, 57u, 65u, 
	90u, 98u, 122u, 95u, 107u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 111u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 110u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 116u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 105u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 110u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 117u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 101u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 111u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 108u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 115u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 101u, 48u, 57u, 65u, 
	90u, 97u, 122u, 95u, 111u, 117u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 114u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 110u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 99u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 116u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 105u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 111u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 110u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 108u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 111u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 98u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 97u, 48u, 57u, 
	65u, 90u, 98u, 122u, 95u, 108u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 102u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 97u, 48u, 57u, 
	65u, 90u, 98u, 122u, 95u, 112u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 101u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 116u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 117u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 114u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 110u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 104u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 105u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 108u, 48u, 57u, 
	65u, 90u, 97u, 122u, 95u, 101u, 48u, 57u, 
	65u, 90u, 97u, 122u, 124u, 0
};

static const char _clex_single_lengths[] = {
	0, 1, 0, 25, 1, 1, 1, 1, 
	1, 1, 0, 0, 1, 1, 1, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 3, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 1
};

static const char _clex_range_lengths[] = {
	0, 2, 3, 7, 0, 3, 0, 0, 
	0, 1, 1, 3, 0, 0, 0, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 0
};

static const short _clex_index_offsets[] = {
	0, 0, 4, 8, 41, 43, 48, 50, 
	52, 54, 57, 59, 63, 65, 67, 69, 
	75, 81, 87, 93, 99, 105, 111, 117, 
	123, 129, 135, 141, 147, 153, 159, 166, 
	172, 178, 184, 190, 196, 202, 208, 214, 
	220, 226, 232, 238, 244, 250, 256, 262, 
	268, 274, 280, 286, 292, 298, 304, 310
};

static const char _clex_indicies[] = {
	0, 0, 0, 1, 3, 3, 3, 2, 
	5, 4, 6, 7, 8, 9, 10, 11, 
	8, 13, 14, 15, 8, 0, 16, 17, 
	18, 19, 20, 21, 22, 23, 24, 25, 
	26, 4, 8, 12, 0, 8, 0, 8, 
	1, 28, 27, 0, 0, 0, 0, 29, 
	30, 27, 31, 27, 34, 33, 36, 12, 
	35, 12, 35, 3, 3, 3, 35, 37, 
	27, 38, 27, 39, 27, 0, 41, 0, 
	0, 0, 40, 0, 42, 0, 0, 0, 
	40, 0, 43, 0, 0, 0, 40, 0, 
	44, 0, 0, 0, 40, 0, 45, 0, 
	0, 0, 40, 0, 46, 0, 0, 0, 
	40, 0, 47, 0, 0, 0, 40, 0, 
	48, 0, 0, 0, 40, 0, 49, 0, 
	0, 0, 40, 0, 50, 0, 0, 0, 
	40, 0, 51, 0, 0, 0, 40, 0, 
	52, 0, 0, 0, 40, 0, 53, 0, 
	0, 0, 40, 0, 54, 0, 0, 0, 
	40, 0, 55, 0, 0, 0, 40, 0, 
	56, 57, 0, 0, 0, 40, 0, 58, 
	0, 0, 0, 40, 0, 59, 0, 0, 
	0, 40, 0, 60, 0, 0, 0, 40, 
	0, 61, 0, 0, 0, 40, 0, 62, 
	0, 0, 0, 40, 0, 63, 0, 0, 
	0, 40, 0, 64, 0, 0, 0, 40, 
	0, 65, 0, 0, 0, 40, 0, 66, 
	0, 0, 0, 40, 0, 67, 0, 0, 
	0, 40, 0, 68, 0, 0, 0, 40, 
	0, 69, 0, 0, 0, 40, 0, 70, 
	0, 0, 0, 40, 0, 71, 0, 0, 
	0, 40, 0, 72, 0, 0, 0, 40, 
	0, 73, 0, 0, 0, 40, 0, 74, 
	0, 0, 0, 40, 0, 75, 0, 0, 
	0, 40, 0, 76, 0, 0, 0, 40, 
	0, 77, 0, 0, 0, 40, 0, 78, 
	0, 0, 0, 40, 0, 79, 0, 0, 
	0, 40, 0, 80, 0, 0, 0, 40, 
	0, 81, 0, 0, 0, 40, 82, 27, 
	0
};

static const char _clex_trans_targs[] = {
	5, 0, 3, 11, 3, 3, 4, 1, 
	3, 6, 7, 9, 10, 12, 13, 14, 
	15, 19, 26, 27, 30, 38, 43, 44, 
	46, 51, 55, 3, 3, 3, 3, 8, 
	3, 8, 3, 3, 2, 3, 3, 3, 
	3, 16, 17, 18, 5, 20, 21, 22, 
	23, 24, 25, 5, 5, 28, 29, 5, 
	31, 32, 5, 33, 34, 35, 36, 37, 
	5, 39, 40, 41, 42, 5, 5, 45, 
	5, 47, 48, 49, 50, 5, 52, 53, 
	54, 5, 3
};

static const char _clex_trans_actions[] = {
	1, 0, 2, 0, 5, 6, 0, 0, 
	7, 0, 0, 8, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 9, 10, 11, 12, 0, 
	13, 14, 15, 16, 0, 17, 18, 19, 
	20, 0, 0, 0, 21, 0, 0, 0, 
	0, 0, 0, 22, 23, 0, 0, 24, 
	0, 0, 25, 0, 0, 0, 0, 0, 
	26, 0, 0, 0, 0, 27, 28, 0, 
	29, 0, 0, 0, 0, 30, 0, 0, 
	0, 31, 32
};

static const char _clex_to_state_actions[] = {
	0, 0, 0, 3, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

static const char _clex_from_state_actions[] = {
	0, 0, 0, 4, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

static const short _clex_eof_trans[] = {
	0, 0, 3, 0, 28, 30, 28, 28, 
	33, 36, 36, 36, 28, 28, 28, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 28
};

static const int clex_start = 3;
static const int clex_error = 0;

static const int clex_en_main = 3;


#line 111 "lexer.rl"

static int rl_identifier_const(YYSTYPE *yylval, struct ragel_lexer_t *YY)
{
    int t, leng = YY->te-YY->ts;

    if (YY->ts[0] == '$'){
	if (CSP_GET_CONST_VALUE(YY->ts+1, leng-1, &yylval->d.num)){
	    uint8_t saved_tok_char;
	    YY_PREPARE_TOK;

	    csp_set_error(CSP_ERR_LEX_CONST_UNDEF, YY->lineno, "Undefined: %s", YY->ts);

	    YY_RELEASE_TOK;
	    return -1;
	}
	return NUM;
    }

    /* check and return function identifier */
    t = func_get(YY->ts, leng);
    if (t >= 0){
	yylval->d.num = t;
	return FUNC;
    }

    /* variable */
    t = visible_var_get(YY->ts, leng);
    if (t >= 0){
	yylval->d.num = t;
	return VAR;
    }

    YY->token = YY->ts;
    YY->token_len = leng;

    return NEW_ID;
}

static int _yylex(YYSTYPE *yylval, struct ragel_lexer_t *YY)
{
    char *p, *pe, *eof=NULL;

    int tok = TOKEN_UNDEFINED;
    char saved_tok_char;
    int buffree, buflen, len;

    if (prog_check_free_space()){
	csp_set_error(CSP_ERR_CODE_OVERFLOW, YY->lineno, "Out of program space");
	return -1;
    }

    if (csp_errno){
	/* if error condition set - fail parser */
	if (csp_errline <= 0)
	    csp_errline = YY->lineno;
	return -1;
    }

    p = YY->buffer + YY->p_offs;
    pe = YY->buffer + YY->pe_offs;

    /* perform a buffer management like in ragel pullscan example */
    while (p == pe && (YY->status == LEX_STATUS_WIP)) {

	if (YY->ts == 0){
	    buflen = 0;
	} else {
	    /* There is data that needs to be shifted over. */
	    buflen = pe - YY->ts;
	    memmove(YY->buffer, YY->ts, buflen);
	    YY->te -= (YY->ts - YY->buffer);
	    YY->ts = YY->buffer;
	}

	p = YY->buffer + buflen;
	buffree = YY->bufsize - buflen;
	pe = p;

	if (buffree == 0) {
	    csp_set_error(CSP_ERR_LEX_OVERFLOW, YY->lineno, "token too long");
	    YY->status = LEX_STATUS_OVERFLOW;
	    break;
	}

	len = CSP_TEXT_INPUT_CALLBACK(p, buffree);
	if (len < 0){
	    YY->status = LEX_STATUS_EOF;
	    break;
	} else {
	    pe += len;
	}
    }

    if (YY->status >= LEX_STATUS_ERR)
	return -1;

    if (YY->status != LEX_STATUS_WIP)
	eof = pe;

    /*******************************/
    
#line 350 "lexer.tab.c"
	{
	int _klen;
	const unsigned char *_keys;
	int _trans;

	if ( p == pe )
		goto _test_eof;
	if (  YY->cs == 0 )
		goto _out;
_resume:
	switch ( _clex_from_state_actions[ YY->cs] ) {
	case 4:
#line 1 "NONE"
	{ YY->ts = p;}
	break;
#line 366 "lexer.tab.c"
	}

	_keys = _clex_trans_keys + _clex_key_offsets[ YY->cs];
	_trans = _clex_index_offsets[ YY->cs];

	_klen = _clex_single_lengths[ YY->cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _clex_range_lengths[ YY->cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _clex_indicies[_trans];
_eof_trans:
	 YY->cs = _clex_trans_targs[_trans];

	if ( _clex_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _clex_trans_actions[_trans] ) {
	case 14:
#line 57 "lexer.rl"
	{ YY->ts = p; }
	break;
	case 8:
#line 1 "NONE"
	{ YY->te = p+1;}
	break;
	case 7:
#line 61 "lexer.rl"
	{ YY->te = p+1;{ tok = YY->ts[0]; {p++; goto _out; } }}
	break;
	case 32:
#line 75 "lexer.rl"
	{ YY->te = p+1;{ tok = SY_OROR; {p++; goto _out; } }}
	break;
	case 12:
#line 76 "lexer.rl"
	{ YY->te = p+1;{ tok = SY_ANDAND; {p++; goto _out; } }}
	break;
	case 19:
#line 78 "lexer.rl"
	{ YY->te = p+1;{ tok = SY_GE; {p++; goto _out; } }}
	break;
	case 17:
#line 79 "lexer.rl"
	{ YY->te = p+1;{ tok = SY_LE; {p++; goto _out; } }}
	break;
	case 18:
#line 80 "lexer.rl"
	{ YY->te = p+1;{ tok = SY_EQ; {p++; goto _out; } }}
	break;
	case 10:
#line 81 "lexer.rl"
	{ YY->te = p+1;{ tok = SY_NEQ; {p++; goto _out; } }}
	break;
	case 5:
#line 106 "lexer.rl"
	{ YY->te = p+1;}
	break;
	case 13:
#line 57 "lexer.rl"
	{ YY->te = p;p--;}
	break;
	case 9:
#line 61 "lexer.rl"
	{ YY->te = p;p--;{ tok = YY->ts[0]; {p++; goto _out; } }}
	break;
	case 16:
#line 83 "lexer.rl"
	{ YY->te = p;p--;{
		char *stft;
		YY_PREPARE_TOK;

		yylval->d.num = strtol(YY->ts, &stft, 0);
		if (*stft != '\0') {
		    /* is it even posttible? */
		    csp_set_error(CSP_ERR_LEX_BAD_NUMBER, YY->lineno, "not a number '%s'", YY->ts);
		    tok = EOS;
		} else {
		    tok = NUM;
		}

		YY_RELEASE_TOK;
		{p++; goto _out; }
	}}
	break;
	case 20:
#line 100 "lexer.rl"
	{ YY->te = p;p--;{
		tok = rl_identifier_const(yylval, YY);

		{p++; goto _out; }
	}}
	break;
	case 2:
#line 83 "lexer.rl"
	{{p = (( YY->te))-1;}{
		char *stft;
		YY_PREPARE_TOK;

		yylval->d.num = strtol(YY->ts, &stft, 0);
		if (*stft != '\0') {
		    /* is it even posttible? */
		    csp_set_error(CSP_ERR_LEX_BAD_NUMBER, YY->lineno, "not a number '%s'", YY->ts);
		    tok = EOS;
		} else {
		    tok = NUM;
		}

		YY_RELEASE_TOK;
		{p++; goto _out; }
	}}
	break;
	case 11:
#line 1 "NONE"
	{	switch(  YY->act ) {
	case 3:
	{{p = (( YY->te))-1;} tok = KW_IF; {p++; goto _out; } }
	break;
	case 4:
	{{p = (( YY->te))-1;} tok = KW_ELSE; {p++; goto _out; } }
	break;
	case 5:
	{{p = (( YY->te))-1;} tok = KW_FOR; {p++; goto _out; } }
	break;
	case 6:
	{{p = (( YY->te))-1;} tok = KW_DO; {p++; goto _out; } }
	break;
	case 7:
	{{p = (( YY->te))-1;} tok = KW_WHILE; {p++; goto _out; } }
	break;
	case 8:
	{{p = (( YY->te))-1;} tok = KW_BREAK; {p++; goto _out; } }
	break;
	case 9:
	{{p = (( YY->te))-1;} tok = KW_CONTINUE; {p++; goto _out; } }
	break;
	case 10:
	{{p = (( YY->te))-1;} tok = KW_RETURN; {p++; goto _out; } }
	break;
	case 11:
	{{p = (( YY->te))-1;} tok = KW_GLOBAL; {p++; goto _out; } }
	break;
	case 12:
	{{p = (( YY->te))-1;} tok = KW_FUNCTION; {p++; goto _out; } }
	break;
	case 13:
	{{p = (( YY->te))-1;} tok = KW_MAP; {p++; goto _out; } }
	break;
	case 21:
	{{p = (( YY->te))-1;}
		tok = rl_identifier_const(yylval, YY);

		{p++; goto _out; }
	}
	break;
	}
	}
	break;
	case 15:
#line 45 "lexer.rl"
	{YY->lineno += 1;}
#line 57 "lexer.rl"
	{ YY->te = p+1;}
	break;
	case 6:
#line 45 "lexer.rl"
	{YY->lineno += 1;}
#line 106 "lexer.rl"
	{ YY->te = p+1;}
	break;
	case 28:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 63 "lexer.rl"
	{ YY->act = 3;}
	break;
	case 24:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 64 "lexer.rl"
	{ YY->act = 4;}
	break;
	case 25:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 65 "lexer.rl"
	{ YY->act = 5;}
	break;
	case 23:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 66 "lexer.rl"
	{ YY->act = 6;}
	break;
	case 31:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 67 "lexer.rl"
	{ YY->act = 7;}
	break;
	case 21:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 68 "lexer.rl"
	{ YY->act = 8;}
	break;
	case 22:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 69 "lexer.rl"
	{ YY->act = 9;}
	break;
	case 30:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 70 "lexer.rl"
	{ YY->act = 10;}
	break;
	case 27:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 71 "lexer.rl"
	{ YY->act = 11;}
	break;
	case 26:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 72 "lexer.rl"
	{ YY->act = 12;}
	break;
	case 29:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 73 "lexer.rl"
	{ YY->act = 13;}
	break;
	case 1:
#line 1 "NONE"
	{ YY->te = p+1;}
#line 100 "lexer.rl"
	{ YY->act = 21;}
	break;
#line 650 "lexer.tab.c"
	}

_again:
	switch ( _clex_to_state_actions[ YY->cs] ) {
	case 3:
#line 1 "NONE"
	{ YY->ts = 0;}
	break;
#line 659 "lexer.tab.c"
	}

	if (  YY->cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _clex_eof_trans[ YY->cs] > 0 ) {
		_trans = _clex_eof_trans[ YY->cs] - 1;
		goto _eof_trans;
	}
	}

	_out: {}
	}

#line 212 "lexer.rl"
    /*******************************/

    YY->p_offs = p - YY->buffer;
    YY->pe_offs = pe - YY->buffer;

    yylval->d.line = YY->lineno;

    if ( YY->cs == clex_error ) {
	/* unexpected token */
	csp_set_error(CSP_ERR_LEX_BAD_TOKEN, YY->lineno, "invalid token");
	YY->status = LEX_STATUS_BAD_TOKEN;
	return -1;
    }

    if (YY->status == LEX_STATUS_EOF &&
		tok == TOKEN_UNDEFINED &&
		p == pe && pe == eof){
	return EOS;
    }

    return tok;
}

int yylex(void *yylval, void *lex_state)
{
    int res;

    do {
	res = _yylex((YYSTYPE*)yylval, (struct ragel_lexer_t*)lex_state);
    } while (res == TOKEN_UNDEFINED);
    /*printf(">>>> %d\n", res);*/
    return res;
}

int scanner_init(struct ragel_lexer_t *YY, char *buffer, uint16_t size)
{
    memset(YY, 0, sizeof(struct ragel_lexer_t));

    
#line 718 "lexer.tab.c"
	{
	 YY->cs = clex_start;
	 YY->ts = 0;
	 YY->te = 0;
	 YY->act = 0;
	}

#line 251 "lexer.rl"

    YY->lineno = 1;

    YY->status = LEX_STATUS_WIP;

    YY->buffer = buffer;
    YY->bufsize = size;

    return 0;
}

