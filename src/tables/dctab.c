#include <string.h>
#include "dctab.h"

// 지시자 선언
Dctab DCTAB[] = {
    // 공간 할당 + 초기화 지시자
    {"DD", DC_CNST, DC_DW}, // 지시자 | 공간 할당 및 초기화 여부 | 공간 크기
    {"DW", DC_CNST, DC_WD},
    {"DB", DC_CNST, DC_BT},

    // 공간 할당 지시자
    {"RD", DC_RESV, DC_DW},
    {"RW", DC_RESV, DC_WD},
    {"RB", DC_RESV, DC_BT}
};

// 지시자 비교 후 구조체 배열 값 넘기기
Dctab* see_DCTAB(char* dcc) {
    unsigned int i;
    for(i = 0; i < MAX_DCTAB; i++) {
        if (!strcmp(DCTAB[i].directive, dcc))
            return (&DCTAB[i]);
    }
    return NULL;
}