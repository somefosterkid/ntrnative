// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_interrupt.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). TwlSDK 5 builds it as one object whose
// twl/ltdmain_begin.h block (OSi_EnterNDmaCallback's body) goes to the LTD autoload's .ltdmain 0x0273126c-0x027312b4
// (ltd_autoload_0's delinks.txt names it os_interrupt@ltd.c: tools/two_modules.py).
#include <nitro.h>

extern OSThreadQueue OSi_IrqThreadQueue;

void OS_InitIrqTable (void)
{
    OS_InitThreadQueue(&OSi_IrqThreadQueue);

}

void OS_SetIrqFunction (OSIrqMask intrBit, OSIrqFunction function)
{
    int i;
    OSIrqCallbackInfo * info;

    for (i = 0; i < OS_IRQ_TABLE_MAX; i++) {
        if (intrBit & 1) {
            info = NULL;

            if (REG_OS_IE_D0_SHIFT <= i && i <= REG_OS_IE_D3_SHIFT) {
                info = &OSi_IrqCallbackInfo[i - REG_OS_IE_D0_SHIFT];
            }
            else if (0x1c <= i && i <= 0x1f) {
                info = &OSi_IrqCallbackInfo[i - 0x18];
            }
            else if (REG_OS_IE_T0_SHIFT <= i && i <= REG_OS_IE_T3_SHIFT) {
                info = &OSi_IrqCallbackInfo[i - REG_OS_IE_T0_SHIFT + OSi_IRQCALLBACK_NO_TIMER0];
            }
            else {
                OS_IRQTable[i] = function;
            }

            if (info) {
                info->func = (void (*)(void *)) function;
                info->arg = 0;
                info->enable = TRUE;
            }
        }

        intrBit >>= 1;
    }
}

OSIrqFunction OS_GetIrqFunction (OSIrqMask intrBit)
{
    int i;
    OSIrqFunction * funcPtr = &OS_IRQTable[0];

    for (i = 0; i < OS_IRQ_TABLE_MAX; i++) {
        if (intrBit & 1) {

            if (REG_OS_IE_D0_SHIFT <= i && i <= REG_OS_IE_D3_SHIFT) {
                return (void (*)(void)) OSi_IrqCallbackInfo[i - REG_OS_IE_D0_SHIFT].func;
            } 
            else if (REG_OS_IE_T0_SHIFT <= i && i <= REG_OS_IE_T3_SHIFT)   {
                return (void (*)(void)) OSi_IrqCallbackInfo[i - REG_OS_IE_T0_SHIFT +
                                                            OSi_IRQCALLBACK_NO_TIMER0].func;
            }

            return *funcPtr;
        }

        intrBit >>= 1;
        funcPtr++;
    }

    return 0;
}

void OSi_EnterDmaCallback (u32 dmaNo, void (*callback)(void *), void * arg)
{
    OSIrqMask imask = (1UL << (REG_OS_IE_D0_SHIFT + dmaNo));

    OSi_IrqCallbackInfo[dmaNo].func = callback;
    OSi_IrqCallbackInfo[dmaNo].arg = arg;
    OSi_IrqCallbackInfo[dmaNo].enable = OS_EnableIrqMask(imask) & imask;
}

#include <twl/ltdmain_begin.h>
// TwlSDK 5's, which this file's SDK 4 headers lack: the new DMA channels' interrupt bits and callbacks
#define REG_OS_IE_ND0_SHIFT 28
#define OSi_IRQCALLBACK_NO_NDMA0 4

// TwlSDK's OSi_EnterNDmaCallback_ltdmain, in the LTD autoload's .ltdmain
void func_0273126c_unk(u32 dmaNo, void (*callback)(void *), void *arg)
{
    OSIrqMask imask = (1UL << (REG_OS_IE_ND0_SHIFT + dmaNo));

    OSi_IrqCallbackInfo[dmaNo + OSi_IRQCALLBACK_NO_NDMA0].func = callback;
    OSi_IrqCallbackInfo[dmaNo + OSi_IRQCALLBACK_NO_NDMA0].arg = arg;

    OSi_IrqCallbackInfo[dmaNo + OSi_IRQCALLBACK_NO_NDMA0].enable = OS_EnableIrqMask(imask) & imask;
}
#include <twl/ltdmain_end.h>

void func_020848ec(u32 ndmaNo, void (*callback)(void *), void *arg)
{
    func_0273126c_unk(ndmaNo, callback, arg);
}




void OSi_EnterTimerCallback (u32 timerNo, void (*callback)(void *), void * arg)
{
    OSIrqMask imask = (1UL << (REG_OS_IE_T0_SHIFT + timerNo));

    OSi_IrqCallbackInfo[OSi_IRQCALLBACK_NO_TIMER0 + timerNo].func = callback;
    OSi_IrqCallbackInfo[OSi_IRQCALLBACK_NO_TIMER0 + timerNo].arg = arg;

    (void)OS_EnableIrqMask(imask);
    OSi_IrqCallbackInfo[OSi_IRQCALLBACK_NO_TIMER0 + timerNo].enable = TRUE;
}

OSIrqMask OS_SetIrqMask (OSIrqMask intr)
{
    BOOL ime = OS_DisableIrq();
    OSIrqMask prep = reg_OS_IE;
    reg_OS_IE = intr;
    (void)OS_RestoreIrq(ime);
    return prep;
}

OSIrqMask OS_EnableIrqMask (OSIrqMask intr)
{
    BOOL ime = OS_DisableIrq();
    OSIrqMask prep = reg_OS_IE;
    reg_OS_IE = prep | intr;
    (void)OS_RestoreIrq(ime);
    return prep;
}

OSIrqMask OS_DisableIrqMask (OSIrqMask intr)
{
    BOOL ime = OS_DisableIrq();
    OSIrqMask prep = reg_OS_IE;
    reg_OS_IE = prep & ~intr;
    (void)OS_RestoreIrq(ime);
    return prep;
}

OSIrqMask OS_ResetRequestIrqMask (OSIrqMask intr)
{
    BOOL ime = OS_DisableIrq();
    OSIrqMask prep = reg_OS_IF;
    reg_OS_IF = intr;
    (void)OS_RestoreIrq(ime);
    return prep;
}

extern void SDK_IRQ_STACKSIZE(void);

    #define  OSi_IRQ_STACK_TOP                (HW_DTCM_SVC_STACK - ((s32)SDK_IRQ_STACKSIZE))
    #define  OSi_IRQ_STACK_BOTTOM             HW_DTCM_SVC_STACK

    #define  OSi_IRQ_STACK_CHECKNUM_BOTTOM     0xfddb597dUL
    #define  OSi_IRQ_STACK_CHECKNUM_TOP        0x7bf9dd5bUL
    #define  OSi_IRQ_STACK_CHECKNUM_WARN       0x597dfbd9UL

static u32 OSi_IrqStackWarningOffset = 0;

static char * OSi_CheckIrqStack_mesg[] = {
    "overflow", "about to overflow", "underflow"
};

void OS_SetIrqStackChecker (void)
{
    *(u32 *)(OSi_IRQ_STACK_BOTTOM - sizeof(u32)) = OSi_IRQ_STACK_CHECKNUM_BOTTOM;
    *(u32 *)(OSi_IRQ_STACK_TOP) = OSi_IRQ_STACK_CHECKNUM_TOP;
}
