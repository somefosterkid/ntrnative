// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_tick.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os.h>

#define OSi_TICK_TIMERCONTROL  (REG_OS_TM0CNT_H_E_MASK | REG_OS_TM0CNT_H_I_MASK | OS_TIMER_PRESCALER_64)
#define OSi_TICK_TIMER         OS_TIMER_0
#define OSi_TICK_IE_TIMER      OS_IE_TIMER0

static u16 OSi_UseTick = FALSE;
vu64 OSi_TickCounter;
BOOL OSi_NeedResetTimer = FALSE;

extern u16 OSi_IsTimerReserved(int timerNum);
extern void OSi_SetTimerReserved(int timerNum);
static void OSi_CountUpTick(void);

void OS_InitTick (void)
{
    if (!OSi_UseTick) {
        OSi_UseTick = TRUE;

        SDK_ASSERT(!OSi_IsTimerReserved(OSi_TICK_TIMER));
        OSi_SetTimerReserved(OSi_TICK_TIMER);

        OSi_TickCounter = 0;
        OS_SetTimerControl(OSi_TICK_TIMER, 0);
        OS_SetTimerCount((OSTimer)OSi_TICK_TIMER, (u16)0);
        OS_SetTimerControl(OSi_TICK_TIMER, (u16)OSi_TICK_TIMERCONTROL);

        OS_SetIrqFunction(OSi_TICK_IE_TIMER, OSi_CountUpTick);

        (void)OS_EnableIrqMask(OSi_TICK_IE_TIMER);

        OSi_NeedResetTimer = FALSE;
    }
}

BOOL OS_IsTickAvailable (void)
{
    return OSi_UseTick;
}

static void OSi_CountUpTick (void)
{
    OSi_TickCounter++;

    if (OSi_NeedResetTimer) {
        OS_SetTimerControl(OSi_TICK_TIMER, 0);
        OS_SetTimerCount((OSTimer)OSi_TICK_TIMER, (u16)0);
        OS_SetTimerControl(OSi_TICK_TIMER, (u16)OSi_TICK_TIMERCONTROL);

        OSi_NeedResetTimer = FALSE;
    }

    OSi_EnterTimerCallback(OSi_TICK_TIMER, (void (*)(void *)) OSi_CountUpTick, 0);
}

OSTick OS_GetTick (void)
{
    vu16 countL;
    vu64 countH;

    OSIntrMode prev = OS_DisableInterrupts();
    SDK_ASSERT(OSi_UseTick);

    countL = *(REGType16 *)((u32)REG_TM0CNT_L_ADDR + OSi_TICK_TIMER * 4);
    countH = OSi_TickCounter & 0xffffffffffffULL;

    if (reg_OS_IF & OSi_TICK_IE_TIMER && !(countL & 0x8000)) 
    {
        countH++;
    }

    (void)OS_RestoreInterrupts(prev);

    return (countH << 16) | countL;
}

#pragma profile off

#pragma profile reset

u16 OS_GetTickLo (void)
{
    SDK_ASSERT(OSi_UseTick);
    return *(REGType16 *)((u32)REG_TM0CNT_L_ADDR + OSi_TICK_TIMER * 4);
}
