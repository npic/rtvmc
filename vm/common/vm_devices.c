#include "vm_devices.h"

/*
errcode_t check_queue(uart_queue_t * q)
{
    if (q->queue_start == q->queue_end)
    {
        return OK_NEGATIVE;
    }
    else
    {
        return OK;
    }
}

errcode_t enqueue(uart_queue_t * q, char c)
{
    if (q->queue_end == q->queue + VM_UART_QUEUE_SIZE)
    {
        return OK_NEGATIVE;
    }
    *(q->queue_end) = c;
    q->queue_end++;
    return OK;
}

errcode_t dequeue(uart_queue_t * q, char * result)
{
    errcode_t retval;
    retval = check_queue(q);
    CHECK_OK_POSITIVE(retval, "");
    *result = *(q->queue_start);
    q->queue_start++;
    if (q->queue_start == q->queue_end)
    {
        q->queue_start = q->queue_end = q->queue;
    }
    return OK;
}

errcode_t uart_write(vm_uart_t * uart, char c)
{
    errcode_t retval;
    retval = enqueue(&uart->out, c);
    CHECK_OK(retval, "Failed to enqueue into the UART queue\n");
    return OK;
}

errcode_t uart_read(vm_uart_t * uart, char * result)
{
    errcode_t retval;
    retval = dequeue(&uart->in, result);
    CHECK_OK(retval, "Failed to dequeue from the UART queue\n");
    return OK;
}
*/

