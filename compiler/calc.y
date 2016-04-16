%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "calc.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(int value);
nodeType *hdr(int ci, int ti);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);

void yyerror(char *s);
int sym[26];                    /* symbol table */
%}

%union {
    int iValue;                 /* integer value */
    char sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <sIndex> VARIABLE
%token WHILE IF OUTPUT INPUT RETURN QUOTE PROCESS CI TI SOFT
%nonassoc IFX
%nonassoc ELSE
%left OR
%left AND
%left '|'
%left '^'
%left '&'
%left EQ NE
%left GE LE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS '~' '!'

%type <nPtr> stmt expr stmt_list softprochdr hardprochdr

%%

program:
        softproc hardprocs        { exit(0); }
        ;

softproc:
        softprochdr stmt        { ex($1); ex($2); printf("stop\n"); freeNode($1); freeNode($2); }
        ;

softprochdr:
        PROCESS SOFT        { $$ = hdr(0, 0); }
        ;

hardprocs:
          hardprocs hardprochdr stmt        { ex($2); ex($3); printf("stop\n"); freeNode($2); freeNode($3); }
        | /* NULL */
        ;

hardprochdr:
          PROCESS CI '=' INTEGER TI '=' INTEGER        { $$ = hdr($4, $7); }
        | PROCESS TI '=' INTEGER CI '=' INTEGER        { $$ = hdr($7, $4); }
        ;

stmt:
          ';'                            { $$ = opr(';', 2, NULL, NULL); }
        | expr ';'                       { $$ = $1; }
        | RETURN ';'                     { $$ = opr(RETURN, 0); }
        | OUTPUT expr expr ';'           { $$ = opr(OUTPUT, 2, $2, $3); }
        | INPUT expr VARIABLE ';'        { $$ = opr(INPUT, 2, $2, id($3)); }
        | VARIABLE '=' expr ';'          { $$ = opr('=', 2, id($1), $3); }
        | WHILE '(' expr ')' stmt        { $$ = opr(WHILE, 2, $3, $5); }
        | IF '(' expr ')' stmt %prec IFX { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt { $$ = opr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'              { $$ = $2; }
        ;

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

expr:
          INTEGER               { $$ = con($1); }
        | VARIABLE              { $$ = id($1); }
        | QUOTE VARIABLE QUOTE  { $$ = con($2); }
        | '!' expr %prec '!'    { $$ = opr('!', 1, $2); }
        | '~' expr %prec '~'    { $$ = opr('~', 1, $2); }
        | '-' expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
        | expr '+' expr         { $$ = opr('+', 2, $1, $3); }
        | expr '-' expr         { $$ = opr('-', 2, $1, $3); }
        | expr '*' expr         { $$ = opr('*', 2, $1, $3); }
        | expr '/' expr         { $$ = opr('/', 2, $1, $3); }
        | expr '%' expr         { $$ = opr('%', 2, $1, $3); }
        | expr '<' expr         { $$ = opr('<', 2, $1, $3); }
        | expr '>' expr         { $$ = opr('>', 2, $1, $3); }
        | expr GE expr          { $$ = opr(GE, 2, $1, $3); }
        | expr LE expr          { $$ = opr(LE, 2, $1, $3); }
        | expr NE expr          { $$ = opr(NE, 2, $1, $3); }
        | expr EQ expr          { $$ = opr(EQ, 2, $1, $3); }
        | expr OR expr          { $$ = opr(OR, 2, $1, $3); }
        | expr AND expr         { $$ = opr(AND, 2, $1, $3); }
        | expr '|' expr         { $$ = opr('|', 2, $1, $3); }
        | expr '^' expr         { $$ = opr('^', 2, $1, $3); }
        | expr '&' expr         { $$ = opr('&', 2, $1, $3); }
        | '(' expr ')'          { $$ = $2; }
        ;

%%

nodeType *hdr(int ci, int ti) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeHdr;
    p->hdr.ci = ci;
    p->hdr.ti = ti;

    return p;
}

nodeType *con(int value) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.value = value;

    return p;
}

nodeType *id(int i) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.i = i;

    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");
    if ((p->opr.op = malloc(nops * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
		free (p->opr.op);
    }
    free (p);
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
