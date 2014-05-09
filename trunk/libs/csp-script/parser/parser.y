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

/* WARNING
 *
 * When new identifier is lexed, lexer returns "new identifier" code
 * and keeps identifier text data in its buffer.
 *
 * It's OK as far as parser handler is called right away, but if parser
 * requires at least one more token to determine what exact handler should
 * be called, data in buffer will be destroyed.
 *
 * At a current state, NEW_ID must not require additional shifts.
 *
 */

%define api.pure
%parse-param {struct ragel_lexer_t *lex_state}
%lex-param   {struct ragel_lexer_t *lex_state}

%code requires {

#include "csp_incl.h"
#include "csp_defs.h"
#include "lexer.h"


    int yyparse(struct ragel_lexer_t *lex_state);

}


%{

#include "csp_incl.h"
#include "csp_api.h"
#include "lexer.h"

#include "gen.h"

#if 0
#	define YYDEBUG 1
#	define YYERROR_VERBOSE 1
int yydebug = 1;
#else
#	undef YYERROR_VERBOSE
#endif

#define YYSTACK_USE_ALLOCA 1
#define YYINITDEPTH CSP_PARSER_STACK_DEPTH
#define YYMAXDEPTH YYINITDEPTH


/* disable dynamic memory management */
#define YYMALLOC(x) NULL
#define YYFREE(x) /**/


static void yyerror(void *lex_state, const char *s)
{
    csp_set_error(CSP_ERR_PARSER_FAILURE, 0, s);
}


static void errhnd(int line, const char *s, const char *s2)
{
    csp_set_error(CSP_ERR_PARSE_ERROR, line, s, s2);
}

#define HANDLE_ERROR(_n_, _msg_, _msg2_) do { errhnd(_n_, _msg_, _msg2_); YYABORT; } while (0)

%}



%union {
struct {
	uint16_t line;
	/* union'ify these */
	union {
		int num;
		int argc;
	};
} d;
}

%token KW_IF 128
%token KW_ELSE 129
%token KW_FOR 130
%token KW_DO 131
%token KW_WHILE 132
%token KW_BREAK 133
%token KW_CONTINUE 134
%token KW_RETURN 135
%token KW_GLOBAL 136
%token KW_FUNCTION 137
%token KW_MAP 138
%token EOS 139

%right '='

%left <d.num> NUM 140
%left <d.num> VAR 141
%left <d.num> FUNC 142
%left <d.num> NEW_ID 143
%left <d.num> NCONST 144

%left '>' '<' SY_GE 145 SY_LE 146 SY_EQ 148 SY_NEQ 149

%left '|' '&' '^' SY_OROR 150 SY_ANDAND 151

%left '+' '-'
%left '/' '*' '%'
%left NEG 152 '~' '!'
%left '['
%start begin

%type <d.num> anyvar new_id_expr

/* if/else conflict */
%expect 1

%%

begin:
	program EOS				{ YYACCEPT; }
	| error					{ HANDLE_ERROR($<d.line>1, "Parse error: %s", "unknown"); }
	;

program:
	globals
	maps
	functions
	| error					{ HANDLE_ERROR($<d.line>1, "Parse error: %s", "program"); }
	;

globals: /**/
	| global_stmst
	;

global_stmst: global
	| global_stmst global
	;

maps: /**/
	| maps_stmts
	;

maps_stmts: map
	| maps_stmts map
	;

functions: function
	| functions function
	;

/**************/

function: KW_FUNCTION NEW_ID 			{ $<d.num>$ = ctx_sw_func_reg((char*)lex_state->token, lex_state->token_len); }
		'(' optargvars ')'		{ $<d.num>$ = ctx_get_local_vars_count(); }
		'{' stmts '}'			{ ctx_cur_func_finalize($<d.num>3, $<d.num>7); }
	| KW_FUNCTION error			{ HANDLE_ERROR($<d.line>1, "Parse error: %s", "func"); }
	;

optargvars: /* none */
	| argvars
	;

argvars: argn
	| argvars ',' argn
	| error					{ HANDLE_ERROR($<d.line>1, "Parse error: %s", "func args"); }
	;

argn: NEW_ID					{ local_var_reg((char*)lex_state->token, lex_state->token_len); }
	;

/**************/

global: KW_GLOBAL globlist ';'
	;

globlist: globn
	| globlist ',' globn
	| error					{ HANDLE_ERROR($<d.line>1, "Parse error: %s", "globals"); }
	;

globn: NEW_ID					{ global_var_reg((char*)lex_state->token, lex_state->token_len); }
	;

/**************/

map: KW_MAP NEW_ID				{ $<d.num>$ = ctx_sw_map_reg((char*)lex_state->token, lex_state->token_len); }
		'{' mapitems '}'		{ ctx_cur_map_finalize($<d.num>3); }
	| KW_MAP error				{ HANDLE_ERROR($<d.line>1, "Parse error: %s", "map"); }
	;

mapitems: mapitem
	| mapitems ',' mapitem
	| error					{ HANDLE_ERROR($<d.line>1, "Parse error: %s", "map item"); }
	;

mapitem: /* */
	| iconst '=' iconst			{ ctx_cur_map_add($<d.num>1, $<d.num>3); }
	;

iconst: NUM
	| NCONST
	;

/**************/
stmts:
	stmt
	| stmts stmt
	;

