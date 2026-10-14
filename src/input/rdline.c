#include "common.h"
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
    char* cmt;             // 주석 위치를 기억하는 변수
    LNO = NULL;            // 줄 번호 - 없을 수 있음
    LABEL = NULL;          // 라벨(심볼) - 없을 수 있음
    OPcode = NULL;         // 연산자
    OPerand = NULL;        // 피연산자 - 없을 수 있음

    /**
     * 읽을 줄이 없으면 -1 반환
     * 원본 줄을 LBUF에 복사하고, 끝의 \n한 글자를 잘라낸다 (출력용)
     * 맨 처음 값이 .일 경우 주석 처리 - 반환 값 3
     * //이 있으면 해당 위치(주소) cmt에 저장
     * 빈칸, 탭, 줄바꿈, \r(윈도우 줄 끝) 기준으로 첫 토큰을 꺼낸다
     * - 토큰이 없는데 // 이 있으면 -> 주석 반환 (3)
     * - 토큰이 없는데 //도 없으면 -> 빈 줄 반환(2)
     */
    if (!fgets(buf, sizeof(buf), fp)) return -1;
    strcpy(LBUF, buf);
    if (LBUF[strlen(LBUF) - 1] == '\n') LBUF[strlen(LBUF) -1] = 0;

    // .으로 시작하면 주석 처리
    if (LBUF[0] == '.') return 3;

    // '//' 위치를 찾아 기억하고 잘라냄
    if ((cmt = strstr(buf, "//"))) *cmt = 0;
    
    if(!(cp = strtok(buf, " \t\n\r"))) {
        if (cmt) return 3;  // '//'가 있는데 토큰이 없음 -> 주석 줄
        else return 2;      // '//'가 없고 토큰이 없음 -> 빈줄
    }

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