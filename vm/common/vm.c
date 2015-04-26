#include <string.h>
#include "vm.h"
#include "vm_instruction.h"

errcode_t init_vm(vm_t * vm)
{
    errcode_t retval;
    vm_mem_ptr_t i;

    retval = init_uart(&vm->uart);
    CHECK_OK(retval, "Failed to init UART\n");

    vm->PP = 0;
    vm->proc_table_size = 0;

    for (i = 0; i < VM_PROC_TABLE_SIZE; i++)
    {
        vm->proc_table[i].period = 0;
        vm->proc_table[i].length = 0;
        vm->proc_table[i].is_released = 0;
        vm->proc_table[i].SP = 0;
        vm->proc_table[i].CP = 0;
        vm->proc_table[i].current_observed_time = 0;
        vm->proc_table[i].max_observed_time = 0;

        for (i = 0; i < VM_STACK_SIZE; i++)
        {
            vm->proc_table[i].stack[i] = 0;
        }

        for (i = 0; i < VM_MEM_SIZE; i++)
        {
            vm->proc_table[i].memory[i] = 0;
        }

        for (i = 0; i < VM_CODE_SIZE; i++)
        {
            vm->proc_table[i].code[i] = 0;
        }
    }

    vm->time = 0;
    vm->is_halted = 0;

    return OK;
}

errcode_t reschedule(vm_t * vm)
{
    vm_mem_ptr_t i;
    vm->PP = 0;
    for (i = 1; i < vm->proc_table_size; i++)
    {
        if (vm->time % vm->proc_table[i].period == 0)
        {
            if (vm->proc_table[i].is_released)
            {
                /* TODO: shouldn't we fail here? */
                log_error("\n\n\nDEADLINE MISSED!\n\n\n");
            }
            vm->proc_table[i].is_released = 1;
        }
        if (vm->proc_table[i].is_released)
        {
            if ( (vm->PP == 0) || (vm->proc_table[i].period < vm->proc_table[vm->PP].period) )
            {
                vm->PP = i;
            }
        }
    }
    return OK;
}

errcode_t step(vm_t * vm)
{
    errcode_t retval;
    uint8_t opcode;

    opcode = vm->proc_table[vm->PP].code[vm->proc_table[vm->PP].CP++];

    vm->proc_table[vm->PP].current_observed_time++;
    switch (opcode)
    {
        case OPC_STOP:
            retval = op_stop(vm);
            break;
        case OPC_NOP:
            retval = OK;
            break;
        case OPC_PUSH:
            retval = op_push(vm);
            break;
        case OPC_DUP:
            retval = op_dup(vm);
            break;
        case OPC_DROP:
            retval = op_drop(vm);
            break;
        case OPC_ADD:
            retval = op_add(vm);
            break;
        case OPC_SUB:
            retval = op_sub(vm);
            break;
        case OPC_MUL:
            retval = op_mul(vm);
            break;
        case OPC_DIV:
            retval = op_div(vm);
            break;
        case OPC_MOD:
            retval = op_mod(vm);
            break;
        case OPC_NOT:
            retval = op_not(vm);
            break;
        case OPC_AND:
            retval = op_and(vm);
            break;
        case OPC_OR:
            retval = op_or(vm);
            break;
        case OPC_BNOT:
            retval = op_bnot(vm);
            break;
        case OPC_BAND:
            retval = op_band(vm);
            break;
        case OPC_BOR:
            retval = op_bor(vm);
            break;
        case OPC_UARTIN:
            retval = op_uartin(vm);
            break;
        case OPC_UARTOUT:
            retval = op_uartout(vm);
            break;
        case OPC_STORE:
            retval = op_store(vm);
            break;
        case OPC_GET:
            retval = op_get(vm);
            break;
        case OPC_JT:
            retval = op_jt(vm);
            break;
        case OPC_EQ:
            retval = op_eq(vm);
            break;
        case OPC_NEQ:
            retval = op_neq(vm);
            break;
        case OPC_GEQ:
            retval = op_geq(vm);
            break;
        case OPC_LEQ:
            retval = op_leq(vm);
            break;
        case OPC_GT:
            retval = op_gt(vm);
            break;
        case OPC_LT:
            retval = op_lt(vm);
            break;
        case OPC_SYNCUART:
            retval = op_syncuart(vm);
            break;
        default:
            vm->is_halted = 1;
            FAIL(BAD_OPCODE, "Unknown opcode\n");
            break;
    }
    vm->time++;
    CHECK_OK_POSITIVE(retval, "");

    return OK;
}

uint16_t read_16_bit(void * data)
{
    return (*((uint8_t*)data) << 8) + *((uint8_t*)data + 1);
}

errcode_t stack_push(vm_t * vm, int8_t * val)
{
    if (vm->proc_table[vm->PP].SP == VM_STACK_SIZE)
    {
        FAIL(MEM_OVERFLOW, "Stack overflow\n");
    }
    memcpy(&(vm->proc_table[vm->PP].stack[vm->proc_table[vm->PP].SP++]), val, sizeof(int8_t));
    return OK;
}

errcode_t stack_pop(vm_t * vm, int8_t * val)
{
    if (vm->proc_table[vm->PP].SP == 0)
    {
        FAIL(MEM_OVERFLOW, "Stack underflow\n");
    }
    memcpy(val, &(vm->proc_table[vm->PP].stack[--vm->proc_table[vm->PP].SP]), sizeof(int8_t));
    return OK;
}

