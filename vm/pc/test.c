#include <stdio.h>
#include "test.h"

errcode_t init_test(test_t * test, const char * programfile, const char * testfile)
{
    errcode_t retval;
    FILE * in;
    uint16_t i;
    char schedname[256];

    retval = init_vm(&test->uut);
    CHECK_OK(retval, "Failed to init vm\n");
    retval = load_program(&test->uut, programfile);
    CHECK_OK(retval, "Failed to load program\n");
    
    in = fopen(testfile, "r");
    CHECK_NOT_NULL(in, IO, "Failed to open test file\n");
    fscanf(in, "%hu", &test->test_length);
    CHECK_NOT_FERROR(in);
    /* TODO: read inputs, timings, open uart_out */
    fclose(in);

    for (i = 0; i < test->uut.proc_table_size; i++)
    {
        sprintf(schedname, "sched_%u.dat", i);
        test->schedules[i] = fopen(schedname, "w");
        CHECK_NOT_NULL(in, IO, "Failed to open schedule file\n");
    }
    return OK;
}

errcode_t test_step(test_t * test)
{
    errcode_t retval;
    uint16_t i;
    retval = reschedule(&test->uut);
    CHECK_OK(retval, "Failed to do scheduling\n");
    for (i = 0; i < test->uut.proc_table_size; i++)
    {
        if (test->uut.PP == i)
        {
            fprintf(test->schedules[i], "%u 1\n", test->uut.time);
        }
        else
        {
            fprintf(test->schedules[i], "%u 0\n", test->uut.time);
        }
    }
    /* TODO: IO stuff */
    retval = step(&test->uut);
    CHECK_OK(retval, "Failed to execute instruction\n");
    if (test->uut.time >= test->test_length)
    {
        finish_test(test);
    }
    return OK;
}

errcode_t finish_test(test_t * test)
{
    uint16_t i;
    test->uut.is_halted = 1;
    for (i = 0; i < test->uut.proc_table_size; i++)
    {
        fclose(test->schedules[i]);
        printf("\nProcess %hu consumed %hu ticks max", i, test->uut.proc_table[i].max_observed_time);
    }
    printf("\n");
    return OK;
}

