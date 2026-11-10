#pragma once
#include "common.h"

#define MAX_CNUM 60

// 목적 버퍼
struct objbuf {
    char type;             // 타입
    int saddr;             // 시작 주소
    int naddr;             // 다음 주소
    int olen;              // 목적코드 길이
    int blen;              // 버퍼 길이
    char buf[MAX_CNUM +1]; // 버퍼 최대 길이 61 (+1은 ?)
};

extern FILE* Ofp;
extern void put_obj_head(char* fname, char* pname, int paddr, int psize);
extern void put_obj_text(int objaddr, unsigned char obj[]);
extern void put_obj_data(int objaddr, unsigned char obj[]);
extern void put_obj_comm(struct objbuf* obp, int objaddr, unsigned char obj[]);
extern void flush_objbuf(struct objbuf* obp);
extern void put_obj_end(int goaddr);