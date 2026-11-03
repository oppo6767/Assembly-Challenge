#include "common.h"
#include "len_drctv.h"
#include "len_drctv_str.h"
#include "len_drctv_num.h"
#include "rdline.h"
#include "str2int.h"
#include "symtab.h"

// 공간 확보 지시 명령어 길이 계산 - RD/RW/RB/DD/DW/DB (패스_1용, 목적 코드는 만들지 않음)
int len_space(Dctab* dp) {
    int len;   // 확보할 공간 길이 (바이트 수)

    // [오류] 피연산자에 값이 없을 경우 오류 출력 및 종료
    if (!OPerand) {
        fprintf(stderr, "%s --> Operand is not defined ...\n", LBUF);
        exit(10);
    }

    /**
     * 지시자 타입이 공간 예약(DC_RESV: RD/RW/RB)일 경우
     * - [오류] 피연산자 처음 값이 숫자가 아니고 피연산자 마지막 값이 h가 아닐 경우
     * - 또는 값 변환이 실패한 경우
     *      -> 오류 출력 및 종료
     * - 아닐 경우 메모리 길이 = 개수 * 메모리 바이트 크기(unit) (예: RD 100 -> 100 * 4 = 400)
     */
    if (dp->type == DC_RESV) {
        if ((!isdigit(OPerand[0]) && OPerand[strlen(OPerand) -1] != 'h') || (len = str2int(OPerand)) == INV_VAL) {
            fprintf(stderr, "%s --> Operand number '%s' is not valid ...\n", LBUF, OPerand);
            exit(11);
        }

        len *= dp->unit;
        return(len);  // 메모리 길이 반환 -> LOC 증가에 사용
    }

    // 공간 + 초기값(DC_CNST: DD/DW/DB)일 경우
    // 만약 메모리 크기가 1(DB)이고 피연산자 시작 값이 "일 경우 문자열 길이 계산
    // 아닐 경우 값 개수로 길이 계산
    if (dp->unit == 1 && OPerand[0] == '\"') len = len_dc_oprnd_string(OPerand);
    else len = len_dc_oprnd(OPerand, dp->unit);

    return(len);  // 메모리 길이 반환 -> LOC 증가에 사용
}