#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "vm_devices.h"

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
    BSP_LED_Init(LED2);
    return OK;
}

