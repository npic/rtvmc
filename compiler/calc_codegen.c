#include <stdio.h>
#include "calc.h"
#include "y.tab.h"

static int lbl;

int ex(nodeType *p) {
    int lbl1, lbl2;

    if (!p) return 0;
    switch(p->type) {
        case typeHdr:       
            printf("proc length %d period %d\n", p->hdr.ci, p->hdr.ti); 
            break;
        case typeCon:       
            printf("push %d\n", p->con.value); 
            break;
        case typeId:        
            printf("var %c\n", p->id.i + 'a'); 
            printf("push $%c\n", p->id.i + 'a'); 
            printf("get\n");
            break;
        case typeOpr:
            switch(p->opr.oper) {
                case WHILE:
                    printf("L%03d:\n", lbl1 = lbl++);
                    ex(p->opr.op[0]);
                    printf("not\n");
                    printf("push @L%03d\n", lbl2 = lbl++);
                    printf("jt\n");
                    ex(p->opr.op[1]);
                    printf("push 1\n");
                    printf("push @L%03d\n", lbl1);
                    printf("jt\n");
                    printf("L%03d:\n", lbl2);
                    break;
                case IF:
                    ex(p->opr.op[0]);
                    if (p->opr.nops > 2) {
                        /* if else */
                        printf("not\n");
                        printf("push @L%03d\n", lbl1 = lbl++);
                        printf("jt\n");
                        ex(p->opr.op[1]);
                        printf("push 1\n");
                        printf("push @L%03d\n", lbl2 = lbl++);
                        printf("jt\n");
                        printf("L%03d:\n", lbl1);
                        ex(p->opr.op[2]);
                        printf("L%03d:\n", lbl2);
                    } else {
                        /* if */
                        printf("not\n");
                        printf("push @L%03d\n", lbl1 = lbl++);
                        printf("jt\n");
                        ex(p->opr.op[1]);
                        printf("L%03d:\n", lbl1);
                    }
                    break;
                case OUTPUT:     
                    ex(p->opr.op[0]);
                    ex(p->opr.op[1]);
                    printf("gpioout\n");
                    break;
                case INPUT:     
                    ex(p->opr.op[0]);
                    printf("gpioin\n");
                    printf("push $%c\n", p->opr.op[0]->id.i + 'a');
                    printf("store\n");
                    break;
                case RETURN:     
                    printf("stop\n");
                    break;
                case '=':       
                    ex(p->opr.op[1]);
                    printf("var %c\n", p->opr.op[0]->id.i + 'a'); 
                    printf("push $%c\n", p->opr.op[0]->id.i + 'a');
                    printf("store\n");
                    break;
                case UMINUS:    
                    printf("push 0\n");
                    ex(p->opr.op[0]);
                    printf("sub\n");
                    break;
                case '!':    
                    ex(p->opr.op[0]);
                    printf("not\n");
                    break;
                case '~':    
                    ex(p->opr.op[0]);
                    printf("bnot\n");
                    break;
                default:
                    ex(p->opr.op[0]);
                    ex(p->opr.op[1]);
                    switch(p->opr.oper) {
                        case '+':   printf("add\n"); break;
                        case '-':   printf("sub\n"); break; 
                        case '*':   printf("mul\n"); break;
                        case '/':   printf("div\n"); break;
                        case '%':   printf("mod\n"); break;
                        case '<':   printf("lt\n"); break;
                        case '>':   printf("gt\n"); break;
                        case GE:    printf("geq\n"); break;
                        case LE:    printf("leq\n"); break;
                        case NE:    printf("neq\n"); break;
                        case EQ:    printf("eq\n"); break;
                        case OR:    printf("or\n"); break;
                        case AND:   printf("and\n"); break;
                        case '|':   printf("bor\n"); break;
                        case '^':   printf("bxor\n"); break;
                        case '&':   printf("band\n"); break;
                    }
            }
    }
    return 0;
}
