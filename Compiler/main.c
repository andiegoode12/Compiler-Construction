/*
 * ========================================================================
 * 
 * main.c ------- In-class example of a main program file to be used in
 *                conjunction with a bison parser.
 * 
 * Programmer --- Andrea Goode
 * 
 * ========================================================================
 */

#include "y.tab.h"
#include "parseHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yyparse();
struct symbolTable *symbTable;
struct statement *syntTree;
struct output *gstalInstr;

int main()
{
   if (yyparse())
        printf("Syntax error\n");
   else {
        gstalInstr = malloc(sizeof(struct output));
        gstalInstr->count = 0;
         //reserve stack space
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ISP %d\n", symbTable->nextAddress);
        gstalInstr->count++;
        //generate code
        codeGen(syntTree);
        //output code
        printGstal();
   }

   return 0;
}

void symbTableInit (struct symbolTable *symbTable) {
    symbTable->entries = 0;
    symbTable->nextAddress = 0;
    return;
}

void symbTableInsert (char *name, int kind, int type, int size) {
    int index = symbTable->entries;
    symbTable->table[index] = malloc(sizeof(struct symbol));
    symbTable->table[index]->name = name;
    symbTable->table[index]->kind = kind;
    symbTable->table[index]->type = type;
    symbTable->table[index]->size = size;
    symbTable->table[index]->address = symbTable->nextAddress;
    symbTable->nextAddress += size;
    symbTable->entries ++;
    return;
}

void codeGen (struct statement *syntTree) {
    struct statement *statement = syntTree;
    //loop through syntax tree and generate gstal code for each statement
    while (statement != NULL) {
        switch (statement->stmntKind) {
            case ST_ASN: {
                assignStmntGen(statement->targetVariable, statement->exp1, statement->exp2);
                break; 
            }
            case ST_PRT: {
                printStmntGen(statement->printList);
                break; 
            }
            case ST_IF: {
                ifStmntGen(statement->exp1, statement->body1);
                break; 
            }
            case ST_IFEL: {
                ifElseStmntGen(statement->exp1, statement->body1, statement->body2);
                break;
            }
            case ST_EXT: {
                gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                sprintf(gstalInstr->arr[gstalInstr->count], "HLT\n");
                gstalInstr->count++;
                break;
            }
            case ST_RD: {
                readStatementGen(statement->targetVariable, statement->exp2);
                break;
            }
            case ST_WHI: {
                whileStatementGen(statement->exp1, statement->body1);
                break;
            }
            case ST_CTG: {
                countingStatementGen(statement->targetVariable, statement->countDirection, statement->exp1, statement->exp2, statement->arrIndex, statement->body1);
                break;
            }
        }
        statement = statement->link;
    }
}

void assignStmntGen (char *varName, struct expression *asgnExp, struct expression *indexExp) {
    int expType;
    struct symbol *sym = malloc(sizeof(struct symbol));
    sym = symbTableLookUp(varName);
    if(sym->error) {
        printf("Error: variable %s does not exist", varName);
        exit(EXIT_FAILURE);
    } if (sym->kind) {
        //arrays
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LAA %d\n", sym->address);
        gstalInstr->count++;
        expType = expressionGen(indexExp);
        if (expType) {
            gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
            sprintf(gstalInstr->arr[gstalInstr->count], "FTI\n");
            gstalInstr->count++;
        }
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ADI\n");
        gstalInstr->count++;
    } else {
        //scalar
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LAA %d\n", sym->address);
        gstalInstr->count++;
    }
    expType = expressionGen(asgnExp);
    //check that data types match
    if (sym->type == 0 && expType == 1) {
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "FTI\n");
        gstalInstr->count++;
    } else if (sym->type == 1 && expType == 0)  {
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ITF\n");
        gstalInstr->count++;
    }
    //store
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "STO\n");
    gstalInstr->count++;
}

