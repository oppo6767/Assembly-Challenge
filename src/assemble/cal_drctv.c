#include "common.h"
#include "cal_drctv.h"
#include "rdline.h"
#include "asm_mnmo.h"
#include "cal_expr.h"

int cal_dc_oprnd(char* oprnd, int unit, unsigned char obj[]) {
    char buf[128];
    char* exp;
    char* op = (char*)(obj +1);
    short* sp = (short*)(obj +1);
    int* ip = (int*)(obj +1);
    int n = 0;
    int v;
    int len = 0;

    strcpy(buf, oprnd);
    exp = strtok(buf, ",");
    do {
        if (++n > MAX_DNUM) {
            fprintf(stderr, "%s --> Operand '%s' is not valid ...\n", LBUF, oprnd);
            exit(11);
        }

        v = cal_one_expr(exp);
        switch(unit) {
            case 4:
                *op = v >> 24;
                op++;
                len++;
                *op = v >> 16;
                op++;
                len++;
                //break;
            case 2:
                *op = v >> 8;
                op++;
                len++;
                //break;
            case 1:
                *op = v;
                op++;
                len++;
                //break;
        }
    } while ((exp = strtok(NULL, ",")));
    
    obj[0] = len;
    
    return(len);
}

int cal_dc_oprnd_string(char* oprnd, unsigned char obj[]) {
    int i;
    for (i = 1; oprnd[i] && oprnd[i] != '\"' && i <= MAX_DNUM; i++)
        obj[i] = oprnd[i];
    
    if (oprnd[i] != '\"' || oprnd[i +1]) {
        fprintf(stderr, "%s --> Operand '%s' is not valid ...\n", LBUF, oprnd);
        exit(11);
    }

    obj[i] = '\0'; obj[0] = i - 1;
    
    return (i - 1);
}