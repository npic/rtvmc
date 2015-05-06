%{
#include <stdlib.h>
#include "calc.h"
#include "y.tab.h"
void yyerror(char *);
%}

%%

[a-z]       { 
                yylval.sIndex = *yytext - 'a';
                return VARIABLE;
            }

0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[-()<>=+*/;{}.%&|!~^] {
                return *yytext;
             }

"'"             return QUOTE;
"&&"            return AND;
"||"            return OR;
">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"output"        return OUTPUT;
"input"         return INPUT;
"return"        return RETURN;

"process"       return PROCESS;
"Ci"            return CI;
"Ti"            return TI;
"soft"          return SOFT;

[ \t\n]+        ;       /* ignore whitespace */

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
