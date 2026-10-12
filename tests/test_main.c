#include "test_framework.h"
#include "test_suites.h"

int main(void) {
    register_dctab_tests();
    register_optab_tests();
    register_symtab_tests();
    return testRun();
}