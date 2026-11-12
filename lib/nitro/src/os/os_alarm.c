// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/os/src/os_alarm.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/os.h>

#define OSi_ALARM_TIMERCONTROL    (REG_OS_TM0CNT_H_E_MASK | REG_OS_TM0CNT_H_I_MASK | OS_TIMER_PRESCALER_64)
#define OSi_ALARM_TIMER            OS_TIMER_1
#define OSi_ALARM_IE_TIMER         OS_IE_TIMER1

static u16 OSi_UseAlarm = FALSE;

static struct OSiAlarmQueue OSi_AlarmQueue;

u16 OSi_IsTimerReserved(int timerNum);
void OSi_SetTimerReserved(int timerNum);
void OSi_UnsetTimerReserved(int timerNum);
static void OSi_SetTimer(OSAlarm * alarm);
static void OSi_InsertAlarm(OSAlarm * alarm, OSTick fire);
static void OSi_AlarmHandler(void * arg);
static void OSi_ArrangeTimer(void);

static void OSi_SetTimer (OSAlarm * alarm)
{
    s64 delta;
    OSTick tick = OS_GetTick();
    u16 timerCount;

    OS_SetTimerControl(OSi_ALARM_TIMER, 0);
    delta = (s64)(alarm->fire - tick);
    OSi_EnterTimerCallback(OSi_ALARM_TIMER, OSi_AlarmHandler, NULL);

    if (delta < 0) {
        timerCount = (u16) ~1;
    } else if (delta < 0x10000)   {
        timerCount = (u16)(~delta);
    } else {
        timerCount = 0;
    }

    OS_SetTimerCount((OSTimer)OSi_ALARM_TIMER, timerCount);
    OS_SetTimerControl(OSi_ALARM_TIMER, (u16)OSi_ALARM_TIMERCONTROL);

    (void)OS_EnableIrqMask(OSi_ALARM_IE_TIMER);
}

void OS_InitAlarm (void)
{
    if (!OSi_UseAlarm) {
        OSi_UseAlarm = TRUE;

        SDK_ASSERTMSG(OS_IsTickAvailable(), "OS_InitAlarm: alarm system needs of tick system.");
        SDK_ASSERT(!OSi_IsTimerReserved(OSi_ALARM_TIMER));

        OSi_SetTimerReserved(OSi_ALARM_TIMER);

        OSi_AlarmQueue.head = NULL;
        OSi_AlarmQueue.tail = NULL;

        (void)OS_DisableIrqMask(OSi_ALARM_IE_TIMER);
    }
}

BOOL OS_IsAlarmAvailable (void)
{
    return OSi_UseAlarm;
}

void OS_CreateAlarm (OSAlarm * alarm)
{
    SDK_ASSERT(OSi_UseAlarm);
    SDK_ASSERT(alarm);

    alarm->handler = 0;
    alarm->tag = 0;
}

static void OSi_InsertAlarm (OSAlarm * alarm, OSTick fire)
{
    OSAlarm * prev;
    OSAlarm * next;

    if (alarm->period > 0) {
        OSTick tick = OS_GetTick();

        fire = alarm->start;
        if (alarm->start < tick) {
            fire += alarm->period * ((tick - alarm->start) / alarm->period + 1);
        }
    }

    alarm->fire = fire;

    for (next = OSi_AlarmQueue.head; next; next = next->next) {

        if ((s64)(fire - next->fire) >= 0) {
            continue;
        }

        alarm->prev = next->prev;
        next->prev = alarm;
        alarm->next = next;
        prev = alarm->prev;

        if (prev) {
            prev->next = alarm;
        } else {
            OSi_AlarmQueue.head = alarm;
            OSi_SetTimer(alarm);
        }

        return;
    }

    alarm->next = 0;
    prev = OSi_AlarmQueue.tail;
    OSi_AlarmQueue.tail = alarm;
    alarm->prev = prev;

    if (prev) {
        prev->next = alarm;
    } else {
        OSi_AlarmQueue.head = OSi_AlarmQueue.tail = alarm;
        OSi_SetTimer(alarm);
    }
}

void OS_SetAlarm (OSAlarm * alarm, OSTick tick, OSAlarmHandler handler, void * arg)
{
    OSIntrMode enabled;

    SDK_ASSERT(OSi_UseAlarm);
    SDK_ASSERTMSG(handler, "OS_SetAlarm: handler must not be NULL.");

    if (!alarm || alarm->handler) {
        OS_Panic("");
    }

    enabled = OS_DisableInterrupts();

    alarm->period = 0;
    alarm->handler = handler;
    alarm->arg = arg;

    OSi_InsertAlarm(alarm, OS_GetTick() + tick);

    (void)OS_RestoreInterrupts(enabled);

}

void OS_CancelAlarm (OSAlarm * alarm)
{
    OSAlarm * next;
    u32 enabled;

    SDK_ASSERT(OSi_UseAlarm);
    SDK_ASSERT(alarm);

    enabled = OS_DisableInterrupts();

    if (alarm->handler == NULL) {
        (void)OS_RestoreInterrupts(enabled);
        return;
    }

    next = alarm->next;

    if (next == NULL) {
        OSi_AlarmQueue.tail = alarm->prev;
    } else {
        next->prev = alarm->prev;
    }

    if (alarm->prev) {
        alarm->prev->next = next;
    } else {
        OSi_AlarmQueue.head = next;
        if (next) {
            OSi_SetTimer(next);
        }
    }

    alarm->handler = NULL;
    alarm->period = 0;

    (void)OS_RestoreInterrupts(enabled);
}

#include <nitro/code32.h>

#ifndef __MWERKS__
// C for a PC build (MWCC, which defines __MWERKS__, compiles the asm under #else)
void OSi_AlarmHandler(void *arg)
{
    (void)arg;
    OSi_ArrangeTimer();
}
#else
asm void OSi_AlarmHandler (void * arg)
{
    stmfd sp !, {r0, lr}
    bl OSi_ArrangeTimer
    ldmfd sp !, {r0, lr}
    bx lr
}
#endif

#include <nitro/codereset.h>

static void OSi_ArrangeTimer (void)
{
    OSTick tick;
    OSAlarm * alarm;
    OSAlarm * next;
    OSAlarmHandler handler;

    OS_SetTimerControl(OSi_ALARM_TIMER, 0);
    (void)OS_DisableIrqMask(OSi_ALARM_IE_TIMER);
    OS_SetIrqCheckFlag(OSi_ALARM_IE_TIMER);

    tick = OS_GetTick();
    alarm = OSi_AlarmQueue.head;

    if (alarm == NULL) {
        return;
    }

    if (tick < alarm->fire) {
        OSi_SetTimer(alarm);
        return;
    }

    next = alarm->next;
    OSi_AlarmQueue.head = next;

    if (next == NULL) {
        OSi_AlarmQueue.tail = NULL;
    } else {
        next->prev = NULL;
    }

    handler = alarm->handler;

    if (alarm->period == 0) {
        alarm->handler = NULL;
    }

    if (handler) {
        (handler) (alarm->arg);
    }

    if (alarm->period > 0) {
        alarm->handler = handler;
        OSi_InsertAlarm(alarm, 0);
    }

    if (OSi_AlarmQueue.head) {
        OSi_SetTimer(OSi_AlarmQueue.head);
    }
}
