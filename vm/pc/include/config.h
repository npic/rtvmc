#ifndef CONFIG_H
#define CONFIG_H

typedef unsigned char uint8_t;
typedef char int8_t;
typedef short int int16_t;
typedef unsigned short int uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;

#define VM_STACK_SIZE 255 /* per process */
#define VM_MEM_SIZE 255 /* per process */
#define VM_CODE_SIZE 2048 /* per process */
#define VM_PROC_TABLE_SIZE 255
/*
#define VM_UART_QUEUE_SIZE 255
*/
#define TEST_SIZE 65536
#define TEST_PIN_COUNT 8

typedef uint16_t vm_mem_ptr_t;

#endif

