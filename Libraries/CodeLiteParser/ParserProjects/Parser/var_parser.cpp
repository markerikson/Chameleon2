#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define yyparse cl_var_parse
#define yylex cl_var_lex
#define yyerror cl_var_error
#define yychar cl_var_char
#define yyval cl_var_val
#define yylval cl_var_lval
#define yydebug cl_var_debug
#define yynerrs cl_var_nerrs
#define yyerrflag cl_var_errflag
#define yyss cl_var_ss
#define yyssp cl_var_ssp
#define yyvs cl_var_vs
#define yyvsp cl_var_vsp
#define yylhs cl_var_lhs
#define yylen cl_var_len
#define yydefred cl_var_defred
#define yydgoto cl_var_dgoto
#define yysindex cl_var_sindex
#define yyrindex cl_var_rindex
#define yygindex cl_var_gindex
#define yytable cl_var_table
#define yycheck cl_var_check
#define yyname cl_var_name
#define yyrule cl_var_rule
#define YYPREFIX "cl_var_"
/* Copyright Eran Ifrah(c)*/
/*************** Includes and Defines *****************************/
#include "string"
#include "vector"
#include "stdio.h"
#include "map"
#include "variable.h"
#include "cl_typedef.h"

#ifdef yylex
#undef yylex
#define yylex cl_scope_lex
#endif

#define YYSTYPE std::string
#define YYDEBUG 0        /* get the pretty debugging code to compile*/

void cl_scope_error(char *string);
int  cl_var_parse();
void syncParser();
void var_consumeDefaultValue(char c1, char c2);

static  VariableList *           gs_vars = NULL;
static  std::vector<std::string> gs_names;
static  bool                     g_isUsedWithinFunc = false;
static  std::string              s_tmpString;
static  Variable                 curr_var;
static  std::string              s_templateInitList;
/*---------------------------------------------*/
/* externs defined in the lexer*/
/*---------------------------------------------*/
extern char *cl_scope_text;
extern int cl_scope_lex();
extern void cl_scope_less(int count);

