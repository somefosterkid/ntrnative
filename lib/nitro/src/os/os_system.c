// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_system.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os.h>
#include <nitro/code32.h>

// TwlSDK's DSi OS_GetTick control (twl/hw/ARM9/ioreg_SCFG.h), which libntr lacks: bit 0 runs the ARM9 at twice the DS's speed
#define reg_SCFG_CLK (*(REGType16v *)(HW_REG_BASE + 0x4004))
#define REG_SCFG_CLK_CPUSPD_MASK 0x0001

asm OSIntrMode OS_EnableInterrupts (void)
{
    mrs r0, cpsr
    bic r1, r0, #HW_PSR_IRQ_DISABLE
    msr cpsr_c, r1
    and r0, r0, #HW_PSR_IRQ_DISABLE
    bx lr
}

asm OSIntrMode OS_DisableInterrupts (void)
{
    mrs r0, cpsr
    orr r1, r0, #HW_PSR_IRQ_DISABLE
    msr cpsr_c, r1
    and r0, r0, #HW_PSR_IRQ_DISABLE
    bx lr
}

asm OSIntrMode OS_RestoreInterrupts (register OSIntrMode state)
{
    mrs r1, cpsr
    bic r2, r1, #HW_PSR_IRQ_DISABLE
    orr r2, r2, r0
    msr cpsr_c, r2
    and r0, r1, #HW_PSR_IRQ_DISABLE
    bx lr
}

asm OSIntrMode OS_DisableInterrupts_IrqAndFiq (void)
{
    mrs r0, cpsr
    orr r1, r0, #HW_PSR_IRQ_FIQ_DISABLE
    msr cpsr_c, r1
    and r0, r0, #HW_PSR_IRQ_FIQ_DISABLE
    bx lr
}

asm OSIntrMode OS_RestoreInterrupts_IrqAndFiq (OSIntrMode state)
{
    mrs r1, cpsr
    bic r2, r1, #HW_PSR_IRQ_FIQ_DISABLE
    orr r2, r2, r0
    msr cpsr_c, r2
    and r0, r1, #HW_PSR_IRQ_FIQ_DISABLE
    bx lr
}

asm OSIntrMode_Irq OS_GetCpsrIrq (void)
{
    mrs r0, cpsr
    and r0, r0, #HW_PSR_IRQ_DISABLE
    bx lr
}

asm OSProcMode OS_GetProcMode (void)
{
    mrs r0, cpsr
    and r0, r0, #0x1f
    bx lr
}

// libntr's OS_SpinWaitCpuCycles: the loop, 4 cycles a turn
asm void OS_SpinWaitCpuCycles (u32 cycle)
{
_1: subs r0, r0, #4
    bcs _1
    bx lr
}

// TwlSDK's: the cycles are the DS's, twice as many when the DSi runs the ARM9 at double speed
void OS_SpinWaitSysCycles (u32 cycle)
{
    cycle <<= (reg_SCFG_CLK & REG_SCFG_CLK_CPUSPD_MASK) == REG_SCFG_CLK_CPUSPD_MASK ? 2 : 1;
    if (cycle > 16) {
        OS_SpinWaitCpuCycles(cycle - 16);
    }
}

#include <nitro/codereset.h>

void OS_WaitInterrupt (BOOL clear, OSIrqMask irqFlags)
{
    OSIntrMode cpsrIrq = OS_DisableInterrupts();
    BOOL ime = OS_EnableIrq();

    if (clear) {
        (void)OS_ClearIrqCheckFlag(irqFlags);
    }

    while (!(OS_GetIrqCheckFlag() & irqFlags)) {
        OS_Halt();
        (void)OS_EnableInterrupts();
        (void)OS_DisableInterrupts();
    }

    (void)OS_ClearIrqCheckFlag(irqFlags);
    (void)OS_RestoreIrq(ime);
    (void)OS_RestoreInterrupts(cpsrIrq);
}

void OS_WaitVBlankIntr (void)
{
    SVC_WaitByLoop(1);
#if defined(SDK_ENABLE_ARM7_PRINT) && defined(SDK_ARM9)

    OS_PrintServer();
#endif
    OS_WaitIrq(TRUE, OS_IE_V_BLANK);
}
