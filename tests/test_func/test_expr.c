#include "test_framework.h"
#include "test_suites.h"
#include "cal_term.h"
#include "cal_expr.h"
#include "symtab.h"

/* 주의: cal_one_expr은 받은 문자열 안의 '+', '-' 자리에 0을 써 넣어서 문자열을 바꿈.
 * 그래서 "10h+10" 같은 문자열 상수를 바로 넘기면 안 되고, 고칠 수 있는 배열에 담아서 넘김. */

/* ============================================================
 * cnv_one_term : 항 하나 → 숫자 (첫 글자/끝 글자 보고 3갈래)
 * ============================================================ */
static bool test_term_decimal(void)   { char t[] = "100";  return cnv_one_term(t) == 100; }
static bool test_term_hex(void)       { char t[] = "1Ah";  return cnv_one_term(t) == 26; }
static bool test_term_hex_letter_start(void) { char t[] = "FFh"; return cnv_one_term(t) == 255; }  /* 글자로 시작해도 끝이 h면 16진수 */
static bool test_term_ascii(void)     { char t[] = "'AB'"; return cnv_one_term(t) == 0x4142; }
static bool test_term_symbol(void) {
    char t[] = "VAR1";
    ins_SYMTAB(0x1300, "VAR1");
    return cnv_one_term(t) == 0x1300;
}
static bool test_term_RND(void)       { char t[] = "RND";  return cnv_one_term(t) == RND_OPR; }

/* ============================================================
 * issign : 부호 판별
 * ============================================================ */
static bool test_issign_plus(void)    { return issign('+') == 1; }
static bool test_issign_minus(void)   { return issign('-') == -1; }
static bool test_issign_other(void)   { return issign('A') == 0; }

/* ============================================================
 * cal_one_expr : 항 1개 또는 "항 ± 항"
 * (VAR1 = 0x1300 은 위 test_term_symbol 에서 등록됨 → 등록 순서 중요)
 * ============================================================ */
static bool test_expr_single(void)    { char e[] = "100";    return cal_one_expr(e) == 100; }
static bool test_expr_plus(void)      { char e[] = "10h+10"; return cal_one_expr(e) == 26; }
static bool test_expr_minus(void)     { char e[] = "100h-1"; return cal_one_expr(e) == 0xFF; }
static bool test_expr_symbol_plus(void)  { char e[] = "VAR1+4"; return cal_one_expr(e) == 0x1304; }
static bool test_expr_symbol_minus(void) { char e[] = "VAR1-4"; return cal_one_expr(e) == 0x12FC; }
static bool test_expr_leading_minus(void){ char e[] = "-4";     return cal_one_expr(e) == -4; }
static bool test_expr_ascii_plus(void)   { char e[] = "'A'+1";  return cal_one_expr(e) == 0x42; }

void register_expr_tests(void) {
    testRegister("test_term_decimal",          test_term_decimal);
    testRegister("test_term_hex",              test_term_hex);
    testRegister("test_term_hex_letter_start", test_term_hex_letter_start);
    testRegister("test_term_ascii",            test_term_ascii);
    testRegister("test_term_symbol",           test_term_symbol);
    testRegister("test_term_RND",              test_term_RND);
    testRegister("test_issign_plus",           test_issign_plus);
    testRegister("test_issign_minus",          test_issign_minus);
    testRegister("test_issign_other",          test_issign_other);
    testRegister("test_expr_single",           test_expr_single);
    testRegister("test_expr_plus",             test_expr_plus);
    testRegister("test_expr_minus",            test_expr_minus);
    testRegister("test_expr_symbol_plus",      test_expr_symbol_plus);
    testRegister("test_expr_symbol_minus",     test_expr_symbol_minus);
    testRegister("test_expr_leading_minus",    test_expr_leading_minus);
    testRegister("test_expr_ascii_plus",       test_expr_ascii_plus);
}