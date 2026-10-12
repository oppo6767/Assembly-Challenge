#include <string.h>
#include "test_framework.h"
#include "test_suites.h"
#include "rdline.h"

/* 테스트용 입력 파일 (make test는 프로젝트 맨 위 폴더에서 실행됨) */
#define RDLINE_SAMPLE "tests/data/rdline_sample.s"

/* ============================================================
 * 도우미
 * ============================================================ */

/* 둘 다 NULL이면 같음, 한쪽만 NULL이면 다름, 아니면 문자열 비교 */
static bool str_eq(const char *actual, const char *expected) {
    if (actual == NULL || expected == NULL) return actual == expected;
    return strcmp(actual, expected) == 0;
}

/* 파일을 처음부터 열어 read_line을 n번 부르고, 마지막 반환값을 돌려줌
 * (read_line의 buf가 static이라 파일을 닫아도 LABEL 등은 그대로 유효) */
static int read_nth_line(int n) {
    FILE *fp = fopen(RDLINE_SAMPLE, "r");
    int result = -1;
    if (fp == NULL) return -2;              /* 파일을 못 열면 -2 */
    for (int i = 0; i < n; i++) result = read_line(fp);
    fclose(fp);
    return result;
}

/* n번째 줄을 읽었을 때 네 칸이 기대값과 모두 같은가 */
static bool line_is(int n, const char *lno, const char *label,
                    const char *opcode, const char *operand) {
    if (read_nth_line(n) != 1) return false;
    return str_eq(LNO, lno) && str_eq(LABEL, label)
        && str_eq(OPcode, opcode) && str_eq(OPerand, operand);
}

/* ============================================================
 * 테스트
 * ============================================================ */

/* 주석 줄, 빈 줄은 건너뛰고 첫 내용 줄을 읽는다 (줄번호 + 라벨 + START) */
static bool test_rdline_skips_comment_and_blank(void) {
    return line_is(1, "000", "S_203", "START", "1300h");
}

/* 줄번호 + 라벨 + 지시자 + 피연산자 */
static bool test_rdline_label_directive(void) {
    return line_is(2, "010", "VAR1", "DD", "10");
}

/* 원본 줄은 LBUF에 그대로, 줄 끝 줄바꿈만 제거 */
static bool test_rdline_LBUF_keeps_original(void) {
    if (read_nth_line(2) != 1) return false;
    return strcmp(LBUF, "010 VAR1  DD    10") == 0;
}

/* 줄번호, 라벨 없음 + 줄 끝 주석 제거 */
static bool test_rdline_no_label_trailing_comment(void) {
    return line_is(3, NULL, NULL, "LDA", "VAR1");
}

/* 피연산자 없는 명령어 */
static bool test_rdline_no_operand(void) {
    return line_is(4, NULL, NULL, "HALT", NULL);
}

/* 마지막 줄에 줄바꿈이 없어도 마지막 글자가 잘리지 않는다 */
static bool test_rdline_last_line_without_newline(void) {
    if (!line_is(5, NULL, NULL, "END", "MAIN")) return false;
    return strcmp(LBUF, "END MAIN") == 0;
}

/* 더 읽을 줄이 없으면 -1 */
static bool test_rdline_returns_minus1_at_eof(void) {
    return read_nth_line(6) == -1;
}

/* ============================================================
 * 등록
 * ============================================================ */
void register_rdline_tests(void) {
    testRegister("test_rdline_skips_comment_and_blank",   test_rdline_skips_comment_and_blank);
    testRegister("test_rdline_label_directive",           test_rdline_label_directive);
    testRegister("test_rdline_LBUF_keeps_original",       test_rdline_LBUF_keeps_original);
    testRegister("test_rdline_no_label_trailing_comment", test_rdline_no_label_trailing_comment);
    testRegister("test_rdline_no_operand",                test_rdline_no_operand);
    testRegister("test_rdline_last_line_without_newline", test_rdline_last_line_without_newline);
    testRegister("test_rdline_returns_minus1_at_eof",     test_rdline_returns_minus1_at_eof);
}