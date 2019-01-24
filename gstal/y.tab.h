/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     WHITE_SPACE = 258,
     END_LINE = 259,
     TRASH = 260,
     POSINT = 261,
     NEGINT = 262,
     POSFLOAT = 263,
     NEGFLOAT = 264,
     COMMENT = 265,
     ADD = 266,
     SUB = 267,
     MLT = 268,
     DIV = 269,
     EQL = 270,
     NEQ = 271,
     LTH = 272,
     LEQ = 273,
     GTH = 274,
     GEQ = 275,
     STO = 276,
     STM = 277,
     NEG = 278,
     LOD = 279,
     PTN = 280,
     PTC = 281,
     JPF = 282,
     PTL = 283,
     INP = 284,
     RET = 285,
     HLT = 286,
     ISP = 287,
     DSP = 288,
     LAA = 289,
     LRA = 290,
     JMP = 291,
     CAL = 292,
     PAR = 293,
     ADI = 294,
     ADF = 295,
     SBI = 296,
     SBF = 297,
     MLI = 298,
     MLF = 299,
     DVI = 300,
     DVF = 301,
     EQI = 302,
     EQF = 303,
     NEI = 304,
     NEF = 305,
     LTI = 306,
     LTF = 307,
     LEI = 308,
     LEF = 309,
     GTI = 310,
     GTF = 311,
     GEI = 312,
     GEF = 313,
     NGI = 314,
     NGF = 315,
     PTI = 316,
     PTF = 317,
     INI = 318,
     INF = 319,
     LLI = 320,
     LLF = 321,
     FTI = 322,
     ITF = 323,
     NOP = 324
   };
#endif
/* Tokens.  */
#define WHITE_SPACE 258
#define END_LINE 259
#define TRASH 260
#define POSINT 261
#define NEGINT 262
#define POSFLOAT 263
#define NEGFLOAT 264
#define COMMENT 265
#define ADD 266
#define SUB 267
#define MLT 268
#define DIV 269
#define EQL 270
#define NEQ 271
#define LTH 272
#define LEQ 273
#define GTH 274
#define GEQ 275
#define STO 276
#define STM 277
#define NEG 278
#define LOD 279
#define PTN 280
#define PTC 281
#define JPF 282
#define PTL 283
#define INP 284
#define RET 285
#define HLT 286
#define ISP 287
#define DSP 288
#define LAA 289
#define LRA 290
#define JMP 291
#define CAL 292
#define PAR 293
#define ADI 294
#define ADF 295
#define SBI 296
#define SBF 297
#define MLI 298
#define MLF 299
#define DVI 300
#define DVF 301
#define EQI 302
#define EQF 303
#define NEI 304
#define NEF 305
#define LTI 306
#define LTF 307
#define LEI 308
#define LEF 309
#define GTI 310
#define GTF 311
#define GEI 312
#define GEF 313
#define NGI 314
#define NGF 315
#define PTI 316
#define PTF 317
#define INI 318
#define INF 319
#define LLI 320
#define LLF 321
#define FTI 322
#define ITF 323
#define NOP 324




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 32 "gst_parser.y"
{
   int opcode;
   int intval;
   float floatval;
}
/* Line 1529 of yacc.c.  */
#line 193 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

