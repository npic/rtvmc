#include <stdio.h>
#include "vm.h"

errcode_t load_program(vm_t * vm, const char * filename)
{
    FILE * in;
    vm_mem_ptr_t index, i;
    uint8_t byte_of_data;

    in = fopen(filename, "rb");
    CHECK_NOT_NULL(in, IO, "Failed to open program file\n");

    fread(&vm->proc_table_size, 1, 1, in);
    CHECK_NOT_FERROR(in);
    if (vm->proc_table_size > VM_PROC_TABLE_SIZE)
    {
        FAIL(MEM_OVERFLOW, "Too many processes\n");
    }

    for (index = 0; index < vm->proc_table_size; index++)
    {
        vm_mem_ptr_t program_length;

        fread(&byte_of_data, 1, 1, in);
        CHECK_NOT_FERROR(in);
        vm->proc_table[index].period = ((uint16_t)byte_of_data << 8);
        fread(&byte_of_data, 1, 1, in);
        CHECK_NOT_FERROR(in);
        vm->proc_table[index].period += (uint16_t)byte_of_data;

        fread(&byte_of_data, 1, 1, in);
        CHECK_NOT_FERROR(in);
        vm->proc_table[index].length = ((uint16_t)byte_of_data << 8);
        fread(&byte_of_data, 1, 1, in);
        CHECK_NOT_FERROR(in);
        vm->proc_table[index].length += (uint16_t)byte_of_data;

        fread(&byte_of_data, 1, 1, in);
        CHECK_NOT_FERROR(in);
        program_length = ((uint16_t)byte_of_data << 8);
        fread(&byte_of_data, 1, 1, in);
        CHECK_NOT_FERROR(in);
        program_length += (uint16_t)byte_of_data;
        if (program_length > VM_CODE_SIZE)
        {
            FAIL(MEM_OVERFLOW, "Code too big\n");
        }

        i = 0;
        while( (!feof(in)) && (i < program_length) )
        {
            fread(&byte_of_data, 1, 1, in);
            CHECK_NOT_FERROR(in);
            if (!feof(in))
            {
                vm->proc_table[index].code[i] = byte_of_data;
                i++;
            }
        }
    }

    fclose(in);

    return OK;
}

