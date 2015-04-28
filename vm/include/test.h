#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include "config.h"
#include "error.h"
#include "vm.h"

typedef struct test_t test_t;
struct test_t
{
    vm_t uut;
    uint16_t test_length;
    uint16_t input_size;
    unsigned char input[TEST_SIZE];
    uint16_t timings[TEST_SIZE];
    FILE * uart_out;
    FILE * schedules[VM_PROC_TABLE_SIZE];
};

errcode_t init_test(test_t * test, const char * programfile, const char * testfile);
errcode_t test_step(test_t * test);
errcode_t finish_test(test_t * test);

#endif

