// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/vm.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_VM_H
#define BWPORT_SYSTEM_VM_H

#include "types.h"
#include "stddef.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// A script virtual machine, which runs the field event scripts, the trainer AI, the battle move animations and the
// musical scripts, each with its own commands. A script is a sequence of 16-bit command IDs, each followed by its
// arguments. The command reads its arguments from the script, and returns TRUE to make VM_Run return, or FALSE to
// run the next command.

typedef BOOL (*VMCommand)(VM *vm, void *env);

// Black's has no extra commands: Black 2's adds extraCommands, extraCommandCount and extraCommandStart
typedef struct {
    // The number of stack slots, each a uintptr_t (a script value or a return address)
    u16 stackSize;
    // The number of 32-bit registers in the work (VM_Create's workSize * 4 bytes), which the commands set from the
    // script's bytes: values, never pointers, so a table's count is right on a PC target too
    u16 workSize;
    const VMCommand *commands;
    u32 commandCount;
} VMInitParam;

// Black 2's, not checked against Black
// Called before each command. Returning FALSE stops the script
typedef BOOL (*VMVerifier)(VM *vm, void *env, void *arg, u16 cmd);

// Black's is 0x30 bytes, followed by the work area and the stack in the same block
struct VM {
    VMInitParam param;
    u8 stackPos;
    u8 state;
    // The result of the last comparison: less, equal or greater than (0, 1, 2)
    u8 cmpResult;
    VMCommand native;
    const u8 *pc;
    // Script values and the return addresses of VM_Call
    uintptr_t *stack;
    u32 *work;
    void *env;
    // VM_SetCallbackVerifier's
    VMVerifier verifier;
    void *verifierArg;
    // The script data the machine runs, loaded by FieldScript_LoadData
    void *unk_2c;
};

// Stops the machine and clears its stack and work
void VM_Reset(VM *vm);
VM *VM_Create(HeapID heapId, const VMInitParam *param);
void VM_Free(VM *vm);
void VM_ChangeEnv(VM *vm, void *env);
void VM_LoadScript(VM *vm, const void *script);
void VM_Halt(VM *vm);
BOOL VM_Run(VM *vm);
u16 VM_Read16(VM *vm);

static inline u8 VM_Read8(VM *vm) {
    u8 value = *vm->pc;

    vm->pc++;
    return value;
}
// Makes VM_Run call native until it returns TRUE, before running the next command
void VM_SetNativeCallback(VM *vm, VMCommand native);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void *VM_GetEnv(VM *vm);
void VM_SetCallbackVerifier(VM *vm, VMVerifier verifier, void *arg);
u32 VM_Read32(VM *vm);
void VM_StackPush(VM *vm, uintptr_t value);
uintptr_t VM_StackPop(VM *vm);
void VM_Return(VM *vm);
void VM_Jump(VM *vm, const u8 *pc);
void VM_Call(VM *vm, const u8 *pc);

#endif // BWPORT_SYSTEM_VM_H
