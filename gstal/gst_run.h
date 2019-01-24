#ifndef GST_RUN_H
#define GST_RUN_H

#define CODE_MEM_BLOCK 500
#define DATA_MEM_BLOCK 500

#define INTEGER      int
#define REAL         float

#define MAKEINT(n)   (*((INTEGER *)(&n)))

typedef union {
   int ival;
   float fval;
} stack_entry;

struct instruction {
   int opcode;
   int argument;
};

extern struct instruction *codemem;
extern stack_entry *datamem;
extern int code_size;
extern int data_size;
extern int line_count;
extern int will_dump;
extern char *runfile;

int run();
void listProgram();

#endif
