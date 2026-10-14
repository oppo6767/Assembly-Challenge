#include "test_framework.h"
#include "test_suites.h"
#include "cal_mnmo.h"
#include "asm_mnmo.h"
#include "optab.h"
#include "symtab.h"
#include "rdline.h"

/* ============================================================
 * 도우미: 명령어 이름 + 피연산자로 asm_mnemonic을 돌리고
 *         OBJC에 들어간 4바이트가 기대값과 같은지 확인
 * ============================================================ */
static bool assemble_is(char *mnemonic, char *operand,
                        unsigned char b1, unsigned char b2,
                        unsigned char b3, unsigned char b4) {
    Optab *op = see_OPTAB(mnemonic);
    if (op == NULL) return false;
    OPerand = operand;                      /* read_line이 채워 주던 값을 직접 넣음 */
    if (asm_mnemonic(op) != 4) return false;
    return OBJC[0] == 4 && OBJC[1] == b1 && OBJC[2] == b2
        && OBJC[3] == b3 && OBJC[4] == b4;
}

/* ============================================================
 * cal_nm_dmpr : DMPR 전용 피연산자 (레지스터 이름 → 번호)
 * ============================================================ */
static bool test_dmpr_PC(void) { return cal_nm_dmpr("PC") == 0; }
static bool test_dmpr_A(void)  { return cal_nm_dmpr("A")  == 3; }

/* ============================================================
 * cal_nm_oprnd : 일반 피연산자 (수식 계산 + [X] 처리)
 * ============================================================ */
static bool test_oprnd_number(void)  { return cal_nm_oprnd("100") == 100; }
static bool test_oprnd_symbol(void) {
    ins_SYMTAB(0x1300, "VAR1");
    return cal_nm_oprnd("VAR1+4") == 0x1304;
}
static bool test_oprnd_xbit(void)    { return cal_nm_oprnd("100[X]") == (100 | OP_XBIT); }
static bool test_oprnd_minus_mask(void) { return cal_nm_oprnd("-1") == 0x7FFFFF; }  /* 음수는 23비트로 잘림 */

/* ============================================================
 * asm_mnemonic : 연산코드 + 피연산자 → 4바이트 기계어
 * (교재 72쪽 출력 예와 같은 값)
 * ============================================================ */
static bool test_asm_LDA_VAR1(void)   { return assemble_is("LDA",  "VAR1", 0x00, 0x00, 0x13, 0x00); }
static bool test_asm_ADD_number(void) { return assemble_is("ADD",  "10h",  0x10, 0x00, 0x00, 0x10); }
static bool test_asm_HALT(void)       { return assemble_is("HALT", NULL,   0x3F, 0x00, 0x00, 0x00); }  /* 피연산자 없음 */
static bool test_asm_LDA_xbit(void)   { return assemble_is("LDA",  "100[X]", 0x00, 0x80, 0x00, 0x64); }
static bool test_asm_DMPR_X(void)     { return assemble_is("DMPR", "X",    0x38, 0x00, 0x00, 0x02); }

void register_mnemonic_tests(void) {
    testRegister("test_dmpr_PC",            test_dmpr_PC);
    testRegister("test_dmpr_A",             test_dmpr_A);
    testRegister("test_oprnd_number",       test_oprnd_number);
    testRegister("test_oprnd_symbol",       test_oprnd_symbol);
    testRegister("test_oprnd_xbit",         test_oprnd_xbit);
    testRegister("test_oprnd_minus_mask",   test_oprnd_minus_mask);
    testRegister("test_asm_LDA_VAR1",       test_asm_LDA_VAR1);
    testRegister("test_asm_ADD_number",     test_asm_ADD_number);
    testRegister("test_asm_HALT",           test_asm_HALT);
    testRegister("test_asm_LDA_xbit",       test_asm_LDA_xbit);
    testRegister("test_asm_DMPR_X",         test_asm_DMPR_X);
}