#include "common.h"
#include "cal_mnmo.h"
#include "rdline.h"
#include "cal_expr.h"

// 레지스터 이름을 번호로 변경 (ex - X)
int cal_nm_dmpr(char* oprnd) {  // oprand 시작 주소 (*oprnd == oprnd[0] == 첫 문자)
    int opr;                    // 피연산자 값 저장

    /**
     * 해당 문자열이 해당 값과 같지 않으면 피연산자가 잘못됐다고 출력
     * - PC이면 = PC 레지스터 덤프 번호 할당
     * - U이면 = U 레지스터 덤프 번호 할당
     * - X이면 = X 레지스터 덤프 번호 할당
     * - A이면 = A 레지스터 덤프 번호 할당
    */
    if (!strcmp(oprnd, "PC")) opr = 0x000000;
    else if (!strcmp(oprnd, "U"))  opr = 0x000001;
    else if (!strcmp(oprnd, "X"))  opr = 0x000002;
    else if (!strcmp(oprnd, "A"))  opr = 0x000003;
    else {
        fprintf(stderr, "%s --> Operand is not valid ...\n", LBUF);
        exit(10);
    }

    return(opr);  // 피연산자 값 반환
}

// 피연산자 주소 비트 값 만들기 (ex - BUF / VAR3+4 / BUF[X])
int cal_nm_oprnd(char* oprnd) { // oprand 시작 주소 (*oprnd == oprnd[0] == 첫 문자)                               
    char buf[128];              // 문자열 저장 변수
    char* xp;                   // x레지스터 포인터 ('['의 위치)
    int opr;                    // 피연산자 저장 변수(정수 값)

    /**
     * 문자열 복사 -> 문자열에 [가 있으면 해당 문자 주소 xp에 저장 / 없으면 NULL 반환
     * 문자열에 [X]값이 있는지 확인
     * - [는 있는데 [X]가 없으면 피연산자 값이 잘못됐다 출력
     * - 있으면 xp에 해당 주소를 0으로 하여 인덱스와 피연산자 나누기(BUF 0 X])
     */
    strcpy(buf, oprnd);            
    if ((xp = strchr(buf, '['))) {  
        if (strcmp(xp, "[X]")) {
            fprintf(stderr, "%s --> Operand is not valid ...\n", LBUF);
            exit(10);
        }
        *xp = 0;
    }


    opr = cal_one_expr(buf);  // 반환된 수식 결과 값 저장
    opr &= 0x007fffff;        // 결과 값을 해당 비트와 and 연산하여 값 저장(둘 다 1 -> 1로 저장)
                              // 00000026 & 007fffff = 00000026 -> 피연산자 비트열만 남기기 위해서
    if (xp) opr |= OP_XBIT;   // x레지스터를 사용하면 x레지스터 비트 1로 표시 (하나라도 1 -> 1로 저장)
                              // 00000026 | 800000 = 00800026(x 레지스터 사용)

    return(opr);              // 피연산자 값 반환
}