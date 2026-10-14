#include "test_framework.h"
#include "test_suites.h"
#include "str2int.h"
#include "symtab.h"

/* ============================================================
 * dec2int : 10진수 문자열 → 정수
 * ============================================================ */
static bool test_dec2int_basic(void)          { return dec2int("123") == 123; }
static bool test_dec2int_zero(void)           { return dec2int("0") == 0; }
static bool test_dec2int_leading_zero(void)   { return dec2int("010") == 10; }
static bool test_dec2int_rejects_letter(void) { return dec2int("12A") == INV_VAL; }

/* ============================================================
 * hex2int : 16진수 문자열(끝에 h) → 정수
 * ============================================================ */
static bool test_hex2int_basic(void)          { return hex2int("1Ah") == 26; }
static bool test_hex2int_address(void)        { return hex2int("1300h") == 0x1300; }
static bool test_hex2int_all_F(void)          { return hex2int("FFh") == 255; }
static bool test_hex2int_rejects_lower(void)  { return hex2int("1ah") == INV_VAL; }  /* 교재 규칙: 대문자만 */
static bool test_hex2int_rejects_G(void)      { return hex2int("1Gh") == INV_VAL; }

/* ============================================================
 * str2int : 끝 글자 'h'를 보고 10진수/16진수 골라 부르기
 * ============================================================ */
static bool test_str2int_picks_dec(void)      { return str2int("96") == 96; }
static bool test_str2int_picks_hex(void)      { return str2int("96h") == 0x96; }
static bool test_str2int_rejects_bad(void)    { return str2int("9X") == INV_VAL; }

void register_str2int_tests(void) {
    testRegister("test_dec2int_basic",          test_dec2int_basic);
    testRegister("test_dec2int_zero",           test_dec2int_zero);
    testRegister("test_dec2int_leading_zero",   test_dec2int_leading_zero);
    testRegister("test_dec2int_rejects_letter", test_dec2int_rejects_letter);
    testRegister("test_hex2int_basic",          test_hex2int_basic);
    testRegister("test_hex2int_address",        test_hex2int_address);
    testRegister("test_hex2int_all_F",          test_hex2int_all_F);
    testRegister("test_hex2int_rejects_lower",  test_hex2int_rejects_lower);
    testRegister("test_hex2int_rejects_G",      test_hex2int_rejects_G);
    testRegister("test_str2int_picks_dec",      test_str2int_picks_dec);
    testRegister("test_str2int_picks_hex",      test_str2int_picks_hex);
    testRegister("test_str2int_rejects_bad",    test_str2int_rejects_bad);
}