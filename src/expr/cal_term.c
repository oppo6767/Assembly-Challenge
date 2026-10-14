#include "common.h"
#include "cal_term.h"
#include "symtab.h"
#include "rdline.h"
#include "str2int.h"
#include "asc2int.h"

/**
 * 오류 처리를 위한 에러 출력문
 * - 반환 실패 시
 *      문자열 -> 정수 or 아스키 코드 -> 정수 = 피연산자 값이 잘못됬다고 출력
 *      심볼 -> 반환X = 해당 심볼은 없다고 출력
 */
int cnv_one_term(char* term) {
    int val;
    if (isdigit(term[0]) || term[strlen(term) - 1] == 'h') { // term의 시작 문자가 숫자일 경우 또는 term의 마지막 문자가 h일 경우
        if ((val = str2int(term)) == INV_VAL) {
            fprintf(stderr, "%s --> Operand number '%s' is not valid ... \n", LBUF, term);
            exit(11);
        }
    } else if (term[0] == '\'') { // term 시작 주소 '(아스키코드)일 경우
        if ((val = asc2int(term)) == INV_VAL) {
            fprintf(stderr, "%s --> Operand number '%s' is not valid ... \n", LBUF, term);
            exit(11);
        }
    } else { // 아무것도 아니다? -> 심볼 확인
        if ((val = see_SYMTAB(term)) == INV_VAL) {
            fprintf(stderr, "%s --> Operand symbol '%s' is not defined ... \n", LBUF, term);
            exit(12);
        }
    }
    return val; // INV_VAL(반환 실패)가 아닐 경우 정상적인 값 넘기기
}