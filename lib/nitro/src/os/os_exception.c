// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_exception.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py), and the SDK's functions Black's link drops whose statics keep their
// places in its .bss: OSi_DebuggerExceptionHook's stack (0x0215129c-0x021512bc, ahead of OSi_ExContext, which
// func_02086ec8 and func_02086f48 read), with OS_EnableUserExceptionHandlerOnDebugger, the hook's only caller. The SDK's
// OS_SetUserExceptionHandler is left out: func_02086f48 reads OSi_UserExceptionHandlerArg, which keeps it.
#include <nitro/hw/common/armArch.h>
#include <nitro/memorymap.h>
#include <nitro/os.h>

    #define OSi_ExPrintf OS_Printf

asm void OSi_ExceptionHandler(void);
asm void OSi_GetAndDisplayContext(void);

// TwlSDK's has no debug words after exinfo (Black's OSi_SetExContext stores none; 0x70 bytes)
typedef struct {
    OSContext context;
    u32 cp15;
    u32 spsr;
    u32 exinfo;
} OSiExContext;

static OSiExContext OSi_ExContext;
static OSExceptionHandler OSi_UserExceptionHandler;
static void * OSi_UserExceptionHandlerArg;
// the SDK's OSi_DebuggerHandler
static void * data_02151290 = NULL;

#include <nitro/code32.h>

#include <nitro/codereset.h>


    #include <nitro/code32.h>

    static u32 OSi_ExceptionHookStack[8];

    #include <nitro/codereset.h>

#include <nitro/code32.h>


#define OSi_DEBUGGER_HANDLER_START 0x02600000
#define OSi_DEBUGGER_HANDLER_END 0x02800000
#define HW_EXCP_VECTOR_BUF_FOR_DEBUGGER 0x02fffd9c // Black's: the main memory vector with bit 23 set
#define HW_EXCP_VECTOR_MAIN_MASK 0xff7fffff
void func_02086dd8(void) {
    u32 *vector = (u32 *)HW_EXCP_VECTOR_BUF_FOR_DEBUGGER;
    u32 handler = *vector;

    if (handler < OSi_DEBUGGER_HANDLER_START || handler >= OSi_DEBUGGER_HANDLER_END) {
        handler = 0;
    }
    data_02151290 = (void *)handler;
    if (data_02151290 == NULL) {
        *vector = (u32)OSi_ExceptionHandler;
        if (!OS_IsRunOnTwl()) {
            *(u32 *)((u32)vector & HW_EXCP_VECTOR_MAIN_MASK) = (u32)OSi_ExceptionHandler;
        }
    }
    OSi_UserExceptionHandler = NULL;
}

