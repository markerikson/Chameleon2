#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define yyparse cl_func_parse
#define yylex cl_func_lex
#define yyerror cl_func_error
#define yychar cl_func_char
#define yyval cl_func_val
#define yylval cl_func_lval
#define yydebug cl_func_debug
#define yynerrs cl_func_nerrs
#define yyerrflag cl_func_errflag
#define yyss cl_func_ss
#define yyssp cl_func_ssp
#define yyvs cl_func_vs
#define yyvsp cl_func_vsp
#define yylhs cl_func_lhs
#define yylen cl_func_len
#define yydefred cl_func_defred
#define yydgoto cl_func_dgoto
#define yysindex cl_func_sindex
#define yyrindex cl_func_rindex
#define yygindex cl_func_gindex
#define yytable cl_func_table
#define yycheck cl_func_check
#define yyname cl_func_name
#define yyrule cl_func_rule
#define YYPREFIX "cl_func_"
/* Copyright Eran Ifrah(c)*/
/*************** Includes and Defines *****************************/
#include "string"
#include "vector"
#include "stdio.h"
#include "map"
#include "function.h"

#define YYDEBUG_LEXER_TEXT (cl_func_lval)
#define YYSTYPE std::string
#define YYDEBUG 0        /* get the pretty debugging code to compile*/

#ifdef yylex
#undef yylex
#define yylex cl_scope_lex
#endif

int cl_func_parse();
void cl_func_error(char *string);

static FunctionList *g_funcs = NULL;
static clFunction curr_func;

/*---------------------------------------------*/
/* externs defined in the lexer*/
/*---------------------------------------------*/
extern char *cl_func_text;
extern int cl_scope_lex();
extern bool setLexerInput(const std::string &in, const std::map<std::string, std::string> &ignoreTokens);
extern int cl_scope_lineno;
extern void cl_scope_lex_clean();


