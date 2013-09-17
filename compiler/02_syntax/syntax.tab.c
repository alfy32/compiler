/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 2 "syntax.y"

#define YYSTYPE double
#include <math.h>


/* Line 268 of yacc.c  */
#line 77 "syntax.tab.c"

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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NEG_SYM = 258,
     MULTIPLY_SYM = 259,
     DIVIDE_SYM = 260,
     MOD_SYM = 261,
     SUBTRACT_SYM = 262,
     ADD_SYM = 263,
     EQUAL_SYM = 264,
     LT_GT_SYM = 265,
     LT_SYM = 266,
     LT_EQ_SYM = 267,
     GT_SYM = 268,
     GT_EQ_SYM = 269,
     TILDE_SYM = 270,
     AND_SYM = 271,
     OR_SYM = 272,
     COLON_SYM = 273,
     SEMICOLON_SYM = 274,
     L_PAREN_SYM = 275,
     R_PAREN_SYM = 276,
     L_BRACKET_SYM = 277,
     R_BRACKET_SYM = 278,
     ASSIGNMENT_SYM = 279,
     COMMA_SYM = 280,
     DOT_SYM = 281,
     CONST_SYM = 282,
     PROCEDURE_SYM = 283,
     FUNCTION_SYM = 284,
     IDENT_SYM = 285,
     BEGIN_SYM = 286,
     END_SYM = 287,
     IF_SYM = 288,
     ELSE_SYM = 289,
     ELSEIF_SYM = 290,
     WHILE_SYM = 291,
     DO_SYM = 292,
     FOR_SYM = 293,
     RETURN_SYM = 294,
     READ_SYM = 295,
     ARRAY_SYM = 296,
     INT_CONST_SYM = 297,
     CHAR_CONST_SYM = 298,
     STR_CONST_SYM = 299,
     CHAR_SYM = 300,
     FORWARD_SYM = 301,
     OF_SYM = 302,
     ORD_SYM = 303,
     PRED_SYM = 304,
     RECORD_SYM = 305,
     REPEAT_SYM = 306,
     STOP_SYM = 307,
     SUCC_SYM = 308,
     THEN_SYM = 309,
     TO_SYM = 310,
     UNTIL_SYM = 311,
     VAR_SYM = 312,
     WRITE_SYM = 313,
     NEG = 314
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 10 "syntax.y"

        int intVal;
        char* charVal;



/* Line 293 of yacc.c  */
#line 179 "syntax.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 191 "syntax.tab.c"

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
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

