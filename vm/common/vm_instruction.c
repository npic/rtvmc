#include <string.h>
#include "vm.h"
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
    int32_t arg = (int32_t)read_32_bit((void *)&vm->proc_table[vm->PP].code[vm->proc_table[vm->PP].CP]);
    if (vm->proc_table[vm->PP].CP + sizeof(uint32_t) > VM_CODE_SIZE)
    {
        FAIL(MEM_OVERFLOW, "Process tried to read past the end of its code segment\n");
    }
    vm->proc_table[vm->PP].CP += sizeof(uint32_t);
    retval = stack_push(vm, &arg);
    CHECK_OK(retval, "Failed to push into the stack\n");
    return OK;
}

errcode_t op_bxor(vm_t * vm)
{
    errcode_t retval;
    int32_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 ^ op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_drop(vm_t * vm)
{
    errcode_t retval;
    int32_t val;
    retval = stack_pop(vm, &val);
    CHECK_OK(retval, "Failed to pop from the stack\n");
    return OK;
}

errcode_t op_add(vm_t * vm)
{
    errcode_t retval;
    int32_t op1, op2;
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
    int32_t op1, op2;
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
    int32_t op1, op2;
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
    int32_t op1, op2;
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
    int32_t op1, op2;
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
    int32_t op;
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
    int32_t op1, op2;
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
    int32_t op1, op2;
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
    int32_t op;
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
    int32_t op1, op2;
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
    int32_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 | op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_gpioin(vm_t * vm)
{
    errcode_t retval;
    uint8_t result = 0;
    int32_t result32;
    int32_t pin;
    retval = stack_pop(vm, &pin);
    CHECK_OK(retval, "Failed to pop output pin number from the stack\n");
    retval = gpio_get((uint32_t)pin, &result);
    CHECK_OK(retval, "Failed to read from the GPIO pin\n");
    result32 = (int32_t)result;
    retval = stack_push(vm, &result32);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_gpioout(vm_t * vm)
{
    errcode_t retval;
    int32_t pin;
    int32_t value = 0;
    retval = stack_pop(vm, &value);
    CHECK_OK(retval, "Failed to pop op from the stack\n");
    retval = stack_pop(vm, &pin);
    CHECK_OK(retval, "Failed to pop output pin number from the stack\n");
    retval = gpio_set((uint32_t)pin, (value ? 1 : 0));
    CHECK_OK(retval, "Failed to write to the GPIO pin\n");
    return OK;
}

/*
errcode_t op_uartin(vm_t * vm)
{
    errcode_t retval;
    char result = 0;
    int32_t result32;
    retval = uart_read(&vm->uart, &result);
    CHECK_OK(retval, "Failed to read from the UART queue\n");
    result32 = (int32_t)result;
    retval = stack_push(vm, &result32);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_uartout(vm_t * vm)
{
    errcode_t retval;
    int32_t c;
    retval = stack_pop(vm, &c);
    CHECK_OK(retval, "Failed to pop op from the stack\n");
    retval = uart_write(&vm->uart, (char)(c & 0x000000FF));
    CHECK_OK(retval, "Failed to write to the UART queue\n");
    return OK;
}
*/

errcode_t op_store(vm_t * vm)
{
    errcode_t retval;
    int32_t addr;
    int32_t val;
    retval = stack_pop(vm, &addr);
    CHECK_OK(retval, "Failed to pop address from the stack\n");
    retval = stack_pop(vm, &val);
    CHECK_OK(retval, "Failed to pop op from the stack\n");
    vm->proc_table[vm->PP].memory[(uint8_t)(addr & 0x000000FF)] = val;
    return OK;
}

errcode_t op_get(vm_t * vm)
{
    errcode_t retval;
    int32_t addr;
    int32_t val;
    retval = stack_pop(vm, &addr);
    CHECK_OK(retval, "Failed to pop address from the stack\n");
    val = vm->proc_table[vm->PP].memory[(uint8_t)(addr & 0x000000FF)];
    retval = stack_push(vm, &val);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

errcode_t op_jt(vm_t * vm)
{
    errcode_t retval;
    int32_t addr;
    int32_t cond;
    retval = stack_pop(vm, &addr);
    CHECK_OK(retval, "Failed to pop offset from the stack\n");
    retval = stack_pop(vm, &cond);
    CHECK_OK(retval, "Failed to pop condition from the stack\n");
    if (cond)
    {
        vm->proc_table[vm->PP].CP = addr;
        if (vm->proc_table[vm->PP].CP >= VM_CODE_SIZE)
        {
            FAIL(MEM_OVERFLOW, "Process tried to read past the end of its code segment\n");
        }
    }
    return OK;
}

errcode_t op_eq(vm_t * vm)
{
    errcode_t retval;
    int32_t op1, op2;
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
    int32_t op1, op2;
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
    int32_t op1, op2;
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
    int32_t op1, op2;
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
    int32_t op1, op2;
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
    int32_t op1, op2;
    retval = stack_pop(vm, &op2);
    CHECK_OK(retval, "Failed to pop op #2 from the stack\n");
    retval = stack_pop(vm, &op1);
    CHECK_OK(retval, "Failed to pop op #1 from the stack\n");
    op1 = op1 < op2;
    retval = stack_push(vm, &op1);
    CHECK_OK(retval, "Failed to push result into the stack\n");
    return OK;
}

