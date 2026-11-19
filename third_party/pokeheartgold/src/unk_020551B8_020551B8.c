#include "global.h"

#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "sound_02004A44.h"
#include "task.h"
#include "unk_020551B8.h"

s32 sub_020551B8(TaskManager *arg0);

s32 sub_020551B8(TaskManager *taskManager) {
    FieldSystem *fieldSystem;
    s32 v0;
    EncounterEffectTaskData *data;

    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    data = TaskManager_GetEnvironment(taskManager);
    v0 = data->taskState;
    switch (v0) {
    case 0:
        ov01_021EFB64(data->encEffectID);
        ov01_021EFC94(data->encEffectID, fieldSystem, &data->done);
        Sound_SetSceneAndPlayBGM(5U, (u16)data->battleBGM, 1);
        data->taskState = (s32)(data->taskState + 1);
    default:
        break;
    case 1:
        if (data->done == 1) {
            ov01_021EFC04(data->encEffectID);
            Heap_Free(data);
            return 1;
        }
        break;
    }
    return 0;
}
