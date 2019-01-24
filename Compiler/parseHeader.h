#include <stdio.h>

#define EK_INT  0
#define EK_REAL 1
#define EK_VAR  2
#define EK_OP   3

#define OP_ADD  0
#define OP_SUB  1
#define OP_MUL  2
#define OP_DIV  3
#define OP_UMIN 4
#define OP_AND  5
#define OP_OR   6
#define OP_NOT  7
#define OP_LT   8
#define OP_LOE  9
#define OP_GT   10
#define OP_GOE  11
#define OP_EQ   12
#define OP_NEQ  13


#define ST_ASN  0
#define ST_IF   1
#define ST_IFEL 2
#define ST_WHI  3
#define ST_CTG  4
#define ST_PRT  5
#define ST_RD   6
#define ST_EXT  7

struct  syntaxTreeNode {
    struct syntaxTreeNode *link;
    char statementKind;
    struct assignment *assignStmnt;
    struct printStatement *printStmnt;
    struct ifStatement *ifStmnt;
    
};

struct syntaxTree {
    int entries;
    struct  syntaxTreeNode tree [1000];
};

struct symbol {
    char *name;
    int kind; // 1 - array or 0 - scalar
    int type; //0 - int or 1 - real
    int size;
    int address;
    int error;
};

struct symbolTable {
    int entries;
    int nextAddress; 
    struct symbol* table [1000];
};

struct expression {
    char kind;
    char operator;
    struct expression *l_operand;
    struct expression *r_operand;
    char * varName;
    int iValue;
    float rValue;
    int dataType; //0 - int or 1 - real
    struct expression *arrayIndex;
};

struct statement {
    int stmntKind;
    int countDirection; //0 - up or 1 - down
    char *targetVariable;
    struct expression *exp1;
    struct expression *exp2;
    struct statement *body1;
    struct statement *body2;
    struct printListNode *printList;
    struct statement *link;
    struct expression *arrIndex;
};

struct printItem {
    int kind; //0 - exp, 1 - string, 2 - newLine
    struct expression *exp;
    char *string;
    char *newLine;
};

struct printListNode {
    struct printItem *item;
    struct printListNode *link;
};

struct output {
    int count; 
    char* arr[10000];
};

extern struct symbolTable *symbTable;
extern struct statement *syntTree;

void symbTableInsert(char *name, int kind, int type, int size);
void symbTableInit (struct symbolTable *symbTable);
struct symbol* symbTableLookUp (char *varName);
int expressionGen (struct expression *exp);
void codeGen (struct statement *syntTree);
void assignStmntGen (char *varName, struct expression *asgnExp, struct expression *indexExp);
void printStmntGen (struct printListNode *printList);
void ifStmntGen (struct expression *exp, struct statement *body);
void ifElseStmntGen (struct expression *exp, struct statement *ifBody, struct statement *elseBody);
void whileStatementGen(struct expression *exp, struct statement *body);
void countingStatementGen(char *varName, int cntDirection, struct expression *exp1, struct expression *exp2, struct expression *arrIndex, struct statement *body);
void readStatementGen (char * varName, struct expression *indexExp); 
int getExpressionType (struct expression *left, struct expression *right);
void loadAddress (char *varName, struct expression *arrIndex);
void printGstal ();
void printSymbolTable();