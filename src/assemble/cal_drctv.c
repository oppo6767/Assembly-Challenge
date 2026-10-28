#include "common.h"
#include "cal_drctv.h"
#include "rdline.h"
#include "asm_mnmo.h"
#include "cal_expr.h"

// 변수 공간 초기값 할당 - DD/DW/DB (공간 확보는 asm_space에서)
int cal_dc_oprnd(char* oprnd, int unit, unsigned char obj[]) { // 피연산자, 값 하나의 바이트 크기(DD=4, DW=2, DB=1), 목적 코드
    char buf[128];                 // 버퍼 저장 배열
    char* exp;                     // ,를 제외한 값 하나의 시작 주소 (예: "1,2,3" → "1", "2", "3")
    char* op = (char*)(obj +1);    // obj[1]부터 한 바이트씩 써 나가는 위치
    short* sp = (short*)(obj +1);  // 현재 사용안함
    int* ip = (int*)(obj +1);      // 현재 사용안함
    int n = 0;                     // 지금까지 꺼낸 값의 개수
    int v;                         // 정수 값 저장 변수
    int len = 0;                   // 길이

    strcpy(buf, oprnd);            // 피연산자를 버퍼에 저장
    exp = strtok(buf, ",");        // 첫 번째 ,를 \0으로 자르고, ,를 제외한 첫 번째 값("1")의 시작 주소 저장

    /**
     * 꺼낸 값의 개수(n)가 MAX_DNUM(8)개보다 많을 경우 오류 출력 및 종료
     * 아닐 경우
     * - 지금 꺼낸 값 정수로 변환
     * - 초기화 명령어 크기 만큼 메모리 확보 -> op가 가리키는 obj 칸 (8 비트열), 길이(len) 증가
     * 그 다음 ,위치를 찾고 있으면 exp에 , 다음 위치 저장 없으면 NULL 저장
     */
    do {
        if (++n > MAX_DNUM) {
            fprintf(stderr, "%s --> Operand '%s' is not valid ...\n", LBUF, oprnd);
            exit(11);
        }

        /**
         * 비트열 저장 방식 (예: v = 'ABC' = 00000000 01000001 01000010 01000011 = 00 41 42 43)
         * - v는 값 하나(4바이트 int)이고, 줄마다 미는 칸 수만 달라서 다른 바이트를 꺼냄
         *      v >> 24 = 00000000 00000000 00000000 00000000 -> 00
         *      v >> 16 = 00000000 00000000 00000000 01000001 -> 41
         *      v >> 8  = 00000000 00000000 01000001 01000010 -> 42
         *      v       = 00000000 01000001 01000010 01000011 -> 43
         * - op가 가리키는 칸은 1바이트라서 계산 결과의 맨 아래 8비트만 저장됨
         * - 줄 순서가 >>24 -> >>16 -> >>8 -> >>0 이라서 큰 자리 바이트부터 저장됨 -> 00 41 42 43
         * - 저장할 때마다 op++로 바로 옆 칸으로 이동 -> 빈틈없이 이어서 저장
         * - break가 없어서 들어간 case부터 끝까지 실행 -> DD 4바이트, DW 아래 2바이트, DB 맨 아래 1바이트
         * - 값이 여러 개면 do-while이 값마다 반복, op와 len은 이어서 사용
         *      DD 100,30 -> | 00 00 00 64 | 00 00 00 1E | (exp1 4바이트, exp2 4바이트 순서)
         */
        v = cal_one_expr(exp);
        switch(unit) {
            case 4:                   // DD 시작
                *op = v >> 24;        // op 위치 (31 ~ 24)
                op++;                 // obj 주소 한 칸 이동
                len++;
                *op = v >> 16;        // op 위치 (23 ~ 16)
                op++; 
                len++;
                /* fall through */
            case 2:                   // DW 시작
                *op = v >> 8;         // op 위치 (15 ~ 8)
                op++;
                len++;
                /* fall through */
            case 1:                   // DB 시작 
                *op = v;              // op 위치 (7 ~ 0)
                op++;
                len++;
        }
    } while ((exp = strtok(NULL, ",")));
    
    obj[0] = len; // 반복이 끝나면 목적 배열 0에다가 길이 저장
    
    return(len);  // 길이 반환
}

// 변수 공간 초기값(문자열) 할당 - DB "ABC" (공간 확보는 asm_space에서)
int cal_dc_oprnd_string(char* oprnd, unsigned char obj[]) {
    int i; // 문자열 글자의 위치 및 총 길이 역할하는 변수

    /**
     * 피연산자가 문자열일 때 마지막 문자열이 닫는 "를 만나거나, 문자열이 끝나거나, MAX_DNUM(8)글자를 넘을 때까지 반복
     * 1부터 하는 이유: obj[0]은 총 길이 저장, oprnd[0]은 여는 "라서 건너뜀
     * - 목적 배열에 피연산자 i위치 값 할당
     * - 만약 피연산자 i위치 값이 "이 아니거나(닫는 " 없음) 또는 " 뒤에 글자가 더 있으면 오류 출력 및 종료
     * - 아닐 경우 목적 배열 i위치에 \0 저장 및 obj[0] 위치에 문자열 길이 저장
     */
    for (i = 1; oprnd[i] && oprnd[i] != '\"' && i <= MAX_DNUM; i++)
        obj[i] = oprnd[i];
    
    if (oprnd[i] != '\"' || oprnd[i + 1]) {
        fprintf(stderr, "%s --> Operand '%s' is not valid ...\n", LBUF, oprnd);
        exit(11);
    }

    obj[i] = '\0';
    obj[0] = i - 1;
    
    return (i - 1); // 문자열 길이 반환
}