/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 56 "parser.y"


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



/* Line 189 of yacc.c  */
#line 114 "parser.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 209 of yacc.c  */
#line 44 "parser.y"


#include "csp_incl.h"
#include "csp_defs.h"
#include "lexer.h"


    int yyparse(struct ragel_lexer_t *lex_state);




/* Line 209 of yacc.c  */
#line 151 "parser.tab.c"

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

/* Line 214 of yacc.c  */
#line 99 "parser.y"

struct {
	uint16_t line;
	/* union'ify these */
	union {
		int num;
		int argc;
	};
} d;



/* Line 214 of yacc.c  */
#line 207 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 219 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   529

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNRULES -- Number of states.  */
#define YYNSTATES  184

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   257

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,     2,     2,    36,    28,     2,
      41,    42,    35,    32,    45,    33,     2,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    46,
      22,    15,    21,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    47,    29,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,    27,    44,    37,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      16,    17,    18,    19,    20,    26,    25,     2,    24,    23,
      31,    30,    39,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     8,    12,    14,    15,    17,    19,
      22,    23,    25,    27,    30,    32,    35,    36,    37,    48,
      51,    52,    54,    56,    60,    62,    64,    68,    70,    74,
      76,    78,    79,    86,    89,    91,    95,    97,    98,   102,
     104,   106,   108,   111,   114,   116,   118,   121,   123,   126,
     129,   133,   135,   137,   140,   144,   145,   146,   147,   157,
     158,   161,   162,   163,   171,   172,   173,   182,   183,   184,
     185,   198,   199,   201,   203,   205,   207,   211,   216,   223,
     227,   230,   233,   237,   241,   245,   249,   253,   257,   261,
     265,   269,   273,   277,   281,   285,   288,   292,   296,   300,
     302,   306,   311,   312,   314,   316,   318,   322,   324,   326
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      49,     0,    -1,    50,    14,    -1,     1,    -1,    51,    53,
      55,    -1,     1,    -1,    -1,    52,    -1,    62,    -1,    52,
      62,    -1,    -1,    54,    -1,    65,    -1,    54,    65,    -1,
      56,    -1,    55,    56,    -1,    -1,    -1,    12,    19,    57,
      41,    59,    42,    58,    43,    70,    44,    -1,    12,     1,
      -1,    -1,    60,    -1,    61,    -1,    60,    45,    61,    -1,
       1,    -1,    19,    -1,    11,    63,    46,    -1,    64,    -1,
      63,    45,    64,    -1,     1,    -1,    19,    -1,    -1,    13,
      19,    66,    43,    67,    44,    -1,    13,     1,    -1,    68,
      -1,    67,    45,    68,    -1,     1,    -1,    -1,    69,    15,
      69,    -1,    16,    -1,    20,    -1,    71,    -1,    70,    71,
      -1,    89,    46,    -1,    73,    -1,    78,    -1,    81,    46,
      -1,    84,    -1,     8,    46,    -1,     9,    46,    -1,    10,
      89,    46,    -1,     1,    -1,    71,    -1,    43,    44,    -1,
      43,    70,    44,    -1,    -1,    -1,    -1,     3,    74,    41,
      89,    42,    75,    72,    76,    77,    -1,    -1,     4,    72,
      -1,    -1,    -1,     7,    79,    41,    89,    42,    80,    72,
      -1,    -1,    -1,     6,    82,    72,    83,     7,    41,    89,
      42,    -1,    -1,    -1,    -1,     5,    41,    89,    46,    85,
      88,    46,    86,    88,    42,    87,    72,    -1,    -1,    89,
      -1,    16,    -1,    17,    -1,    90,    -1,    93,    15,    89,
      -1,    89,    40,    89,    47,    -1,    89,    40,    89,    47,
      15,    89,    -1,    89,    27,    89,    -1,    37,    89,    -1,
      38,    89,    -1,    89,    28,    89,    -1,    89,    29,    89,
      -1,    89,    36,    89,    -1,    89,    31,    89,    -1,    89,
      30,    89,    -1,    89,    21,    89,    -1,    89,    22,    89,
      -1,    89,    26,    89,    -1,    89,    25,    89,    -1,    89,
      24,    89,    -1,    89,    23,    89,    -1,    89,    32,    89,
      -1,    89,    33,    89,    -1,    33,    89,    -1,    89,    34,
      89,    -1,    89,    35,    89,    -1,    41,    89,    42,    -1,
      94,    -1,    41,     1,    42,    -1,    18,    41,    91,    42,
      -1,    -1,    92,    -1,     1,    -1,    89,    -1,    92,    45,
      89,    -1,    17,    -1,    94,    -1,    19,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   149,   149,   150,   154,   157,   160,   161,   164,   165,
     168,   169,   172,   173,   176,   177,   182,   183,   182,   185,
     188,   189,   192,   193,   194,   197,   202,   205,   206,   207,
     210,   215,   215,   217,   220,   221,   222,   225,   226,   229,
     230,   235,   236,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   252,   253,   254,   259,   260,   261,   259,   265,
     267,   272,   273,   272,   279,   280,   279,   287,   288,   289,
     286,   293,   294,   300,   301,   302,   303,   305,   306,   308,
     309,   310,   311,   312,   313,   315,   316,   318,   319,   320,
     321,   322,   323,   325,   326,   327,   328,   329,   330,   331,
     347,   353,   357,   358,   359,   363,   364,   369,   370,   377
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KW_IF", "KW_ELSE", "KW_FOR", "KW_DO",
  "KW_WHILE", "KW_BREAK", "KW_CONTINUE", "KW_RETURN", "KW_GLOBAL",
  "KW_FUNCTION", "KW_MAP", "EOS", "'='", "NUM", "VAR", "FUNC", "NEW_ID",
  "NCONST", "'>'", "'<'", "SY_NEQ", "SY_EQ", "SY_LE", "SY_GE", "'|'",
  "'&'", "'^'", "SY_ANDAND", "SY_OROR", "'+'", "'-'", "'/'", "'*'", "'%'",
  "'~'", "'!'", "NEG", "'['", "'('", "')'", "'{'", "'}'", "','", "';'",
  "']'", "$accept", "begin", "program", "globals", "global_stmst", "maps",
  "maps_stmts", "functions", "function", "@1", "@2", "optargvars",
  "argvars", "argn", "global", "globlist", "globn", "map", "@3",
  "mapitems", "mapitem", "iconst", "stmts", "stmt", "braced", "stmtif",
  "@4", "@5", "@6", "stmtifelse", "stmtwhile", "@7", "@8", "stmtdo", "@9",
  "@10", "stmtfor", "@11", "@12", "@13", "optexpr", "expr", "call",
  "optargs", "args", "anyvar", "new_id_expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    61,   140,   141,   142,   143,
     144,    62,    60,   149,   148,   146,   145,   124,    38,    94,
     151,   150,    43,    45,    47,    42,    37,   126,    33,   152,
      91,    40,    41,   123,   125,    44,    59,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    49,    50,    50,    51,    51,    52,    52,
      53,    53,    54,    54,    55,    55,    57,    58,    56,    56,
      59,    59,    60,    60,    60,    61,    62,    63,    63,    63,
      64,    66,    65,    65,    67,    67,    67,    68,    68,    69,
      69,    70,    70,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    72,    72,    72,    74,    75,    76,    73,    77,
      77,    79,    80,    78,    82,    83,    81,    85,    86,    87,
      84,    88,    88,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    90,    91,    91,    91,    92,    92,    93,    93,    94
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     3,     1,     0,     1,     1,     2,
       0,     1,     1,     2,     1,     2,     0,     0,    10,     2,
       0,     1,     1,     3,     1,     1,     3,     1,     3,     1,
       1,     0,     6,     2,     1,     3,     1,     0,     3,     1,
       1,     1,     2,     2,     1,     1,     2,     1,     2,     2,
       3,     1,     1,     2,     3,     0,     0,     0,     9,     0,
       2,     0,     0,     7,     0,     0,     8,     0,     0,     0,
      12,     0,     1,     1,     1,     1,     3,     4,     6,     3,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     3,     1,
       3,     4,     0,     1,     1,     1,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     3,     0,     0,     0,    10,     7,     8,    29,    30,
       0,    27,     1,     2,     0,     0,    11,    12,     9,     0,
      26,    33,    31,     0,     4,    14,    13,    28,     0,    19,
      16,    15,     0,     0,    36,    39,    40,     0,    34,     0,
       0,    32,    37,     0,    24,    25,     0,    21,    22,    35,
      38,    17,     0,     0,    23,     0,    51,    55,     0,    64,
      61,     0,     0,     0,    73,    74,     0,   109,     0,     0,
       0,     0,     0,    41,    44,    45,     0,    47,     0,    75,
       0,    99,     0,     0,     0,     0,    48,    49,     0,     0,
      95,    80,    81,     0,     0,    18,    42,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
      52,    65,     0,    50,   104,   105,     0,   103,   100,    98,
      87,    88,    92,    91,    90,    89,    79,    82,    83,    86,
      85,    93,    94,    96,    97,    84,     0,    76,     0,    67,
      53,     0,     0,     0,   101,     0,    77,    56,    71,    54,
       0,    62,   106,     0,     0,     0,    72,     0,     0,    78,
      57,    68,     0,    63,    59,    71,    66,     0,    58,     0,
      60,    69,     0,    70
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    15,    16,    24,    25,    33,
      53,    46,    47,    48,     7,    10,    11,    17,    28,    37,
      38,    39,    72,   120,   121,    74,    82,   164,   174,   178,
      75,    85,   168,    76,    84,   152,    77,   158,   175,   182,
     165,    78,    79,   126,   127,    80,    81
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -155
static const yytype_int16 yypact[] =
{
      62,   -12,     2,    19,    16,    21,    46,  -155,  -155,  -155,
     -29,  -155,  -155,  -155,     3,    20,    21,  -155,  -155,    39,
    -155,  -155,  -155,    14,    20,  -155,  -155,  -155,    17,  -155,
    -155,  -155,    11,    31,  -155,  -155,  -155,   -20,  -155,    71,
      10,  -155,    -7,    -7,  -155,  -155,    34,    42,  -155,  -155,
    -155,  -155,    69,    18,  -155,   245,  -155,  -155,    50,  -155,
    -155,    44,    47,   380,  -155,    81,    60,  -155,   380,   380,
     380,   255,    61,  -155,  -155,  -155,    57,  -155,   303,  -155,
      91,    92,    67,   380,   192,    78,  -155,  -155,   329,   203,
      70,    70,    70,    79,   401,  -155,  -155,  -155,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,  -155,   380,   380,   355,   108,
    -155,  -155,   380,  -155,  -155,   489,    80,    75,  -155,  -155,
     102,   102,   102,   102,   102,   102,    49,    49,    49,    49,
      49,   136,   136,    70,    70,    70,   276,   489,   423,  -155,
    -155,   150,   116,   445,  -155,   380,   113,  -155,   380,  -155,
      98,  -155,   489,   380,   192,    94,   489,   380,   192,   489,
    -155,  -155,   467,  -155,   139,   380,  -155,   192,  -155,   105,
    -155,  -155,   192,  -155
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,   120,  -155,
    -155,  -155,  -155,    96,   144,  -155,   135,   145,  -155,  -155,
     121,   119,    45,   -54,  -154,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
     -10,   -63,  -155,  -155,  -155,  -155,  -155
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -109
static const yytype_int16 yytable[] =
{
      88,    73,    -5,     8,    21,    90,    91,    92,    94,    35,
     170,    44,    34,    36,   173,    29,    19,    20,    96,    12,
     118,     9,    22,   180,    41,    42,   125,    35,   183,    45,
      13,    36,    23,    30,    14,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   -20,   147,   148,   -37,   -37,     2,     9,   153,
      32,    55,    56,     1,    57,    73,    58,    59,    60,    61,
      62,    63,    40,     2,    -6,    -6,    51,    64,    65,    66,
      67,   109,   110,   111,   112,   113,    43,    52,    45,   114,
      86,    83,   162,    87,    68,   166,  -107,    96,    69,    70,
     169,    89,    71,    97,   172,    95,   116,  -108,   117,    56,
     114,    57,   166,    58,    59,    60,    61,    62,    63,   122,
     155,   128,   154,   160,    64,    65,    66,    67,   163,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   167,
     171,    68,   114,   177,    31,    69,    70,   181,    54,    71,
      18,    56,   150,    57,    27,    58,    59,    60,    61,    62,
      63,    26,    50,    49,   151,   179,    64,    65,    66,    67,
     111,   112,   113,     0,     0,     0,   114,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,    69,    70,     0,
       0,    71,     0,    56,   159,    57,     0,    58,    59,    60,
      61,    62,    63,     0,   124,     0,     0,     0,    64,    65,
      66,    67,     0,     0,     0,     0,     0,     0,     0,    64,
      65,    66,    67,     0,     0,    68,     0,     0,     0,    69,
      70,     0,     0,    71,     0,   119,    68,     0,     0,     0,
      69,    70,     0,     0,    71,  -102,    56,     0,    57,     0,
      58,    59,    60,    61,    62,    63,    93,     0,     0,     0,
       0,    64,    65,    66,    67,     0,     0,     0,     0,     0,
       0,    64,    65,    66,    67,     0,     0,     0,    68,     0,
       0,     0,    69,    70,     0,     0,    71,     0,    68,     0,
       0,     0,    69,    70,     0,     0,    71,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,     0,     0,     0,   114,     0,     0,     0,
       0,     0,     0,   156,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
       0,     0,     0,   114,     0,     0,     0,     0,     0,   115,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,     0,     0,     0,   114,
       0,     0,     0,     0,     0,   123,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,     0,     0,     0,   114,    64,    65,    66,    67,
       0,   149,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,    69,    70,     0,
       0,    71,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,     0,     0,
       0,   114,     0,   129,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
       0,     0,     0,   114,     0,   157,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,     0,     0,     0,   114,     0,   161,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,     0,     0,     0,   114,     0,   176,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,     0,     0,     0,   114
};

static const yytype_int16 yycheck[] =
{
      63,    55,    14,     1,     1,    68,    69,    70,    71,    16,
     164,     1,     1,    20,   168,     1,    45,    46,    72,     0,
      83,    19,    19,   177,    44,    45,    89,    16,   182,    19,
      14,    20,    12,    19,    13,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    42,   116,   117,    44,    45,    11,    19,   122,
      43,    43,     1,     1,     3,   119,     5,     6,     7,     8,
       9,    10,    41,    11,    12,    13,    42,    16,    17,    18,
      19,    32,    33,    34,    35,    36,    15,    45,    19,    40,
      46,    41,   155,    46,    33,   158,    15,   151,    37,    38,
     163,    41,    41,    46,   167,    44,    15,    15,    41,     1,
      40,     3,   175,     5,     6,     7,     8,     9,    10,    41,
      45,    42,    42,     7,    16,    17,    18,    19,    15,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    41,
      46,    33,    40,     4,    24,    37,    38,    42,    52,    41,
       6,     1,    44,     3,    19,     5,     6,     7,     8,     9,
      10,    16,    43,    42,   119,   175,    16,    17,    18,    19,
      34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    38,    -1,
      -1,    41,    -1,     1,    44,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,     1,    -1,    -1,    -1,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      17,    18,    19,    -1,    -1,    33,    -1,    -1,    -1,    37,
      38,    -1,    -1,    41,    -1,    43,    33,    -1,    -1,    -1,
      37,    38,    -1,    -1,    41,    42,     1,    -1,     3,    -1,
       5,     6,     7,     8,     9,    10,     1,    -1,    -1,    -1,
      -1,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    16,    17,    18,    19,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    37,    38,    -1,    -1,    41,    -1,    33,    -1,
      -1,    -1,    37,    38,    -1,    -1,    41,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    16,    17,    18,    19,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    38,    -1,
      -1,    41,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    40,    -1,    42,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    -1,    42,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    -1,    42,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,    42,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    40
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    11,    49,    50,    51,    52,    62,     1,    19,
      63,    64,     0,    14,    13,    53,    54,    65,    62,    45,
      46,     1,    19,    12,    55,    56,    65,    64,    66,     1,
      19,    56,    43,    57,     1,    16,    20,    67,    68,    69,
      41,    44,    45,    15,     1,    19,    59,    60,    61,    68,
      69,    42,    45,    58,    61,    43,     1,     3,     5,     6,
       7,     8,     9,    10,    16,    17,    18,    19,    33,    37,
      38,    41,    70,    71,    73,    78,    81,    84,    89,    90,
      93,    94,    74,    41,    82,    79,    46,    46,    89,    41,
      89,    89,    89,     1,    89,    44,    71,    46,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    40,    46,    15,    41,    89,    43,
      71,    72,    41,    46,     1,    89,    91,    92,    42,    42,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    46,
      44,    70,    83,    89,    42,    45,    47,    42,    85,    44,
       7,    42,    89,    15,    75,    88,    89,    41,    80,    89,
      72,    46,    89,    72,    76,    86,    42,     4,    77,    88,
      72,    42,    87,    72
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (lex_state, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, lex_state)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, lex_state); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct ragel_lexer_t *lex_state)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, lex_state)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    struct ragel_lexer_t *lex_state;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (lex_state);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct ragel_lexer_t *lex_state)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, lex_state)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    struct ragel_lexer_t *lex_state;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, lex_state);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, struct ragel_lexer_t *lex_state)
#else
static void
yy_reduce_print (yyvsp, yyrule, lex_state)
    YYSTYPE *yyvsp;
    int yyrule;
    struct ragel_lexer_t *lex_state;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , lex_state);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, lex_state); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, struct ragel_lexer_t *lex_state)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, lex_state)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    struct ragel_lexer_t *lex_state;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (lex_state);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (struct ragel_lexer_t *lex_state);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (struct ragel_lexer_t *lex_state)
