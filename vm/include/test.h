#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include "config.h"
#include "error.h"
#include "vm.h"

typedef struct test_gpio_input_t test_gpio_input_t;
struct test_gpio_input_t
{
    uint32_t time;
    uint32_t pin;
    uint8_t value;
};

typedef struct test_t test_t;
struct test_t
{
    vm_t uut;
    uint32_t test_length;
    uint16_t input_size;
    test_gpio_input_t gpio_input[TEST_SIZE];
    uint8_t gpio_pins[TEST_PIN_COUNT];
    /*
    FILE * uart_in;
    FILE * uart_out;
    */
    FILE * gpio_files[TEST_PIN_COUNT];
    FILE * schedules[VM_PROC_TABLE_SIZE];
};

errcode_t init_test(test_t * test, const char * programfile, const char * testfile);
errcode_t test_step(test_t * test);
errcode_t finish_test(test_t * test);

#endif

