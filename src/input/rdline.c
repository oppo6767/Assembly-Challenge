#include <string.h>
#include <ctype.h>
#include "rdline.h"
#include "optab.h"
#include "dctab.h"

char LBUF[128];
char* LNO;
char* LABEL;
char* OPcode;
char* OPerand;

int read_line(FILE* fp) {
    static char buf[128];  // 한 줄을 읽어 둘 버퍼 (토큰 포인터들이 가리키므로 static으로 유지)
    char* cp;              // 줄 문자열의 토큰 하나 = 줄 번호 -> 라벨 -> 연산자 -> 피연산자
    LNO = NULL;            // 줄 번호 - 없을 수 있음
    LABEL = NULL;          // 라벨(심볼) - 없을 수 있음
    OPcode = NULL;         // 연산자
    OPerand = NULL;        // 피연산자 - 없을 수 있음

    /**
     * 내용이 있는 줄이 나올 때까지 한 줄씩 읽는다
     * - 더 읽을 줄이 없으면 -1 반환
     * - 원본 줄을 LBUF에 복사하고, 끝의 \n한 글자를 잘라낸다 (출력용)
     * - 줄 어디든 "//"가 나오면 그 뒤(주석)를 잘라낸다
     * - 빈칸, 탭, 줄바꿈, \r(윈도우 줄 끝) 기준으로 첫 토큰을 꺼낸다
     * - 토큰이 없으면(빈 줄 또는 주석만 있는 줄) 다음 줄을 읽는다
     */
    do {
        if (!fgets(buf, sizeof(buf), fp)) return -1;
        
        strcpy(LBUF, buf);
        if (LBUF[strlen(LBUF) - 1] == '\n') LBUF[strlen(LBUF) -1] = 0;
        if ((cp = strstr(buf, "//"))) *cp = 0;
    } while(!(cp = strtok(buf, " \t\n\r")));

    // 첫 토큰이 숫자로 시작하면 줄 번호로 가져가고 다음 토큰으로 이동
    if (isdigit(*cp)) {
        LNO = cp;
        cp = strtok(NULL, " \t\n\r");
    }

    // 만약 줄 번호 이후 //인 경우가 있을 수도 있으니 예외 처리하고 싶으면 하셈

    // 명령어(OPTAB), 지시자(DCTAB), START, END 중 어느 것도 아니면 라벨로 보고 다음 토큰으로 이동
    if (strcmp(cp, "END") && strcmp(cp, "START") && !see_OPTAB(cp) && !see_DCTAB(cp)) {
        LABEL = cp;
        cp = strtok(NULL, " \t\n\r");
    }
    
    /**
     * 남은 토큰 정리
     * - 지금 토큰 → 명령어(OPcode)
     * - 다음 토큰 → 피연산자(OPerand), 없으면 NULL (예: HALT)
     */
    OPcode = cp;
    OPerand = strtok(NULL, " \t\n\r");

    return 1;
}