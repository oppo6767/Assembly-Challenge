#pragma once
#include <stdbool.h>

/* 테스트 함수의 모양: 인자 없음, 성공/실패를 bool 로 돌려줌 */
typedef bool (*TestFunc)(void);

/* 테스트 하나를 이름과 함께 표에 등록 */
void testRegister(const char *name, TestFunc func);

/* 등록된 테스트를 전부 실행하고 결과를 출력 */
int testRun(void);