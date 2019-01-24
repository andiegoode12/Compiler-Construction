%{ 

/*
 * ========================================================================
 * 
 * Checkpoint 2
 * 
 * Andrea Goode
 * 
 * ========================================================================
 */
 
#define  DEBUG 0
#include "y.tab.h"
#include <stdio.h>
#include <string.h>

%}

%%

";"             { 
                  if(DEBUG) printf("SEMICOLON: %s\n", yytext); 
                  return (SEMICOLON);
                }

":"             { 
                  if(DEBUG) printf("COLON: %s\n", yytext); 
                  return (COLON);
                }

","             { 
                  if(DEBUG) printf("COMMA: %s\n", yytext);
                  return (COMMA);
                }   

"!"             { 
                  if(DEBUG) printf("EX_POINT: %s\n", yytext); 
                  return (EX_POINT);
                } 
            

"("             { 
                  if(DEBUG) printf("OPEN_PAREN: %s\n", yytext);
                  return (OPEN_PAREN);
                }

")"             { 
                  if(DEBUG) printf("CLOSE_PAREN: %s\n", yytext); 
                  return (CLOSE_PAREN);
                }

"["             { 
                  if(DEBUG) printf("OPEN_BRACKET: %s\n", yytext); 
                  return (OPEN_BRACKET);
                }                          

"]"             { 
                  if(DEBUG) printf("CLOSE_BRACKET: %s\n", yytext); 
                  return (CLOSE_BRACKET);
                } 

":="            { 
                  if(DEBUG) printf("ASSIGN: %s\n", yytext); 
                  return (ASSIGN);
                }

"+"             { 
                  if(DEBUG) printf("PLUS: %s\n", yytext); 
                  return (PLUS);
                } 

"-"              { 
                  if(DEBUG) printf("MINUS: %s\n", yytext); 
                  return (MINUS);
                }  

"/"             { 
                  if(DEBUG) printf("DIVIDE: %s\n", yytext); 
                  return (DIVIDE);
                }

"*"             { 
                  if(DEBUG) printf("MULTIPLY: %s\n", yytext); 
                  return (MULTIPLY);
                } 

"%"             { 
                  if(DEBUG) printf("MODULUS: %s\n", yytext); 
                  return (MODULUS);
                } 

"<"              { 
                  if(DEBUG) printf("LESS_THAN: %s\n", yytext); 
                  return (LESS_THAN);
                } 

"<="            { 
                  if(DEBUG) printf("LESS_OR_EQUAL: %s\n", yytext); 
                  return (LESS_OR_EQUAL);
                }

">"             { 
                  if(DEBUG) printf("GREATER_THAN: %s\n", yytext); 
                  return (GREATER_THAN);
                }

">="            { 
                  if(DEBUG) printf("GREATER_OR_EQUAL: %s\n", yytext); 
                  return (GREATER_OR_EQUAL);
                } 

"="              { 
                  if(DEBUG) printf("EQUAL: %s\n", yytext); 
                  return (EQUAL);
                }

"<>"            { 
                  if(DEBUG) printf("NOT_EQUAL: %s\n", yytext); 
                  return (NOT_EQUAL);
                }

"&"             { 
                  if(DEBUG) printf("AND: %s\n", yytext); 
                  return (AND);
                }

"|"             { 
                  if(DEBUG) printf("OR: %s\n", yytext); 
                  return (OR);
                }

"~"             { 
                  if(DEBUG) printf("NOT: %s\n", yytext); 
                  return (NOT);
                }

[mM][aA][iI][nN] { 
                  if(DEBUG) printf("RW_MAIN: %s\n", yytext); 
                  return (RW_MAIN);
                } 

[eE][nN][dD]     { 
                  if(DEBUG) printf("RW_END: %s\n", yytext); 
                  return (RW_END);
                }

[iI][fF]         { 
                  if(DEBUG) printf("RW_IF: %s\n", yytext); 
                  return (RW_IF);
                }

