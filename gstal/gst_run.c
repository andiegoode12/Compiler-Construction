/*
 * ========================================================================
 * 
 * run.c -------- Description goes here.
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

#include "gst_run.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

int pc;
int tos;
int act;
extern char *runfile;
void runError();


/*
 * ========================================================================
 * listProgram() -- Write a numbered listing to the stdout.
 * ========================================================================
 */

void listProgram(infile)
   FILE *infile;
{
   char line[1024];
   int linecount=0;
   
   while (fgets(line, 1023, infile)) {
      printf("[%d] %s", linecount, line);
      linecount++;
   }
   
   return;

}


/*
 * ========================================================================
 * growStack() -- Allocate more space for the stack.
 * ========================================================================
 */

void growStack()
{
   data_size += DATA_MEM_BLOCK;
   datamem = realloc(datamem, sizeof(stack_entry)*data_size);
   if (!datamem) {
      runError(pc,"stack overflow");
   } 
}


/*
 * ========================================================================
 * push(stack_entry n) -- Push n onto the stack.
 * ========================================================================
 */

void push(n)
   stack_entry n;
{
   tos++;
   if (tos >= data_size) {
      growStack();
   }
   datamem[tos] = n;
   return;
}


/*
 * ========================================================================
 * pop() -- Pop and return entry from the stack.
 * ========================================================================
 */

stack_entry pop()
{
   if (tos < 0) {
      runError(pc,"stack underflow");
   }
   tos--;
   return (datamem[tos+1]);
}


/*
 * ========================================================================
 * runError() -- Report a runtime error and terminate the program.
 * ========================================================================
 */

void runError(pc, msg)
   int pc;
   char *msg;
{  
   int j;
   FILE *dumpfile;
   
   fprintf(stderr, "%s: Execution error on line #%d (%s)\n", runfile, pc, msg);

   /* Stack dump */
   if (will_dump) {
      if ((dumpfile = fopen("stackdump", "w")) == NULL)
         fprintf(stderr, "%s: Can't open file %s\n", runfile, "stackdump");
      else {
         fprintf(stderr, "%s: Stack dump\n", runfile);
         if (tos < 0)
            fprintf(dumpfile, "Stack is empty\n");
         else
            for (j=tos; j>=0; j--)
               fprintf(dumpfile, "[%d] 0x%08x\n", j, datamem[j].ival);
         fclose(dumpfile);
      }
   }
   
   fprintf(stderr, "%s: Run aborted\n", runfile);
   exit(1);
}


/*
 * ========================================================================
 * run() -- Main driver for performing the interpretation.
 * ========================================================================
 */

