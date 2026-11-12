// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/snd/src/snd_alarm.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/snd/common/alarm.h>
#include <nitro/pxi.h>
#include <nitro/snd/common/work.h>

    typedef struct AlarmCallbackInfo {
        void (*func)(void *);
        void * arg;
        u8 id;
        u8 pad;
        u16 pad2;
    } AlarmCallbackInfo;

    static AlarmCallbackInfo sCallbackTable[SND_ALARM_NUM];

    void SND_AlarmInit (void)
    {
        int alarmNo;

        for (alarmNo = 0; alarmNo < SND_ALARM_NUM; alarmNo++) {
            AlarmCallbackInfo * info;

            info = &sCallbackTable[alarmNo];

            info->func = NULL;
            info->arg = NULL;
            info->id = 0;
        }
    }

    void SNDi_IncAlarmId (int alarmNo)
    {
        AlarmCallbackInfo * info;

        SDK_MINMAX_ASSERT(alarmNo, SND_ALARM_MIN, SND_ALARM_MAX);

        info = &sCallbackTable[alarmNo];
        info->id++;
    }

    u8 SNDi_SetAlarmHandler (int alarmNo, SNDAlarmHandler handler, void * arg)
    {
        AlarmCallbackInfo * info;

        SDK_MINMAX_ASSERT(alarmNo, SND_ALARM_MIN, SND_ALARM_MAX);

        info = &sCallbackTable[alarmNo];

        info->func = handler;
        info->arg = arg;
        info->id++;

        return info->id;
    }

    void SNDi_CallAlarmHandler (int msg)
    {
        AlarmCallbackInfo * info;
        int alarmNo = msg & 0xff;
        int id = (msg >> 8) & 0xff;

        SDK_MINMAX_ASSERT(alarmNo, SND_ALARM_MIN, SND_ALARM_MAX);

        info = &sCallbackTable[alarmNo];

        if (id == info->id) {
            if (info->func != NULL) {
                info->func(info->arg);
            }
        }
    }
