#include "common.h"
#include "cal_expr.h"
#include "cal_term.h"

// 수식 계산 - ex) "10h+10"
int cal_one_expr(char* expr) { // expr = 수식 포인터(문자 하나임 동치내용(expr[i]))
    int expv;                  // 수식
    int v1;                    // 값1
    int v2;                    // 값2
    int sign;                  // 부호
    char* pp = NULL;           // 플러스 포인터
    char* mp = NULL;           // 마이너스 포인터
    char* v2p = NULL;          // 2항

    // expr 문자 부호를 확인 후 0이면 sign에 1을 아니면 반환값 할당 후 다음 문자 확인(1항만 확인)
    if ((sign = issign(*expr)) != 0) expr++;
    else sign = 1;

    if ((pp = strchr(expr, '+'))) *pp = 0;      // 문자열에 +가 있으면 pp에 해당 문자 주소 저장 아닐 경우 0을 할당
    else if ((mp = strchr(expr, '-'))) *mp = 0; // 문자열에 -가 있으면 mp에 해당 문자 주소 저장 아닐 경우 0을 할당
    
    // pp가 참이면 2항에 pp + 1하기 / pp에 값이 없으며 mp에 값이 있을 경우 2하에 mp + 1하기 -> 맞나?
    if (pp) v2p = pp + 1;
    else if (mp) v2p = mp + 1;

    v1 = sign * cnv_one_term(expr);   // 1항 = 부호값 * 함수 정상 반환값
    if (v2p) v2 = cnv_one_term(v2p);  // 2항이 있을 경우 -> 2항 = 함수 정상 반환값

    // pp가 참이면 값1 + 값2 / mp가 참이면 값1 - 값2 / 둘다 아니면 값1
    if (pp) expv = v1 + v2;
    else if (mp) expv = v1 - v2;
    else expv = v1;

    return expv; // 수식 결과 반환
}

/**
 * 부호함수
 * - c가 0보다 크면 = 1
 * - c가 0보다 작으면 = -1
 * - c가 0과 같다면 = 0
 */
int issign(int c) {
    switch (c) {
        case '+': return 1;
        case '-': return -1;
        default: return 0;
    }
}