stmt:
	expr ';'				{ pop_count(1); }
	| stmtif
	| stmtwhile
	| stmtdo ';'
	| stmtfor
	| KW_BREAK ';'				{ op_loopctl(OP_BREAK); }
	| KW_CONTINUE ';'			{ op_loopctl(OP_CONTINUE); }
	| KW_RETURN expr ';'			{ op_return(); }
	| error					{ HANDLE_ERROR($<d.line>1, "Parse error: %s", "stmt"); }
	;

braced:
	stmt
	| '{' '}'
	| '{' stmts '}'
	;

/**************/
stmtif:
	KW_IF					{ $<d.num>$ = get_pc(SIZE_OP_IF); }
		'(' expr ')'			{ $<d.num>$ = get_pc(0); }
		braced				{ $<d.num>$ = get_pc(0); }
		stmtifelse			{ op_if($<d.num>2, $<d.num>6, $<d.num>8); }
	;

stmtifelse:
	/* no else */ 
	| KW_ELSE braced
	;

/**************/
stmtwhile:
	KW_WHILE				{ $<d.num>$ = get_pc(SIZE_OP_WHILE); }
		'(' expr ')'			{ $<d.num>$ = get_pc(0); }
		braced				{ op_while($<d.num>2, $<d.num>6); }
	;

/**************/
stmtdo:
	KW_DO					{ $<d.num>$ = get_pc(SIZE_OP_DOWHILE); }
		braced				{ $<d.num>$ = get_pc(0); }
		KW_WHILE '(' expr ')'		{ op_dowhile($<d.num>4, $<d.num>2); }
	;

/**************/
stmtfor:
	KW_FOR '('	/* for (x;y;z) -> x;for(;y;z) */
		expr ';'			{ pop_count(1); $<d.num>$ = get_pc(SIZE_OP_FOR);  /* FIXME XXX pop iff expr not empty*/ }
		optexpr ';'			{ $<d.num>$ = get_pc(0); }
		optexpr ')'			{ $<d.num>$ = get_pc(0); }
			braced			{ op_for($<d.num>5, $<d.num>8, $<d.num>11); }
	;

optexpr: /* none */
	| expr
	;

/**************/

expr:
	NUM					{ push_num($1);  }
	| VAR					{ push_var($1);  }
	| call
	| anyvar '=' expr			{ set_var($1); }

	| expr '[' expr ']'			{ op_call(MACRO_OP_FUNC_GETIDX); }
	| expr '[' expr ']' '=' expr		{ op_call(MACRO_OP_FUNC_SETIDX); }

	| expr '|' expr				{ binary_op(OP_BIN_OR);  }
	| '~' expr				{ unary_op(OP_UN_BITNEG);  }
	| '!' expr				{ unary_op(OP_UN_NOT);  }
	| expr '&' expr				{ binary_op(OP_BIN_AND);  }
	| expr '^' expr				{ binary_op(OP_BIN_XOR);  }
	| expr '%' expr				{ binary_op(OP_BIN_MOD);  }

	| expr SY_OROR expr			{ binary_op(OP_BIN_OROR);  }
	| expr SY_ANDAND expr			{ binary_op(OP_BIN_ANDAND);  }

	| expr '>' expr				{ binary_op(OP_BIN_GR);  }
	| expr '<' expr				{ binary_op(OP_BIN_LS);  }
	| expr SY_GE expr			{ binary_op(OP_BIN_GE);  }
	| expr SY_LE expr			{ binary_op(OP_BIN_LE);  }
	| expr SY_EQ expr			{ binary_op(OP_BIN_EQ);  }
	| expr SY_NEQ expr			{ binary_op(OP_BIN_NE);  }

	| expr '+' expr				{ binary_op(OP_BIN_ADD);  }
	| expr '-' expr				{ binary_op(OP_BIN_SUB);  }
	| '-' expr %prec NEG			{ unary_op(OP_UN_NEG);  }
	| expr '/' expr				{ binary_op(OP_BIN_DIV);  }
	| expr '*' expr				{ binary_op(OP_BIN_MUL);  }
	| '(' expr ')'
	| new_id_expr				{
							const char *buf;
							int len;
							char token_value[CSP_LEX_BUFSIZE];
							
							if (visible_var_get_name($1, &buf, &len) == 0){
								if (len > CSP_LEX_BUFSIZE-1)
								    len = CSP_LEX_BUFSIZE-1;
								memcpy(token_value, buf, len);
								token_value[len] = 0;
							} else {
							    token_value[0] = '?';
							    token_value[1] = 0;
							}
							HANDLE_ERROR($<d.line>1, "Undefined: %s", token_value);
						}
	| '(' error ')'				{ HANDLE_ERROR($<d.line>2, "Parse error: %s", "expr"); }
	;

/**************/

call:
	FUNC '(' optargs ')'			{ push_num($<d.argc>3); op_call($1); }
	;

optargs:
	/* none */				{ $<d.argc>$ = 0; }
	| args					{ $<d.argc>$ = $<d.argc>1; }
	| error					{ HANDLE_ERROR($<d.line>1, "Parse error: %s", "args"); }
	;

args:
	expr					{ $<d.argc>$ = 1; }
	| args ',' expr				{ $<d.argc>$ = $<d.argc>1 + 1; }
	;

/**************/

anyvar:	VAR
	| new_id_expr
	;

new_id_expr:
	/* See warning at the top.
	 * Resolved extra shift: "expr:new_id_expr" and "anyvar:new_id_expr"
	 */
	NEW_ID					{ $$ = local_var_reg((char*)lex_state->token, lex_state->token_len); }
%%
