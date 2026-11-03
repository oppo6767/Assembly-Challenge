#include "common.h"
#include "len_drctv_str.h"
#include "asm_mnmo.h"
#include "rdline.h"

// 변수 공간 초기값(문자열) 길이 계산 - DB "ABC" (패스_1용, 글자는 복사하지 않고 개수만 셈)
int len_dc_oprnd_string(char* oprnd) {
    int i;        // 문자열 글자의 위치
    int len = 0;  // 글자 수

    /**
     * 닫는 "를 만나거나, 문자열이 끝나거나, MAX_DNUM(8)글자를 넘을 때까지 글자 수 세기
     * 1부터 하는 이유: oprnd[0]은 여는 "라서 건너뜀
     * - [오류] 피연산자 i위치 값이 "이 아니거나(닫는 " 없음) 또는 " 뒤에 글자가 더 있으면 오류 출력 및 종료
     */
    for (i = 1; oprnd[i] && oprnd[i] != '\"' && i <= MAX_DNUM; i++)
        len++;

    if (oprnd[i] != '\"' || oprnd[i + 1]) {
        fprintf(stderr, "%s --> Operand '%s' is not valid ...\n", LBUF, oprnd);
        exit(11);
    }

    return (len + 1);  // 글자 수 + '\0' 1바이트 (패스_2 cal_dc_oprnd_string의 반환값 i와 같아야 함)
}