#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define VM_STACK_SIZE 255 /* per process */
#define VM_MEM_SIZE 255 /* per process */
#define VM_CODE_SIZE 2048 /* per process */
#define VM_PROC_TABLE_SIZE 16
#define PIN_COUNT 16
/*
#define VM_UART_QUEUE_SIZE 255
*/

typedef uint16_t vm_mem_ptr_t;

#endif

