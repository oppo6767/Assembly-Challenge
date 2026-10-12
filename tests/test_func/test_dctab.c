#include <stdio.h>
#include "test_framework.h"
#include "test_suites.h"
#include "dctab.h"

static bool test_dctab_finds_DD(void)          { return see_DCTAB("DD"); }
static bool test_dctab_rejects_lowercase(void) { return !see_DCTAB("rw"); }

void register_dctab_tests(void) {
    testRegister("test_dctab_finds_DD",          test_dctab_finds_DD);
    testRegister("test_dctab_rejects_lowercase", test_dctab_rejects_lowercase);
}