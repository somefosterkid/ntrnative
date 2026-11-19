#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "field_system.h"
#include "field_types_def.h"
#include "gf_rtc.h"
#include "heap.h"
#include "task.h"
#include "overlay_104_105_args.h"
#include "unk_02055418.h"

BOOL sub_02097A48(TaskManager *param_1);

void sub_020979EC(FieldSystem *param_1, u8 param_2, void *param_3);

void sub_020979EC(FieldSystem *param_1, u8 param_2, void *param_3) {
    Ov105AppArgs *env;
    TIMEOFDAY TVar1;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(Ov105AppArgs));
    env->lights = param_3;
    env->mode = param_2;
    TVar1 = Field_GetTimeOfDay(param_1);
    switch (TVar1) {
    case RTC_TIMEOFDAY_MORN:
    case RTC_TIMEOFDAY_DAY:
        env->timeOfDay = 0;
        break;
    case RTC_TIMEOFDAY_EVE:
    case RTC_TIMEOFDAY_NITE:
    case RTC_TIMEOFDAY_LATE:
        env->timeOfDay = 1;
        break;
    default:
        GF_AssertFail();
        env->timeOfDay = 0;
    }
    TaskManager_Call(param_1->taskman, sub_02097A48, env);
    return;
}
