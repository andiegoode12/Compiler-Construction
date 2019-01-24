%{

/*
 * ========================================================================
 * 
 * parser.y ----- Description goes here.
 *                   
 * 
 * Programmer --- Bryan Crawley
 * 
 * Date --------- 
 * 
 * Modification History:
 * 
 *       <none>
 * 
 * ========================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gst_run.h"

int error_occured = 0;
extern int lineno;
void checkMem(int);

%}


%union {
   int opcode;
   int intval;
   float floatval;
}

%token WHITE_SPACE
%token END_LINE
%token TRASH

%token <intval> POSINT
%token <intval> NEGINT
%token <floatval> POSFLOAT
%token <floatval> NEGFLOAT
%token COMMENT

%token ADD
%token SUB
%token MLT
%token DIV
%token EQL
%token NEQ
%token LTH
%token LEQ
%token GTH
%token GEQ
%token STO
%token STM
%token NEG
%token LOD
%token PTN
%token PTC
%token JPF
%token PTL
%token INP
%token RET
%token HLT
%token ISP
%token DSP
%token LAA
%token LRA
%token JMP
%token CAL
%token PAR

%token ADI
%token ADF
%token SBI
%token SBF
%token MLI
%token MLF
%token DVI
%token DVF
%token EQI
%token EQF
%token NEI
%token NEF
%token LTI
%token LTF
%token LEI
%token LEF
%token GTI
%token GTF
%token GEI
%token GEF

%token NGI
%token NGF
%token PTI
%token PTF
%token INI
%token INF

%token LLI
%token LLF
%token FTI
%token ITF

%token NOP

%type <opcode> op1
%type <opcode> op2
%type <opcode> op3
%type <opcode> op4
%type <intval> int_const
%type <floatval> float_const

%%

program     : linelist
                  {
                    if (error_occured)
                       YYABORT;
                    else
                       YYACCEPT;
                  }
            ;

linelist    : line
            | linelist  line
            ;

line        : op1  trailer
                  {
                    checkMem(line_count);
                    codemem[line_count].opcode = $1;
                    line_count++;
                  }
            | op2  WHITE_SPACE  POSINT  trailer
                  {
                    checkMem(line_count);
                    codemem[line_count].opcode = $1;
                    codemem[line_count].argument = $3;
                    line_count++;
                  }
            | op3  WHITE_SPACE  int_const  trailer
                  {
                    checkMem(line_count);
                    codemem[line_count].opcode = $1;
                    codemem[line_count].argument = $3;
                    line_count++;
                  }
            | op4  WHITE_SPACE  float_const  trailer
                  {
                    checkMem(line_count);
                    codemem[line_count].opcode = $1;
                    codemem[line_count].argument = MAKEINT($3);
                    line_count++;
                  }
             | error trailer
                  {
                    yyclearin;
                    yyerrok;
                    error_occured = 1;
                  }
            ;

int_const   : POSINT    {$$ = $1;}
            | NEGINT    {$$ = $1;}
            ;

float_const : POSFLOAT  {$$ = $1;}
            | NEGFLOAT  {$$ = $1;}
            ;

trailer     : END_LINE
            | WHITE_SPACE  END_LINE
            | COMMENT  END_LINE
            | WHITE_SPACE  COMMENT  END_LINE
            ;

op1         : ADD     {$$ = ADD;}
            | SUB     {$$ = SUB;}
            | MLT     {$$ = MLT;}
            | DIV     {$$ = DIV;}
            | EQL     {$$ = EQL;}
            | NEQ     {$$ = NEQ;}
            | LTH     {$$ = LTH;}
            | LEQ     {$$ = LEQ;}
            | GTH     {$$ = GTH;}
            | GEQ     {$$ = GEQ;}
            | STO     {$$ = STO;}
            | STM     {$$ = STM;}
            | NEG     {$$ = NEG;}
            | LOD     {$$ = LOD;}
            | PTN     {$$ = PTN;}
            | PTC     {$$ = PTC;}
            | PTL     {$$ = PTL;}
            | INP     {$$ = INP;}
            | RET     {$$ = RET;}
            | HLT     {$$ = HLT;}
            | ADI     {$$ = ADI;}
            | ADF     {$$ = ADF;}
            | SBI     {$$ = SBI;}
            | SBF     {$$ = SBF;}
            | MLI     {$$ = MLI;}
            | MLF     {$$ = MLF;}
            | DVI     {$$ = DVI;}
            | DVF     {$$ = DVF;}
            | EQI     {$$ = EQI;}
            | EQF     {$$ = EQF;}
            | NEI     {$$ = NEI;}
            | NEF     {$$ = NEF;}
            | LTI     {$$ = LTI;}
            | LTF     {$$ = LTF;}
            | LEI     {$$ = LEI;}
            | LEF     {$$ = LEF;}
            | GTI     {$$ = GTI;}
            | GTF     {$$ = GTF;}
            | GEI     {$$ = GEI;}
            | GEF     {$$ = GEF;}
            | NGI     {$$ = NGI;}
            | NGF     {$$ = NGF;}
            | PTI     {$$ = PTI;}
            | PTF     {$$ = PTF;}
            | INI     {$$ = INI;}
            | INF     {$$ = INF;}
            | FTI     {$$ = FTI;}
            | ITF     {$$ = ITF;}
            | NOP     {$$ = NOP;}
            ;

op2         : JPF     {$$ = JPF;}
            | ISP     {$$ = ISP;}
            | DSP     {$$ = DSP;}
            | LAA     {$$ = LAA;}
            | LRA     {$$ = LRA;}
            | JMP     {$$ = JMP;}
            | CAL     {$$ = CAL;}
            | PAR     {$$ = PAR;}
            ;

op3         : LLI     {$$ = LLI;}
            ;

op4         : LLF     {$$ = LLF;}
            ;


%%

/*
 * ========================================================================
 * checkMem(n) 
 * ========================================================================
 */

void checkMem(n)
   int n;
{
   if (n >= code_size) {
      code_size += CODE_MEM_BLOCK;
      codemem = realloc(codemem, code_size*sizeof(struct instruction));
      if (codemem == NULL) {
         fprintf(stderr, "%s: out of code memory\n", runfile);
         exit(1);
      }
   }
   return;
}


/*
 * ========================================================================
 * yyerror(s) -- yacc requires this function, but it is not used in this
 *               project.
 * ========================================================================
 */


yyerror(msg)
   char *msg;
{
   extern char yytext[];
   fprintf(stderr, "%s: Syntax error on line #%d at '%s'\n", runfile, lineno, yytext);
   return 0;
}