# define YYCOPY_NEEDED 1

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

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
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
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   607

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  123
/* YYNRULES -- Number of states.  */
#define YYNSTATES  266

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   314

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     9,    11,    12,    14,    15,    17,    18,
      21,    23,    25,    26,    33,    39,    47,    56,    67,    78,
      79,    81,    85,    90,    94,    99,   103,   109,   116,   118,
     120,   122,   124,   128,   133,   139,   140,   149,   151,   154,
     157,   162,   168,   170,   174,   176,   178,   180,   182,   184,
     186,   188,   190,   192,   194,   196,   200,   208,   213,   216,
     217,   220,   221,   227,   232,   242,   244,   246,   249,   254,
     256,   260,   265,   267,   271,   275,   280,   281,   285,   289,
     293,   297,   301,   305,   309,   313,   317,   321,   325,   329,
     333,   336,   339,   343,   347,   352,   357,   362,   367,   372,
     374,   376,   378,   382,   387,   391,   395,   399,   403,   407,
     411,   415,   419,   423,   427,   431,   435,   439,   442,   445,
     449,   451,   453,   455
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      61,     0,    -1,    62,    63,    64,    65,    72,    -1,    66,
      -1,    -1,    73,    -1,    -1,    80,    -1,    -1,    65,    65,
      -1,    67,    -1,    68,    -1,    -1,    27,    30,     9,   101,
      19,    66,    -1,    27,    30,     9,   101,    19,    -1,    28,
      30,    20,    69,    21,    19,    46,    -1,    28,    30,    20,
      69,    21,    19,    71,    19,    -1,    29,    30,    20,    69,
      21,    18,    74,    19,    46,    19,    -1,    29,    30,    20,
      69,    21,    18,    74,    19,    71,    19,    -1,    -1,    70,
      -1,    70,    19,    70,    -1,    57,    79,    18,    74,    -1,
      79,    18,    74,    -1,    62,    63,    64,    72,    -1,    31,
      82,    32,    -1,    74,    30,     9,    74,    19,    -1,    74,
      30,     9,    74,    19,    73,    -1,    75,    -1,    76,    -1,
      78,    -1,    30,    -1,    50,    77,    32,    -1,    79,    18,
      74,    19,    -1,    79,    18,    74,    19,    77,    -1,    -1,
      41,    22,   101,    18,   101,    23,    47,    74,    -1,    30,
      -1,    30,    79,    -1,    57,    81,    -1,    79,    18,    74,
      19,    -1,    79,    18,    74,    19,    81,    -1,    83,    -1,
      83,    19,    83,    -1,    84,    -1,    85,    -1,    88,    -1,
      89,    -1,    90,    -1,    91,    -1,    92,    -1,    93,    -1,
      95,    -1,    97,    -1,    98,    -1,   100,    24,    99,    -1,
      33,    99,    54,    82,    86,    87,    32,    -1,    35,    99,
      54,    82,    -1,    86,    86,    -1,    -1,    34,    82,    -1,
      -1,    36,    99,    37,    82,    32,    -1,    51,    82,    56,
      99,    -1,    38,    30,    24,    99,    55,    99,    37,    82,
      32,    -1,    52,    -1,    39,    -1,    39,    99,    -1,    40,
      20,    94,    21,    -1,   100,    -1,   100,    25,    94,    -1,
      58,    20,    96,    21,    -1,    99,    -1,    99,    25,    96,
      -1,    30,    20,    21,    -1,    30,    20,    96,    21,    -1,
      -1,    99,    17,    99,    -1,    99,    16,    99,    -1,    99,
       9,    99,    -1,    99,    10,    99,    -1,    99,    12,    99,
      -1,    99,    14,    99,    -1,    99,    11,    99,    -1,    99,
      13,    99,    -1,    99,     8,    99,    -1,    99,     7,    99,
      -1,    99,     4,    99,    -1,    99,     5,    99,    -1,    99,
       6,    99,    -1,    15,    99,    -1,     3,    99,    -1,    20,
      99,    21,    -1,    30,    20,    21,    -1,    30,    20,    96,
      21,    -1,    45,    20,    99,    21,    -1,    48,    20,    99,
      21,    -1,    49,    20,    99,    21,    -1,    53,    20,    99,
      21,    -1,   100,    -1,   101,    -1,    30,    -1,    30,    26,
      30,    -1,    30,    22,    99,    23,    -1,   101,    17,   101,
      -1,   101,    16,   101,    -1,   101,     9,   101,    -1,   101,
      10,   101,    -1,   101,    12,   101,    -1,   101,    14,   101,
      -1,   101,    11,   101,    -1,   101,    13,   101,    -1,   101,
       8,   101,    -1,   101,     7,   101,    -1,   101,     4,   101,
      -1,   101,     5,   101,    -1,   101,     6,   101,    -1,    15,
     101,    -1,     3,   101,    -1,    20,   101,    21,    -1,    42,
      -1,    43,    -1,    44,    -1,    30,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    84,    84,    87,    88,    91,    92,    95,    96,    99,
     100,   101,   102,   107,   108,   112,   113,   116,   117,   120,
     121,   122,   125,   126,   129,   132,   138,   139,   142,   143,
     144,   147,   150,   153,   154,   155,   158,   161,   162,   167,
     170,   171,   176,   177,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   193,   196,   199,   200,   201,
     204,   205,   208,   211,   214,   217,   220,   221,   224,   227,
     228,   231,   234,   235,   238,   239,   242,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   273,   274,   275,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NEG_SYM", "MULTIPLY_SYM", "DIVIDE_SYM",
  "MOD_SYM", "SUBTRACT_SYM", "ADD_SYM", "EQUAL_SYM", "LT_GT_SYM", "LT_SYM",
  "LT_EQ_SYM", "GT_SYM", "GT_EQ_SYM", "TILDE_SYM", "AND_SYM", "OR_SYM",
  "COLON_SYM", "SEMICOLON_SYM", "L_PAREN_SYM", "R_PAREN_SYM",
  "L_BRACKET_SYM", "R_BRACKET_SYM", "ASSIGNMENT_SYM", "COMMA_SYM",
  "DOT_SYM", "CONST_SYM", "PROCEDURE_SYM", "FUNCTION_SYM", "IDENT_SYM",
  "BEGIN_SYM", "END_SYM", "IF_SYM", "ELSE_SYM", "ELSEIF_SYM", "WHILE_SYM",
  "DO_SYM", "FOR_SYM", "RETURN_SYM", "READ_SYM", "ARRAY_SYM",
  "INT_CONST_SYM", "CHAR_CONST_SYM", "STR_CONST_SYM", "CHAR_SYM",
  "FORWARD_SYM", "OF_SYM", "ORD_SYM", "PRED_SYM", "RECORD_SYM",
  "REPEAT_SYM", "STOP_SYM", "SUCC_SYM", "THEN_SYM", "TO_SYM", "UNTIL_SYM",
  "VAR_SYM", "WRITE_SYM", "NEG", "$accept", "Program", "CDecl", "TDecl",
  "VDecl", "PFDecl", "ConstantDecl", "ProcedureDecl", "FunctionDecl",
  "FormalParameters", "FormalParameter", "Body", "Block", "TypeDecl",
  "Type", "SimpleType", "RecordType", "RecordItem", "ArrayType",
  "IdentList", "VarDecl", "VarDeclStuff", "StatementSequence", "Statement",
  "Assignemnt", "IfStatement", "ElseIfPart", "ElsePart", "WhileStatement",
  "RepeatStatement", "ForStatement", "StopStatement", "ReturnStatement",
  "ReadStatement", "LValStuff", "WriteStatement", "ExpressionList",
  "ProcedureCall", "NullStatement", "Expression", "LValue",
  "ConstExpression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    60,    61,    62,    62,    63,    63,    64,    64,    65,
      65,    65,    65,    66,    66,    67,    67,    68,    68,    69,
      69,    69,    70,    70,    71,    72,    73,    73,    74,    74,
      74,    75,    76,    77,    77,    77,    78,    79,    79,    80,
      81,    81,    82,    82,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    84,    85,    86,    86,    86,
      87,    87,    88,    89,    90,    91,    92,    92,    93,    94,
      94,    95,    96,    96,    97,    97,    98,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,   100,   100,   100,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     1,     0,     1,     0,     1,     0,     2,
       1,     1,     0,     6,     5,     7,     8,    10,    10,     0,
       1,     3,     4,     3,     4,     3,     5,     6,     1,     1,
       1,     1,     3,     4,     5,     0,     8,     1,     2,     2,
       4,     5,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     7,     4,     2,     0,
       2,     0,     5,     4,     9,     1,     1,     2,     4,     1,
       3,     4,     1,     3,     3,     4,     0,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     3,     3,     4,     4,     4,     4,     4,     1,
       1,     1,     3,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     3,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,     6,     3,     0,     1,    31,     0,    35,
       8,     5,     0,    28,    29,    30,     0,     0,    37,     0,
       0,     0,    12,     7,     0,     0,     0,     0,   123,   120,
     121,   122,     0,     0,    38,    32,     0,     0,    39,     0,
       0,     0,    10,    11,     0,   118,   117,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,     0,    76,     9,     2,
       0,   119,   114,   115,   116,   113,   112,   106,   107,   110,
     108,   111,   109,   105,   104,    13,     0,    33,     0,    19,
      19,   101,     0,     0,     0,    66,     0,    76,    65,     0,
       0,    42,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,     0,    26,     0,    34,    40,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,    99,   100,     0,     0,    67,
       0,     0,     0,    25,    76,     0,    27,     0,    41,     0,
       0,     0,     0,     0,    74,     0,    72,     0,   102,    91,
     100,    90,   100,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,    76,     0,   101,     0,    69,     0,
       0,    43,    55,    36,     0,     4,    21,    23,     0,    75,
       0,   103,    92,    93,     0,     0,     0,     0,     0,    87,
      88,    89,    86,    85,    79,    80,    83,    81,    84,    82,
      78,    77,    59,     0,     0,    68,     0,    63,    71,    22,
      15,     6,     0,     0,    73,    94,    95,    96,    97,    98,
       0,    59,    62,     0,    70,     8,    16,     4,     0,    76,
      58,     0,     0,     0,     0,     0,    76,    60,    56,    76,
      24,    17,    18,    57,     0,    64
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,   231,    10,    22,    68,     4,    42,    43,   119,
     120,   232,    69,    11,    12,    13,    14,    19,    15,   121,
      23,    38,   100,   101,   102,   103,   250,   251,   104,   105,
     106,   107,   108,   109,   187,   110,   155,   111,   112,   156,
     135,   136
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -130
static const yytype_int16 yypact[] =
{
      -1,   -28,    21,   -23,  -130,    28,  -130,  -130,    18,    12,
     -13,  -130,    25,  -130,  -130,  -130,   136,   136,    12,    40,
      56,    12,    -6,  -130,    66,   136,   136,   136,  -130,  -130,
    -130,  -130,   527,   543,  -130,  -130,   -23,    58,  -130,    54,
      59,   126,  -130,  -130,   -23,   558,    -2,   419,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,    -1,   136,    89,   -23,    90,    91,   302,    -6,  -130,
      93,  -130,   133,   133,   133,    69,    69,   174,   174,   174,
     174,   174,   174,    98,  -130,  -130,   379,    12,    99,   -25,
     -25,   -16,   250,   250,   101,   250,   128,   302,  -130,   132,
     130,   139,  -130,  -130,  -130,  -130,  -130,  -130,  -130,  -130,
    -130,  -130,  -130,   144,   -23,   122,  -130,    12,    12,   149,
     153,   155,   154,   218,   250,   146,   250,   250,   250,    23,
     157,   161,   184,   198,   189,  -130,   558,   300,   150,   572,
     200,   178,   250,  -130,   302,   250,  -130,   -23,  -130,   217,
     221,   -25,   -23,   223,  -130,   224,   357,   399,  -130,   572,
     558,    30,    -2,   437,   419,   234,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   302,   302,   250,   -14,   225,   211,   250,
     226,  -130,   572,  -130,   -23,   -18,  -130,  -130,   -23,  -130,
     250,  -130,  -130,  -130,   229,   455,   473,   491,   509,   215,
     215,   215,   581,   581,   590,   590,   590,   590,   590,   590,
     227,  -130,   207,   219,   116,  -130,   200,   572,  -130,  -130,
    -130,   -23,   233,   237,  -130,  -130,  -130,  -130,  -130,  -130,
     250,    19,  -130,   250,  -130,   -13,  -130,    -7,   203,   302,
     207,   236,   314,   228,   239,   253,   302,  -130,  -130,   302,
    -130,  -130,  -130,  -130,   241,  -130
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -130,  -130,   269,    43,    36,   262,   214,  -130,  -130,   195,
     135,    41,    37,   175,   -33,  -130,  -130,   204,  -130,    20,
    -130,   179,   -96,   156,  -130,  -130,    75,  -130,  -130,  -130,
    -130,  -130,  -130,  -130,    76,  -130,  -129,  -130,  -130,   -76,
     -67,     8
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
     113,   141,     5,    63,   123,    18,   124,     7,   124,     1,
     125,    70,   125,   190,    59,    60,   134,   137,     8,   139,
       1,     6,    39,    40,    32,    33,     1,     9,   230,    20,
     113,    88,   118,    45,    46,    47,   204,    16,    34,   254,
      17,    37,    18,   165,    21,   124,   181,   182,   157,   125,
     159,   161,   163,   249,   240,    24,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,   192,
      86,   234,    35,   188,    36,    44,    64,   113,    53,    54,
      55,    56,    57,    58,    65,    59,    60,   222,   223,    66,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,    20,    87,   224,
      89,    90,   114,   227,   193,    60,   113,   113,   117,   197,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   138,   181,   182,   160,   162,   164,    37,   149,    25,
      51,    52,    53,    54,    55,    56,    57,    58,   140,    59,
      60,    26,   142,   257,    39,    40,    27,    67,   144,   188,
     263,   229,   143,   264,   248,   233,    28,   252,   145,   147,
     150,   243,   151,   152,   185,   153,   158,   166,    29,    30,
      31,   167,   113,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      59,    60,   113,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   168,   181,   182,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   169,   181,
     182,   126,   173,   174,   175,   176,   177,   178,   179,   180,
     186,   181,   182,   127,   189,   194,   226,   126,   128,   154,
     195,   198,   240,   183,   182,   199,   225,   228,   129,   127,
     235,   242,   246,   126,   128,   203,   247,   256,   261,    67,
      29,    30,    31,   130,   129,   127,   131,   132,   258,     3,
     128,   133,   262,   265,   245,    85,    29,    30,    31,   130,
     129,   253,   131,   132,    41,   122,   196,   133,   255,   146,
     260,   116,    29,    30,    31,   130,   148,   241,   131,   132,
     191,     0,   244,   133,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,   181,   182,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
     181,   182,    91,     0,     0,    92,     0,   184,    93,     0,
      94,    95,    96,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,    97,    98,     0,     0,     0,     0,     0,
      99,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,     0,   181,   182,     0,     0,     0,     0,     0,
       0,     0,   200,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,     0,    59,    60,     0,     0,     0,
       0,     0,   115,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,     0,   181,   182,     0,     0,     0,
       0,     0,   201,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,     0,    59,    60,     0,     0,     0,
      71,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,     0,   181,   182,     0,     0,     0,   202,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
       0,   181,   182,     0,     0,     0,   236,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,     0,   181,
     182,     0,     0,     0,   237,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,     0,   181,   182,     0,
       0,     0,   238,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,     0,   181,   182,     0,     0,     0,
     239,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,     0,    59,    60,     0,    61,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,     0,    59,
      60,    62,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,     0,    59,    60,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,   181,   182,
     175,   176,   177,   178,   179,   180,     0,   181,   182,    -1,
      -1,    -1,    -1,    -1,    -1,     0,   181,   182
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-130))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-1))

