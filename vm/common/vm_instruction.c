#include <string.h>
#include "vm_devices.h"
#include "vm_instruction.h"

errcode_t op_stop(vm_t * vm)
{
    vm->proc_table[vm->PP].SP = 0;
    vm->proc_table[vm->PP].CP = 0;
    vm->proc_table[vm->PP].is_released = 0;
    if (vm->proc_table[vm->PP].current_observed_time > vm->proc_table[vm->PP].max_observed_time)
    {
        vm->proc_table[vm->PP].max_observed_time = vm->proc_table[vm->PP].current_observed_time;
    }
    vm->proc_table[vm->PP].current_observed_time = 0;
    return OK;
}

errcode_t op_push(vm_t * vm)
{
    errcode_t retval;
    int8_t arg = vm->proc_table[vm->PP].code[vm->proc_table[vm->PP].CP++];
    retval = stack_push(vm, &arg);
    CHECK_OK(retval, "Failed to push into the stack\n");
    return OK;
}

errcode_t op_dup(vm_t * vm)
{
    errcode_t retval;
    int8_t val;
    retval = stack_pop(vm, &val);
    CHECK_OK(retval, "Failed to pop from the stack\n");
    retval = stack_push(vm, &val);
    CHECK_OK(retval, "Failed to push into the stack\n");
    retval = stack_push(vm, &val);
    CHECK_OK(retval, "Failed to push into the stack\n");
    return OK;
}

errcode_t op_drop(vm_t * vm)
{
    errcode_t retval;
    int8_t val;
    retval = stack_pop(vm, &val);
    CHECK_OK(retval, "Failed to pop from the stack\n");
    return OK;
}

errcode_t op_add(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 + op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_sub(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 - op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_mul(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 * op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_div(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 / op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_mod(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 % op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_not(vm_t * vm)
{
    errcode_t retval;
    int8_t op;
    retval = stack_pop(vm, &op);
    CHECK_OK(retval, "Failed to pop op from the stack\n");
    op = !op;
    retval = stack_push(vm, &op);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_and(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 && op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_or(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 || op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_bnot(vm_t * vm)
{
    errcode_t retval;
    int8_t op;
    retval = stack_pop(vm, &op);
    CHECK_OK(retval, "Failed to pop op from the stack\n");
    op = ~op;
    retval = stack_push(vm, &op);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_band(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 & op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_bor(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 | op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_uartin(vm_t * vm)
{
    errcode_t retval;
    char result = 0;
    retval = uart_read(&vm->uart, &result);
    CHECK_OK(retval, "Failed to read from the UART queue\n");
    retval = stack_push(vm, &result);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_uartout(vm_t * vm)
{
    errcode_t retval;
    char c;
    retval = stack_pop(vm, &c);
    CHECK_OK(retval, "Failed to pop op from the stack\n");
    retval = uart_write(&vm->uart, c);
    CHECK_OK(retval, "Failed to read from the UART queue\n");
    return OK;
}

errcode_t op_store(vm_t * vm)
{
    errcode_t retval;
    int8_t addr;
    int8_t val;
    retval = stack_pop(vm, &addr);
    CHECK_OK(retval, "Failed to pop address from the stack\n");
    retval = stack_pop(vm, &val);
    CHECK_OK(retval, "Failed to pop op from the stack\n");
    vm->proc_table[vm->PP].memory[(uint8_t)addr] = val;
    return OK;
}

errcode_t op_get(vm_t * vm)
{
    errcode_t retval;
    int8_t addr;
    int8_t val;
    retval = stack_pop(vm, &addr);
    CHECK_OK(retval, "Failed to pop address from the stack\n");
    val = vm->proc_table[vm->PP].memory[(uint8_t)addr];
    retval = stack_push(vm, &val);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_jt(vm_t * vm)
{
    errcode_t retval;
    int8_t offset;
    int8_t cond;
    retval = stack_pop(vm, &offset);
    CHECK_OK(retval, "Failed to pop offset from the stack\n");
    retval = stack_pop(vm, &cond);
    CHECK_OK(retval, "Failed to pop condition from the stack\n");
    if (cond)
    {
        vm->proc_table[vm->PP].CP += offset;
    }
    return OK;
}

errcode_t op_eq(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 == op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_neq(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 != op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_geq(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 >= op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_leq(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 <= op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_gt(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 > op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_lt(vm_t * vm)
{
    errcode_t retval;
    int8_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 < op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

