#include "common.h"
#include "asm_space.h"
#include "dctab.h"
#include "rdline.h"
#include "asm_mnmo.h"
#include "symtab.h"
#include "str2int.h"
#include "cal_drctv.h"

int asm_space(Dctab *dp) {
    int len;
    if (!OPerand) {
        fprintf(stderr, "%s --> Operand is not defined ...\n", LBUF);
        exit(10);
    }

    if (dp->type == DC_RESV) {
        if ((!isdigit(OPerand[0]) && OPerand[strlen(OPerand) -1] != 'h') || (len = str2int(OPerand)) == INV_VAL) {
            fprintf(stderr, "%s --> Operand number '%s' is not valid ...\n", LBUF, OPerand);
            exit(11);
        }

        len *= dp->unit;
        OBJC[0] = 0;
        
        return(len);
    }
    
    if (dp->unit == 1 && OPerand[0] == '\"') len = cal_dc_oprnd_string(OPerand, OBJC);
    else len = cal_dc_oprnd(OPerand, dp->unit, OBJC);
    
    return(len);
}