// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/vm.c: the script machine (its heap
// allocation passes "vm.c"). Black's link has it after version.c, as pokebw2 has; its head was in pass_power.c's range.
#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "system/vm.h"

void VM_Reset(VM *vm)
{
    vm->pc = NULL;
    vm->cmpResult = 0;
    vm->state = 0;
    vm->native = NULL;
    vm->stackPos = 0;
    MI_CpuFill8(vm->stack, 0, vm->param.stackSize * sizeof(uintptr_t));
    MI_CpuFill8(vm->work, 0, vm->param.workSize * 4);
    vm->verifier = NULL;
    vm->verifierArg = NULL;
}

VM *VM_Create(HeapID heapId, const VMInitParam *param)
{
    VM *vm;
    u32 workSize = param->workSize * 4;
    u32 stackSize = param->stackSize * sizeof(uintptr_t);

    vm = GFL_HeapAllocate(heapId, sizeof(VM) + workSize + stackSize, FALSE, "vm.c", 0x60);
    MI_CpuFill8(vm, 0, sizeof(VM) + workSize + stackSize);
    vm->param = *param;
    vm->work = (u32 *)(vm + 1);
    vm->stack = (uintptr_t *)(vm->work + param->workSize);
    return vm;
}

void VM_Free(VM *vm)
{
    GFL_HeapFree(vm);
}

void VM_ChangeEnv(VM *vm, void *env)
{
    VM_Reset(vm);
    vm->env = env;
}

void VM_LoadScript(VM *vm, const void *script)
{
    VM_Reset(vm);
    vm->pc = script;
    vm->state = 1;
}

void VM_Halt(VM *vm) {
    vm->state = 0;
    vm->pc = (u8 *)0x0;
}

BOOL VM_Run(VM *vm) {
    u16 cmd;
    VMVerifier verifier;
    VMCommand command;

    switch (vm->state) {
    case 0:
        return FALSE;
    case 2:
        if (vm->native != NULL) {
            if (vm->native(vm, vm->env) == TRUE) {
                vm->native = NULL;
                vm->state = 1;
            }
            return TRUE;
        }
        vm->state = 1;
        /* fall through */
    case 1:
        while (TRUE) {
            if (vm->pc == NULL) {
                vm->state = 0;
                return FALSE;
            }
            cmd = VM_Read16(vm);
            if (cmd >= vm->param.commandCount) {
                vm->state = 0;
                return FALSE;
            }
            verifier = vm->verifier;
            if (verifier != NULL && verifier(vm, vm->env, vm->verifierArg, cmd) == FALSE) {
                vm->state = 0;
                return FALSE;
            }
            command = vm->param.commands[cmd];
            if (command(vm, vm->env) == TRUE) {
                break;
            }
        }
        break;
    default:
        vm->state = 0;
        break;
    }
    return TRUE;
}


void *VM_GetEnv(VM *vm) {
    return vm->env;
}

void VM_SetCallbackVerifier(VM *vm, VMVerifier verifier, void *arg) {
    vm->verifier = verifier;
    vm->verifierArg = arg;
}

u16 VM_Read16(VM *vm) {
    u8 lo = VM_Read8(vm);
    u8 hi = VM_Read8(vm);

    return lo + (u16)(hi << 8);
}

u32 VM_Read32(VM *vm) {
    u8 b0 = VM_Read8(vm);
    u8 b1 = VM_Read8(vm);
    u8 b2 = VM_Read8(vm);
    u8 b3 = VM_Read8(vm);
    u32 value = 0;

    value += b3;
    value <<= 8;
    value += b2;
    value <<= 8;
    value += b1;
    value <<= 8;
    value += b0;
    return value;
}



void VM_StackPush(VM *vm, uintptr_t value) {
    if (vm->stackPos + 1 < vm->param.stackSize) {
        vm->stack[vm->stackPos] = value;
        vm->stackPos++;
    }
}


uintptr_t VM_StackPop(VM *vm) {
    if (vm->stackPos == 0) {
        return 0;
    }
    vm->stackPos--;
    return vm->stack[vm->stackPos];
}


void VM_Jump(VM *vm, const u8 *pc) {
    vm->pc = pc;
}

void VM_Call(VM *vm, const u8 *pc) {
    VM_StackPush(vm, (uintptr_t)vm->pc);
    vm->pc = pc;
}

void VM_Return(VM *vm) {
    vm->pc = (u8 *)VM_StackPop(vm);
}

void VM_SetNativeCallback(VM *vm, VMCommand native) {
    vm->state = 2;
    vm->native = native;
}
