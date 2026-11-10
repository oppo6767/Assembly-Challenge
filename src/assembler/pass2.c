#include "common.h"
#include "assembler.h"
#include "optab.h"
#include "dctab.h"
#include "rdline.h"
#include "asm_mnmo.h"
#include "asm_space.h"
#include "put_list.h"
#include "obj.h"  // 목적 프로그램 출력 함수
#include "ftab.h"

// 패스_2: 소스를 처음부터 다시 읽으며 목적 코드 생성 + 리스트 출력
// (오류 검사는 패스_1에서 끝났으므로 생략)
void Pass_2(char* sfile) {
    FILE* fp;     // 파일 포인터
    Optab* op;    // 기계 명령어 포인터 (OPTAB 항목)
    Dctab* dp;    // 지시어 포인터 (DCTAB 항목)
    int len;      // 길이
    int comment;  // 줄 종류 (1 정상, 2 빈 줄, 3 주석, -1 파일 끝)

    fp = fopen(sfile, "r");                      // 파일 존재 여부는 패스_1에서 확인함
    put_list_head();                             // [출력 1] 초반 틀 출력하기 (빈 줄 + 제목 줄 + --- 줄)
    read_line(fp);                               // 첫 줄(START) 읽기
    if (!LABEL) LABEL = "ASMBLY";                // 라벨(프로그램 이름)이 없을 경우 기본 이름 ASMBLY 할당
    put_obj_head(sfile, LABEL, LDaddr, PGsize);  // 목적 프로그램: 헤드(H) 레코드 출력 (프로그램 이름, 시작 주소, 크기)
    put_obj_ftab();
    if (!LNO) LNO = null_LNO;                    // 줄 번호가 없을 경우 빈줄 할당
    if (!OPerand) OPerand = null_OPR;            // 피연산자가 없을 경우 빈줄 할당
    LOC = LDaddr;                                // 패스_1에서 끝까지 올라간 주소를 시작 주소로 되돌리기
    put_list();                                  // [출력 2] START 줄 출력

    // 코멘트가 0보다 클 경우 반복 (파일 끝 -1이면 종료)
    while ((comment = read_line(fp)) > 0) {
        // [출력 3] 코멘트가 3이면 빈칸 16칸 + 주석 출력 (Label 칸 위치에 맞춤)
        if (comment == 3) {
            printf("                %s\n", LBUF);
            continue;
        }

        // [출력 4] 코멘트가 2이면 줄 바꿈
        if (comment == 2) {
            printf("\n");
            continue;
        }

        if (!strcmp(OPcode, "END")) break;      // 연산자가 END이면 멈추기
        // if (!strcmp(OPcode, "FILE")) continue;  // 교재 그대로 (패스_1에서 지운 줄이라 실제로는 걸리지 않음)
        if (!strcmp(OPcode, "FILE")) { // 패스 - 22
            if (!LNO) LNO = null_LNO;
            if (!LABEL) LABEL = null_LBL;
            put_list();
            continue;
        }

        // 기계 명령어면 asm_mnemonic, 지시어면 asm_space로 목적 코드 생성 및 길이 저장
        // (OPTAB, DCTAB 둘 다 아닌 경우는 패스_1에서 이미 오류 처리됨)
        if ((op = see_OPTAB(OPcode))) {
            len = asm_mnemonic(op);
            put_obj_text(LOC, OBJC);  // 목적 프로그램: 텍스트(T) 레코드에 기계 명령어 목적 코드 추가
        } else {
            dp = see_DCTAB(OPcode);
            len = asm_space(dp);
            put_obj_data(LOC, OBJC);  // 목적 프로그램: 데이터(D) 레코드에 변수 공간 목적 코드 추가
        }

        if (!OPerand) OPerand = null_OPR;   // 피연산자가 없으면 빈줄 할당
        if (!LNO) LNO = null_LNO;           // 줄 번호가 없으면 빈줄 할당
        if (!LABEL) LABEL = null_LBL;       // 라벨이 없으면 빈줄 할당
        put_list();                         // [출력 5] 명령어/지시어 한 줄 출력

        LOC += len;                         // LOC = LOC + 길이(len)할당
    }

    if (!OPerand) OPerand = null_OPR;  // 피연산자가 없을 경우 빈줄 할당
    if (!LNO) LNO = null_LNO;          // 줄번호가 없으면 빈줄 할당
    LABEL = null_LBL;                  // 라벨에 빈줄 할당
    put_obj_end(GOaddr);               // 목적 프로그램: 남은 레코드 출력 + 엔드(E) 레코드 출력 후 파일 닫기
    put_list();                        // [출력 6] END 줄 출력

    fclose(fp);
}