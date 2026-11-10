#pragma once
#include "common.h"

#define MAX_FNAME 8
#define MAX_FTAB 4

typedef struct ftab {
    char fname[MAX_FNAME + 1];
    int port;
    char in_out;
} Ftab;

extern int FCNT;
extern Ftab FTAB[MAX_FTAB];

extern void ins_FTAB(void);
extern void see_FTAB(char* fname);
extern void put_obj_ftab(void);