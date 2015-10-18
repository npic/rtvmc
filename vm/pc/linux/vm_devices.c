#include "vm_devices.h"
#include "test.h"

extern test_t test;

/*
errcode_t init_uart(vm_uart_t * uart)
{
    uint16_t i;
    for (i = 0; i < VM_UART_QUEUE_SIZE; i++)
    {
        uart->in.queue[i] = '\0';
        uart->out.queue[i] = '\0';
    }
    uart->in.queue_start = uart->in.queue_end = uart->in.queue;
    uart->out.queue_start = uart->out.queue_end = uart->out.queue;
    return OK;
}
*/

errcode_t init_hw()
{
    uint16_t i;
    for (i = 0; i < TEST_PIN_COUNT; i++)
    {
        test.gpio_pins[i] = 0;
    }
    return OK;
}

errcode_t gpio_get(uint32_t pin, uint8_t * result)
{
    if (pin >= TEST_PIN_COUNT)
    {
        FAIL(BAD_ARGUMENT, "No such GPIO pin\n");
    }
    *result = test.gpio_pins[pin];
    return OK;
}

errcode_t gpio_set(uint32_t pin, uint8_t value)
{
    if (pin >= TEST_PIN_COUNT)
    {
        FAIL(BAD_ARGUMENT, "No such GPIO pin\n");
    }
    test.gpio_pins[pin] = (value ? 1 : 0);
    return OK;
}
