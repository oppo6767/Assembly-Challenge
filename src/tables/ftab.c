#include "common.h"
#include "ftab.h"
#include "rdline.h"
#include "str2int.h"
#include "symtab.h"
#include "obj.h"

int FCNT = 0;
Ftab FTAB[MAX_FTAB];

void ins_FTAB(void) {
    char* pn;
    char* io;
    char* fn;
    int port;

    if (!(pn = strtok(OPerand, ",")) || !(io = strtok(NULL, ",")) || !(fn = strtok(NULL, " \t\n\r"))) {
        fprintf(stderr, "\n%s --> FILE operand is not valid ...\n", LBUF);
        exit(31);
    }

    if (strlen(fn) > MAX_FNAME) {
        fprintf(stderr, "\n%s --> File name '%s' is too long ...\n", LBUF, fn);
        exit(34);
    }

    if (FCNT >= MAX_FTAB) {
        fprintf(stderr, "\n%s --> File Table full ...\n", LBUF);
        exit(35);
    }

    see_FTAB(fn);

    if ((port = str2int(pn)) == INV_VAL || port < 0) {
        fprintf(stderr, "\n%s --> Port number is not valid ...\n", LBUF);
        exit(32);
    }

    if (strcmp(io, "I") && strcmp(io, "O")) {
        fprintf(stderr, "\n%s --> Port type('I' or 'O') is not valid ...\n", LBUF);
        exit(33);
    }

    strcpy(FTAB[FCNT].fname, fn);
    FTAB[FCNT].port = port;
    FTAB[FCNT].in_out = *io;
    FCNT++;
}

void see_FTAB(char* fname) {
    int i;
    for (i = 0; i < FCNT; i++) {
        if (!strcmp(FTAB[i].fname, fname)) {
            fprintf(stderr, "File name '%s' is duplicated...\n", fname);
            exit(30);
        }
    }
}

void put_obj_ftab(void) {
    int i;
    for (i = 0; i < FCNT; i++)
        fprintf(Ofp, "F:%s:%c:%.6X\n", FTAB[i].fname, FTAB[i].in_out, FTAB[i].port);
}