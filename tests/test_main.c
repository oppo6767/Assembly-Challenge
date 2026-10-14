#include "test_framework.h"
#include "test_suites.h"

int main(void) {
    register_dctab_tests();
    register_optab_tests();
    register_symtab_tests();
    register_str2int_tests();
    register_asc2int_tests();
    register_expr_tests();
    register_mnemonic_tests();
    register_directive_tests();
    return testRun();
}