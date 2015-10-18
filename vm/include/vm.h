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
    int32_t stack[VM_STACK_SIZE];
    int32_t memory[VM_MEM_SIZE];
    uint8_t code[VM_CODE_SIZE];
    uint16_t current_observed_time;
    uint16_t max_observed_time;
};

typedef struct vm_t vm_t;
struct vm_t
{
    vm_mem_ptr_t PP;
    vm_mem_ptr_t NPP;
    vm_mem_ptr_t proc_table_size;
    vm_proc_descr_t proc_table[VM_PROC_TABLE_SIZE];
    /*vm_uart_t uart;*/
    uint32_t time;
    uint8_t is_halted;
};

errcode_t init_vm(vm_t * vm);
errcode_t load_program(vm_t * vm, const char * filename);
errcode_t reschedule(vm_t * vm);
errcode_t step(vm_t * vm);
uint32_t read_32_bit(void * data);
errcode_t stack_push(vm_t * vm, int32_t * val);
errcode_t stack_pop(vm_t * vm, int32_t * val);

#endif