void printStmntGen (struct printListNode *printList) {
    while (printList != NULL) {
        struct printItem *item = malloc(sizeof(struct printItem));
        item = printList->item;
        switch (item->kind) {
            case 0: {
                    int expType = expressionGen(item->exp);
                    if (expType) {
                        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                        sprintf(gstalInstr->arr[gstalInstr->count], "PTF\n");
                        gstalInstr->count++;
                    } else {
                        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                        sprintf(gstalInstr->arr[gstalInstr->count], "PTI\n");
                        gstalInstr->count++;
                    }
                    break;
            }
            case 1: {
                    for (int i = 1; item->string[i+1] != '\0'; i++) {
                        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                        sprintf(gstalInstr->arr[gstalInstr->count], "LLI %d\n", item->string[i]);
                        gstalInstr->count++;
                        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                        sprintf(gstalInstr->arr[gstalInstr->count], "PTC\n");
                        gstalInstr->count++;
                    }
                    break;
            }
            case 2: {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "PTL\n");
                    gstalInstr->count++;
                    break;
            }
        }
        printList = printList->link;
    }

    free(printList);
}

void ifStmntGen (struct expression *exp, struct statement *body) {
    //evaluate expression
    int expType = expressionGen(exp);
    //place holder for JPF
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "JPF \n");
    //save address of JPF
    int jumpAddress = gstalInstr->count;
    gstalInstr->count++;
    //generate if body
    codeGen(body);
    //address for JPF to jump to
    int jumpToAddress = gstalInstr->count;
    //back patch JPF
    sprintf(gstalInstr->arr[jumpAddress], "JPF %d\n", jumpToAddress);
}

void ifElseStmntGen (struct expression *exp, struct statement *ifBody, struct statement *elseBody) {
    //evaluate expression
    int expType = expressionGen(exp);
    //place holder for JPF
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "JPF \n");
    //save address of JPF
    int jumpAddress1 = gstalInstr->count;
    gstalInstr->count++;
    //generate if body
    codeGen(ifBody);
    //Add placeholder JMP to jump over else
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "JMP \n");
    //save address of JMP
    int jumpAddress2 = gstalInstr->count;
    gstalInstr->count++;
    //address for JPF to jump to
    int jumpToAddress1 = gstalInstr->count;
    //generate else body
    codeGen(elseBody);
    int jumpToAddress2 = gstalInstr->count;
    //back patch JPF
    sprintf(gstalInstr->arr[jumpAddress1], "JPF %d\n", jumpToAddress1);
    //back patch JMP
    sprintf(gstalInstr->arr[jumpAddress2], "JMP %d\n", jumpToAddress2);

}

void whileStatementGen(struct expression *exp, struct statement *body) {
    //Address to jump to for looping
    int jumpToAddress1 = gstalInstr->count;
    //evaluate expression
    int expType = expressionGen(exp);
    //place holder for JPF
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "JPF \n");
    //save address of JPF
    int jumpAddress = gstalInstr->count;
    gstalInstr->count++;
    //generate while loop body
    codeGen(body);
    //Add JMP statement to loop back to top
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "JMP %d\n", jumpToAddress1);
    gstalInstr->count++;
    //address for JPF to jump to
    int jumpToAddress2 = gstalInstr->count;
    //back patch JPF
    sprintf(gstalInstr->arr[jumpAddress], "JPF %d\n", jumpToAddress2);

}

