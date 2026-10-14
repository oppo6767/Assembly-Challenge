#include "common.h"
#include "optab.h"

// optab 정의
Optab OPTAB[] = {
	// 읽기 연산자
	{"LDA", 0x00, 1},  // 코드 이름 | 연산코드 | 피연산자 필요 여부
	{"LDAW", 0x01, 1},
	{"LDAB", 0x02, 1},
	{"LDX", 0x04, 1},
	{"LDU", 0x05, 1},
	
	// 저장 연산자
	{"STA", 0x08, 1},
	{"STAW", 0x09, 1},
	{"STAB", 0x0A, 1},
	{"STX", 0x0C, 1},
	{"STU", 0x0D, 1},
	
	// 수학 연산자
	{"ADD", 0x10, 1},
	{"ADDW", 0x11, 1},
	{"ADDB", 0x12, 1},
	{"SUB", 0x14, 1},
	{"SUBW", 0x15, 1},
	{"SUBB", 0x16, 1},
	{"MOD", 0x17, 1},
	
	// 논리 연산자
	{"AND", 0x18, 1},
	{"OR", 0x19, 1},
	{"XOR", 0x1A, 1},
	{"SHL", 0x1C, 1},
	{"SHR", 0x1D, 1},
	
	// 비교 연산자
	{"CMP", 0x20, 1},
	{"CMPW", 0x21, 1},
	{"CMPB", 0x22, 1},
	{"IXC", 0x24, 1},
	{"IXWC", 0x25, 1},
	{"IXBC", 0x26, 1},
	
	// 분기 연산자
	{"JZ", 0x28, 1},
	{"JNZ", 0x29, 1},
	{"JC", 0x2A, 1},
	{"JMP", 0x2B, 1},
	{"GSUB", 0x2C, 1},
	{"RSUB", 0x2D, 0},
	
	// 입/출력 연산자
	{"CID", 0x30, 1},
	{"COD", 0x32, 1},
	{"IN", 0x34, 1},
	{"OUT", 0x36, 1},
	
	// 메모리/레지스터 덤프
	{"DMPR", 0x38, 1},
	{"DMP", 0x39, 1},
	{"DMPW", 0x3A, 1},
	{"DMPB", 0x3B, 1},
	{"DMPS", 0x3C, 1},
	
	// 실행 종료 연산자
	{"HALT", 0x3F, 0}
};

// 연산자 비교 후 구조체 배열 값 넘기기
Optab* see_OPTAB(char* opc) {
    unsigned int i;
    for(i = 0; i < MAX_OPTAB; i++) {
        if(!strcmp(OPTAB[i].mnemonic, opc))
            return (&OPTAB[i]);
    }
    return NULL;
} 