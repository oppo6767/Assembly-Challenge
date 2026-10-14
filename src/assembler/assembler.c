#include "common.h"
#include "assembler.h"
#include "optab.h"
#include "dctab.h"
#include "symtab.h"
#include "rdline.h"
#include "put_list.h"
#include "str2int.h"
#include "asm_space.h"
#include "cal_mnmo.h"
#include "asm_mnmo.h"

int LOC = 0;
int LDaddr;
int GOaddr;
int PGsize;
char* null_LNO = "   ";
char* null_LBL = "        ";
char* null_OPR = "        ";

void OnePassAssemble(char *sfile) {
    FILE* fp;
    Optab* op;
    Dctab* dp;
    int len;

    if (!(fp = fopen(sfile, "r"))) {
        fprintf(stderr, "File '%s' not found...\n", sfile);
        exit(1);
    }

    put_list_head();
    read_line(fp);
    //if (!LABEL) fprintf(stderr, "\n%s --> Program name is not defined ...\n", LBUF), exit(1);
    //
    if (!LABEL) LABEL = "ASMBLY";
    if (!OPcode || strcmp(OPcode, "START")) {
        fprintf(stderr, "\n%s --> Opcode is not 'START' ...\n", LBUF);
        exit(1);
    }

    if (OPerand && (LOC = str2int(OPerand)) < 0) {
        fprintf(stderr, "\n%s --> Start address is invalid ...\n", LBUF);
        exit(2);
    }

    ins_SYMTAB(LOC, LABEL);
    put_list();
    LDaddr = GOaddr = LOC;

    while (read_line(fp) > 0) {
        if (!OPcode) {
            fprintf(stderr, "\n%s --> Opcode is not defined ...\n", LBUF);
            exit(4);
        }

        if (!strcmp(OPcode, "END")) break;
        if (LABEL) {
            switch (ins_SYMTAB(LOC, LABEL)) {
                case SYM_TOO_LONG:
                    fprintf(stderr, "\n%s --> Symbol '%s' is too long ...\n", LBUF, LABEL);
                    exit(5);
                    break;
                case SYM_DUPLICATED:
                    fprintf(stderr, "\n%s --> Symbol '%s' is duplicated ...\n", LBUF, LABEL);
                    exit(5);
                    break;
                case SYM_TABLE_FULL:
                    fprintf(stderr, "Symbol Table full ...\n");
                    exit(5);
                    break;
            }
        }

        if ((op = see_OPTAB(OPcode))) len = asm_mnemonic(op);
        else if ((dp = see_DCTAB(OPcode))) len = asm_space(dp);
        else {
            fprintf(stderr, "\n%s --> Opcode '%s' is not valid ...\n", LBUF, OPcode);
            exit(6);
        }

        if (!OPerand) OPerand = null_OPR;
        if (!LNO)     LNO = null_LNO;
        if (!LABEL)   LABEL = null_LBL;
        put_list();
        LOC += len;
    }

    if (!OPcode || strcmp(OPcode, "END")) {
        fprintf(stderr, " --> 'END' opcode is not defined ...\n");
        exit(7);
    }

    if (OPerand) GOaddr = cal_nm_oprnd(OPerand);
    else OPerand = null_OPR;
    
    PGsize = LOC - LDaddr;
    LABEL = null_LBL;
    put_list();

    exit(0);
}