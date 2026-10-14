#include <stdio.h>
#include "test_framework.h"

/* 테스트 한 칸: 이름 + 함수 주소 */
typedef struct {
    const char* name;
    TestFunc func;
} TestCase;

#define MAX_TESTS 150

static TestCase tests[MAX_TESTS];   /* 등록 표 */
static int test_count = 0;     /* 등록된 개수 */

void testRegister(const char *name, TestFunc func) {
    if (test_count >= MAX_TESTS) {
        printf("[경고] 표가 가득 차서 %s는 등록 못 함\n", name);
        return;
    }

    tests[test_count].name = name;
    tests[test_count].func = func;
    test_count++;
}

int testRun(void) {
    int passed = 0;
    int failed = 0;

    for (int i = 0; i < test_count; i++) {
        bool result = tests[i].func();

        if (result) {
            printf("pass func name: %-6s\n", tests[i].name);
            passed++;
        } else {
            printf("fail func name: %-6s\n", tests[i].name);
            failed++;
        }
    }

    printf("결과\n");
    printf("pass: %d / fail: %d\n", passed, failed);
    return failed;
}