// Black's link drops it with OS_EnableUserExceptionHandlerOnDebugger, which nothing calls; OSi_ExceptionHookStack, which
// only it uses, keeps its place in the file's .bss before OSi_ExContext
asm static void OSi_DebuggerExceptionHook(void)
{
    ldr r12, =OSi_ExceptionHookStack
    stmia r12, {r0-r4, sp, lr}
    mrs r4, CPSR
    mrs r0, CPSR
    and r0, r0, #0x1f
    teq r0, #0x17
    beq user_exception
    teq r0, #0x1b
    bne user_exception_end
is_und:
    bic r0, sp, #1
    ldr r1, [r0, #4]
    ldr r0, [r0, #12]
    tst r1, #0x20
    beq is_und_arm
is_und_thumb:
    bic r0, r0, #1
    ldrh r0, [r0, #-2]
    ldr r1, =0x0000EFFF
    cmp r0, r1
    beq user_exception_end
    ldr r1, =0x0000DEFE
    cmp r0, r1
    beq user_exception_end
    ldr r1, =0x0000BE00
    cmp r0, r1
    beq user_exception_end
    b user_exception
is_und_arm:
    bic r0, r0, #3
    ldr r0, [r0, #-4]
    ldr r1, =0xE7FFFFFF
    cmp r0, r1
    beq user_exception_end
    ldr r1, =0xE7FDDEFE
    cmp r0, r1
    beq user_exception_end
    ldr r1, =0xE6000010
    cmp r0, r1
    beq user_exception_end
    b user_exception
user_exception:
    ldmia r12, {r0-r1}
    ldr r12, =HW_ITCM_END
    stmfd r12!, {r0-r3, sp, lr}
    and r0, sp, #1
    mov sp, r12
    bl OSi_GetAndDisplayContext
    ldmfd sp!, {r0-r3, r12, lr}
    mov sp, r12
user_exception_end:
    msr CPSR_cxsf, r4
    ldr r12, =OSi_ExceptionHookStack
    ldmia r12, {r0-r4, sp, lr}
    ldr r12, =data_02151290
    ldr r12, [r12]
    cmp r12, #0
    bxne r12
    bx lr
}

#include <nitro/codereset.h>

// Black's link drops it (nothing calls it)
void OS_EnableUserExceptionHandlerOnDebugger(void)
{
    if (data_02151290) {
        *(u32 *)(HW_EXCP_VECTOR_BUF_FOR_DEBUGGER) = (u32)OSi_DebuggerExceptionHook;
    }
}

#include <nitro/code32.h>


asm void OSi_ExceptionHandler(void)
{
    ldr ip, =data_02151290
    ldr ip, [ip, #0x0]
    cmp ip, #0x0
    movne lr, pc
    bxne ip
    ldr ip, =0x2000000
    stmdb ip!, {r0, r1, r2, r3, sp, lr}
    and r0, sp, #0x1
    mov sp, ip
    mrs r1, cpsr
    and r1, r1, #0x1f
    teq r1, #0x17
    bne @10
    bl OSi_GetAndDisplayContext
    b @usr_return
@10:
    teq r1, #0x1b
    bne @usr_return
    bl OSi_GetAndDisplayContext
@usr_return:
    ldr ip, =data_02151290
    ldr ip, [ip, #0x0]
    cmp ip, #0x0
@1:
    beq @1
@2:
    mov r0, r0
    b @2
    ldmia sp!, {r0, r1, r2, r3, ip, lr}
    mov sp, ip
    bx lr
}

asm void func_02086ec8(void);
void func_02086f48(void);
asm void OSi_GetAndDisplayContext(void)
{
    stmdb sp!, {r0, lr}
    bl func_02086ec8
    bl func_02086f48
    ldmia sp!, {r0, lr}
    bx lr
}

asm void func_02086ec8(void)
{
    ldr r1, =OSi_ExContext
    str r0, [r1, #0x6c]
    ldr r0, [ip, #0x0]
    str r0, [r1, #0x4]
    ldr r0, [ip, #0x4]
    str r0, [r1, #0x8]
    ldr r0, [ip, #0x8]
    str r0, [r1, #0xc]
    ldr r0, [ip, #0xc]
    str r0, [r1, #0x10]
    ldr r2, [ip, #0x10]
    bic r2, r2, #0x1
    add r0, r1, #0x14
    stmia r0, {r4, r5, r6, r7, r8, r9, r10, r11}
    ldr r0, [r2, #0x0]
    str r0, [r1, #0x64]
    ldr r3, [r2, #0x4]
    str r3, [r1, #0x0]
    ldr r0, [r2, #0x8]
    str r0, [r1, #0x34]
    ldr r0, [r2, #0xc]
    str r0, [r1, #0x40]
    mrs r0, cpsr
    orr r3, r3, #0x80
    bic r3, r3, #0x20
    msr cpsr_fsxc, r3
    str sp, [r1, #0x38]
    str lr, [r1, #0x3c]
    mrs r2, spsr
    msr cpsr_fsxc, r0
    bx lr
}

static void func_02086f48(void)
{
    if (OSi_UserExceptionHandler == NULL) {
        return;
    }
    asm {
        mrs r2, cpsr
        mov r0, sp
        ldr r1, =0x9f
        msr cpsr_fsxc, r1
        mov r1, sp
        mov sp, r0
        stmdb sp!, {r1, r2}
        bl OS_EnableProtectionUnit
        ldr r0, =OSi_ExContext
        ldr r1, =OSi_UserExceptionHandlerArg
        ldr r1, [r1, #0]
        ldr ip, =OSi_UserExceptionHandler
        ldr ip, [ip, #0]
        ldr lr, =@return
        bx ip
    @return:
        bl OS_DisableProtectionUnit
        ldmia sp!, {r1, r2}
        mov sp, r1
        msr cpsr_fsxc, r2
    }
}




#include <nitro/codereset.h>
