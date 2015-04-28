#ifndef VM_DEVICES_H
#define VM_DEVICES_H

#include "config.h"
#include "error.h"

typedef struct uart_queue_t uart_queue_t;
struct uart_queue_t
{
    char queue[VM_UART_QUEUE_SIZE];
    char * queue_start;
    char * queue_end;
};

typedef struct vm_uart_t vm_uart_t;
struct vm_uart_t
{
    uart_queue_t in;
    uart_queue_t out;
};

errcode_t init_uart(vm_uart_t * uart);
errcode_t check_queue(uart_queue_t * q);
errcode_t enqueue(uart_queue_t * q, char c);
errcode_t dequeue(uart_queue_t * q, char * result);
errcode_t uart_write(vm_uart_t * uart, char c);
errcode_t uart_read(vm_uart_t * uart, char * result);

#endif

