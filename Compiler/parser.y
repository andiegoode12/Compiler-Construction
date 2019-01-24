%{
/*
 * ========================================================================
 * 
 * Andrea Goode
 * 
 * Checkpoint 4
 * 
 * ========================================================================
 */
#include "parseHeader.h"
#include <stdio.h>
#include <stdlib.h>

int yyerror(char *error);
int yylex();
int tempType;
struct expression *arrayIndex;

%}

%union {
   char *sval;
   int ival;
   float rval;
   struct symbolTable *symbTable;
   struct statement *stmnt;
   struct expression *expression;
   struct assignment *assignment;
   struct ifStatement *ifStmnt;
   struct printStatement *printStmnt;
   struct printItem *printItem;
   struct printListNode *printList;
   struct symbol *symbol;

}


%token        SEMICOLON
%token        COLON
%token        COMMA
%token        EX_POINT
%token        OPEN_PAREN
%token        CLOSE_PAREN
%token        OPEN_BRACKET
%token        CLOSE_BRACKET
%token        ASSIGN
%token        PLUS
%token        MINUS
%token        DIVIDE
%token        MULTIPLY
%token        MODULUS
%token        LESS_THAN
%token        LESS_OR_EQUAL
%token        GREATER_THAN
%token        GREATER_OR_EQUAL
%token        EQUAL
%token        NOT_EQUAL
%token <ival> AND
%token <ival> OR
%token        NOT
%token        RW_MAIN
%token        RW_END
%token        RW_IF
%token        RW_ELSE
%token        RW_WHILE
%token        RW_COUNTING
%token        RW_UPWARD
%token        RW_DOWNWARD
%token        RW_TO
%token        RW_DATA
%token        RW_ALGORITHM
%token        RW_EXIT
%token        RW_READ
%token        RW_PRINT
%token        RW_INTEGER
%token        RW_REAL
%token <ival> INT_CONST
%token <rval> REAL_CONST
%token <sval> STRING_CONST
%token <sval> VARIABLE_NAME
%token        WHITE_SPACE
%token        NEW_LINE
%token        COMMENT
%token        TRASH

%type <stmnt>      algorithm
%type <stmnt>      statement
%type <stmnt>      assignStatement
%type <stmnt>      printStatement
%type <printItem>  printItem
%type <printList>  printList
%type <stmnt>      ifStatement
%type <stmnt>      ifElseStatement
%type <stmnt>      whileStatement
%type <stmnt>      countingStatement
%type <stmnt>      readStatement
%type <stmnt>      exitStatement
%type <expression> startWhile
%type <expression> startIf
%type <sval>       startCounting
%type <ival>       countDirection
%type <expression> expression
%type <expression> compare
%type <expression> term
%type <expression> factor
%type <expression> atom
%type <expression> quark
%type <ival>       boolOperator
%type <ival>       relatOperator
%type <sval>       assignVariable



%%
main            : startMain body endMain 
                ;

body            : dataHeader data algorithmHeader algorithm {
                        syntTree = $4;
                    }  
                ;

data            : declStatement data  
                | declStatement
                ; 

declStatement   : type COLON varlist SEMICOLON 
                ;

varlist         : declVariable COMMA varlist 
                | declVariable
                ;

declVariable    : VARIABLE_NAME {
                        if (symbTableLookUp($1)->error) {
                            symbTableInsert($1, 0, tempType, 1);
                        } else {
                            yyerror("This variable already exists.\n");
                        }
                    }
                | VARIABLE_NAME OPEN_BRACKET INT_CONST CLOSE_BRACKET {
                        symbTableInsert($1, 1, tempType, $3);
                    }
                ;

algorithm       : statement algorithm {
                        $$ = $1;
                        $$->link = $2;
                    }
                | statement {
                        $$ = $1;
                        $$->link = NULL;
                    }
                ;

statement       : assignStatement {
                        $$ = $1;
                    }
                | printStatement {
                        $$ = $1;
                    }
                | ifStatement {
                        $$ = $1;
                    }
                | ifElseStatement {
                        $$ = $1;
                    }
                | readStatement {
                    $$ = $1;
                }
                | exitStatement {
                    $$ = $1;
                }
                | whileStatement {
                    $$ = $1;
                }
                | countingStatement {
                    $$ = $1;
                }
                ;

assignStatement : assignVariable ASSIGN expression SEMICOLON {
                        $$ = malloc(sizeof(struct statement));
                        $$->stmntKind = ST_ASN;
                        $$->targetVariable = $1;

                        $$->exp1 = $3;
                        $$->exp2 = arrayIndex;
                        
                        }
                    ;

printStatement  : RW_PRINT printList SEMICOLON {
                        $$ = malloc(sizeof(struct statement));
                        $$->stmntKind = ST_PRT;
                        $$->printList = $2;
                        }
                    ;

printList       : printItem COMMA printList {
                        $$ = malloc(sizeof(struct printListNode));
                        $$->item = $1;
                        $$->link = $3;
                    }
                | printItem {
                        $$ = malloc(sizeof(struct printListNode));
                        $$->item = $1;
                        $$->link = NULL;
                    }
                ;

printItem       : expression {
                        $$ = malloc(sizeof(struct printItem));
                        $$->kind = 0;
                        $$->exp = $1;
                    }
                | STRING_CONST {
                        $$ = malloc(sizeof(struct printItem));
                        $$->kind = 1;
                        $$->string = $1;
                    }
                | EX_POINT {
                        $$ = malloc(sizeof(struct printItem));
                        $$->kind = 2;
                    }
                ;
            
