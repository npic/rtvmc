#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include "vm_devices.h"

errcode_t init_uart(vm_uart_t * uart)
{
    uint16_t i;
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    for (i = 0; i < VM_UART_QUEUE_SIZE; i++)
    {
        uart->in.queue[i] = '\0';
        uart->out.queue[i] = '\0';
    }
    uart->in.queue_start = uart->in.queue_end = uart->in.queue;
    uart->out.queue_start = uart->out.queue_end = uart->out.queue;
    return OK;
}

errcode_t sync_uart(vm_uart_t * uart)
{
    errcode_t retval;
    struct timeval tv;
    fd_set fs;
    tv.tv_usec = tv.tv_sec = 0;
     
    FD_ZERO(&fs);
    FD_SET(STDIN_FILENO, &fs);
    select(STDIN_FILENO + 1, &fs, 0, 0, &tv);
     
    if (FD_ISSET(STDIN_FILENO, &fs))
    {
        retval = enqueue(&uart->in, getchar());
        CHECK_OK(retval, "Failed to enqueue char from UART\n");
    }

    retval = check_queue(&uart->out);
    if (retval == OK)
    {
        char c;
        retval = dequeue(&uart->out, &c);
        putchar(c);
        CHECK_NOT_FERROR(stdout);
    }
    return OK;
}

