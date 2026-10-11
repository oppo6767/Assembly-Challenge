#pragma once
#define MAX_DLEN 8                                        // 최대 지시자 문자열 길이
#define DC_CNST 0                                         // 공간 할당 + 초기화
#define DC_RESV 1                                         // 공간 할당
#define DC_DW 4                                           // 공간 할당 - 4byte
#define DC_WD 2                                           // 공간 할당 - 2byte
#define DC_BT 1                                           // 공간 할당 - 1byte
#define MAX_DCTAB (sizeof(DCTAB) / sizeof(struct dctab))  // 지시자 선언 몇 개 있는지 확인 = 구조체 배열 크기 / 구조체 크기

/**
 * type, unit을 char로 쓴 이유
 * char - 1바이트 정수형 (이름만 문자일 뿐 숫자를 담는 자료형)
 * type은 0(CNST) 또는 1(RESV), unit은 1 / 2 / 4만 저장함
 * 전부 작은 양수라서 1바이트로 충분함
 */
typedef struct dctab {
    char directive[MAX_DLEN];   // 지시자 이름
    char type;                  // 지시자 타입 - 초기화(CNST) or 공간할당(RESV)
    char unit;                  // 메모리 크기 - 4 / 2 / 1
} Dctab;

extern Dctab DCTAB[];
extern Dctab* see_DCTAB(char* dcc);