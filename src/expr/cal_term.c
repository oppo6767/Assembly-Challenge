#include "common.h"
#include "cal_term.h"
#include "symtab.h"
#include "rdline.h"
#include "str2int.h"
#include "asc2int.h"

/**
 * 오류 처리를 위한 에러 출력 및 변환 값 반환
 * - 반환 실패 시
 *      문자열 -> 정수 or 아스키 코드 -> 정수 = 피연산자 값이 잘못됐다고 출력
 *      심볼 -> 반환X = 해당 심볼은 없다고 출력
 */
int cnv_one_term(char* term) { // ex - BUF / 10h / 'AB'
    int val;
    if (isdigit(term[0]) || term[strlen(term) - 1] == 'h') { // term의 첫 문자가 숫자일 경우 또는 term의 마지막 문자가 h일 경우
        if ((val = str2int(term)) == INV_VAL) {              // 변환 실패일 경우
            fprintf(stderr, "%s --> Operand number '%s' is not valid ... \n", LBUF, term);
            exit(11);
        }
    } else if (term[0] == '\'') {                // term 첫 문자가 '(아스키코드)일 경우
        if ((val = asc2int(term)) == INV_VAL) {  // 변환 실패일 경우
            fprintf(stderr, "%s --> Operand number '%s' is not valid ... \n", LBUF, term);
            exit(11);
        }
    } else {                                        // 아무것도 아니다? -> 심볼 확인
        if ((val = see_SYMTAB(term)) == INV_VAL) {  // 심볼을 찾을 수 없을 경우
            fprintf(stderr, "%s --> Operand symbol '%s' is not defined ... \n", LBUF, term);
            exit(12);
        }
    }
    return val; // INV_VAL(반환 실패)가 아닐 경우 변환 값 넘기기
}