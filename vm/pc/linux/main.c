#include <stdlib.h>
#include "test.h"
#include "vm.h"

int main(int argc, char ** argv)
{
    errcode_t retval;
    static test_t test;

    if (argc != 3)
    {
        FAIL(BAD_ARGUMENT, "Usage: vm [PROGRAM] [TEST]\n");
    }
    retval = init_test(&test, argv[1], argv[2]);
    CHECK_OK(retval, "Failed to init test environment\n");
    while (!test.uut.is_halted)
    {
        retval = test_step(&test);
        CHECK_OK(retval, "Failure during test step\n");
    }
    return OK;
}

