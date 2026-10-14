#include <string.h>
#include "test_framework.h"
#include "test_suites.h"
#include "cal_drctv.h"
#include "asm_space.h"
#include "asm_mnmo.h"
#include "dctab.h"
#include "rdline.h"

/* ============================================================
 * 도우미: 지시자 + 피연산자로 asm_space를 돌리고
 *         돌려준 길이와 OBJC에 들어간 바이트가 기대값과 같은지 확인
 *  - expected_len : asm_space가 돌려줘야 하는 길이 (주소가 얼마나 늘어나는지)
 *  - bytes / n    : OBJC[1]부터 들어 있어야 하는 바이트들 (n == 0 이면 OBJC를 안 채우는 경우)
 * ============================================================ */
static bool space_is(char *directive, char *operand, int expected_len,
                     const unsigned char *bytes, int n) {
    Dctab *dp = see_DCTAB(directive);
    if (dp == NULL) return false;
    OPerand = operand;                       /* read_line이 채워 주던 값을 직접 넣음 */
    if (asm_space(dp) != expected_len) return false;
    if (OBJC[0] != n) return false;
    return memcmp(&OBJC[1], bytes, n) == 0;
}

/* ============================================================
 * cal_dc_oprnd : 쉼표로 나뉜 초기값들을 unit 바이트씩 쌓기
 * (break 없이 아래 case로 이어지는 부분을 확인)
 * ============================================================ */
static bool test_dc_oprnd_4byte(void) {
    unsigned char o[33];
    char e[] = "10";
    const unsigned char want[] = { 0x00, 0x00, 0x00, 0x0A };
    return cal_dc_oprnd(e, 4, o) == 4 && o[0] == 4 && memcmp(&o[1], want, 4) == 0;
}
static bool test_dc_oprnd_2byte(void) {
    unsigned char o[33];
    char e[] = "300";                         /* 300 = 0x012C */
    const unsigned char want[] = { 0x01, 0x2C };
    return cal_dc_oprnd(e, 2, o) == 2 && memcmp(&o[1], want, 2) == 0;
}
static bool test_dc_oprnd_many(void) {
    unsigned char o[33];
    char e[] = "1,2,3";
    const unsigned char want[] = { 0x01, 0x02, 0x03 };
    return cal_dc_oprnd(e, 1, o) == 3 && memcmp(&o[1], want, 3) == 0;
}

/* ============================================================
 * cal_dc_oprnd_string : "..." 문자열을 한 바이트씩 (\0은 길이에 안 셈)
 * ============================================================ */
static bool test_dc_string_EOT(void) {
    unsigned char o[33];
    char e[] = "\"EOT\"";
    const unsigned char want[] = { 'E', 'O', 'T' };
    return cal_dc_oprnd_string(e, o) == 3 && memcmp(&o[1], want, 3) == 0;
}

/* ============================================================
 * asm_space : 지시자 종류에 따라 처리
 * ============================================================ */
/* 공간만 확보(RD/RW/RB): 길이 = 개수 × 단위, OBJC는 비움 */
static bool test_space_RD_100(void)  { return space_is("RD", "100", 400, NULL, 0); }
static bool test_space_RW_hex(void)  { return space_is("RW", "10h", 32,  NULL, 0); }

/* 초기값 지정(DD/DW/DB) */
static bool test_space_DD_10(void) {
    const unsigned char b[] = { 0x00, 0x00, 0x00, 0x0A };
    return space_is("DD", "10", 4, b, 4);
}
static bool test_space_DW_mixed(void) {      /* 교재 예: DW 10+2,'2A',30 */
    const unsigned char b[] = { 0x00, 0x0C, 0x32, 0x41, 0x00, 0x1E };
    return space_is("DW", "10+2,'2A',30", 6, b, 6);
}
static bool test_space_DB_chars(void) {       /* DB 'A',10,'B' */
    const unsigned char b[] = { 0x41, 0x0A, 0x42 };
    return space_is("DB", "'A',10,'B'", 3, b, 3);
}
static bool test_space_DB_string(void) {      /* 교재 예: DB "EOT" (\0은 길이에 안 셈) */
    const unsigned char b[] = { 'E', 'O', 'T' };
    return space_is("DB", "\"EOT\"", 3, b, 3);
}

void register_directive_tests(void) {
    testRegister("test_dc_oprnd_4byte",   test_dc_oprnd_4byte);
    testRegister("test_dc_oprnd_2byte",   test_dc_oprnd_2byte);
    testRegister("test_dc_oprnd_many",    test_dc_oprnd_many);
    testRegister("test_dc_string_EOT",    test_dc_string_EOT);
    testRegister("test_space_RD_100",     test_space_RD_100);
    testRegister("test_space_RW_hex",     test_space_RW_hex);
    testRegister("test_space_DD_10",      test_space_DD_10);
    testRegister("test_space_DW_mixed",   test_space_DW_mixed);
    testRegister("test_space_DB_chars",   test_space_DB_chars);
    testRegister("test_space_DB_string",  test_space_DB_string);
}