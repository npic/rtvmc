#include <stdio.h>
#include "vm.h"

errcode_t load_program(vm_t * vm, const char * filename)
{
    vm_mem_ptr_t index, i, seek;

    uint8_t injected_code[] = {
        /* INJECTION START */
        /* INJECTION STOP */
    };

    vm->proc_table_size = injected_code[0];

    for (index = 0, seek = 1; index < vm->proc_table_size; index++)
    {
        vm_mem_ptr_t program_length;

        vm->proc_table[index].period = ((uint16_t)injected_code[seek++] << 8);
        vm->proc_table[index].period += (uint16_t)injected_code[seek++];

        vm->proc_table[index].length = ((uint16_t)injected_code[seek++] << 8);
        vm->proc_table[index].length += (uint16_t)injected_code[seek++];

        program_length = ((uint16_t)injected_code[seek++] << 8);
        program_length += (uint16_t)injected_code[seek++];

        i = 0;
        while (i < program_length)
        {
            vm->proc_table[index].code[i] = injected_code[seek++];
            i++;
        }
    }
    
    return OK;
}

