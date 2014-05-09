/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2009, 2010 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 1685 of yacc.c  */
#line 44 "parser.y"


#include "csp_incl.h"
#include "csp_defs.h"
#include "lexer.h"


    int yyparse(struct ragel_lexer_t *lex_state);




/* Line 1685 of yacc.c  */
#line 52 "parser.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     KW_IF = 128,
     KW_ELSE = 129,
     KW_FOR = 130,
     KW_DO = 131,
     KW_WHILE = 132,
     KW_BREAK = 133,
     KW_CONTINUE = 134,
     KW_RETURN = 135,
     KW_GLOBAL = 136,
     KW_FUNCTION = 137,
     KW_MAP = 138,
     EOS = 139,
     NUM = 140,
     VAR = 141,
     FUNC = 142,
     NEW_ID = 143,
     NCONST = 144,
     SY_NEQ = 149,
     SY_EQ = 148,
     SY_LE = 146,
     SY_GE = 145,
     SY_ANDAND = 151,
     SY_OROR = 150,
     NEG = 152
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 99 "parser.y"

struct {
	uint16_t line;
	/* union'ify these */
	union {
		int num;
		int argc;
	};
} d;



/* Line 1685 of yacc.c  */
#line 108 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




