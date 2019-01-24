%{

#include <string.h>
#include <stdlib.h>
#include "y.tab.h"
#include "gst_debug.h"

extern YYSTYPE yylval;
int lineno = 0;

%}

%%

[\t ]+                     {
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"WHITE_SPACE");
                             return(WHITE_SPACE);
                           }

";".*                      { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"COMMENT");
                             return(COMMENT);
                           }

"+"?[0-9]+                 { 
                             yylval.intval = atoi((char *)yytext);
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"POSINT");
                             return(POSINT);
                           }

"-"[0-9]+                  { 
                             yylval.intval = atoi((char *)yytext);
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"NEGINT");
                             return(NEGINT);
                           }

"+"?(([0-9]+\.[0-9]+)|([0-9]+(\.[0-9]+)?[Ee][+-]?[0-9]+)) { 
                             yylval.floatval = atof((char *)yytext);
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"POSFLOAT");
                             return(POSFLOAT);
                           }

"-"(([0-9]+\.[0-9]+)|([0-9]+(\.[0-9]+)?[Ee][+-]?[0-9]+)) { 
                             yylval.floatval = atof((char *)yytext);
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"NEGFLOAT");
                             return(NEGFLOAT);
                           }

^[\t ]*[Aa][Dd][Dd]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"ADD");
                             return(ADD);
                           }

^[\t ]*[Ss][Uu][Bb]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"SUB");
                             return(SUB);
                           }

^[\t ]*[Mm][Ll][Tt]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"MLT");
                             return(MLT);
                           }

^[\t ]*[Dd][Ii][Vv]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"DIV");
                             return(DIV);
                           }

^[\t ]*[Ee][Qq][Ll]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"EQL");
                             return(EQL);
                           }

^[\t ]*[Nn][Ee][Qq]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"NEQ");
                             return(NEQ);
                           }

^[\t ]*[Ll][Tt][Hh]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LTH");
                             return(LTH);
                           }

^[\t ]*[Ll][Ee][Qq]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LEQ");
                             return(LEQ);
                           }

^[\t ]*[Gg][Tt][Hh]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"GTH");
                             return(GTH);
                           }

^[\t ]*[Gg][Ee][Qq]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"GEQ");
                             return(GEQ);
                           }

^[\t ]*[Ss][Tt][Oo]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"STO");
                             return(STO);
                           }

^[\t ]*[Ss][Tt][Mm]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"STM");
                             return(STM);
                           }

^[\t ]*[Nn][Ee][Gg]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"NEG");
                             return(NEG);
                           }

^[\t ]*[Ll][Oo][Dd]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LOD");
                             return(LOD);
                           }

^[\t ]*[Pp][Tt][Nn]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"PTN");
                             return(PTN);
                           }

^[\t ]*[Pp][Tt][Cc]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"PTC");
                             return(PTC);
                           }

^[\t ]*[Jj][Pp][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"JPF");
                             return(JPF);
                           }

^[\t ]*[Pp][Tt][Ll]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"PTL");
                             return(PTL);
                           }

^[\t ]*[Ii][Nn][Pp]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"INP");
                             return(INP);
                           }

^[\t ]*[Rr][Ee][Tt]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"RET");
                             return(RET);
                           }

^[\t ]*[Hh][Ll][Tt]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"HLT");
                             return(HLT);
                           }

^[\t ]*[Ii][Ss][Pp]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"ISP");
                             return(ISP);
                           }

^[\t ]*[Dd][Ss][Pp]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"DSP");
                             return(DSP);
                           }

^[\t ]*[Ll][Aa][Aa]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LAA");
                             return(LAA);
                           }

^[\t ]*[Ll][Rr][Aa]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LRA");
                             return(LRA);
                           }

^[\t ]*[Jj][Mm][Pp]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"JMP");
                             return(JMP);
                           }

^[\t ]*[Cc][Aa][Ll]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"CAL");
                             return(CAL);
                           }

^[\t ]*[Pp][Aa][Rr]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"PAR");
                             return(PAR);
                           }


^[\t ]*[Aa][Dd][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"ADI");
                             return(ADI);
                           }


^[\t ]*[Aa][Dd][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"ADF");
                             return(ADF);
                           }


^[\t ]*[Ss][Bb][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"SBI");
                             return(SBI);
                           }


^[\t ]*[Ss][Bb][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"SBF");
                             return(SBF);
                           }


^[\t ]*[Mm][Ll][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"MLI");
                             return(MLI);
                           }


^[\t ]*[Mm][Ll][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"MLF");
                             return(MLF);
                           }


^[\t ]*[Dd][Vv][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"DVI");
                             return(DVI);
                           }


^[\t ]*[Dd][Vv][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"DVF");
                             return(DVF);
                           }


^[\t ]*[Ee][Qq][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"EQI");
                             return(EQI);
                           }


^[\t ]*[Ee][Qq][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"EQF");
                             return(EQF);
                           }


^[\t ]*[Nn][Ee][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"NEI");
                             return(NEI);
                           }


^[\t ]*[Nn][Ee][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"NEF");
                             return(NEF);
                           }


^[\t ]*[Ll][Tt][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LTI");
                             return(LTI);
                           }


^[\t ]*[Ll][Tt][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LTF");
                             return(LTF);
                           }


^[\t ]*[Ll][Ee][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LEI");
                             return(LEI);
                           }


^[\t ]*[Ll][Ee][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LEF");
                             return(LEF);
                           }


^[\t ]*[Gg][Tt][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"GTI");
                             return(GTI);
                           }


^[\t ]*[Gg][Tt][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"GTF");
                             return(GTF);
                           }


^[\t ]*[Gg][Ee][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"GEI");
                             return(GEI);
                           }


^[\t ]*[Gg][Ee][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"GEF");
                             return(GEF);
                           }


^[\t ]*[Nn][Gg][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"NGI");
                             return(NGI);
                           }


^[\t ]*[Nn][Gg][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"NGF");
                             return(NGF);
                           }


^[\t ]*[Pp][Tt][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"PTI");
                             return(PTI);
                           }


^[\t ]*[Pp][Tt][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"PTF");
                             return(PTF);
                           }


^[\t ]*[Ii][Nn][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"INI");
                             return(INI);
                           }


^[\t ]*[Ii][Nn][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"INF");
                             return(INF);
                           }


^[\t ]*[Ll][Ll][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LLI");
                             return(LLI);
                           }


^[\t ]*[Ll][Ll][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"LLF");
                             return(LLF);
                           }


^[\t ]*[Ff][Tt][Ii]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"FTI");
                             return(FTI);
                           }


^[\t ]*[Ii][Tt][Ff]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"ITF");
                             return(ITF);
                           }


^[\t ]*[Nn][Oo][Pp]        { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"NOP");
                             return(NOP);
                           }


\n                         { 
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"END_LINE");
                             lineno++;
                             return(END_LINE);
                           }

.                          { 
                             /*********** Everything else ***********/
                             if (DEBUG) printf("text: %s   token: %s\n",yytext,"TRASH");
                             return(TRASH);
                           }

%%
