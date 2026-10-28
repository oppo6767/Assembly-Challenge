#include "common.h"
#include "asm_mnmo.h"
#include "optab.h"
#include "rdline.h"
#include "cal_mnmo.h"

// 목적 코드 저장 배열: 비트 8개 * 4바이트 = 32바이트 + OBJC[0](길이 칸)
// + 1은 \0이 아님 - OBJC는 한 칸씩 값을 넣어서 \0이 붙지 않음 (\0은 문자열 리터럴(s[] = "ABC")이나 문자열 함수를 쓸 때만 붙음)
unsigned char OBJC[MAX_DNUM*4 + 1];

// 기계 명령어 조립 - 연산자 비트열 + 피연산자 비트열
int asm_mnemonic(Optab* op) { // 연산자 코드 정보 가져오기
    // char* obp = OBJC; 
    unsigned int obc; // 목적 코드 값(비트 표시)

    /**
     * 연산 코드를 왼쪽으로 24칸 밀어 연산자 비트열(31~24번 비트)로 위치시키기
     * 해당 피연산자가 필요한 연산자인지 확인
     * - 피연산자가 필요한데 해당 값이 없으면 오류 출력 후 종료
     * - 피연산자가 필요한데 값이 있으면 연산자 이름이 DMPR 인지 확인
     *      - DMPR이면 레지스터 이름(PC, U, X, A)을 번호로 바꿔 obc에 합치기
     *      - 아니면 피연산자 주소(+ X 비트)를 계산해 obc에 합치기
     */
    obc = (op->opcode << 24);
    if (op->n_operand) {
        if (!OPerand) {
            fprintf(stderr, "%s --> Operand is not defined ...\n", LBUF);
            exit(10);
        }

        // | -> obc에 있는 연산자 비트에 피연산자 비트를 합치기
        if (!strcmp(op->mnemonic, "DMPR")) obc |= cal_nm_dmpr(OPerand);
        else obc |= cal_nm_oprnd(OPerand);
    }

    // 목적 코드에 비트열 저장 - 배열을 일일이 넣는 이유: 기계마다 int 저장 순서가 달라서
    OBJC[0] = 4;             // 4바이트(메모리 크기) 저장
    OBJC[1] = obc >> 24;     // 연산자 비트열 (31 ~ 24 비트)
    OBJC[2] = obc >> 16;     // 피연산자 비트열 (23 ~ 16비트)
    OBJC[3] = obc >> 8;      // 피연산자 비트열 (15 ~ 8비트)
    OBJC[4] = obc >> 0;      // 피연산자 비트열 (7 ~ 0비트)

    return(OBJC[0]);         // 목적 코드 길이 반환
}