// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_context.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os.h>
#include <nitro/memorymap.h>

    #include <nitro/cp/context.h>

#include <nitro/code32.h>

asm void OS_InitContext (register OSContext * context, register u32 newpc, register u32 newsp)
{
    add newpc, newpc, #4
    str newpc, [context, #OS_CONTEXT_PC_PLUS4]
#ifdef SDK_CONTEXT_HAS_SP_SVC
    str newsp, [context, #OS_CONTEXT_SP_SVC]
    sub newsp, newsp, #HW_SVC_STACK_SIZE
#endif
    tst newsp, #4
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    bne @subne1
    b @subne2
@subne1:
    sub newsp, newsp, #4
@subne2:
#else
    subne newsp, newsp, #4
#endif
    str newsp, [context, #OS_CONTEXT_SP]
    ands r1, newpc, #1
#ifdef SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS
    bne @movne1
    b @movne2
@movne1:
    mov r1, #HW_PSR_SYS_MODE|HW_PSR_THUMB_STATE
@movne2:
    beq @moveq1
    b @moveq2
@moveq1:
    mov r1, #HW_PSR_SYS_MODE|HW_PSR_ARM_STATE
@moveq2:
#else
    movne r1, #HW_PSR_SYS_MODE | HW_PSR_THUMB_STATE
    moveq r1, #HW_PSR_SYS_MODE | HW_PSR_ARM_STATE
#endif
    str r1, [context, #OS_CONTEXT_CPSR]
    mov r1, #0
    str r1, [context, #OS_CONTEXT_R0]
    str r1, [context, #OS_CONTEXT_R1]
    str r1, [context, #OS_CONTEXT_R2]
    str r1, [context, #OS_CONTEXT_R3]
    str r1, [context, #OS_CONTEXT_R4]
    str r1, [context, #OS_CONTEXT_R5]
    str r1, [context, #OS_CONTEXT_R6]
    str r1, [context, #OS_CONTEXT_R7]
    str r1, [context, #OS_CONTEXT_R8]
    str r1, [context, #OS_CONTEXT_R9]
    str r1, [context, #OS_CONTEXT_R10]
    str r1, [context, #OS_CONTEXT_R11]
    str r1, [context, #OS_CONTEXT_R12]
    str r1, [context, #OS_CONTEXT_LR]
    bx lr
}

asm BOOL OS_SaveContext (register OSContext * context)
{
    stmfd sp !, { lr, r0 }
    add r0, r0, #OS_CONTEXT_CP_CONTEXT
    ldr r1, = CP_SaveContext
    blx r1
    ldmfd sp !, { lr, r0 }
    add r1, r0, #OS_CONTEXT_CPSR
    mrs r2, cpsr
    str r2, [r1], #OS_CONTEXT_R0 - OS_CONTEXT_CPSR
#ifdef SDK_CONTEXT_HAS_SP_SVC
    mov r0, #HW_PSR_SVC_MODE | HW_PSR_IRQ_DISABLE | HW_PSR_FIQ_DISABLE | HW_PSR_ARM_STATE
    msr cpsr_c, r0
    str sp, [r1, #OS_CONTEXT_SP_SVC - OS_CONTEXT_R0]
    msr cpsr_c, r2
#endif
    mov r0, #1
    stmia r1, {r0 - r14}
    add r0, pc, #8
    str r0, [r1, #OS_CONTEXT_PC_PLUS4 - OS_CONTEXT_R0]
    mov r0, #0
    bx lr
}

#define OFFSETOF(x, y) (int)(&(((x *)0)->y))

asm void OS_LoadContext(OSContext *context) {
    stmdb sp!, {r0, lr}
    add r0, r0, #0x48
    ldr r1, =CPi_RestoreContext
    blx r1
    ldmia sp!, {r0, lr}
    mrs r1, cpsr
    bic r1, r1, #0x1f
    orr r1, r1, #0xd3
    msr cpsr_c, r1
    ldr r1, [r0], #4
    msr spsr_fsxc, r1
    ldr sp, [r0, #0x40]
    ldr lr, [r0, #0x3c]
    ldmia r0, {r0-r12, sp, lr}^
    mov r0, r0
    stmdb sp!, {r0-r3, r12, lr}
    bl OS_IsRunOnTwl
    beq @done
    ldr r0, =0x4000280
@wait:
    ldr r1, [r0, #0]
    ands r1, r1, #0x8000
    bne @wait
@done:
    ldmia sp!, {r0-r3, r12, lr}
    subs pc, lr, #4
}

#include <nitro/codereset.h>
