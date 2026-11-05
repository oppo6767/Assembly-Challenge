#pragma once
#include "common.h"

#define MAX_CNUM 60

struct objbuf {
    char type;
    int saddr;
    int naddr;
    int olen;
    int blen;
    char buf[MAX_CNUM +1];
};

extern void put_obj_head(char* fname, char* pname, int paddr, int psize);
extern void put_obj_text(int objaddr, unsigned char obj[]);
extern void put_obj_data(int objaddr, unsigned char obj[]);
extern void put_obj_comm(struct objbuf* obp, int objaddr, unsigned char obj[]);
extern void flush_objbuf(struct objbuf* obp);
extern void put_obj_end(int goaddr);