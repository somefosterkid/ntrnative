// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_timer.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os.h>

#define REG_OS_TMCNT_H_E_MASK     REG_OS_TM0CNT_H_E_MASK
#define REG_OS_TMCNT_H_I_MASK     REG_OS_TM0CNT_H_I_MASK
#define REG_OS_TMCNT_H_CH_MASK    REG_OS_TM1CNT_H_CH_MASK

static u16 OSi_TimerReserved = 0;

#define OSi_TIMER_RESERVED_0   1
#define OSi_TIMER_RESERVED_1   2
#define OSi_TIMER_RESERVED_2   4
#define OSi_TIMER_RESERVED_3   8

void OSi_SetTimerReserved(int timerNum);

void OSi_SetTimerReserved (int timerNum)
{
    OSi_TimerReserved |= (1 << timerNum);
}
