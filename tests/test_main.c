#include <stddef.h> // NULL이 들어 있음
#include "test_framework.h"
#include "dctab.h"
#include "optab.h"

/* ===== 도우미: 찾은 칸의 값이 기대값과 같은지 확인 ===== */
static bool opcode_is(char *name, unsigned char expected) {
    Optab *op = see_OPTAB(name);
    if (op == NULL) return false;          /* 못 찾으면 실패 (값 꺼내기 전에 차단) */
    return op->opcode == expected;
}

static bool n_operand_is(char *name, unsigned char expected) {
    Optab *op = see_OPTAB(name);
    if (op == NULL) return false;
    return op->n_operand == expected;
}

bool test_example_dctab_one(void) { return see_DCTAB("DD"); }
bool test_example_dctab_two(void) { return !see_DCTAB("rw"); }
bool test_example_optab_one(void) { return see_OPTAB("LDA"); }
bool test_example_optab_two(void) { return !see_OPTAB("LGA"); }

/* ===== 테스트: 도우미 덕분에 한 줄씩 ===== */
bool test_optab_LDA_opcode_is_00(void)   { return opcode_is("LDA", 0x00); }
bool test_optab_HALT_opcode_is_3F(void)  { return opcode_is("HALT", 0x3F); }
bool test_optab_HALT_has_no_operand(void){ return n_operand_is("HALT", 0); }
bool test_optab_LDA_has_operand(void)    { return n_operand_is("LDA", 1); }

int main() {
    testRegister("test_example_dctab_one", test_example_dctab_one);
    testRegister("test_example_dctab_two", test_example_dctab_two);
    testRegister("test_example_optab_one", test_example_optab_one);
    testRegister("test_example_optab_two", test_example_optab_two);
    testRegister("test_optab_LDA_opcode_is_00",    test_optab_LDA_opcode_is_00);
    testRegister("test_optab_HALT_opcode_is_3F",   test_optab_HALT_opcode_is_3F);
    testRegister("test_optab_HALT_has_no_operand", test_optab_HALT_has_no_operand);
    testRegister("test_optab_LDA_has_operand",     test_optab_LDA_has_operand);
    int failed = testRun();
    return failed;
}