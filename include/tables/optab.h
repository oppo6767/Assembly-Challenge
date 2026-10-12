#pragma once
#define MAX_OLEN 8                                       // 연산자 최대 문자 길이
#define MAX_OPTAB (sizeof(OPTAB) / sizeof(struct optab)) // 연산자 선언 몇 개 있는지 확인 = 구조체 배열 크기 / 구조체 크기

/**
 * unsigned char를 쓴 이유
 * char     - 1바이트 정수형 (이름만 문자일 뿐 숫자를 담는 자료형)
 * unsigned - 음수 없이 양수로만 사용 → 0 ~ 255 저장 가능
 * 연산코드는 최대 0x3F(= 63)라서 1바이트로 충분함
 * 0x3F는 16진수로 "적는 방식"일 뿐, 메모리에는 2진수로 저장됨
 */
typedef struct optab {
	char mnemonic[MAX_OLEN];  // 연산자 이름
	unsigned char opcode;     // 연산코드
	unsigned char n_operand;  // 피연산자 필요 여부
} Optab;

extern Optab OPTAB[];
extern Optab* see_OPTAB(char* opc);