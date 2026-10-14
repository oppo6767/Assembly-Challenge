#include "common.h"
#include "symtab.h"

int SYMCNT = 1;                                  // 0번 칸 - RND / 1번 칸 이후부터 등록
Symtab SYMTAB[MAX_SYM] = { {"RND", RND_OPR} };   // 심볼 테이블, 0번 칸에 예약 심볼 RND를 미리 등록

/**
 * 심볼 등록
 * 이름이 칸보다 길면      → SYM_TOO_LONG
 * 표가 꽉 찼으면          → SYM_TABLE_FULL
 * 같은 이름이 이미 있으면  → SYM_DUPLICATED
 * 모두 통과하면
 * - 다음 빈칸(SYMCNT 위치)에 심볼 이름과 값(주소) 저장
 * - SYMCNT를 1 늘려 다음 빈칸 위치로 이동
 * - SYM_OK 반환
 */
int ins_SYMTAB(int value, char* sym) {
    if (strlen(sym) > MAX_SLEN - 1) return SYM_TOO_LONG;
    if (SYMCNT >= MAX_SYM) return SYM_TABLE_FULL;
    if (see_SYMTAB(sym) != INV_VAL) return SYM_DUPLICATED;

    // 아닐 경우
    strcpy(SYMTAB[SYMCNT].symbol, sym);    
    SYMTAB[SYMCNT].value = value;          
    SYMCNT++;                             
    return SYM_OK;                                
}

/**
 * 심볼 찾기
 * 등록된 심볼 중 같은 이름이 있으면 → 그 심볼의 값 반환
 * 없으면 → INV_VAL("못 찾음" 표시값) 반환
 */
int see_SYMTAB(char* sym) {
    int i = 0;
    for(i = 0; i < SYMCNT; i++) {
        if (!strcmp(SYMTAB[i].symbol, sym))
            return (SYMTAB[i].value);
    }
    return INV_VAL;
}