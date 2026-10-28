#include "common.h"
#include "asm_space.h"
#include "dctab.h"
#include "rdline.h"
#include "asm_mnmo.h"
#include "symtab.h"
#include "str2int.h"
#include "cal_drctv.h"

// 공간 확보 지시 명령어 - RD/RW/RB/DD/DW/DB
int asm_space(Dctab *dp) {
    int len;   // 확보할 공간 길이 (바이트 수)

    // 피연산자에 값이 없을 경우 오류 출력 및 종료
    if (!OPerand) {
        fprintf(stderr, "%s --> Operand is not defined ...\n", LBUF);
        exit(10);
    }

    /**
     * 지시자 타입이 공간 예약(DC_RESV: RD/RW/RB)일 경우
     * - 피연산자 처음 값이 숫자가 아니고 피연산자 마지막 값이 h가 아닐 경우
     * - 또는 값 변환이 실패한 경우
     *      -> 오류 출력 및 종료
     */
    if (dp->type == DC_RESV) {
        if ((!isdigit(OPerand[0]) && OPerand[strlen(OPerand) -1] != 'h') || (len = str2int(OPerand)) == INV_VAL) {
            fprintf(stderr, "%s --> Operand number '%s' is not valid ...\n", LBUF, OPerand);
            exit(11);
        }

        len *= dp->unit; // 메모리 길이 = 개수(len) * 메모리 바이트 크기(unit) (예: RD 100 -> 100 * 4 = 400)
        OBJC[0] = 0;     // 목적 코드 0의 위치에 길이 0할당 (공간만 예약, 목적 코드 없음 -> 출력 칸 비움)
        
        return(len); // 메모리 길이 반환 -> LOC 증가에 사용
    }
    
    // 공간 + 초기값(DC_CNST: DD/DW/DB)일 경우
    // 만약 메모리 크기가 1(DB)이고 피연산자 시작 값이 "일 경우 문자열 초기화 할당
    // 아닐 경우 초기화 할당하기
    if (dp->unit == 1 && OPerand[0] == '\"') len = cal_dc_oprnd_string(OPerand, OBJC);
    else len = cal_dc_oprnd(OPerand, dp->unit, OBJC);
    
    return(len); // 메모리 길이 반환 -> LOC 증가에 사용
}