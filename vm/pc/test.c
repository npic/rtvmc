#include <stdio.h>
#include "test.h"
#include "vm_devices.h"

errcode_t init_test(test_t * test, const char * programfile, const char * testfile)
{
    errcode_t retval;
    FILE * in;
    uint16_t i;
    uint16_t next_gpio_value;
    char filename[256];

    retval = init_vm(&test->uut);
    CHECK_OK(retval, "Failed to init vm\n");
    retval = load_program(&test->uut, programfile);
    CHECK_OK(retval, "Failed to load program\n");
    
    in = fopen(testfile, "r");
    CHECK_NOT_NULL(in, IO, "Failed to open test file\n");
    fscanf(in, "%u", &test->test_length);
    CHECK_NOT_FERROR(in);

    /*
    test->uart_in = fopen("uartin.dat", "w");
    CHECK_NOT_NULL(test->uart_in, IO, "Failed to open UART in file\n");
    test->uart_out = fopen("uartout.dat", "w");
    CHECK_NOT_NULL(test->uart_out, IO, "Failed to open UART out file\n");
    */
    for (i = 0; i < TEST_PIN_COUNT; i++)
    {
        sprintf(filename, "gpio_%u.dat", i);
        test->gpio_files[i] = fopen(filename, "w");
        CHECK_NOT_NULL(test->gpio_files[i], IO, "Failed to open GPIO file\n");
    }

    test->input_size = 0;
    while (!feof(in))
    {
        fscanf(in, "%u %u %hu", &test->gpio_input[test->input_size].time, &test->gpio_input[test->input_size].pin, &next_gpio_value);
        test->gpio_input[test->input_size].value = (uint8_t)next_gpio_value;
        test->input_size++;
    }
    test->input_size--;  /* discard excessive read */
    fclose(in);

    for (i = 0; i < test->uut.proc_table_size; i++)
    {
        sprintf(filename, "sched_%u.dat", i);
        test->schedules[i] = fopen(filename, "w");
        CHECK_NOT_NULL(test->schedules[i], IO, "Failed to open schedule file\n");
    }
    return OK;
}

errcode_t test_step(test_t * test)
{
    errcode_t retval;
    uint16_t i;
    uint8_t pin_val;
    /*
    char uart_out_char = 0;
    */
    retval = reschedule(&test->uut);
    CHECK_OK(retval, "Failed to do scheduling\n");
    test->uut.PP = test->uut.NPP;
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
        if (test->gpio_input[i].time == test->uut.time)
        {
            retval = gpio_set(test->gpio_input[i].pin, test->gpio_input[i].value);
            CHECK_OK(retval, "Failed to set test pin\n");
        }
        /*
        if (test->timings[i] == test->uut.time)
        {
            retval = enqueue(&test->uut.uart.in, test->input[i]);
            CHECK_OK(retval, "Failed to enqueue a test input\n");
            fprintf(test->uart_in, "%hu %hd\n", test->uut.time, (int16_t)test->input[i]);
            CHECK_NOT_FERROR(test->uart_in);
            break;
        }
        */
    }
    for (i = 0; i < TEST_PIN_COUNT; i++)
    {
        retval = gpio_get(i, &pin_val);
        CHECK_OK(retval, "Failed to get test pin\n");
        fprintf(test->gpio_files[i], "%u %u\n", test->uut.time, pin_val);
    }
    /*
    if (i == test->input_size)  / * i.e. there is no input this time * /
    {
        fprintf(test->uart_in, "%hu 0\n", test->uut.time);
        CHECK_NOT_FERROR(test->uart_in);
    }
    retval = dequeue(&test->uut.uart.out, &uart_out_char);
    CHECK_OK(retval, "Failed to dequeue from UART out\n");
    fprintf(test->uart_out, "%hu %hd\n", test->uut.time, (int16_t)uart_out_char);
    CHECK_NOT_FERROR(test->uart_out);
    */
    test->uut.proc_table[test->uut.PP].current_observed_time++;
    retval = step(&test->uut);
    test->uut.time++;
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
    /*
    fclose(test->uart_in);
    fclose(test->uart_out);
    */
    for (i = 0; i < test->uut.proc_table_size; i++)
    {
        fclose(test->schedules[i]);
        printf("Process %hu consumed %hu ticks max\n", i, test->uut.proc_table[i].max_observed_time);
    }
    for (i = 0; i < TEST_PIN_COUNT; i++)
    {
        fclose(test->gpio_files[i]);
    }
    return OK;
}

