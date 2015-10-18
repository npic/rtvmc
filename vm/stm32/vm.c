#include <stdio.h>
#include "vm.h"

errcode_t load_program(vm_t * vm, const char * filename)
{
    vm_mem_ptr_t index, i, seek;

    /* This array MUST go to .rodata to be put in flash instead of RAM */
    /* do NOT touch INJECTION strings, the binary goes here on build */
    static const uint8_t injected_code[] = {
        /* INJECTION START */
        /* INJECTION STOP */
    };

    vm->proc_table_size = injected_code[0];
    if (vm->proc_table_size > VM_PROC_TABLE_SIZE)
    {
        FAIL(MEM_OVERFLOW, "Too many processes\n");
    }

    for (index = 0, seek = 1; index < vm->proc_table_size; index++)
    {
        vm_mem_ptr_t program_length;

        vm->proc_table[index].period = ((uint16_t)injected_code[seek++] << 8);
        vm->proc_table[index].period += (uint16_t)injected_code[seek++];

        vm->proc_table[index].length = ((uint16_t)injected_code[seek++] << 8);
        vm->proc_table[index].length += (uint16_t)injected_code[seek++];

        program_length = ((uint16_t)injected_code[seek++] << 8);
        program_length += (uint16_t)injected_code[seek++];
        if (program_length > VM_CODE_SIZE)
        {
            FAIL(MEM_OVERFLOW, "Code too big\n");
        }

        i = 0;
        while (i < program_length)
        {
            vm->proc_table[index].code[i] = injected_code[seek++];
            i++;
        }
    }
    
    return OK;
}