void countingStatementGen(char *varName, int cntDirection, struct expression *exp1, struct expression *exp2, struct expression *arrIndex, struct statement *body) {
    int expType;
    int jumpAddress;
    struct symbol *sym = malloc(sizeof(struct symbol));
    sym = symbTableLookUp(varName);
    //assign start value to variable
    assignStmntGen(varName, exp1, arrIndex);
    //Address for JMP to jump to
    int jumpToAddress1 = gstalInstr->count;
    //load variable
    loadAddress(varName, arrIndex);
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "LOD\n");
    gstalInstr->count++;
    if(sym->type == 0) {
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ITF\n");
        gstalInstr->count++;
    }
    //generate end value expression
    int toExpType = expressionGen(exp2);
    if(toExpType == 0) {
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ITF\n");
        gstalInstr->count++;
    }
    if (cntDirection) {
        // check if var<=endValue
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "GEF\n");
        gstalInstr->count++;
        //place holder for JPF
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "JPF \n");
        //save address of JPF
        jumpAddress = gstalInstr->count;
        gstalInstr->count++;
        //generate counting loop body
        codeGen(body);
        //sub 1 to variable
        loadAddress (varName, arrIndex);
        loadAddress (varName, arrIndex);
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LOD\n");
        gstalInstr->count++;
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LLI %d\n", 1);
        gstalInstr->count++;
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "SBI\n");
        gstalInstr->count++;
    } else {
        // check if var<=endValue
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LEF\n");
        gstalInstr->count++;
        //place holder for JPF
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "JPF \n");
        //save address of JPF
        jumpAddress = gstalInstr->count;
        gstalInstr->count++;
        //generate counting loop body
        codeGen(body);
        //add 1 to variable
        loadAddress (varName, arrIndex);
        loadAddress (varName, arrIndex);
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LOD\n");
        gstalInstr->count++;
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LLI %d\n", 1);
        gstalInstr->count++;
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ADI\n");
        gstalInstr->count++;
    }
    
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "STO\n");
    gstalInstr->count++;
    //Add JMP statement to loop back to top
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "JMP %d\n", jumpToAddress1);
    gstalInstr->count++;
    //address for JPF to jump to
    int jumpToAddress2 = gstalInstr->count;
    //back patch JPF
    sprintf(gstalInstr->arr[jumpAddress], "JPF %d\n", jumpToAddress2);
}

void loadAddress (char *varName, struct expression *arrIndex) {
    int expType;
    struct symbol *sym = malloc(sizeof(struct symbol));
    sym = symbTableLookUp(varName);
    if (sym->kind) {
        //arrays
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LAA %d\n", sym->address);
        gstalInstr->count++;
        expType = expressionGen(arrIndex);
        if (expType) {
            gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
            sprintf(gstalInstr->arr[gstalInstr->count], "FTI\n");
            gstalInstr->count++;
        }
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ADI\n");
        gstalInstr->count++;
    } else {
        //scalar
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LAA %d\n", sym->address);
        gstalInstr->count++;
    }
}

void readStatementGen(char * varName, struct expression *indexExp) {
    int expType;
    struct symbol *sym = malloc(sizeof(struct symbol));
    sym = symbTableLookUp(varName);
    if(sym->error) {
        printf("Error: variable does not exist");
        exit(EXIT_FAILURE);
    } else if (sym->kind) {
        //arrays
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LAA %d\n", sym->address);
        gstalInstr->count++;
        expType = expressionGen(indexExp);
        if (expType) {
            gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
            sprintf(gstalInstr->arr[gstalInstr->count], "FTI\n");
            gstalInstr->count++;
        }
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ADI\n");
        gstalInstr->count++;
    } else {
        //scalar
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LAA %d\n", sym->address);
        gstalInstr->count++;
    }

    if(sym->type) {
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "INF\n");
        gstalInstr->count++;
    } else {
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "INI\n");
        gstalInstr->count++;
    }
    //store
    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
    sprintf(gstalInstr->arr[gstalInstr->count], "STO\n");
    gstalInstr->count++;

}