[eE][lL][sS][eE]  { 
                  if(DEBUG) printf("RW_ELSE: %s\n", yytext); 
                  return (RW_ELSE);
                }

[wW][hH][iI][lL][eE] { 
                  if(DEBUG) printf("RW_WHILE: %s\n", yytext); 
                  return (RW_WHILE);
                }      

[cC][oO][uU][nN][tT][iI][nN][gG] { 
                  if(DEBUG) printf("RW_COUNTING: %s\n", yytext); 
                  return (RW_COUNTING);
                }                   

[uU][pP][wW][aA][rR][dD] { 
                  if(DEBUG) printf("RW_UPWARD: %s\n", yytext); 
                  return (RW_UPWARD);
                }                        

[dD][oO][wW][nN][wW][aA][rR][dD] { 
                  if(DEBUG) printf("RW_DOWNWARD: %s\n", yytext); 
                  return (RW_DOWNWARD);
                }                       

[tT][oO]        { 
                  if(DEBUG) printf("RW_TO: %s\n", yytext); 
                  return (RW_TO);
                }  

[dD][aA][tT][aA] { 
                  if(DEBUG) printf("RW_DATA: %s\n", yytext); 
                  return (RW_DATA);
                }						             

[aA][lL][gG][oO][rR][iI][tT][hH][mM] { 
                  if(DEBUG) printf("RW_ALGORITHM: %s\n", yytext); 
                  return (RW_ALGORITHM);
                } 

[eE][xX][iI][tT] { 
                  if(DEBUG) printf("RW_EXIT: %s\n", yytext); 
                  return (RW_EXIT);
                }                      

[rR][eE][aA][dD] { 
                  if(DEBUG) printf("RW_READ: %s\n", yytext); 
                  return (RW_READ);
                }   

[pP][rR][iI][nN][tT] { 
                  if(DEBUG) printf("RW_PRINT: %s\n", yytext); 
                  return (RW_PRINT);
                }             

[iI][nN][tT][eE][gG][eE][rR] { 
                  if(DEBUG) printf("RW_INTEGER: %s\n", yytext); 
                  yylval.sval = strdup(yytext);
                  return (RW_INTEGER);
                }                

[rR][eE][aA][lL] { 
                  if(DEBUG) printf("RW_REAL: %s\n", yytext); 
                  yylval.sval = strdup(yytext);
                  return (RW_REAL);
                }  

[0-9]+          { 
                  if(DEBUG) printf("INT_CONST: %s\n", yytext); 
                  yylval.ival = atoi(yytext);
                  return (INT_CONST);
                }                     

((-+)?[0-9]+\.[0-9]+)|((-+)?[0-9](\.[0-9]+)?[eE](-+)?[0-9]+) { 
                  if(DEBUG) printf("REAL_CONST: %s\n", yytext); 
                   yylval.rval = atof(yytext);
                  return (REAL_CONST);
                }    

\"(([^"\n])|(\"\"))*["\n] { 
                  if(DEBUG) printf("STRING_CONST: %s\n", yytext); 
                  yylval.sval = strdup(yytext);
                  return (STRING_CONST);
                }  

[a-zA-Z][a-zA-Z0-9]* { 
                  if(DEBUG) printf("VARIABLE_NAME: %s\n", yytext); 
                  yylval.sval = strdup(yytext);
                  return (VARIABLE_NAME);
                }  

[\t ]+			{ 
                  if(DEBUG) printf("WHITE_SPACE: %s\n", yytext); 
                }				

\n              { 
                  if(DEBUG) printf("NEW_LINE: %s\n", yytext);
                }            

#.*             { 
                  if(DEBUG) printf("COMMENT: %s\n", yytext); 
                }         

.				{ 
                  if(DEBUG) printf("TRASH: %s\n", yytext); 
                  return (TRASH);
                }

%%