static const yytype_int16 yycheck[] =
{
      67,    97,    30,    36,    20,    30,    22,    30,    22,    27,
      26,    44,    26,   142,    16,    17,    92,    93,    41,    95,
      27,     0,    28,    29,    16,    17,    27,    50,    46,     9,
      97,    64,    57,    25,    26,    27,   165,     9,    18,    46,
      22,    21,    30,    20,    57,    22,    16,    17,   124,    26,
     126,   127,   128,    34,    35,    30,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,   145,
      62,   200,    32,   140,    18,     9,    18,   144,     9,    10,
      11,    12,    13,    14,    30,    16,    17,   183,   184,    30,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,    87,    19,   185,
      20,    20,    19,   189,   147,    17,   183,   184,    19,   152,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    30,    16,    17,   126,   127,   128,   117,   118,     3,
       7,     8,     9,    10,    11,    12,    13,    14,    20,    16,
      17,    15,    20,   249,    28,    29,    20,    31,    19,   226,
     256,   194,    32,   259,   240,   198,    30,   243,    24,    47,
      21,    55,    19,    18,    24,    21,    30,    20,    42,    43,
      44,    20,   249,     9,    10,    11,    12,    13,    14,   256,
      16,    17,   259,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    20,    16,    17,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    20,    16,
      17,     3,     7,     8,     9,    10,    11,    12,    13,    14,
      30,    16,    17,    15,    56,    18,    25,     3,    20,    21,
      19,    18,    35,    54,    17,    21,    21,    21,    30,    15,
      21,    32,    19,     3,    20,    21,    19,    54,    19,    31,
      42,    43,    44,    45,    30,    15,    48,    49,    32,     0,
      20,    53,    19,    32,   231,    61,    42,    43,    44,    45,
      30,   245,    48,    49,    22,    90,   151,    53,   247,   114,
     253,    87,    42,    43,    44,    45,   117,   222,    48,    49,
     144,    -1,   226,    53,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    16,    17,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    -1,
      16,    17,    30,    -1,    -1,    33,    -1,    37,    36,    -1,
      38,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    51,    52,    -1,    -1,    -1,    -1,    -1,
      58,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    -1,    16,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    23,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    23,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    -1,    16,    17,    -1,    -1,    -1,
      21,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    -1,    16,    17,    -1,    -1,    -1,    21,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      -1,    16,    17,    -1,    -1,    -1,    21,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    16,
      17,    -1,    -1,    -1,    21,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    -1,    16,    17,    -1,
      -1,    -1,    21,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    -1,    16,    17,    -1,    -1,    -1,
      21,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    -1,    16,    17,    -1,    19,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    -1,    16,    17,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    -1,    16,    17,
       9,    10,    11,    12,    13,    14,    -1,    16,    17,     9,
      10,    11,    12,    13,    14,    -1,    16,    17
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    27,    61,    62,    66,    30,     0,    30,    41,    50,
      63,    73,    74,    75,    76,    78,     9,    22,    30,    77,
      79,    57,    64,    80,    30,     3,    15,    20,    30,    42,
      43,    44,   101,   101,    79,    32,    18,    79,    81,    28,
      29,    65,    67,    68,     9,   101,   101,   101,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    16,
      17,    19,    18,    74,    18,    30,    30,    31,    65,    72,
      74,    21,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,    66,   101,    19,    74,    20,
      20,    30,    33,    36,    38,    39,    40,    51,    52,    58,
      82,    83,    84,    85,    88,    89,    90,    91,    92,    93,
      95,    97,    98,   100,    19,    23,    77,    19,    57,    69,
      70,    79,    69,    20,    22,    26,     3,    15,    20,    30,
      45,    48,    49,    53,    99,   100,   101,    99,    30,    99,
      20,    82,    20,    32,    19,    24,    73,    47,    81,    79,
      21,    19,    18,    21,    21,    96,    99,    99,    30,    99,
     101,    99,   101,    99,   101,    20,    20,    20,    20,    20,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    16,    17,    54,    37,    24,    30,    94,   100,    56,
      96,    83,    99,    74,    18,    19,    70,    74,    18,    21,
      25,    23,    21,    21,    96,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    82,    82,    99,    21,    25,    99,    21,    74,
      46,    62,    71,    74,    96,    21,    21,    21,    21,    21,
      35,    86,    32,    55,    94,    63,    19,    19,    99,    34,
      86,    87,    99,    64,    46,    71,    54,    82,    32,    37,
      72,    19,    19,    82,    82,    32
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
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
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


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
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
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

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
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

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
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
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
  if (yypact_value_is_default (yyn))
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
      if (yytable_value_is_error (yyn))
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
      

/* Line 1806 of yacc.c  */
#line 1729 "syntax.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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
		      yytoken, &yylval);
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
      if (!yypact_value_is_default (yyn))
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
		  yystos[yystate], yyvsp);
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
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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



/* Line 2067 of yacc.c  */
#line 300 "syntax.y"