#else
int
yyparse (lex_state)
    struct ragel_lexer_t *lex_state;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1464 of yacc.c  */
#line 149 "parser.y"
    { YYACCEPT; ;}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 150 "parser.y"
    { HANDLE_ERROR((yyvsp[(1) - (1)].d.line), "Parse error: %s", "unknown"); ;}
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 157 "parser.y"
    { HANDLE_ERROR((yyvsp[(1) - (1)].d.line), "Parse error: %s", "program"); ;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 182 "parser.y"
    { (yyval.d.num) = ctx_sw_func_reg((char*)lex_state->token, lex_state->token_len); ;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 183 "parser.y"
    { (yyval.d.num) = ctx_get_local_vars_count(); ;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 184 "parser.y"
    { ctx_cur_func_finalize((yyvsp[(3) - (10)].d.num), (yyvsp[(7) - (10)].d.num)); ;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 185 "parser.y"
    { HANDLE_ERROR((yyvsp[(1) - (2)].d.line), "Parse error: %s", "func"); ;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 194 "parser.y"
    { HANDLE_ERROR((yyvsp[(1) - (1)].d.line), "Parse error: %s", "func args"); ;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 197 "parser.y"
    { local_var_reg((char*)lex_state->token, lex_state->token_len); ;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 207 "parser.y"
    { HANDLE_ERROR((yyvsp[(1) - (1)].d.line), "Parse error: %s", "globals"); ;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 210 "parser.y"
    { global_var_reg((char*)lex_state->token, lex_state->token_len); ;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 215 "parser.y"
    { (yyval.d.num) = ctx_sw_map_reg((char*)lex_state->token, lex_state->token_len); ;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 216 "parser.y"
    { ctx_cur_map_finalize((yyvsp[(3) - (6)].d.num)); ;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 217 "parser.y"
    { HANDLE_ERROR((yyvsp[(1) - (2)].d.line), "Parse error: %s", "map"); ;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 222 "parser.y"
    { HANDLE_ERROR((yyvsp[(1) - (1)].d.line), "Parse error: %s", "map item"); ;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 226 "parser.y"
    { ctx_cur_map_add((yyvsp[(1) - (3)].d.num), (yyvsp[(3) - (3)].d.num)); ;}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 240 "parser.y"
    { pop_count(1); ;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 245 "parser.y"
    { op_loopctl(OP_BREAK); ;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 246 "parser.y"
    { op_loopctl(OP_CONTINUE); ;}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 247 "parser.y"
    { op_return(); ;}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 248 "parser.y"
    { HANDLE_ERROR((yyvsp[(1) - (1)].d.line), "Parse error: %s", "stmt"); ;}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 259 "parser.y"
    { (yyval.d.num) = get_pc(SIZE_OP_IF); ;}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 260 "parser.y"
    { (yyval.d.num) = get_pc(0); ;}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 261 "parser.y"
    { (yyval.d.num) = get_pc(0); ;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 262 "parser.y"
    { op_if((yyvsp[(2) - (9)].d.num), (yyvsp[(6) - (9)].d.num), (yyvsp[(8) - (9)].d.num)); ;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 272 "parser.y"
    { (yyval.d.num) = get_pc(SIZE_OP_WHILE); ;}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 273 "parser.y"
    { (yyval.d.num) = get_pc(0); ;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 274 "parser.y"
    { op_while((yyvsp[(2) - (7)].d.num), (yyvsp[(6) - (7)].d.num)); ;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 279 "parser.y"
    { (yyval.d.num) = get_pc(SIZE_OP_DOWHILE); ;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 280 "parser.y"
    { (yyval.d.num) = get_pc(0); ;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 281 "parser.y"
    { op_dowhile((yyvsp[(4) - (8)].d.num), (yyvsp[(2) - (8)].d.num)); ;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 287 "parser.y"
    { pop_count(1); (yyval.d.num) = get_pc(SIZE_OP_FOR);  /* FIXME XXX pop iff expr not empty*/ ;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 288 "parser.y"
    { (yyval.d.num) = get_pc(0); ;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 289 "parser.y"
    { (yyval.d.num) = get_pc(0); ;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 290 "parser.y"
    { op_for((yyvsp[(5) - (12)].d.num), (yyvsp[(8) - (12)].d.num), (yyvsp[(11) - (12)].d.num)); ;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 300 "parser.y"
    { push_num((yyvsp[(1) - (1)].d.num));  ;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 301 "parser.y"
    { push_var((yyvsp[(1) - (1)].d.num));  ;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 303 "parser.y"
    { set_var((yyvsp[(1) - (3)].d.num)); ;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 305 "parser.y"
    { op_call(MACRO_OP_FUNC_GETIDX); ;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 306 "parser.y"
    { op_call(MACRO_OP_FUNC_SETIDX); ;}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 308 "parser.y"
    { binary_op(OP_BIN_OR);  ;}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 309 "parser.y"
    { unary_op(OP_UN_BITNEG);  ;}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 310 "parser.y"
    { unary_op(OP_UN_NOT);  ;}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 311 "parser.y"
    { binary_op(OP_BIN_AND);  ;}
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 312 "parser.y"
    { binary_op(OP_BIN_XOR);  ;}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 313 "parser.y"
    { binary_op(OP_BIN_MOD);  ;}
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 315 "parser.y"
    { binary_op(OP_BIN_OROR);  ;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 316 "parser.y"
    { binary_op(OP_BIN_ANDAND);  ;}
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 318 "parser.y"
    { binary_op(OP_BIN_GR);  ;}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 319 "parser.y"
    { binary_op(OP_BIN_LS);  ;}
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 320 "parser.y"
    { binary_op(OP_BIN_GE);  ;}
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 321 "parser.y"
    { binary_op(OP_BIN_LE);  ;}
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 322 "parser.y"
    { binary_op(OP_BIN_EQ);  ;}
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 323 "parser.y"
    { binary_op(OP_BIN_NE);  ;}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 325 "parser.y"
    { binary_op(OP_BIN_ADD);  ;}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 326 "parser.y"
    { binary_op(OP_BIN_SUB);  ;}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 327 "parser.y"
    { unary_op(OP_UN_NEG);  ;}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 328 "parser.y"
    { binary_op(OP_BIN_DIV);  ;}
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 329 "parser.y"
    { binary_op(OP_BIN_MUL);  ;}
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 331 "parser.y"
    {
							const char *buf;
							int len;
							char token_value[CSP_LEX_BUFSIZE];
							
							if (visible_var_get_name((yyvsp[(1) - (1)].d.num), &buf, &len) == 0){
								if (len > CSP_LEX_BUFSIZE-1)
								    len = CSP_LEX_BUFSIZE-1;
								memcpy(token_value, buf, len);
								token_value[len] = 0;
							} else {
							    token_value[0] = '?';
							    token_value[1] = 0;
							}
							HANDLE_ERROR((yyvsp[(1) - (1)].d.line), "Undefined: %s", token_value);
						;}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 347 "parser.y"
    { HANDLE_ERROR((yyvsp[(2) - (3)].d.line), "Parse error: %s", "expr"); ;}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 353 "parser.y"
    { push_num((yyvsp[(3) - (4)].d.argc)); op_call((yyvsp[(1) - (4)].d.num)); ;}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 357 "parser.y"
    { (yyval.d.argc) = 0; ;}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 358 "parser.y"
    { (yyval.d.argc) = (yyvsp[(1) - (1)].d.argc); ;}
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 359 "parser.y"
    { HANDLE_ERROR((yyvsp[(1) - (1)].d.line), "Parse error: %s", "args"); ;}
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 363 "parser.y"
    { (yyval.d.argc) = 1; ;}
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 364 "parser.y"
    { (yyval.d.argc) = (yyvsp[(1) - (3)].d.argc) + 1; ;}
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 377 "parser.y"
    { (yyval.d.num) = local_var_reg((char*)lex_state->token, lex_state->token_len); ;}
    break;



/* Line 1464 of yacc.c  */
#line 2154 "parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (lex_state, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (lex_state, yymsg);
	  }
	else
	  {
	    yyerror (lex_state, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, lex_state);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, lex_state);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (lex_state, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, lex_state);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, lex_state);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1684 of yacc.c  */
#line 378 "parser.y"


