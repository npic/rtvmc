#include <stdio.h>
#include "test.h"
#include "vm_devices.h"

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

    test->uart_in = fopen("uartin.dat", "w");
    CHECK_NOT_NULL(test->uart_in, IO, "Failed to open UART in file\n");
    test->uart_out = fopen("uartout.dat", "w");
    CHECK_NOT_NULL(test->uart_out, IO, "Failed to open UART out file\n");

    test->input_size = 0;
    while (!feof(in))
    {
        fscanf(in, "%hu %hu", &test->timings[test->input_size], &i);
        test->input[test->input_size] = (unsigned char)i;
        test->input_size++;
    }
    test->input_size--;  /* discard excessive read */
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
    char uart_out_char = 0;
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
    for (i = 0; i < test->input_size; i++)
    {
        if (test->timings[i] == test->uut.time)
        {
            retval = enqueue(&test->uut.uart.in, test->input[i]);
            CHECK_OK(retval, "Failed to enqueue a test input\n");
            fprintf(test->uart_in, "%hu %hu\n", test->uut.time, (uint16_t)test->input[i]);
            CHECK_NOT_FERROR(test->uart_in);
            break;
        }
    }
    if (i == test->input_size)  /* i.e. there is no input this time */
    {
        fprintf(test->uart_in, "%hu 0\n", test->uut.time);
        CHECK_NOT_FERROR(test->uart_in);
    }
    retval = dequeue(&test->uut.uart.out, &uart_out_char);
    CHECK_OK(retval, "Failed to dequeue from UART out\n");
    fprintf(test->uart_out, "%hu %hu\n", test->uut.time, (uint16_t)uart_out_char);
    CHECK_NOT_FERROR(test->uart_out);
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
    fclose(test->uart_in);
    fclose(test->uart_out);
    for (i = 0; i < test->uut.proc_table_size; i++)
    {
        fclose(test->schedules[i]);
        printf("Process %hu consumed %hu ticks max\n", i, test->uut.proc_table[i].max_observed_time);
    }
    return OK;
}