/*************** Standard ytab.c continues here *********************/
#define LE_AUTO 257
#define LE_DOUBLE 258
#define LE_INT 259
#define LE_STRUCT 260
#define LE_BREAK 261
#define LE_ELSE 262
#define LE_LONG 263
#define LE_SWITCH 264
#define LE_CASE 265
#define LE_ENUM 266
#define LE_REGISTER 267
#define LE_TYPEDEF 268
#define LE_CHAR 269
#define LE_EXTERN 270
#define LE_RETURN 271
#define LE_UNION 272
#define LE_CONST 273
#define LE_FLOAT 274
#define LE_SHORT 275
#define LE_UNSIGNED 276
#define LE_BOOL 277
#define LE_CONTINUE 278
#define LE_FOR 279
#define LE_SIGNED 280
#define LE_VOID 281
#define LE_DEFAULT 282
#define LE_GOTO 283
#define LE_SIZEOF 284
#define LE_VOLATILE 285
#define LE_DO 286
#define LE_IF 287
#define LE_STATIC 288
#define LE_WHILE 289
#define LE_NEW 290
#define LE_DELETE 291
#define LE_THIS 292
#define LE_OPERATOR 293
#define LE_CLASS 294
#define LE_PUBLIC 295
#define LE_PROTECTED 296
#define LE_PRIVATE 297
#define LE_VIRTUAL 298
#define LE_FRIEND 299
#define LE_INLINE 300
#define LE_OVERLOAD 301
#define LE_TEMPLATE 302
#define LE_TYPENAME 303
#define LE_THROW 304
#define LE_CATCH 305
#define LE_IDENTIFIER 306
#define LE_STRINGliteral 307
#define LE_FLOATINGconstant 308
#define LE_INTEGERconstant 309
#define LE_CHARACTERconstant 310
#define LE_OCTALconstant 311
#define LE_HEXconstant 312
#define LE_POUNDPOUND 313
#define LE_CComment 314
#define LE_CPPComment 315
#define LE_NAMESPACE 316
#define LE_USING 317
#define LE_TYPEDEFname 318
#define LE_ARROW 319
#define LE_ICR 320
#define LE_DECR 321
#define LE_LS 322
#define LE_RS 323
#define LE_LE 324
#define LE_GE 325
#define LE_EQ 326
#define LE_NE 327
#define LE_ANDAND 328
#define LE_OROR 329
#define LE_ELLIPSIS 330
#define LE_CLCL 331
#define LE_DOTstar 332
#define LE_ARROWstar 333
#define LE_MULTassign 334
#define LE_DIVassign 335
#define LE_MODassign 336
#define LE_PLUSassign 337
#define LE_MINUSassign 338
#define LE_LSassign 339
#define LE_RSassign 340
#define LE_ANDassign 341
#define LE_ERassign 342
#define LE_ORassign 343
#define LE_MACRO 344
#define LE_DYNAMIC_CAST 345
#define LE_STATIC_CAST 346
#define LE_CONST_CAST 347
#define LE_REINTERPRET_CAST 348
#define YYERRCODE 256
short cl_func_lhs[] = {                                        -1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    2,    2,    2,    0,    0,    5,    3,    3,
    6,    6,    8,    8,    7,    7,    9,    9,   10,   10,
   10,   11,   11,   11,   15,   15,   15,   16,   16,   16,
   16,   16,   16,   16,   16,   16,   16,   16,   16,   16,
   16,   16,   16,   16,   16,   16,   16,   16,   16,   16,
   16,   16,   16,   16,   16,   16,   16,   16,   20,    4,
   21,   21,   23,   23,   13,   13,   22,   22,   24,   25,
   24,   18,   18,   12,   12,   26,   26,   27,   27,   14,
   17,   17,   17,   19,   19,   19,
};
short cl_func_len[] = {                                         2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    2,    2,    1,    0,    2,    0,    2,    1,
    0,    2,    1,    3,    1,    1,    0,    4,    0,    1,
    3,    4,    4,    7,    1,    2,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    1,    1,    1,    0,   13,
    0,    4,    1,    1,    0,    2,    0,    2,    2,    0,
    4,    0,    1,    0,    1,    0,    1,    0,    2,    2,
    0,    1,    1,    3,    3,    6,
};
short cl_func_defred[] = {                                     16,
    0,   20,   17,    0,   92,   93,   19,    0,    0,    0,
    0,   83,    0,   25,   26,   23,    0,    0,   85,   75,
   22,    0,   28,    0,   75,   24,    6,    1,    0,    2,
    5,    3,    0,   10,    0,    9,    0,   15,   88,   76,
    0,   14,   13,    4,    8,    7,   11,   12,   79,    0,
   95,    0,   94,    0,    0,    0,    0,    0,   30,   75,
    0,   89,   87,   90,   66,   67,   54,   58,   59,   50,
   51,   60,   61,   62,   63,   52,   53,   57,   55,   68,
   48,   49,   46,   38,   39,   40,   41,   42,   43,   44,
   45,   47,   56,    0,    0,   37,   80,   36,   69,    0,
   88,    0,   81,   64,   65,    0,   31,   96,    0,   88,
    0,    0,   32,   33,    0,    0,    0,    0,    0,    0,
   88,    0,   78,   73,   74,   70,   34,   72,
};
short cl_func_dgoto[] = {                                       1,
   38,   39,    3,    7,    4,   16,   17,   18,   10,   58,
   59,   60,   24,   51,   57,   96,    8,   13,   25,  106,
  116,  120,  126,   40,   61,   64,   52,
};
short cl_func_sindex[] = {                                      0,
 -220,    0,    0,    5,    0,    0,    0, -251,   16, -221,
 -259,    0, -194,    0,    0,    0, -223,    3,    0,    0,
    0, -259,    0,  -53,    0,    0,    0,    0, -189,    0,
    0,    0,  131,    0,  131,    0,  -59,    0,    0,    0,
 -120,    0,    0,    0,    0,    0,    0,    0,    0, -194,
    0,   40,    0,  -33,  -58, -222,   45,    6,    0,    0,
 -245,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   48,   -1,    0,    0,    0,    0, -194,
    0,   98,    0,    0,    0, -194,    0,    0,  -57,    0,
 -208, -194,    0,    0,   60,   41,   10, -194, -206,  -42,
    0,   -3,    0,    0,    0,    0,    0,    0,
};
short cl_func_rindex[] = {                                      0,
   62,    0,    0, -135,    0,    0,    0, -115,    0,  -81,
   13,    0,  107,    0,    0,    0,    0,    0,    0,    0,
    0,   13,    0,    0,    0,    0,    0,    0,  -22,    0,
    0,    0,    4,    0,   11,    0,   -8,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -29,
    0,  -23,    0,    0,   66,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  107,
    0,    0,    0,    0,    0,  -36,    0,    0,   18,    0,
  -35,  -29,    0,    0,    0,  -28,    0,   53,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
};
short cl_func_gindex[] = {                                      0,
   36,    9,    0,    0,    0,   85,    0,    0,    0,  -75,
    8,   -5,    7,  -11,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 413
short cl_func_table[] = {                                      92,
   50,   97,  112,   88,   90,   56,   94,   20,   86,   84,
   80,   85,   93,   87,   29,    4,  125,   86,    4,    4,
   86,    4,   84,   71,   84,   71,   81,   53,   82,   88,
   77,   41,   29,   88,   14,    2,  117,  128,   86,    4,
  100,    8,  122,   15,    8,    8,   22,    8,    7,  100,
    9,    7,    7,  100,    7,   88,   21,   95,   88,   88,
   89,   88,    6,    5,   23,    8,  102,  101,   47,   42,
   48,  121,    7,   43,   21,   11,   12,   63,   19,   88,
  124,   62,   21,   98,   99,  103,   84,   71,  104,  108,
   91,  105,   83,   29,   77,  115,   29,  113,  114,  118,
  111,  119,   86,    4,  123,   35,   26,  107,    0,  127,
  110,    0,    0,    0,    0,    0,    0,   88,    0,   18,
   18,    0,   91,   91,    0,    0,    0,   91,    0,    8,
    0,    0,    0,   91,    0,    0,    7,   91,   91,   91,
   91,   91,   27,   27,   91,   91,    0,   27,    0,    0,
    0,    0,    0,   27,    0,    0,    0,   27,   27,   27,
   27,   27,   91,    0,   27,   27,   91,    0,    0,    0,
   91,    0,   54,    0,    0,    0,   82,   82,    0,    0,
    0,   82,   27,    0,    0,   55,    0,   82,    0,    0,
   27,   82,   82,   82,   82,   82,    0,    0,   82,   82,
    0,    0,    0,    0,   27,   28,    0,    0,    0,   29,
    0,    0,    0,    0,    0,   30,    0,    0,    0,    0,
   31,   32,   33,   34,   82,    0,   35,   36,   84,   84,
    0,    0,    0,   84,    0,    0,    0,    0,    0,   84,
    0,    0,    0,    0,   84,   84,   84,   84,    0,    0,
   84,   84,   37,    0,    0,    0,   65,   66,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   84,    0,   86,
    4,   49,   49,   49,    0,    0,   84,    0,    0,    0,
    0,    0,   86,    4,   88,   67,   68,   69,   70,   71,
   72,   73,   74,   75,   76,   77,    8,   88,   78,   79,
    0,   80,    0,    7,    0,    0,    0,    0,    0,    8,
   84,   84,    0,    0,    0,   84,    7,    0,    0,   18,
   18,   84,    0,    0,   18,    0,   84,   84,   84,   84,
   18,    0,   84,   84,   18,   18,   18,   18,   18,    0,
    0,   18,   18,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   27,   28,    0,   84,   18,
   29,    0,    0,   18,   84,   84,   30,   18,    0,   84,
    0,   31,   32,   33,   34,   84,    0,   35,   36,    0,
   84,   84,   84,   84,    0,    0,   84,   84,   27,   28,
    0,    0,    0,   44,    0,    0,    0,    0,    0,   30,
    0,    0,    0,  109,   31,   32,   45,   34,    0,    0,
   46,   36,   84,
};
short cl_func_check[] = {                                      33,
   60,   60,   60,   37,   38,  126,   40,   13,   42,   43,
   44,   45,   46,   47,   44,   38,   59,   41,   41,   42,
   44,   44,   59,   59,   61,   61,   60,   39,   62,   38,
   59,   25,   62,   42,  294,  256,  112,   41,   62,   62,
   44,   38,  118,  303,   41,   42,   44,   44,   38,   44,
  302,   41,   42,   44,   44,   38,   44,   91,   41,   42,
   94,   44,   58,   59,   62,   62,   60,   62,   33,  259,
   35,   62,   62,  263,   62,   60,  298,   38,  273,   62,
  123,   42,  306,  306,   40,  331,  123,  123,   41,  101,
  124,   93,  126,   41,  123,  304,   44,  109,  110,   40,
  106,   61,  126,  126,  311,   40,   22,  100,   -1,  121,
  102,   -1,   -1,   -1,   -1,   -1,   -1,  126,   -1,   58,
   59,   -1,  258,  259,   -1,   -1,   -1,  263,   -1,  126,
   -1,   -1,   -1,  269,   -1,   -1,  126,  273,  274,  275,
  276,  277,  258,  259,  280,  281,   -1,  263,   -1,   -1,
   -1,   -1,   -1,  269,   -1,   -1,   -1,  273,  274,  275,
  276,  277,  298,   -1,  280,  281,  302,   -1,   -1,   -1,
  306,   -1,  293,   -1,   -1,   -1,  258,  259,   -1,   -1,
   -1,  263,  298,   -1,   -1,  306,   -1,  269,   -1,   -1,
  306,  273,  274,  275,  276,  277,   -1,   -1,  280,  281,
   -1,   -1,   -1,   -1,  258,  259,   -1,   -1,   -1,  263,
   -1,   -1,   -1,   -1,   -1,  269,   -1,   -1,   -1,   -1,
  274,  275,  276,  277,  306,   -1,  280,  281,  258,  259,
   -1,   -1,   -1,  263,   -1,   -1,   -1,   -1,   -1,  269,
   -1,   -1,   -1,   -1,  274,  275,  276,  277,   -1,   -1,
  280,  281,  306,   -1,   -1,   -1,  290,  291,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  304,   -1,  293,
  293,  331,  331,  331,   -1,   -1,  306,   -1,   -1,   -1,
   -1,   -1,  306,  306,  293,  319,  320,  321,  322,  323,
  324,  325,  326,  327,  328,  329,  293,  306,  332,  333,
   -1,  331,   -1,  293,   -1,   -1,   -1,   -1,   -1,  306,
  258,  259,   -1,   -1,   -1,  263,  306,   -1,   -1,  258,
  259,  269,   -1,   -1,  263,   -1,  274,  275,  276,  277,
  269,   -1,  280,  281,  273,  274,  275,  276,  277,   -1,
   -1,  280,  281,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,  259,   -1,  306,  298,
  263,   -1,   -1,  302,  258,  259,  269,  306,   -1,  263,
   -1,  274,  275,  276,  277,  269,   -1,  280,  281,   -1,
  274,  275,  276,  277,   -1,   -1,  280,  281,  258,  259,
   -1,   -1,   -1,  263,   -1,   -1,   -1,   -1,   -1,  269,
   -1,   -1,   -1,  306,  274,  275,  276,  277,   -1,   -1,
  280,  281,  306,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 348
#if YYDEBUG
char *cl_func_name[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,"'%'","'&'",0,"'('","')'","'*'","'+'","','","'-'","'.'","'/'",0,0,0,
0,0,0,0,0,0,0,"':'","';'","'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,"'['",0,"']'","'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,"'{'","'|'",0,"'~'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"LE_AUTO","LE_DOUBLE",
"LE_INT","LE_STRUCT","LE_BREAK","LE_ELSE","LE_LONG","LE_SWITCH","LE_CASE",
"LE_ENUM","LE_REGISTER","LE_TYPEDEF","LE_CHAR","LE_EXTERN","LE_RETURN",
"LE_UNION","LE_CONST","LE_FLOAT","LE_SHORT","LE_UNSIGNED","LE_BOOL",
"LE_CONTINUE","LE_FOR","LE_SIGNED","LE_VOID","LE_DEFAULT","LE_GOTO","LE_SIZEOF",
"LE_VOLATILE","LE_DO","LE_IF","LE_STATIC","LE_WHILE","LE_NEW","LE_DELETE",
"LE_THIS","LE_OPERATOR","LE_CLASS","LE_PUBLIC","LE_PROTECTED","LE_PRIVATE",
"LE_VIRTUAL","LE_FRIEND","LE_INLINE","LE_OVERLOAD","LE_TEMPLATE","LE_TYPENAME",
"LE_THROW","LE_CATCH","LE_IDENTIFIER","LE_STRINGliteral","LE_FLOATINGconstant",
"LE_INTEGERconstant","LE_CHARACTERconstant","LE_OCTALconstant","LE_HEXconstant",
"LE_POUNDPOUND","LE_CComment","LE_CPPComment","LE_NAMESPACE","LE_USING",
"LE_TYPEDEFname","LE_ARROW","LE_ICR","LE_DECR","LE_LS","LE_RS","LE_LE","LE_GE",
"LE_EQ","LE_NE","LE_ANDAND","LE_OROR","LE_ELLIPSIS","LE_CLCL","LE_DOTstar",
"LE_ARROWstar","LE_MULTassign","LE_DIVassign","LE_MODassign","LE_PLUSassign",
"LE_MINUSassign","LE_LSassign","LE_RSassign","LE_ANDassign","LE_ERassign",
"LE_ORassign","LE_MACRO","LE_DYNAMIC_CAST","LE_STATIC_CAST","LE_CONST_CAST",
"LE_REINTERPRET_CAST",
};
char *cl_func_rule[] = {
"$accept : translation_unit",
"basic_type_name_inter : LE_INT",
"basic_type_name_inter : LE_CHAR",
"basic_type_name_inter : LE_SHORT",
"basic_type_name_inter : LE_LONG",
"basic_type_name_inter : LE_FLOAT",
"basic_type_name_inter : LE_DOUBLE",
"basic_type_name_inter : LE_SIGNED",
"basic_type_name_inter : LE_UNSIGNED",
"basic_type_name_inter : LE_VOID",
"basic_type_name_inter : LE_BOOL",
"basic_type_name : LE_UNSIGNED basic_type_name_inter",
"basic_type_name : LE_SIGNED basic_type_name_inter",
"basic_type_name : LE_LONG LE_LONG",
"basic_type_name : LE_LONG LE_INT",
"basic_type_name : basic_type_name_inter",
"translation_unit :",
"translation_unit : translation_unit external_decl",
"$$1 :",
"external_decl : $$1 function_decl",
"external_decl : error",
"template_arg :",
"template_arg : template_specifiter LE_IDENTIFIER",
"template_arg_list : template_arg",
"template_arg_list : template_arg_list ',' template_arg",
"template_specifiter : LE_CLASS",
"template_specifiter : LE_TYPENAME",
"opt_template_qualifier :",
"opt_template_qualifier : LE_TEMPLATE '<' template_arg_list '>'",
"template_parameter_list :",
"template_parameter_list : template_parameter",
"template_parameter_list : template_parameter_list ',' template_parameter",
"template_parameter : const_spec nested_scope_specifier LE_IDENTIFIER special_star_amp",
"template_parameter : const_spec nested_scope_specifier basic_type_name special_star_amp",
"template_parameter : const_spec nested_scope_specifier LE_IDENTIFIER '<' template_parameter_list '>' special_star_amp",
"func_name : LE_IDENTIFIER",
"func_name : '~' LE_IDENTIFIER",
"func_name : LE_OPERATOR any_operator",
"any_operator : '+'",
"any_operator : '-'",
"any_operator : '*'",
"any_operator : '/'",
"any_operator : '%'",
"any_operator : '^'",
"any_operator : '&'",
"any_operator : '|'",
"any_operator : '~'",
"any_operator : '!'",
"any_operator : '<'",
"any_operator : '>'",
"any_operator : LE_LS",
"any_operator : LE_RS",
"any_operator : LE_ANDAND",
"any_operator : LE_OROR",
"any_operator : LE_ARROW",
"any_operator : LE_ARROWstar",
"any_operator : '.'",
"any_operator : LE_DOTstar",
"any_operator : LE_ICR",
"any_operator : LE_DECR",
"any_operator : LE_LE",
"any_operator : LE_GE",
"any_operator : LE_EQ",
"any_operator : LE_NE",
"any_operator : '(' ')'",
"any_operator : '[' ']'",
"any_operator : LE_NEW",
"any_operator : LE_DELETE",
"any_operator : ','",
"$$2 :",
"function_decl : stmnt_starter opt_template_qualifier virtual_spec const_spec variable_decl nested_scope_specifier func_name '(' $$2 const_spec declare_throw opt_pure_virtual func_postfix",
"declare_throw :",
"declare_throw : LE_THROW '(' template_parameter_list ')'",
"func_postfix : '{'",
"func_postfix : ';'",
"nested_scope_specifier :",
"nested_scope_specifier : nested_scope_specifier scope_specifier",
"opt_pure_virtual :",
"opt_pure_virtual : '=' LE_OCTALconstant",
"scope_specifier : LE_IDENTIFIER LE_CLCL",
"$$3 :",
"scope_specifier : LE_IDENTIFIER '<' $$3 LE_CLCL",
"virtual_spec :",
"virtual_spec : LE_VIRTUAL",
"const_spec :",
"const_spec : LE_CONST",
"amp_item :",
"amp_item : '&'",
"star_list :",
"star_list : star_list '*'",
"special_star_amp : star_list amp_item",
"stmnt_starter :",
"stmnt_starter : ';'",
"stmnt_starter : ':'",
"variable_decl : nested_scope_specifier basic_type_name special_star_amp",
"variable_decl : nested_scope_specifier LE_IDENTIFIER special_star_amp",
"variable_decl : nested_scope_specifier LE_IDENTIFIER '<' template_parameter_list '>' special_star_amp",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
void yyerror(char *s) {}

void func_consumeFuncArgList()
{
	curr_func.m_signature = "(";

	int depth = 1;
	while(depth > 0)
	{
		int ch = cl_scope_lex();
		if(ch == 0)
		{
			break;
		}

		curr_func.m_signature += cl_func_lval;
		curr_func.m_signature += " ";
		if(ch == ')')
		{
			depth--;
			continue;
		}
		else if(ch == '(')
		{
			depth ++ ;
			continue;
		}
	}
}

/**
 * consume all token until matching closing brace is found
 */
void func_consumeDecl()
{
	int depth = 1;
	while(depth > 0)
	{
		int ch = cl_scope_lex();
		//printf("ch=%d\n", ch);
		//fflush(stdout);
		if(ch ==0)
		{
			break;
		}
		if(ch == '}')
		{
			depth--;
			continue;
		}
		else if(ch == '{')
		{
			depth ++ ;
			continue;
		}
	}

}

void func_consumeTemplateDecl()
{
	int depth = 1;
	while(depth > 0)
	{
		int ch = cl_scope_lex();
		//printf("ch=%d\n", ch);
		//fflush(stdout);
		if(ch ==0){
			break;
		}

		if(ch == '>')
		{
			depth--;
			continue;
		}
		else if(ch == '<')
		{
			depth ++ ;
			continue;
		}
	}
}

// return the scope name at the end of the input string
void get_functions(const std::string &in, FunctionList &li, const std::map<std::string, std::string> &ignoreTokens)
{
	if( !setLexerInput(in, ignoreTokens) )
	{
		return;
	}

	g_funcs = &li;

	//call tghe main parsing routine
	cl_func_parse();
	g_funcs = NULL;

	//do the lexer cleanup
	cl_scope_lex_clean();
}
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
{ yyval = yyvsp[0]; }
break;
case 2:
{ yyval = yyvsp[0]; }
break;
case 3:
{ yyval = yyvsp[0]; }
break;
case 4:
{ yyval = yyvsp[0]; }
break;
case 5:
{ yyval = yyvsp[0]; }
break;
case 6:
{ yyval = yyvsp[0]; }
break;
case 7:
{ yyval = yyvsp[0]; }
break;
case 8:
{ yyval = yyvsp[0]; }
break;
case 9:
{ yyval = yyvsp[0]; }
break;
case 10:
{ yyval = yyvsp[0]; }
break;
case 11:
{ yyval = yyvsp[-1] + " " + yyvsp[0]; }
break;
case 12:
{ yyval = yyvsp[-1] + " " + yyvsp[0]; }
break;
case 13:
{ yyval = yyvsp[-1] + " " + yyvsp[0]; }
break;
case 14:
{ yyval = yyvsp[-1] + " " + yyvsp[0]; }
break;
case 15:
{ yyval = yyvsp[0];            }
break;
case 18:
{curr_func.Reset();}
break;
case 20:
{
							/*printf("CodeLite: syntax error, unexpected token '%s' found\n", cl_func_lval.c_str());*/
						}
break;
case 21:
{ yyval = "";}
break;
case 22:
{yyval = yyvsp[-1] + " " + yyvsp[0];}
break;
case 23:
{ yyval = yyvsp[0]; }
break;
case 24:
{ yyval = yyvsp[-2] + " " + yyvsp[-1] + " " + yyvsp[0]; }
break;
case 25:
{ yyval = yyvsp[0]; }
break;
case 26:
{ yyval = yyvsp[0]; }
break;
case 28:
{ yyval = yyvsp[-3] + yyvsp[-2] + yyvsp[-1] + yyvsp[0];}
break;
case 29:
{yyval = "";}
break;
case 30:
{yyval = yyvsp[0];}
break;
case 31:
{yyval = yyvsp[-2] + yyvsp[-1] + yyvsp[0];}
break;
case 32:
{
							yyval = yyvsp[-3] +  yyvsp[-2] + yyvsp[-1] +yyvsp[0];
						}
break;
case 33:
{
							yyval = yyvsp[-3] +  yyvsp[-2] + yyvsp[-1] +yyvsp[0];
						}
break;
case 34:
{
							yyval = yyvsp[-6] + yyvsp[-5] + yyvsp[-4] +yyvsp[-3] + yyvsp[-2] + yyvsp[-1] + yyvsp[0] + " " ;
						}
break;
case 35:
{yyval = yyvsp[0];}
break;
case 36:
{yyval = yyvsp[-1] + yyvsp[0];}
break;
case 37:
{yyval = yyvsp[-1] + yyvsp[0];}
break;
case 69:
{func_consumeFuncArgList();}
break;
case 70:
{
						/*trim down trailing '::' from scope name*/
						yyvsp[-7].erase(yyvsp[-7].find_last_not_of(":")+1);
						curr_func.m_isVirtual = yyvsp[-10].find("virtual") != std::string::npos;
						curr_func.m_isPureVirtual = yyvsp[-1].find("=") != std::string::npos;
						curr_func.m_isConst = yyvsp[-3].find("const") != std::string::npos;
						curr_func.m_name = yyvsp[-6];
						curr_func.m_scope = yyvsp[-7];
						curr_func.m_retrunValusConst = yyvsp[-9];
						curr_func.m_lineno = cl_scope_lineno;
						if(g_funcs)
						{
							g_funcs->push_back(curr_func);
						}
						curr_func.Reset();
					}
break;
case 71:
{yyval = "";}
break;
case 72:
{yyval = yyvsp[-1];}
break;
case 75:
{yyval = "";}
break;
case 76:
{	yyval = yyvsp[-1] + yyvsp[0];}
break;
case 77:
{yyval = "";}
break;
case 78:
{yyval = yyvsp[-1] + yyvsp[0];}
break;
case 79:
{yyval = yyvsp[-1]+ yyvsp[0];}
break;
case 80:
{func_consumeTemplateDecl();}
break;
case 81:
{yyval = yyvsp[-3] + yyvsp[0];}
break;
case 82:
{yyval = ""; }
break;
case 83:
{ yyval = yyvsp[0]; }
break;
case 84:
{yyval = ""; }
break;
case 85:
{ yyval = yyvsp[0]; }
break;
case 86:
{yyval = ""; }
break;
case 87:
{ yyval = yyvsp[0]; }
break;
case 88:
{yyval = ""; }
break;
case 89:
{yyval = yyvsp[-1] + yyvsp[0];}
break;
case 90:
{ yyval = yyvsp[-1] + yyvsp[0]; }
break;
case 91:
{yyval = "";}
break;
case 92:
{ yyval = ";";}
break;
case 93:
{ yyval = ":";}
break;
case 94:
{
								yyvsp[-2].erase(yyvsp[-2].find_last_not_of(":")+1);
								curr_func.m_returnValue.m_type = yyvsp[-1];
								curr_func.m_returnValue.m_typeScope = yyvsp[-2];
								curr_func.m_returnValue.m_starAmp = yyvsp[0];
								curr_func.m_returnValue.m_isPtr = (yyvsp[0].find("*") != (size_t)-1);
								yyval = yyvsp[-2] + yyvsp[-1] + yyvsp[0];
							}
break;
case 95:
{
								yyvsp[-2].erase(yyvsp[-2].find_last_not_of(":")+1);
								curr_func.m_returnValue.m_type = yyvsp[-1];
								curr_func.m_returnValue.m_typeScope = yyvsp[-2];
								curr_func.m_returnValue.m_starAmp = yyvsp[0];
								curr_func.m_returnValue.m_isPtr = (yyvsp[0].find("*") != (size_t)-1);
								yyval = yyvsp[-2] + yyvsp[-1] + yyvsp[0]  ;
							}
break;
case 96:
{
								yyvsp[-5].erase(yyvsp[-5].find_last_not_of(":")+1);
								curr_func.m_returnValue.m_type = yyvsp[-4];
								curr_func.m_returnValue.m_typeScope = yyvsp[-5];
								curr_func.m_returnValue.m_starAmp = yyvsp[0];
								curr_func.m_returnValue.m_isPtr = (yyvsp[0].find("*") != (size_t)-1);
								curr_func.m_returnValue.m_isTemplate = true;
								curr_func.m_returnValue.m_templateDecl = yyvsp[-2];
								yyval = yyvsp[-5] + yyvsp[-4] + yyvsp[-3]  + yyvsp[-2] + yyvsp[-1] + yyvsp[0] ;
							}
break;
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