int expressionGen (struct expression *exp) {
    if (exp->kind == EK_INT) {
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LLI %d\n", exp->iValue);
        gstalInstr->count++;
    } else if (exp->kind == EK_REAL) {
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "LLF %f\n", exp->rValue);
        gstalInstr->count++;
    } else if (exp->kind == EK_VAR) {
        struct symbol *sym = symbTableLookUp(exp->varName);
        if (sym->kind) {
            gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
            sprintf(gstalInstr->arr[gstalInstr->count], "LAA %d\n", sym->address);
            gstalInstr->count++;
            int expType = expressionGen(exp->arrayIndex);
            if (expType) {
                gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                sprintf(gstalInstr->arr[gstalInstr->count], "FTI\n");
                gstalInstr->count++;
            }
            gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
            sprintf(gstalInstr->arr[gstalInstr->count], "ADI\n");
            gstalInstr->count++;
            gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
            sprintf(gstalInstr->arr[gstalInstr->count], "LOD\n");
            gstalInstr->count++;
        } else {
            gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
            sprintf(gstalInstr->arr[gstalInstr->count], "LAA %d\n", sym->address);
            gstalInstr->count++;
            gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
            sprintf(gstalInstr->arr[gstalInstr->count], "LOD\n");
            gstalInstr->count++;
        }
    } else {
        switch (exp->operator) {
            case OP_ADD: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "ADF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "ADI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_SUB: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "SBF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "SBI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_MUL: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "MLF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "MLI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_DIV: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "DVF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "DVI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_UMIN: {
                expressionGen(exp->r_operand);
                exp->dataType = exp->r_operand->dataType;
                if (exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "NGF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "NGI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_AND: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "MLF\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LLF 0.0\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "NEF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "MLI\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LLI 0\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "NEI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_OR: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "ADF\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LLF 0.0\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "NEF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "ADI\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LLI 0\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "NEI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_NOT: {
                expressionGen(exp->r_operand);
                exp->dataType = exp->r_operand->dataType;
                if (exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LLF 0\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "EQF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LLI 0\n");
                    gstalInstr->count++;
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "EQI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_LT: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LTF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LTI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_LOE: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LEF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "LEI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_GT: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "GTF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "GTI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_GOE: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "GEF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "GEI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_EQ: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "EQF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "EQI\n");
                    gstalInstr->count++;
                }
                break;
            }
            case OP_NEQ: {
                exp->dataType = getExpressionType(exp->l_operand, exp->r_operand);
                if(exp->dataType) {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "NEF\n");
                    gstalInstr->count++;
                } else {
                    gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
                    sprintf(gstalInstr->arr[gstalInstr->count], "NEI\n");
                    gstalInstr->count++;
                }
                break;
            }
        }
    }
    return exp->dataType;
}

int getExpressionType(struct expression *l_operand, struct expression *r_operand) {
    if(l_operand->dataType == 0 && r_operand->dataType == 1) {
        expressionGen(l_operand);
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ITF\n");
        gstalInstr->count++;
        expressionGen(r_operand);
        return 1;
    } else if (l_operand->dataType == 1 && r_operand->dataType == 0) {
        expressionGen(l_operand);
        expressionGen(r_operand);
        gstalInstr->arr[gstalInstr->count] = malloc(20 * sizeof(char));
        sprintf(gstalInstr->arr[gstalInstr->count], "ITF\n");
        gstalInstr->count++;
        return 1;
    } else if (l_operand->dataType == 1 && r_operand->dataType == 1) {
        expressionGen(l_operand);
        expressionGen(r_operand);
        return 1;
    } else {
        expressionGen(l_operand);
        expressionGen(r_operand);
        return 0;
    }
}

struct symbol* symbTableLookUp (char *varName) {
    //basic linear search
    for (int i=0; i < symbTable->entries; i++) {
        if(strcmp(symbTable->table[i]->name, varName) == 0) {
            struct symbol *temp = symbTable->table[i];
            return temp;
        }
    }
    struct symbol *err = malloc(sizeof(struct symbol));
    err->error = 1;
    return err;
}

void printGstal() {
    int index = gstalInstr->count;
    FILE *gstalFile;
    gstalFile = fopen("gstalFile.gstal", "w");
    for(int i=0; i<index && index<10000; i++) {
        fprintf(gstalFile, "%s", gstalInstr->arr[i]);
    }
    fprintf(gstalFile, "HLT");
    fprintf(gstalFile, "\n");
    fclose(gstalFile);
}

void  printSymbolTable() {
    for (int i = 0; i < symbTable->entries; i++) {
        printf("Entry into Symbol Table at index %d:\n", i);
        printf("\tName: %s\n", symbTable->table[i]->name);
        printf("\tType (int (0) real (1)): %d\n", symbTable->table[i]->type);
        printf("\tKind (Scalar(0) or Array(1)): %d\n", symbTable->table[i]->kind);
        printf("\tSize: %d\n", symbTable->table[i]->size);
        printf("\tBase Address:  %d\n", symbTable->table[i]->address);
    }
    return;
}
