#include "common.h"
#include "assembler.h"
#include "optab.h"
#include "dctab.h"
#include "symtab.h"
#include "rdline.h"
#include "put_list.h"
#include "str2int.h"
#include "asm_space.h"
#include "cal_mnmo.h"
#include "asm_mnmo.h"

/*
전체 흐름
1, 2: 시작할 때 1번씩
3, 4, 5: while 안에서 소스 줄마다 반복
6: 끝날 때 1번
[오류]: 정상 출력이 아니라 오류 메시지(stderr)로 나오고, 바로 종료
*/

int LOC = 0;  // LOC = Location Counter (현재 주소)
int LDaddr;   // LDaddr = LoaD address (시작 주소, START 피연산자 / 없으면 0)
int GOaddr;   // GOaddr = GO address (실행 시작 주소, END)
int PGsize;   // GOaddr = GO address (실행 시작 주소, END 피연산자 / 없으면 LDaddr)
char* null_LNO = "   ";
char* null_LBL = "        ";
char* null_OPR = "        ";

void OnePassAssemble(char *sfile) {
    FILE* fp;     // 파일 포인터
    Optab* op;    // 기계 명령어 포인터 (OPTAB 항목)
    Dctab* dp;    // 지시어 포인터 (DCTAB 항목)
    int len;      // 길이
    int comment;  // 줄 종류 (1 정상, 2 빈 줄, 3 주석, -1 파일 끝)

    // [오류] 파일이 없으면 오류 출력 및 종료
    if (!(fp = fopen(sfile, "r"))) {
        fprintf(stderr, "File '%s' not found...\n", sfile);
        exit(1);
    }

    put_list_head();  // [출력 1] 초반 틀 출력하기 (빈 줄 + 제목 줄 + --- 줄)
    read_line(fp);    // 파일 라인 읽기
    //if (!LABEL) fprintf(stderr, "\n%s --> Program name is not defined ...\n", LBUF), exit(1);
    if (!LABEL) LABEL = "ASMBLY";              // 라벨(프로그램 이름)이 없을 경우 기본 이름 ASMBLY 할당 (시작 주소와는 상관없음)
    if (!OPcode || strcmp(OPcode, "START")) {  // [오류] 연산자가 START가 아니거나 연산자에 아무 값도 없을 경우 오류 출력 및 종료
        fprintf(stderr, "\n%s --> Opcode is not 'START' ...\n", LBUF);
        exit(1);
    }

    // [오류] 피연산자에 값이 있고 값 정수 변환이 0보다 작은 경우 오류 출력 및 종료
    if (OPerand && (LOC = str2int(OPerand)) < 0) {
        fprintf(stderr, "\n%s --> Start address is invalid ...\n", LBUF);
        exit(2);
    }

    ins_SYMTAB(LOC, LABEL);            // 심볼 등록  
    if (!LNO) LNO = null_LNO;          // 줄 번호가 없을 경우 빈줄 할당
    if (!OPerand) OPerand = null_OPR;  // 피연산자가 없을 경우 빈줄 할당
    put_list();                        // [출력 2] START 줄 출력
    LDaddr = LOC;   // 시작 주소 = 로드 주소
    GOaddr = LOC;   // 실행 시작 주소도 일단 시작 주소로 (END 뒤에 이름이 있으면 변경됨)

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

        // [오류] 연산자가 없을 경우 오류 출력 및 종료
        if (!OPcode) {
            fprintf(stderr, "\n%s --> Opcode is not defined ...\n", LBUF);
            exit(4);
        }

        // 연산자가 END이면 멈추기
        if (!strcmp(OPcode, "END")) break;
        
        /**
         * [오류] 라벨이 있을 경우
         * - 라벨이 제한한 길이(8자)보다 길 경우 오류 출력 및 종료
         * - 라벨이 중복되면 오류 출력 및 종료
         * - 기호 표(SYMTAB)가 가득 찰 경우 오류 출력 및 종료
         */
        if (LABEL) {
            switch (ins_SYMTAB(LOC, LABEL)) {
                case SYM_TOO_LONG:
                    fprintf(stderr, "\n%s --> Symbol '%s' is too long ...\n", LBUF, LABEL);
                    exit(5);
                    break;
                case SYM_DUPLICATED:
                    fprintf(stderr, "\n%s --> Symbol '%s' is duplicated ...\n", LBUF, LABEL);
                    exit(5);
                    break;
                case SYM_TABLE_FULL:
                    fprintf(stderr, "Symbol Table full ...\n");
                    exit(5);
                    break;
            }
        }

        // 연산자가 OPTAB에 있으면(기계 명령어) op에 할당하고 asm_mnemonic으로 목적 코드 길이(4)를 len에 저장
        // 연산자가 DCTAB에 있으면(지시어) dp에 할당하고 asm_space로 공간 할당 길이를 len에 저장
        // [오류] 둘 다 아닐 경우 오류 출력 및 종료
        if ((op = see_OPTAB(OPcode))) len = asm_mnemonic(op);
        else if ((dp = see_DCTAB(OPcode))) len = asm_space(dp);
        else {
            fprintf(stderr, "\n%s --> Opcode '%s' is not valid ...\n", LBUF, OPcode);
            exit(6);
        }

        if (!OPerand) OPerand = null_OPR;   // 피연산자가 없으면 빈줄 할당
        if (!LNO) LNO = null_LNO;           // 줄 번호가 없으면 빈줄 할당
        if (!LABEL) LABEL = null_LBL;       // 라벨이 없으면 빈줄 할당
        put_list();                         // [출력 5] 명령어/지시어 한 줄 출력
        LOC += len;                         // LOC = LOC + 길이(len)할당
    }

    // [오류] 연산자가 END가 아님 or 연산자가 없으면 오류 출력 및 종료
    if (!OPcode || strcmp(OPcode, "END")) {
        fprintf(stderr, " --> 'END' opcode is not defined ...\n");
        exit(7);
    }

    if (OPerand) GOaddr = cal_nm_oprnd(OPerand);  // 피연산자가 있을 경우 피연산자 주소를 실행 시작 주소로 할당 (데이터가 코드 앞에 있을 때 사용)
    else OPerand = null_OPR;                      // 없을 경우 빈줄 할당
    
    PGsize = LOC - LDaddr;     // 프로그램 크기 = 끝 주소(LOC) - 시작 주소
    LABEL = null_LBL;          // 라벨에 빈줄 할당
    if (!LNO) LNO = null_LNO;  // 줄번호가 없으면 빈줄 할당
    put_list();                // [출력 6] END 줄 출력

    exit(0); // 종료
}