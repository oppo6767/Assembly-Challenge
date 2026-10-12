#include <stdio.h>
#include "test_framework.h"
#include "test_suites.h"
#include "symtab.h"

/* 표가 테스트 사이에 초기화되지 않으니 등록 순서 중요 */
static bool test_symtab_RND_predefined(void)     { return see_SYMTAB("RND") == RND_OPR; }
static bool test_symtab_insert_new_ok(void)      { return ins_SYMTAB(0x1300, "VAR1") == SYM_OK; }
static bool test_symtab_insert_duplicate(void) {
    ins_SYMTAB(0x1304, "VAR2");
    return ins_SYMTAB(0x1308, "VAR2") == SYM_DUPLICATED;
}
static bool test_symtab_8chars_ok(void)          { return ins_SYMTAB(0x1310, "ABCDEFGH") == SYM_OK; }
static bool test_symtab_9chars_too_long(void)    { return ins_SYMTAB(0x1314, "ABCDEFGHI") == SYM_TOO_LONG; }
static bool test_symtab_find_value(void)         { return see_SYMTAB("VAR1") == 0x1300; }
static bool test_symtab_unknown_is_INV_VAL(void) { return see_SYMTAB("NOPE") == INV_VAL; }

void register_symtab_tests(void) {
    testRegister("test_symtab_RND_predefined",     test_symtab_RND_predefined);
    testRegister("test_symtab_insert_new_ok",      test_symtab_insert_new_ok);
    testRegister("test_symtab_insert_duplicate",   test_symtab_insert_duplicate);
    testRegister("test_symtab_8chars_ok",          test_symtab_8chars_ok);
    testRegister("test_symtab_9chars_too_long",    test_symtab_9chars_too_long);
    testRegister("test_symtab_find_value",         test_symtab_find_value);
    testRegister("test_symtab_unknown_is_INV_VAL", test_symtab_unknown_is_INV_VAL);
}