ifStatement     : startIf algorithm endIf {
                        $$ = malloc(sizeof(struct statement));
                        $$->stmntKind = ST_IF;
                        $$->exp1 = $1;
                        $$->body1 = $2;
                    }
                ;

ifElseStatement : startIf algorithm startElse algorithm endIf {
                        $$ = malloc(sizeof(struct statement));
                        $$->stmntKind = ST_IFEL;
                        $$->exp1 = $1;
                        $$->body1 = $2;
                        $$->body2 = $4;
                    }
                ;

startIf         : RW_IF expression SEMICOLON {
                        $$ = $2;
                    }
                ;

startElse       : RW_ELSE SEMICOLON;

endIf           : RW_END RW_IF SEMICOLON;

whileStatement  : startWhile algorithm endWhile {
                        $$ = malloc(sizeof(struct statement));
                        $$->stmntKind = ST_WHI;
                        $$->exp1 = $1;
                        $$->body1 = $2;
                    }
                ;

startWhile      : RW_WHILE expression SEMICOLON {
                        $$ = $2;
                    }
                ;

endWhile        : RW_END RW_WHILE SEMICOLON;

countingStatement: startCounting countDirection expression RW_TO expression SEMICOLON algorithm endCounting {
                        $$ = malloc(sizeof(struct statement));
                        $$->stmntKind = ST_CTG;
                        $$->targetVariable = $1;
                        $$->countDirection = $2;
                        $$->exp1 = $3;
                        $$->exp2 = $5;
                        $$->body1 = $7;
                        $$->arrIndex = arrayIndex;
                    }
                ;

startCounting   : RW_COUNTING assignVariable {
                        $$ = $2;
                    }
                ;

endCounting     : RW_END RW_COUNTING SEMICOLON;

countDirection  : RW_UPWARD {
                        $$ = 0;
                    }
                | RW_DOWNWARD {
                        $$ = 1;
                    }
                    ;

readStatement : RW_READ assignVariable SEMICOLON{
                        $$ = malloc(sizeof(struct statement));
                        $$->stmntKind = ST_RD;
                        $$->targetVariable = $2;
                        $$->exp2 = arrayIndex;
                    }

assignVariable  : VARIABLE_NAME {
                        $$=$1;
                        arrayIndex = NULL;
                    }
                | VARIABLE_NAME OPEN_BRACKET expression CLOSE_BRACKET {
                        $$=$1;
                        arrayIndex = $3;
                    }
                    ;

exitStatement   : RW_EXIT SEMICOLON {
                        $$ = malloc(sizeof(struct statement));
                        $$->stmntKind = ST_EXT;
                    }
                    ;

expression     : expression boolOperator compare{
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_OP;
                        $$->operator = $2;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                    }
                | NOT expression {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_OP;
                        $$->operator = OP_NOT;
                        $$->l_operand = $2;
                    }
                | NOT compare {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_OP;
                        $$->operator = OP_NOT;
                        $$->r_operand = $2;
                    }
                | compare {
                        $$=$1;
                    }
                ;

compare         : compare relatOperator term {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_OP;
                        $$->operator = $2;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                    }
                | term {
                        $$=$1;
                    }
                ;

term            : term PLUS factor {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_OP;
                        $$->operator = OP_ADD;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                    }
                | term MINUS factor {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_OP;
                        $$->operator = OP_SUB;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                    }
                | factor {
                        $$=$1;
                    }
                ;

factor          : factor MULTIPLY atom {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_OP;
                        $$->operator = OP_MUL;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                    }
                | factor DIVIDE atom {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_OP;
                        $$->operator = OP_DIV;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                    }
                | atom {
                        $$=$1;
                    }
                ;

atom            : quark {
                        $$=$1;
                    }
                | MINUS quark {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_OP;
                        $$->operator = OP_UMIN;
                        $$->r_operand = $2;
                    }
                ;

quark           : INT_CONST {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_INT;
                        $$->iValue = $1;
                        $$->dataType = 0;
                    }
                | REAL_CONST {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_REAL;
                        $$->rValue = $1;
                        $$->dataType = 1;
                    }
                | assignVariable {
                        $$=malloc(sizeof(struct expression));
                        $$->kind = EK_VAR;
                        $$->varName = $1;
                        $$->dataType = symbTableLookUp($1)->type;
                        $$->arrayIndex = arrayIndex;
                    }
                | OPEN_PAREN expression CLOSE_PAREN {
                        $$=$2;
                    }
                ;

boolOperator    : AND {
                        $$=OP_AND;
                    }
                | OR {
                        $$=OP_OR;
                    }
                ;

relatOperator   : LESS_THAN {
                        $$=OP_LT;
                    } 
                | LESS_OR_EQUAL {
                        $$=OP_LOE;
                    }
                | GREATER_THAN {
                        $$=OP_GT;
                    }
                | GREATER_OR_EQUAL {
                        $$=OP_GOE;
                    }
                | EQUAL {
                        $$=OP_EQ;
                    }
                | NOT_EQUAL{
                        $$=OP_NEQ;
                    }
                ;

type            : RW_INTEGER {
                        tempType = 0;
                    }
                | RW_REAL{
                        tempType = 1;
                    }
                ;

startMain       : RW_MAIN SEMICOLON
                ;

endMain         : RW_END RW_MAIN SEMICOLON
                ;

dataHeader      : RW_DATA COLON {
                        symbTable = malloc(sizeof(struct symbolTable));
                        symbTableInit(symbTable);
                    }
                ;

algorithmHeader : RW_ALGORITHM COLON {
}
                ;


%%

int yyerror(char * error) {
   printf("Called yyerror()\n");
   printf("Error: %s\n", error);
   exit(EXIT_FAILURE);
   return  0;
}
