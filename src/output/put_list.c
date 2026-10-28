#include "common.h"
#include "put_list.h"
#include "rdline.h"
#include "asm_mnmo.h"
#include "assembler.h"

// 어셈블리 해석 전체적인 틀 출력
void put_list_head() {
    printf("\n");
    printf(" LNO  Location  Label    Opcode   Operand         Object Code\n");
    printf(" ---  --------  -----    ------   -----------     -----------\n");
}

// 어셈블리 해석 내용 출력
void put_list() {
    int i;
    printf(" %4s %.7X   %-8s %-8s %-15s ", LNO, LOC, LABEL, OPcode, OPerand);
    fflush(stdout); // 버퍼에 모인 출력을 강제로 화면에 내보내기
    
    // 목적 코드 출력
    for (i = 0; i < OBJC[0]; i++)
        printf("%.2X", OBJC[i + 1]);
    printf("\n");
    
    OBJC[0] = 0; // 길이 초기화
}