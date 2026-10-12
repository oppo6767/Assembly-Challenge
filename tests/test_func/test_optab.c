#include <stdio.h>
#include <stddef.h>
#include "test_framework.h"
#include "test_suites.h"
#include "optab.h"

/* 도우미 */
static bool opcode_is(char *name, unsigned char expected) {
    Optab *op = see_OPTAB(name);
    if (op == NULL) return false;
    return op->opcode == expected;
}
static bool n_operand_is(char *name, unsigned char expected) {
    Optab *op = see_OPTAB(name);
    if (op == NULL) return false;
    return op->n_operand == expected;
}

static bool test_optab_finds_LDA(void)           { return see_OPTAB("LDA"); }
static bool test_optab_rejects_unknown(void)     { return !see_OPTAB("LGA"); }
static bool test_optab_LDA_opcode_is_00(void)    { return opcode_is("LDA", 0x00); }
static bool test_optab_HALT_opcode_is_3F(void)   { return opcode_is("HALT", 0x3F); }
static bool test_optab_HALT_has_no_operand(void) { return n_operand_is("HALT", 0); }
static bool test_optab_LDA_has_operand(void)     { return n_operand_is("LDA", 1); }

void register_optab_tests(void) {
    testRegister("test_optab_finds_LDA",           test_optab_finds_LDA);
    testRegister("test_optab_rejects_unknown",     test_optab_rejects_unknown);
    testRegister("test_optab_LDA_opcode_is_00",    test_optab_LDA_opcode_is_00);
    testRegister("test_optab_HALT_opcode_is_3F",   test_optab_HALT_opcode_is_3F);
    testRegister("test_optab_HALT_has_no_operand", test_optab_HALT_has_no_operand);
    testRegister("test_optab_LDA_has_operand",     test_optab_LDA_has_operand);
}