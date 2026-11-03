#include "common.h"
#include "len_drctv_num.h"
#include "asm_mnmo.h"
#include "rdline.h"

// 변수 공간 초기값 길이 계산 - DD/DW/DB (패스_1용, 값 자체는 계산하지 않고 개수만 셈)
int len_dc_oprnd(char* oprnd, int unit) { // 피연산자, 값 하나의 바이트 크기(DD=4, DW=2, DB=1)
    char buf[128];   // 버퍼 저장 배열
    char* exp;       // ,를 제외한 값 하나의 시작 주소 (예: "1,2,3" → "1", "2", "3")
    int n = 0;       // 지금까지 꺼낸 값의 개수
    int len = 0;     // 길이

    strcpy(buf, oprnd);       // 피연산자를 버퍼에 저장
    exp = strtok(buf, ",");   // 첫 번째 ,를 \0으로 자르고, 첫 번째 값의 시작 주소 저장

    /**
     * [오류] 꺼낸 값의 개수(n)가 MAX_DNUM(8)개보다 많을 경우 오류 출력 및 종료
     * 아닐 경우 값 하나당 unit 바이트씩 길이 증가 (예: DW 1,2,3 -> 2 + 2 + 2 = 6)
     * 그 다음 ,위치를 찾고 있으면 exp에 , 다음 위치 저장 없으면 NULL 저장
     */
    do {
        if (++n > MAX_DNUM) {
            fprintf(stderr, "%s --> Operand '%s' is not valid ...\n", LBUF, oprnd);
            exit(11);
        }

        len += unit;
    } while ((exp = strtok(NULL, ",")));

    return(len);  // 길이 반환
}