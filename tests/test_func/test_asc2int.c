#include "test_framework.h"
#include "test_suites.h"
#include "asc2int.h"
#include "symtab.h"

/* 글자 하나 → 그 글자의 아스키 코드 */
static bool test_asc2int_one_char(void)        { return asc2int("'A'") == 0x41; }

/* 글자 두 개 → 한 바이트씩 쌓임 ('A'=0x41, 'B'=0x42) */
static bool test_asc2int_two_chars(void)       { return asc2int("'AB'") == 0x4142; }

/* 숫자 글자도 숫자가 아니라 아스키 코드로 ('1'=0x31, '2'=0x32) */
static bool test_asc2int_digit_chars(void)     { return asc2int("'12'") == 0x3132; }

/* 4글자 = int 4바이트를 꽉 채움 */
static bool test_asc2int_four_chars(void)      { return asc2int("'ABCD'") == 0x41424344; }

/* 따옴표 사이가 비어 있으면 0 */
static bool test_asc2int_empty_quotes(void)    { return asc2int("''") == 0; }

/* 닫는 따옴표가 없으면 변환 실패 */
static bool test_asc2int_rejects_no_close(void) { return asc2int("'AB") == INV_VAL; }

void register_asc2int_tests(void) {
    testRegister("test_asc2int_one_char",         test_asc2int_one_char);
    testRegister("test_asc2int_two_chars",        test_asc2int_two_chars);
    testRegister("test_asc2int_digit_chars",      test_asc2int_digit_chars);
    testRegister("test_asc2int_four_chars",       test_asc2int_four_chars);
    testRegister("test_asc2int_empty_quotes",     test_asc2int_empty_quotes);
    testRegister("test_asc2int_rejects_no_close", test_asc2int_rejects_no_close);
}