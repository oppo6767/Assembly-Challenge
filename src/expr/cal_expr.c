#include "common.h"
#include "cal_expr.h"
#include "cal_term.h"

// 수식 계산 - ex) "10h+10"
int cal_one_expr(char* expr) { // expr = 수식 포인터 시작주소임(*expr == expr[0] = 첫 문자)
    int expv;                  // 결과 값
    int v1;                    // 값1
    int v2;                    // 값2
    int sign;                  // 1항 부호
    char* pp = NULL;           // 플러스 포인터
    char* mp = NULL;           // 마이너스 포인터
    char* v2p = NULL;          // 2항

    // 1항 부호 확인
    if ((sign = issign(*expr)) != 0) expr++;  // 첫 문자가 부호면 sign에 저장 후 다음 문자로 이동
    else sign = 1;                            // 부호가 없으면 sign에 1 저장(양수)

    // 2항이 존재하는지 확인
    if ((pp = strchr(expr, '+'))) *pp = 0;      // 문자열에 +가 있으면 pp에 해당 문자 주소(expr[i]) 저장 그 자리를 0으로 저장 -> 1항/2항으로 나눔
    else if ((mp = strchr(expr, '-'))) *mp = 0; // 문자열에 -가 있으면 mp에 해당 문자 주소(expr[i]) 저장 그 자리를 0으로 저장 -> 1항/2항으로 나눔
    
    // 2항 값으로 이동시키기
    if (pp) v2p = pp + 1;       // pp가 참이면 2항 = pp + 1하기('+' 다음 주소로 이동)
    else if (mp) v2p = mp + 1;  // mp가 참이면 2항 = mp + 1하기('-' 다음 주소로 이동)

    // 각 항을 정수 값으로 변환
    v1 = sign * cnv_one_term(expr);   // 1항 = 부호값 * 변환 값
    if (v2p) v2 = cnv_one_term(v2p);  // 2항이 있을 경우 -> 2항 = 변환 값

    // 정수 계산
    if (pp) expv = v1 + v2;       // pp가 참이면 값1 + 값2
    else if (mp) expv = v1 - v2;  // mp가 참이면 값1 - 값2 
    else expv = v1;               // 둘다 아니면 값1

    return expv; // 결과 값 반환
}

/**
 * 부호함수(수학 sgn(c) vs 코드 풀이)
 * - c가 0보다 크면 = 1    <-> + 이면 1 반환
 * - c가 0보다 작으면 = -1 <-> - 이면 -1 반환
 * - c가 0과 같다면 = 0    <-> 아무것도 없으면 0 반환(아스키코드)
 */
int issign(int c) {
    switch (c) {
        case '+': return 1;
        case '-': return -1;
        default: return 0;
    }
}