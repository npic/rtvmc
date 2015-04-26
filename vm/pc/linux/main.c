#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "test.h"
#include "vm.h"

void restore_terminal(void)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    return;
}

int main(int argc, char ** argv)
{
    errcode_t retval;
    static test_t test;

    atexit(restore_terminal);
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

