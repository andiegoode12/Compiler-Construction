#include "y.tab.h"
#include "gst_run.h"
#include "gst_strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_count = 0;
int code_size;
int data_size;
struct instruction *codemem;
char *runfile;
stack_entry *datamem;
int will_dump;

main (argc, argv)
   int argc;
   char *argv[];
{
   int j;
   int p;
   extern FILE *yyin;
   int will_list;
   char *source_file;

   will_dump = 0;
   will_list = 0;

   if (argc == 2){
      source_file = strdup(argv[1]);
      if (source_file[0] == '-') {
         fprintf(stderr, "Usage: %s [-d|-l] file\n", argv[0]);
         exit(1);
      }
   }
   else if (argc == 3) {
      source_file = strdup(argv[2]);
      if (strcmp(argv[1],"-d") == 0)
         will_dump = 1;
      else if (strcmp(argv[1],"-l") == 0)
         will_list = 1;
      else {
         fprintf(stderr, "Usage: %s [-d|-l] file\n", argv[0]);
         exit(1);
      }
   }
   else {
      fprintf(stderr, "Usage: %s [-d|-l] file\n", argv[0]);
      exit(1);
   }

   runfile = strdup(argv[0]);

   if ((yyin = fopen(source_file, "r")) == NULL) {
      fprintf(stderr, "%s: Can't open file %s\n", argv[0], source_file);
      fprintf(stderr, "%s: Run aborted\n", argv[0]);
      exit(1);
   }

   if (will_list) {
      listProgram(yyin);
      exit(0);
   }

   codemem = calloc(CODE_MEM_BLOCK, sizeof(struct instruction));
   code_size = CODE_MEM_BLOCK;

   if ((p = yyparse()) != 0) {
      fprintf(stderr, "%s: Run aborted\n", argv[0]);
      exit(1);
   }
   else
      run();

   exit(0);
}
