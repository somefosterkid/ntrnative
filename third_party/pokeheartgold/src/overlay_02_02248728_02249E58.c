#include "global.h"

#include "sprite_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"

void ov02_02249E90(SysTask *task, void *data);

void ov02_02249E58(SysTask *task, void *hmCutInPtr);

void ov02_02249E90(SysTask *arg0, void *arg1);

void ov02_02249E58(SysTask *task, void *hmCutInPtr) {
    HMCutIn *cutIn = hmCutInPtr;
    SpriteResource *charResObj;

    charResObj = SpriteResourceCollection_Find(cutIn->charLocation, 0);
    if (cutIn->resourcesLoaded == 0) {
        SpriteTransfer_CreateCharTransferTask_AllocAtEnd(charResObj);
        SysTask_CreateOnVWaitQueue(ov02_02249E90, hmCutInPtr, 0x80U);
        cutIn->resourcesLoaded = (s32)(cutIn->resourcesLoaded + 1);
    }
}

void ov02_02249E90(SysTask *task, void *arg1) {
    HMCutIn *cutIn = arg1;
    SpriteResource *obj;

    obj = SpriteResourceCollection_Find(cutIn->charLocation, 0);
    if (cutIn->resourcesLoaded == 1) {
        sub_0200A740(obj);
        cutIn->resourceSourcesFreed = 1;
        SysTask_Destroy(task);
    }
}