extern int cl_scope_lineno;
extern std::vector<std::string> currentScope;
extern bool setLexerInput(const std::string &in, const std::map<std::string, std::string> &ignoreMap);
extern void setUseIgnoreMacros(bool ignore);
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
short cl_var_lhs[] = {                                        -1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    2,    2,    2,    2,    0,    0,    5,    3,
    3,    6,    6,    6,    7,    7,    7,    4,    4,    4,
    4,    4,    4,   17,   17,   13,   13,   16,   16,   15,
   15,   15,   15,   14,   14,   14,   14,   14,   18,   18,
    9,    9,    8,    8,    8,    8,    8,   19,   19,   20,
   20,   10,   11,   11,   11,   11,   11,   12,   12,   12,
   21,   12,   12,
};
short cl_var_len[] = {                                         2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    2,    2,    2,    1,    0,    2,    0,    2,
    1,    0,    1,    3,    4,    4,    7,    6,    6,    6,
    5,    5,    3,    1,    1,    1,    4,    1,    1,    0,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    5,
    0,    2,    0,    1,    2,    2,    3,    0,    1,    0,
    2,    2,    0,    1,    1,    1,    1,    2,    3,    6,
    0,    6,    4,
};
short cl_var_defred[] = {                                     17,
    0,   21,   18,    0,    0,    0,   64,   65,   66,   67,
   20,    0,    0,    0,    0,   60,   60,   60,    0,    0,
   56,    6,    1,   51,    0,    2,    5,    0,    0,   10,
    0,    9,   16,   68,    0,    0,    0,    0,    0,   33,
   57,    0,   15,   14,    4,    3,    8,    7,   13,   11,
   12,    0,   52,    0,   61,   59,   62,    0,    0,    0,
   49,    0,    0,   38,   39,   32,    0,   31,   36,    0,
    0,   71,    0,   23,   51,   42,   41,   43,   30,   29,
   60,   47,   46,   45,   48,   44,   28,    0,   72,    0,
    0,    0,    0,    0,   24,   50,    0,   60,   37,    0,
   25,   26,    0,    0,   27,
};
short cl_var_dgoto[] = {                                       1,
   33,   34,    3,   11,    4,   73,   74,   75,   35,   36,
   12,   16,   70,   87,   79,   66,   13,   53,   57,   37,
   89,
};
short cl_var_sindex[] = {                                      0,
 -252,    0,    0,   33, -260, -260,    0,    0,    0,    0,
    0, -260, -302,  -33,  165,    0,    0,    0,    3,   12,
    0,    0,    0,    0, -176,    0,    0,  174,  174,    0,
  174,    0,    0,    0, -235, -260,   46, -260, -260,    0,
    0, -220,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -58,    0,  -34,    0,    0,    0,  -15, -213,  -60,
    0, -260,   15,    0,    0,    0,   15,    0,    0,   38,
 -260,    0,   -5,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    6,    0, -260,
 -236,  -92, -206, -236,    0,    0,  -52,    0,    0, -260,
    0,    0,   36, -236,    0,
};
short cl_var_rindex[] = {                                      0,
   45,    0,    0,  108, -128,  -65,    0,    0,    0,    0,
    0,  132,    0,  -38, -205,    0,    0,    0,    0,   72,
    0,    0,    0,    0,  -26,    0,    0,  -19,  -11,    0,
   -4,    0,    0,    0,    0,   -9,  -20,   -9, -204,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   11,    0,    0,    0,    0,    0,    0,    0,   23,
    0,   81,    1,    0,    0,    0,    1,    0,    0,    0,
   81,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  141,
   28,    0,    0,    0,    0,    0,   52,    0,    0,   81,
    0,    0,    0,   52,    0,
};
short cl_var_gindex[] = {                                      0,
   89,   17,    0,    0,    0,  -54,   20,   69,  -13,   30,
    0,    8,    0,    0,   48,   54,    0,    0,    0,    0,
    0,
};
#define YYTABLESIZE 455
short cl_var_table[] = {                                      71,
   40,   62,   54,    2,   21,   54,   65,  100,   20,   64,
   42,    4,   14,   17,    4,    4,   88,    4,    3,   18,
   58,    3,    3,   58,    3,   65,    8,   19,   64,    8,
    8,   53,    8,    7,   53,    4,    7,    7,   90,    7,
   40,   58,    3,   40,   40,  103,   38,   39,   69,   90,
    8,   69,   69,   41,   69,   76,   91,    7,   40,   40,
   73,   92,   72,   73,   73,   70,   73,   94,   70,   70,
   52,   70,    6,   15,   15,   77,    5,   82,   83,   90,
   15,   81,   43,   56,   19,   60,   44,   55,   19,   60,
   10,    7,   69,   60,   96,   60,   86,  104,   84,   99,
   51,   53,   19,   19,   54,   78,   58,   59,   98,   95,
   93,   68,   55,   60,   80,   55,   49,   50,    0,   51,
    0,    0,    0,   40,   22,   40,  101,  102,   85,   53,
   53,   53,    0,  105,   53,    0,    0,    0,    0,    0,
   53,    0,   22,    0,    0,   53,   53,   53,   53,    0,
    0,   53,   53,    0,    0,    8,    0,    9,    0,    0,
    0,    0,    0,    0,    0,   22,   23,   19,    0,   19,
   25,    0,    0,    0,    0,    0,   26,   53,    0,    0,
    0,   27,   28,   29,   30,    0,    0,   31,   32,    0,
    0,    0,   53,   53,   53,    0,    0,   53,    0,    0,
    0,   35,    0,   53,    0,    0,    0,    0,   53,   53,
   53,   53,    0,   97,   53,   53,    0,    0,    0,   54,
   54,   54,    0,    0,   54,    0,    0,    0,    0,    0,
   54,    0,    0,    0,    0,   54,   54,   54,   54,    0,
   53,   54,   54,    0,    0,    0,    4,    0,    0,    0,
    0,    0,   58,    3,    0,    0,   40,    0,   40,   40,
   40,    8,    0,   40,   34,    0,    0,   54,    7,   40,
   61,   63,   61,   40,   40,   40,   40,   40,   61,    4,
   40,   40,    0,   69,    0,   58,    3,    0,    0,    0,
   67,    0,    0,    0,    8,   73,   53,    0,    0,    0,
   70,    7,   19,   19,   19,    0,   40,   19,    0,    0,
    0,    0,    0,   19,    0,    0,   69,   19,   19,   19,
   19,   19,    0,    0,   19,   19,    0,    0,   73,   55,
   55,   55,    0,   70,   55,    0,    0,    0,   53,   53,
   55,    0,    0,   53,    0,   55,   55,   55,   55,   53,
   19,   55,   55,    0,   53,   53,   53,   53,    0,    0,
   53,   53,    0,    0,    0,   63,   63,   63,    0,    0,
   63,    0,    0,    0,    0,    0,   63,   55,    0,    0,
   63,   63,   63,   63,   63,    0,   53,   63,   63,   53,
   53,   53,    0,    0,   53,    0,    0,    0,   53,   53,
   53,    0,    0,   53,    0,   53,   53,   53,   53,   53,
    0,   53,   53,   63,   53,   53,   53,   53,    0,    0,
   53,   53,   22,   23,   24,    0,    0,   25,    0,    0,
    0,   22,   23,   26,    0,    0,   45,   53,   27,   28,
   29,   30,   26,    0,   31,   32,   53,   27,   46,   47,
   30,    0,    0,   48,   32,
};
short cl_var_check[] = {                                      60,
    0,   60,   41,  256,   38,   44,   41,   60,   42,   44,
   24,   38,  273,    6,   41,   42,   71,   44,   38,   12,
   41,   41,   42,   44,   44,   41,   38,  330,   44,   41,
   42,   41,   44,   38,   44,   62,   41,   42,   44,   44,
   40,   62,   62,   41,   44,  100,   17,   18,   38,   44,
   62,   41,   42,   42,   44,   41,   62,   62,   58,   59,
   38,   75,  123,   41,   42,   38,   44,   62,   41,   42,
  306,   44,   40,    5,    6,   61,   44,   40,   41,   44,
   12,   44,  259,   38,   40,  306,  263,   42,   44,   38,
   58,   59,  306,   42,  331,   44,   59,   62,   61,  306,
  306,  306,   58,   59,   36,   91,   38,   39,   92,   90,
   81,   58,   41,   62,   67,   44,   28,   29,   -1,   31,
   -1,   -1,   -1,  123,   44,  125,   97,   98,   91,  258,
  259,  260,   -1,  104,  263,   -1,   -1,   -1,   -1,   -1,
  269,   -1,   62,   -1,   -1,  274,  275,  276,  277,   -1,
   -1,  280,  281,   -1,   -1,  123,   -1,  125,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,  259,  123,   -1,  125,
  263,   -1,   -1,   -1,   -1,   -1,  269,  306,   -1,   -1,
   -1,  274,  275,  276,  277,   -1,   -1,  280,  281,   -1,
   -1,   -1,  258,  259,  260,   -1,   -1,  263,   -1,   -1,
   -1,  330,   -1,  269,   -1,   -1,   -1,   -1,  274,  275,
  276,  277,   -1,  306,  280,  281,   -1,   -1,   -1,  258,
  259,  260,   -1,   -1,  263,   -1,   -1,   -1,   -1,   -1,
  269,   -1,   -1,   -1,   -1,  274,  275,  276,  277,   -1,
  306,  280,  281,   -1,   -1,   -1,  273,   -1,   -1,   -1,
   -1,   -1,  273,  273,   -1,   -1,  256,   -1,  258,  259,
  260,  273,   -1,  263,  330,   -1,   -1,  306,  273,  269,
  331,  306,  331,  273,  274,  275,  276,  277,  331,  306,
  280,  281,   -1,  273,   -1,  306,  306,   -1,   -1,   -1,
  306,   -1,   -1,   -1,  306,  273,  306,   -1,   -1,   -1,
  273,  306,  258,  259,  260,   -1,  306,  263,   -1,   -1,
   -1,   -1,   -1,  269,   -1,   -1,  306,  273,  274,  275,
  276,  277,   -1,   -1,  280,  281,   -1,   -1,  306,  258,
  259,  260,   -1,  306,  263,   -1,   -1,   -1,  258,  259,
  269,   -1,   -1,  263,   -1,  274,  275,  276,  277,  269,
  306,  280,  281,   -1,  274,  275,  276,  277,   -1,   -1,
  280,  281,   -1,   -1,   -1,  258,  259,  260,   -1,   -1,
  263,   -1,   -1,   -1,   -1,   -1,  269,  306,   -1,   -1,
  273,  274,  275,  276,  277,   -1,  306,  280,  281,  258,
  259,  260,   -1,   -1,  263,   -1,   -1,   -1,  258,  259,
  269,   -1,   -1,  263,   -1,  274,  275,  276,  277,  269,
   -1,  280,  281,  306,  274,  275,  276,  277,   -1,   -1,
  280,  281,  258,  259,  260,   -1,   -1,  263,   -1,   -1,
   -1,  258,  259,  269,   -1,   -1,  263,  306,  274,  275,
  276,  277,  269,   -1,  280,  281,  306,  274,  275,  276,
  277,   -1,   -1,  280,  281,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 348
#if YYDEBUG
char *cl_var_name[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,"'&'",0,"'('","')'","'*'",0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,"':'","';'",
"'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,"LE_AUTO","LE_DOUBLE","LE_INT","LE_STRUCT","LE_BREAK",
"LE_ELSE","LE_LONG","LE_SWITCH","LE_CASE","LE_ENUM","LE_REGISTER","LE_TYPEDEF",
"LE_CHAR","LE_EXTERN","LE_RETURN","LE_UNION","LE_CONST","LE_FLOAT","LE_SHORT",
"LE_UNSIGNED","LE_BOOL","LE_CONTINUE","LE_FOR","LE_SIGNED","LE_VOID",
"LE_DEFAULT","LE_GOTO","LE_SIZEOF","LE_VOLATILE","LE_DO","LE_IF","LE_STATIC",
"LE_WHILE","LE_NEW","LE_DELETE","LE_THIS","LE_OPERATOR","LE_CLASS","LE_PUBLIC",
"LE_PROTECTED","LE_PRIVATE","LE_VIRTUAL","LE_FRIEND","LE_INLINE","LE_OVERLOAD",
"LE_TEMPLATE","LE_TYPENAME","LE_THROW","LE_CATCH","LE_IDENTIFIER",
"LE_STRINGliteral","LE_FLOATINGconstant","LE_INTEGERconstant",
"LE_CHARACTERconstant","LE_OCTALconstant","LE_HEXconstant","LE_POUNDPOUND",
"LE_CComment","LE_CPPComment","LE_NAMESPACE","LE_USING","LE_TYPEDEFname",
"LE_ARROW","LE_ICR","LE_DECR","LE_LS","LE_RS","LE_LE","LE_GE","LE_EQ","LE_NE",
"LE_ANDAND","LE_OROR","LE_ELLIPSIS","LE_CLCL","LE_DOTstar","LE_ARROWstar",
"LE_MULTassign","LE_DIVassign","LE_MODassign","LE_PLUSassign","LE_MINUSassign",
"LE_LSassign","LE_RSassign","LE_ANDassign","LE_ERassign","LE_ORassign",
"LE_MACRO","LE_DYNAMIC_CAST","LE_STATIC_CAST","LE_CONST_CAST",
"LE_REINTERPRET_CAST",
};
char *cl_var_rule[] = {
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
"basic_type_name : LE_SHORT basic_type_name_inter",
"basic_type_name : LE_LONG LE_LONG",
"basic_type_name : LE_LONG LE_INT",
"basic_type_name : basic_type_name_inter",
"translation_unit :",
"translation_unit : translation_unit external_decl",
"$$1 :",
"external_decl : $$1 variables",
"external_decl : error",
"parameter_list :",
"parameter_list : template_parameter",
"parameter_list : parameter_list ',' template_parameter",
"template_parameter : const_spec nested_scope_specifier LE_IDENTIFIER special_star_amp",
"template_parameter : const_spec nested_scope_specifier basic_type_name special_star_amp",
"template_parameter : const_spec nested_scope_specifier LE_IDENTIFIER '<' parameter_list '>' special_star_amp",
"variables : stmnt_starter variable_decl special_star_amp const_spec variable_name_list postfix",
"variables : '(' variable_decl special_star_amp const_spec LE_IDENTIFIER postfix2",
"variables : ',' variable_decl special_star_amp const_spec LE_IDENTIFIER postfix2",
"variables : '(' variable_decl special_star_amp const_spec postfix3",
"variables : ',' variable_decl special_star_amp const_spec postfix3",
"variables : ellipsis_prefix LE_ELLIPSIS ')'",
"ellipsis_prefix : '('",
"ellipsis_prefix : ','",
"variable_name_list : LE_IDENTIFIER",
"variable_name_list : variable_name_list ',' special_star_amp LE_IDENTIFIER",
"postfix3 : ','",
"postfix3 : ')'",
"postfix2 :",
"postfix2 : '='",
"postfix2 : ')'",
"postfix2 : '['",
"postfix : ';'",
"postfix : '='",
"postfix : ')'",
"postfix : '('",
"postfix : '['",
"scope_specifier : LE_IDENTIFIER LE_CLCL",
"scope_specifier : LE_IDENTIFIER '<' parameter_list '>' LE_CLCL",
"nested_scope_specifier :",
"nested_scope_specifier : nested_scope_specifier scope_specifier",
"const_spec :",
"const_spec : LE_CONST",
"const_spec : LE_CONST '*'",
"const_spec : LE_CONST '&'",
"const_spec : LE_CONST '*' '*'",
"amp_item :",
"amp_item : '&'",
"star_list :",
"star_list : star_list '*'",
"special_star_amp : star_list amp_item",
"stmnt_starter :",
"stmnt_starter : ';'",
"stmnt_starter : '{'",
"stmnt_starter : '}'",
"stmnt_starter : ':'",
"variable_decl : const_spec basic_type_name",
"variable_decl : const_spec nested_scope_specifier LE_IDENTIFIER",
"variable_decl : const_spec nested_scope_specifier LE_IDENTIFIER '<' parameter_list '>'",
"$$2 :",
"variable_decl : const_spec LE_STRUCT nested_scope_specifier LE_IDENTIFIER '{' $$2",
"variable_decl : const_spec LE_STRUCT nested_scope_specifier LE_IDENTIFIER",
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


std::string var_consumBracketsContent(char openBrace)
{
	char closeBrace;

	switch(openBrace) {
	case '(': closeBrace = ')'; break;
	case '[': closeBrace = ']'; break;
	case '<': closeBrace = '>'; break;
	case '{': closeBrace = '}'; break;
	default:
    	openBrace = '(';
    	closeBrace = ')';
    	break;
    }

	std::string consumedData;
	int depth = 1;
	while(depth > 0)
    {
    	int ch = cl_scope_lex();
    	if(ch == 0){
        	break;
        }


    	if(ch == closeBrace)
        {
        	consumedData.erase(0, consumedData.find_first_not_of(" "));
        	consumedData.erase(consumedData.find_last_not_of(" ")+1);
        	consumedData += cl_scope_text;

        	depth--;
        	continue;
        }
    	else if(ch == openBrace)
        {
        	consumedData.erase(0, consumedData.find_first_not_of(" "));
        	consumedData.erase(consumedData.find_last_not_of(" ")+1);
        	consumedData += cl_scope_text;

        	depth ++ ;
        	continue;
        }

    	consumedData += cl_scope_text;
    	consumedData += " ";
    }

	return consumedData;
}

void var_consumeDefaultValue(char c1, char c2)
{
	int depth = 0;
	bool cont(true);

	while (depth >= 0) {
    	int ch = cl_scope_lex();
    	if(ch == 0) { break;}

    	if(ch == c1 && depth == 0) {
        	cl_scope_less(0);
        	break;
        }

    	if(ch == c2 && depth == 0) {
        	cl_scope_less(0);
        	break;
        }

    	curr_var.m_defaultValue += cl_scope_text;
    	if(ch == ')' || ch == '}'){
        	depth--;
        	continue;
        } else if(ch == '(' || ch == '{') {
        	depth ++ ;
        	continue;
        }
    }
}

void clean_up()
{
	gs_vars = NULL;

    // restore settings
	setUseIgnoreMacros(true);
	g_isUsedWithinFunc = false;
	
    //do the lexer cleanup
	cl_scope_lex_clean();
}

// return the scope name at the end of the input string
void get_variables(const std::string &in, VariableList &li, const std::map<std::string, std::string> &ignoreMap, bool isUsedWithinFunc)
{
    //provide the lexer with new input
	if( !setLexerInput(in, ignoreMap) ){
    	return;
    }

    //set the parser local output to our variable list
	gs_vars = &li;
	setUseIgnoreMacros(false);

    // the 'g_isUsedWithinFunc' allows us to parse variabels without name
    // this is typical when used as function declaration (e.g. void setValue(bool);)
	g_isUsedWithinFunc = isUsedWithinFunc;

    //call tghe main parsing routine
	cl_var_parse();
	clean_up();
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
{ yyval = yyvsp[-1] + " " + yyvsp[0]; }
break;
case 16:
{ yyval = yyvsp[0]; }
break;
case 19:
{curr_var.Reset(); gs_names.clear(); s_tmpString.clear(); s_templateInitList.clear();}
break;
case 21:
{
                            yyclearin;    /*clear lookahead token*/
                            yyerrok;
/*                            printf("CodeLite: syntax error, unexpected token '%s' found at line %d \n", cl_var_lval.c_str(), cl_scope_lineno);*/
                            }
break;
case 22:
{yyval = "";}
break;
case 23:
{yyval = yyvsp[0];}
break;
case 24:
{yyval = yyvsp[-2] + yyvsp[-1] + " " + yyvsp[0];}
break;
case 25:
{
                            yyval = yyvsp[-3] +  yyvsp[-2] + yyvsp[-1] +yyvsp[0];
                        }
break;
case 26:
{
                            yyval = yyvsp[-3] +  yyvsp[-2] + yyvsp[-1] +yyvsp[0];
                        }
break;
case 27:
{
                            yyval = yyvsp[-6] + yyvsp[-5] + yyvsp[-4] +yyvsp[-3] + yyvsp[-2] + yyvsp[-1] + yyvsp[0] + " " ;
                        }
break;
case 28:
{
                        	if(gs_vars)
                            {
                            	Variable var;
                            	std::string pattern;
                            	curr_var.m_isPtr = (yyvsp[-3].find("*") != (size_t)-1);
                            	curr_var.m_starAmp = yyvsp[-3];
                            	curr_var.m_lineno = cl_scope_lineno;
								curr_var.m_rightSideConst = yyvsp[-2];
								if(curr_var.m_templateDecl.empty())
									curr_var.m_templateDecl = s_templateInitList;
								s_templateInitList.clear();

                            	for(size_t i=0; i< gs_names.size(); i++)
                                {
                                    /*create new variable for every variable name found*/
                                	var = curr_var;
                                	var.m_pattern = "/^" + yyvsp[-5] + " " + yyvsp[-4] + " " + yyvsp[-3] + " " + yyvsp[-2] +  " " + yyvsp[-1] + " $/";
                                	var.m_name = gs_names.at(i);
                                	gs_vars->push_back(var);
                                }
                            	curr_var.Reset();
                            	gs_names.clear();
                            }
                        }
break;
case 29:
{
                        	if(gs_vars)
                            {
                            	Variable var;
                            	std::string pattern;
                            	curr_var.m_pattern       = "/^";
                            	curr_var.m_pattern       += yyvsp[-5] + " " + yyvsp[-4] + " " + yyvsp[-3] + " " + yyvsp[-2] + " " + yyvsp[-1] + " " + yyvsp[0] + "$/";
                            	curr_var.m_isPtr         = (yyvsp[-3].find("*") != (size_t)-1);
                            	curr_var.m_starAmp       = yyvsp[-3];
                            	curr_var.m_arrayBrackets = yyvsp[0];
								curr_var.m_rightSideConst= yyvsp[-2];
                            	curr_var.m_lineno        = cl_scope_lineno;
								if(curr_var.m_templateDecl.empty())
									curr_var.m_templateDecl = s_templateInitList;
								s_templateInitList.clear();	
								
								/*create new variable for every variable name found*/
                                var = curr_var;
                            	var.m_name               = yyvsp[-1];
                            	gs_vars->push_back(var);
                            	curr_var.Reset();
                            	gs_names.clear();
                            }
                        }
break;
case 30:
{
                        	if(gs_vars && g_isUsedWithinFunc)
                            {
                            	Variable var;
                            	std::string pattern;
                            	curr_var.m_pattern         = "/^";
                            	curr_var.m_pattern        += yyvsp[-5] + " " + yyvsp[-4] + " " + yyvsp[-3] + " " + yyvsp[-2] + " " + yyvsp[-1] + " " + yyvsp[0] + " $/";
                            	curr_var.m_isPtr           = (yyvsp[-3].find("*") != (size_t)-1);
                            	curr_var.m_starAmp         = yyvsp[-3];
                            	curr_var.m_arrayBrackets   = yyvsp[0];
								curr_var.m_rightSideConst  = yyvsp[-2];
                            	curr_var.m_lineno          = cl_scope_lineno;
								
								if(curr_var.m_templateDecl.empty())
									curr_var.m_templateDecl = s_templateInitList;
								s_templateInitList.clear();	
								
                                /*create new variable for every variable name found*/
                            	var = curr_var;
                            	var.m_name = yyvsp[-1];
                            	gs_vars->push_back(var);

                            	curr_var.Reset();
                            	gs_names.clear();
                            }
                        }
break;
case 31:
{
                        	if(gs_vars && g_isUsedWithinFunc)
                            {
                            	Variable var;
                            	std::string pattern;
                            	curr_var.m_pattern       = "/^";
                            	curr_var.m_pattern       += yyvsp[-4] + " " + yyvsp[-3] + " " + yyvsp[-2] + " " + yyvsp[-1] + " $/";
                            	curr_var.m_isPtr         = (yyvsp[-2].find("*") != (size_t)-1);
                            	curr_var.m_starAmp       = yyvsp[-2];
								curr_var.m_rightSideConst= yyvsp[-1];
                            	curr_var.m_lineno        = cl_scope_lineno;
								if(curr_var.m_templateDecl.empty())
									curr_var.m_templateDecl = s_templateInitList;
								s_templateInitList.clear();	
								
                                /*create new variable for every variable name found*/
                            	var = curr_var;
                            	var.m_name = "";
                            	gs_vars->push_back(var);

                            	curr_var.Reset();
                            	gs_names.clear();
                            }
                        	if(yyvsp[0] == ",") {
                            	cl_scope_less(0);
                            }
                        }
break;
case 32:
{
                        	if(gs_vars && g_isUsedWithinFunc)
                            {
                            	Variable var;
                            	std::string pattern;
                            	curr_var.m_pattern = "/^";
                            	curr_var.m_pattern += yyvsp[-4] + " " + yyvsp[-3] + " " + yyvsp[-2] + " " + yyvsp[-1] + "$/";
                            	curr_var.m_isPtr = (yyvsp[-2].find("*") != (size_t)-1);
                            	curr_var.m_starAmp = yyvsp[-2];
                            	curr_var.m_lineno = cl_scope_lineno;
								curr_var.m_rightSideConst= yyvsp[-1];
								if(curr_var.m_templateDecl.empty())
									curr_var.m_templateDecl = s_templateInitList;
								s_templateInitList.clear();	
								
                                /*create new variable for every variable name found*/
                            	var = curr_var;
                            	var.m_name = "";
                            	gs_vars->push_back(var);

                            	curr_var.Reset();
                            	gs_names.clear();
                            }
                        	if(yyvsp[0] == ",") {
                            	cl_scope_less(0);
                            }
                        }
break;
case 33:
{
                            /* special type of argument: Ellipsis, can only be at the end of function argument */
                        	if(gs_vars && g_isUsedWithinFunc)
                            {
                            	Variable var;
                            	std::string pattern;
                            	curr_var.m_pattern = "/^";
                            	curr_var.m_pattern += yyvsp[-2] + " " + yyvsp[-1] + " " + yyvsp[0] + " " + "$/";
                            	curr_var.m_isPtr = false;
                            	curr_var.m_starAmp = "";
                            	curr_var.m_lineno = cl_scope_lineno;
                            	curr_var.m_isEllipsis = true;

                            	var = curr_var;
                            	gs_vars->push_back(var);

                            	curr_var.Reset();
                            	gs_names.clear();
                            }
                        }
break;
case 34:
{yyval = yyvsp[0];}
break;
case 35:
{yyval = yyvsp[0];}
break;
case 36:
{
                        	gs_names.push_back(yyvsp[0]);
                            yyval = yyvsp[0];
                        }
break;
case 37:
{
                            /*collect all the names*/
                        	gs_names.push_back(yyvsp[0]);
                            yyval = yyvsp[-3] + yyvsp[-2] + " " + yyvsp[-1] + yyvsp[0];
                        }
break;
case 40:
{yyval = "";}
break;
case 41:
{var_consumeDefaultValue(',', ')'); yyval = ""; }
break;
case 42:
{ yyval = ""; }
break;
case 43:
{ yyval = yyvsp[0] + var_consumBracketsContent('[');}
break;
case 47:
{ yyval = yyvsp[0] + var_consumBracketsContent('(');}
break;
case 48:
{ yyval = yyvsp[0] + var_consumBracketsContent('[');}
break;
case 49:
{yyval = yyvsp[-1]+ yyvsp[0]; }
break;
case 50:
{yyval = yyvsp[-4] ; s_templateInitList = yyvsp[-3] + yyvsp[-2] + yyvsp[-1];}
break;
case 51:
{yyval = "";}
break;
case 52:
{    yyval = yyvsp[-1] + yyvsp[0];}
break;
case 53:
{yyval = "";                          }
break;
case 54:
{ yyval = yyvsp[0];                         }
break;
case 55:
{ yyval = yyvsp[-1] + yyvsp[0]; curr_var.m_rightSideConst = yyval;     }
break;
case 56:
{ yyval = yyvsp[-1] + yyvsp[0]; curr_var.m_rightSideConst = yyval;     }
break;
case 57:
{ yyval = yyvsp[-2] + yyvsp[-1] + yyvsp[0]; curr_var.m_rightSideConst = yyval;}
break;
case 58:
{yyval = ""; }
break;
case 59:
{ yyval = yyvsp[0]; }
break;
case 60:
{yyval = ""; }
break;
case 61:
{yyval = yyvsp[-1] + yyvsp[0];}
break;
case 62:
{ yyval = yyvsp[-1] + yyvsp[0]; }
break;
case 63:
{yyval = "";}
break;
case 64:
{ yyval = ";";}
break;
case 65:
{ yyval = "{";}
break;
case 66:
{ yyval = "}";}
break;
case 67:
{ yyval = ":";}
break;
case 68:
{
                            yyval = yyvsp[-1] + " " + yyvsp[0];
                            yyvsp[0].erase(yyvsp[0].find_last_not_of(":")+1);
                        	curr_var.m_type = yyvsp[0];
							curr_var.m_isBasicType = true;
                        	curr_var.m_isConst = !yyvsp[-1].empty();
                        }
break;
case 69:
{
							yyval = yyvsp[-2] + " " + yyvsp[-1] + yyvsp[0];
                            yyvsp[-1].erase(yyvsp[-1].find_last_not_of(":")+1);
                        	curr_var.m_typeScope = yyvsp[-1];
                        	curr_var.m_type = yyvsp[0];
                        	curr_var.m_isConst = !yyvsp[-2].empty();
                        	s_tmpString.clear();
                        }
break;
case 70:
{
                            yyval = yyvsp[-5] + " " + yyvsp[-4] + yyvsp[-3] + " " + yyvsp[-2] + yyvsp[-1] + yyvsp[0];
                            yyvsp[-4].erase(yyvsp[-4].find_last_not_of(":")+1);
                        	curr_var.m_typeScope = yyvsp[-4];
                        	curr_var.m_type = yyvsp[-3];
                        	curr_var.m_isTemplate = true;
                        	curr_var.m_templateDecl = yyvsp[-2] +yyvsp[-1] +yyvsp[0];
                        	curr_var.m_isConst = !yyvsp[-5].empty();
                        	s_tmpString.clear();
                        }
break;
case 71:
{s_tmpString = var_consumBracketsContent('{');}
break;
case 72:
{
                            yyval = yyvsp[-5] + " " + yyvsp[-4] + " " + yyvsp[-3] + " " + yyvsp[-2] + yyvsp[-1] + yyvsp[0] + s_tmpString;
                            yyvsp[-3].erase(yyvsp[-3].find_last_not_of(":")+1);
                        	curr_var.m_typeScope = yyvsp[-3];
                        	curr_var.m_type = yyvsp[-2];
                        	curr_var.m_isConst = !yyvsp[-5].empty();
                        	s_tmpString.clear();
                        }
break;
case 73:
{
                            yyval = yyvsp[-4];
                            yyvsp[-1].erase(yyvsp[-1].find_last_not_of(":")+1);
                        	curr_var.m_typeScope = yyvsp[-1];
                        	curr_var.m_type = yyvsp[0];
                        	curr_var.m_isTemplate = !curr_var.m_templateDecl.empty();
                        	curr_var.m_isConst = !yyvsp[-3].empty();
                        	s_tmpString.clear();
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
