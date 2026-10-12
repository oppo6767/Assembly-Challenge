#pragma once
#define MAX_SYM 100            // 심볼 테이블에 담을 수 있는 최대 심볼 개수
#define MAX_SLEN (8+1)         // 심볼 이름 최대 길이(8) + 끝 표시 '\0'
#define INV_VAL (-0x7fffffff)  // "못 찾음" 표시값 (실제 주소로는 나올 수 없는 값) - 음수 중에서 가장 안쓰는 값으로 지정(안전용)
#define RND_OPR 0x7fffff       // 랜덤 값

typedef struct symtab {
    char symbol[MAX_SLEN]; // 심볼 이름
    int value;             // 심볼 값(라벨 - 주소, RND - 값)
} Symtab;

typedef enum {
    SYM_OK = 0,        /* 등록 성공 */
    SYM_DUPLICATED,    /* 같은 이름이 이미 있음 */
    SYM_TABLE_FULL,    /* 표가 꽉 참 */
    SYM_TOO_LONG       /* 이름이 칸보다 김 */
} SymResult;

extern int SYMCNT;
extern Symtab SYMTAB[MAX_SYM];

extern int ins_SYMTAB(int value, char* sym);
extern int see_SYMTAB(char* sym);