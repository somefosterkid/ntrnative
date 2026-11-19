#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "pokemon_cry_duration.h"
#include "sound.h"
#include "sys_task.h"
#include "sys_task_api.h"

void sub_020068F8(void);

void sub_02006884(SysTask *task, void *arg);

void sub_02006838(u32 param_1, enum HeapID param_2);

void sub_02006838(u32 param_1, enum HeapID param_2) {
    SysTask **taskSlot;
    PokemonCryDurationParam *data;
    SysTask *task;

    taskSlot = GF_SdatGetAttrPtr(0x23);
    sub_020068F8();
    data = Heap_Alloc(param_2, sizeof(PokemonCryDurationParam));
    if (data == NULL) {
        GF_AssertFail();
        return;
    }
    memset(data, 0, sizeof(PokemonCryDurationParam));
    data->remainingFrames = param_1;
    task = SysTask_CreateOnMainQueue(sub_02006884, data, 0);
    data->task = task;
    *taskSlot = task;
    return;
}