int run()
{
   int halt_addr;
   int op;
   int arg;
   stack_entry a, b, c;
   stack_entry x, y, z;
   
   datamem = NULL;
   data_size = 0;

   pc  = 0;
   tos = -1;
   act = 0;
   halt_addr = line_count;

   while (pc < halt_addr) {
      op  = codemem[pc].opcode;
      arg = codemem[pc].argument;
      switch (op) {
         case ADD:
            b = pop();
            a = pop();
            c.ival = a.ival + b.ival;
            push(c);
            pc++;
            break;
         case SUB:
            b = pop();
            a = pop();
            c.ival = a.ival - b.ival;
            push(c);
            pc++;
            break;
         case MLT:
            b = pop();
            a = pop();
            c.ival = a.ival * b.ival;
            push(c);
            pc++;
            break;
         case DIV:
            b = pop();
            a = pop();
            if (b.ival==0) {
               push(a);
               push(b);
               runError(pc,"integer division by zero");
            }
            c.ival = a.ival / b.ival;
            push(c);
            pc++;
            break;
         case EQL:
            b = pop();
            a = pop();
            c.ival = a.ival == b.ival;
            push(c);
            pc++;
            break;
         case NEQ:
            b = pop();
            a = pop();
            c.ival = a.ival != b.ival;
            push(c);
            pc++;
            break;
         case LTH:
            b = pop();
            a = pop();
            c.ival = a.ival < b.ival;
            push(c);
            pc++;
            break;
         case LEQ:
            b = pop();
            a = pop();
            c.ival = a.ival <= b.ival;
            push(c);
            pc++;
            break;
         case GTH:
            b = pop();
            a = pop();
            c.ival = a.ival > b.ival;
            push(c);
            pc++;
            break;
         case GEQ:
            b = pop();
            a = pop();
            c.ival = a.ival >= b.ival;
            push(c);
            pc++;
            break;
         case STO:
            b = pop();
            a = pop();
            if (a.ival<0 || a.ival>tos) {
               push(a);
               push(b);
               runError(pc,"memory reference out of range on STO");
            }
            datamem[a.ival] = b;
            pc++;
            break;
         case STM:
            b = pop();
            a = pop();
            if (b.ival<0 || b.ival>tos) {
               push(a);
               push(b);
               runError(pc,"memory reference out of range on STM");
            }
            datamem[b.ival] = a;
            push(b);
            pc++;
            break;

         case NEG:
            b = pop();
            c.ival = -b.ival;
            push(c);
            pc++;
            break;
         case LOD:
            b = pop();
            if (b.ival<0 || b.ival>tos) {
               push(b);
               runError(pc,"memory reference out of range on LOD");
            }
            push(datamem[b.ival]);
            pc++;
            break;
         case PTN:
            a = pop();
            printf("%d", a.ival);
            pc++;
            break;
         case PTC:
            a = pop();
            printf("%c", a.ival);
            pc++;
            break;
         case JPF:
            if ((arg >= line_count) || (arg < 0)) {
               runError(pc,"JPF target address out of range");
            }
            b = pop();
            if (b.ival==0)
               pc = arg;
            else
               pc++;
            break;

         case PTL:
            printf("\n");
            pc++;
            break;
         case INP:
            scanf("%d", &a.ival);
            push(a);
            pc++;
            break;
         case RET:
            if (act<0 || act+1>tos) {
               runError(pc,"RET return address out of range");
            }
            pc = datamem[act+1].ival;
            tos = act-1;
            act = datamem[act].ival;
            pc++;
            break;
         case HLT:
            pc = halt_addr;
            break;

         case ISP:
            tos = tos + arg;
            while (tos >= data_size)
               growStack();
            pc++;
            break;
         case DSP:
            tos = tos - arg;
            pc++;
            break;
         case LAA:
            a.ival = arg;
            push(a);
            pc++;
            break;
         case LRA:
            a.ival = act + arg;
            push(a);
            pc++;
            break;
         case JMP:
            if ((arg >= line_count) || (arg < 0)) {
               runError(pc,"JMP target address out of range");
            }
            pc = arg;
            break;
         case CAL:
            a.ival = act;
            push(a);
            act = tos;
            b.ival = pc;
            push(b);
            pc = arg;
            break;
         case PAR:
            a.ival = act - arg;
            push(a);
            pc++;
            break;

         case ADI:
            b = pop();
            a = pop();
            c.ival = a.ival + b.ival;
            push(c);
            pc++;
            break;
         case SBI:
            b = pop();
            a = pop();
            c.ival = a.ival - b.ival;
            push(c);
            pc++;
            break;
         case MLI:
            b = pop();
            a = pop();
            c.ival = a.ival * b.ival;
            push(c);
            pc++;
            break;
         case DVI:
            b = pop();
            a = pop();
            if (b.ival==0) {
               push(a);
               push(b);
               runError(pc,"integer division by zero");
            }
            c.ival = a.ival / b.ival;
            push(c);
            pc++;
            break;

         case ADF:
            y = pop();
            x = pop();
            z.fval = x.fval + y.fval;
            push(z);
            pc++;
            break;
         case SBF:
            y = pop();
            x = pop();
            z.fval = x.fval - y.fval;
            push(z);
            pc++;
            break;
         case MLF:
            y = pop();
            x = pop();
            z.fval = x.fval * y.fval;
            push(z);
            pc++;
            break;
         case DVF:
            y = pop();
            x = pop();
            if (y.fval==0.0) {
               push(x);
               push(y);
               runError(pc,"floating-point division by zero");
            }
            z.fval = x.fval / y.fval;
            push(z);
            pc++;
            break;

         case EQI:
            b = pop();
            a = pop();
            c.ival = a.ival == b.ival;
            push(c);
            pc++;
            break;
         case NEI:
            b = pop();
            a = pop();
            c.ival = a.ival != b.ival;
            push(c);
            pc++;
            break;
         case LTI:
            b = pop();
            a = pop();
            c.ival = a.ival < b.ival;
            push(c);
            pc++;
            break;
         case LEI:
            b = pop();
            a = pop();
            c.ival = a.ival <= b.ival;
            push(c);
            pc++;
            break;
         case GTI:
            b = pop();
            a = pop();
            c.ival = a.ival > b.ival;
            push(c);
            pc++;
            break;
         case GEI:
            b = pop();
            a = pop();
            c.ival = a.ival >= b.ival;
            push(c);
            pc++;
            break;

         case EQF:
            y = pop();
            x = pop();
            c.ival = x.fval == y.fval;
            push(c);
            pc++;
            break;
         case NEF:
            y = pop();
            x = pop();
            c.ival = x.fval != y.fval;
            push(c);
            pc++;
            break;
         case LTF:
            y = pop();
            x = pop();
            c.ival = x.fval < y.fval;
            push(c);
            pc++;
            break;
         case LEF:
            y = pop();
            x = pop();
            c.ival = x.fval <= y.fval;
            push(c);
            pc++;
            break;
         case GTF:
            y = pop();
            x = pop();
            c.ival = x.fval > y.fval;
            push(c);
            pc++;
            break;
         case GEF:
            y = pop();
            x = pop();
            c.ival = x.fval >= y.fval;
            push(c);
            pc++;
            break;

         case NGI:
            a = pop();
            c.ival = -a.ival;
            push(c);
            pc++;
            break;
         case NGF:
            x = pop();
            z.fval = -x.fval;
            push(z);
            pc++;
            break;
         case PTI:
            a = pop();
            printf("%d", a.ival);
            pc++;
            break;
         case PTF:
            x = pop();
            printf("%e", x.fval);
            pc++;
            break;
         case INI:
            scanf("%d", &a.ival);
            push(a);
            pc++;
            break;
         case INF:
            scanf("%f", &x.fval);
            push(x);
            pc++;
            break;
         case LLI:
         	a.ival = arg;
            push(a);
            pc++;
            break;
         case LLF:
         	a.ival = arg;  /* arg is already an int */
            push(a);
            pc++;
            break;
         case FTI:
            x = pop();
            a.ival = (INTEGER) x.fval;
            push(a);
            pc++;
            break;
         case ITF:
            a = pop();
            x.fval = (REAL) a.ival;
            push(x);
            pc++;
            break;
         case NOP:
	    /* Do nothing. Go on to next instruction. */
            pc++;
            break;

         }
      }
   return (0);
}
