#ifndef VM_INSTRUCTION_H
#define VM_INSTRUCTION_H

#include "config.h"
#include "error.h"
#include "vm.h"

#define OPC_STOP 0x00
#define OPC_NOP 0x01
#define OPC_PUSH 0x02
#define OPC_BXOR 0x03
#define OPC_DROP 0x04
#define OPC_ADD 0x05
#define OPC_SUB 0x06
#define OPC_MUL 0x07
#define OPC_DIV 0x08
#define OPC_MOD 0x09
#define OPC_NOT 0x0A
#define OPC_AND 0x0B
#define OPC_OR 0x0C
#define OPC_BNOT 0x0D
#define OPC_BAND 0x0E
#define OPC_BOR 0x0F
#define OPC_GPIOIN 0x10
#define OPC_GPIOOUT 0x11
/*
#define OPC_UARTIN 0x10
#define OPC_UARTOUT 0x11
*/
#define OPC_STORE 0x12
#define OPC_GET 0x13
#define OPC_JT 0x14
#define OPC_EQ 0x15
#define OPC_NEQ 0x16
#define OPC_GEQ 0x17
#define OPC_LEQ 0x18
#define OPC_GT 0x19
#define OPC_LT 0x1A

errcode_t op_stop(vm_t * vm);
errcode_t op_push(vm_t * vm);
errcode_t op_bxor(vm_t * vm);
errcode_t op_drop(vm_t * vm);
errcode_t op_add(vm_t * vm);
errcode_t op_sub(vm_t * vm);
errcode_t op_mul(vm_t * vm);
errcode_t op_div(vm_t * vm);
errcode_t op_mod(vm_t * vm);
errcode_t op_not(vm_t * vm);
errcode_t op_and(vm_t * vm);
errcode_t op_or(vm_t * vm);
errcode_t op_bnot(vm_t * vm);
errcode_t op_band(vm_t * vm);
errcode_t op_bor(vm_t * vm);
errcode_t op_gpioin(vm_t * vm);
errcode_t op_gpioout(vm_t * vm);
/*
errcode_t op_uartin(vm_t * vm);
errcode_t op_uartout(vm_t * vm);
*/
errcode_t op_store(vm_t * vm);
errcode_t op_get(vm_t * vm);
errcode_t op_jt(vm_t * vm);
errcode_t op_eq(vm_t * vm);
errcode_t op_neq(vm_t * vm);
errcode_t op_geq(vm_t * vm);
errcode_t op_leq(vm_t * vm);
errcode_t op_gt(vm_t * vm);
errcode_t op_lt(vm_t * vm);

#endif

