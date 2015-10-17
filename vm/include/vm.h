#ifndef VM_H
#define VM_H

#include "config.h"
#include "error.h"
#include "vm_devices.h"

typedef struct vm_proc_descr_t vm_proc_descr_t;
struct vm_proc_descr_t
{
    uint16_t period;
    uint16_t length;
    uint8_t is_released;
    vm_mem_ptr_t SP;
    vm_mem_ptr_t CP;
    int8_t stack[VM_STACK_SIZE];
    int8_t memory[VM_MEM_SIZE]; /* TODO: make mem and stack 32 bit */
    uint8_t code[VM_CODE_SIZE];
    uint32_t current_observed_time;
    uint32_t max_observed_time;
};

typedef struct vm_t vm_t;
struct vm_t
{
    vm_mem_ptr_t PP;
    vm_mem_ptr_t NPP;
    vm_mem_ptr_t proc_table_size;
    vm_proc_descr_t proc_table[VM_PROC_TABLE_SIZE];
    vm_uart_t uart; /* TODO: rename UART to GPIO */
    uint32_t time;
    uint8_t is_halted;
};

errcode_t init_vm(vm_t * vm);
errcode_t load_program(vm_t * vm, const char * filename);
errcode_t reschedule(vm_t * vm);
errcode_t step(vm_t * vm);
uint16_t read_16_bit(void * data); /* TODO: do we need this? */
errcode_t stack_push(vm_t * vm, int8_t * val);
errcode_t stack_pop(vm_t * vm, int8_t * val);

#endif

