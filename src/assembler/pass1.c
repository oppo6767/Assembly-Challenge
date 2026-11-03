#include "common.h"
#include "assembler.h"
#include "optab.h"
#include "dctab.h"
#include "symtab.h"
#include "rdline.h"
#include "str2int.h"
#include "cal_mnmo.h"
#include "len_mnmo.h"
#include "len_drctv.h"

/*
전체 흐름 (2-패스)
패스_1: 소스를 끝까지 읽으며 라벨 주소만 SYMTAB에 등록 (길이만 계산, 출력 없음)
패스_2: 소스를 처음부터 다시 읽으며 목적 코드 생성 + 리스트 출력
-> 패스_1에서 모든 라벨 주소가 등록되므로 패스_2에서 전방 참조 가능
[오류]: 정상 출력이 아니라 오류 메시지(stderr)로 나오고, 바로 종료
*/

int LOC = 0;  // LOC = Location Counter (현재 주소)
int LDaddr;   // LDaddr = LoaD address (시작 주소, START 피연산자 / 없으면 0)
int GOaddr;   // GOaddr = GO address (실행 시작 주소, END 피연산자 / 없으면 LDaddr)
int PGsize;   // PGsize = ProGram size (프로그램 크기 = 끝 주소 - 시작 주소)
char* null_LNO = "   ";
char* null_LBL = "        ";
char* null_OPR = "        ";

// 2-패스 어셈블: 패스_1(주소 계산) -> 패스_2(목적 코드 생성)
void TwoPassAssemble(char* sfile) {
    Pass_1(sfile);
    Pass_2(sfile);
}

// 패스_1: 라벨 주소 등록 및 길이 계산 (목적 코드는 만들지 않음)
void Pass_1(char* sfile) {
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

    read_line(fp);    // 파일 라인 읽기
    if (!OPcode || strcmp(OPcode, "START")) {  // [오류] 연산자가 START가 아니거나 연산자에 아무 값도 없을 경우 오류 출력 및 종료
        fprintf(stderr, "\n%s --> Opcode is not 'START' ...\n", LBUF);
        exit(2);
    }

    // [오류] 피연산자에 값이 있고 값 정수 변환이 0보다 작은 경우 오류 출력 및 종료
    if (OPerand && (LOC = str2int(OPerand)) < 0) {
        fprintf(stderr, "\n%s --> Start address is invalid ...\n", LBUF);
        exit(3);
    }

    if (!LABEL) LABEL = "ASMBLY";  // 라벨(프로그램 이름)이 없을 경우 기본 이름 ASMBLY 할당 (시작 주소와는 상관없음)
    ins_SYMTAB(LOC, LABEL);        // 프로그램 이름 등록

    LDaddr = LOC;   // 시작 주소 = 로드 주소
    GOaddr = LOC;   // 실행 시작 주소도 일단 시작 주소로 (END 뒤에 이름이 있으면 변경됨)

    // 코멘트가 0보다 클 경우 반복 (파일 끝 -1이면 종료)
    while ((comment = read_line(fp)) > 0) {
        // 주석(3)과 빈 줄(2)은 주소와 상관없으므로 건너뛰기 (출력은 패스_2에서)
        if (comment == 3 || comment == 2) continue;

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
         * (교재의 MAX_SYM 길이 검사는 ins_SYMTAB 안의 MAX_SLEN 검사가 대신함)
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

        // 연산자가 OPTAB에 있으면(기계 명령어) op에 할당하고 len_mnemonic으로 목적 코드 길이(4)를 len에 저장
        // 연산자가 DCTAB에 있으면(지시어) dp에 할당하고 len_space로 공간 할당 길이를 len에 저장
        // [오류] 둘 다 아닐 경우 오류 출력 및 종료
        if ((op = see_OPTAB(OPcode))) len = len_mnemonic(op);
        else if ((dp = see_DCTAB(OPcode))) len = len_space(dp);
        else {
            fprintf(stderr, "\n%s --> Opcode '%s' is not valid ...\n", LBUF, OPcode);
            exit(6);
        }

        LOC += len;  // LOC = LOC + 길이(len)할당
    }

    // [오류] 연산자가 END가 아님 or 연산자가 없으면 오류 출력 및 종료
    if (!OPcode || strcmp(OPcode, "END")) {
        fprintf(stderr, " --> 'END' opcode is not defined ...\n");
        exit(7);
    }

    if (OPerand) GOaddr = cal_nm_oprnd(OPerand);  // 피연산자가 있을 경우 피연산자 주소를 실행 시작 주소로 할당 (데이터가 코드 앞에 있을 때 사용)
    PGsize = LOC - LDaddr;                        // 프로그램 크기 = 끝 주소(LOC) - 시작 주소

    fclose(fp);
}