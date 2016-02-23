#include "vm.h"

vm_t vm;

int main(int argc, char ** argv)
{
    errcode_t retval;

    if (argc != 2)
    {
        FAIL(BAD_ARGUMENT, "Usage: vm [PROGRAM]\n");
    }
    retval = init_vm(&vm);
    CHECK_OK(retval, "Failed to init vm\n");
    retval = load_program(&vm, argv[1]);
    CHECK_OK(retval, "Failed to load program\n");
    retval = reschedule(&vm);
    CHECK_OK(retval, "Failed to do initial scheduling\n");
    while (!vm.is_halted)
    {
        vm.PP = vm.NPP;
        retval = step(&vm);
        CHECK_OK(retval, "Failed to execute instruction\n");
    }
    return OK;
}

