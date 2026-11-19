#include "global.h"
#include "constants/heap.h"
#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "field_types_def.h"
#include "map_matrix.h"
#include "options.h"
#include "overlay_manager.h"
#include "overworld_anim_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"

s32 ov02_0224A6D0(void *arg0);

s32 ov02_02249968(HMCutIn *cutIn);

BOOL ov02_0224997C(HMCutIn *cutIn);

void ov02_0224A32C(HMCutIn *cutIn);

extern s32 (*ov02_02253588[23])(HMCutIn *);

void ov02_02249984(SysTask *task, void *param);

extern s32 (*ov02_022534F0[12])(HMCutIn *);

void ov02_022499B8(SysTask *cutInTask, void *hmCutInPtr);

s32 ov02_02249968(HMCutIn *cutIn) {
    ov02_0224A6D0(cutIn);
    cutIn->state += 1;
    return 0;
}

BOOL ov02_0224997C(HMCutIn *cutIn) {
    cutIn->isFinished = 1;
    return 0;
}

void ov02_02249984(SysTask *task, void *param) {
    HMCutIn *cutIn = param;
    s32 iVar1;

    do {
        iVar1 = ov02_02253588[cutIn->state](cutIn);
    } while (iVar1 == 1);
    if (cutIn->updateSprites == 1) {
        if (cutIn->animManagerList != NULL) {
            sub_02068BAC(cutIn->animManagerList);
        }
        ov02_0224A32C(cutIn);
    }
    return;
}

void ov02_022499B8(SysTask *cutInTask, void *hmCutInPtr) {
    HMCutIn *cutIn = hmCutInPtr;
    s32 iVar1;

    do {
        iVar1 = ov02_022534F0[cutIn->state](cutIn);
    } while (iVar1 == 1);
    if (cutIn->updateSprites == 1) {
        if (cutIn->animManagerList != NULL) {
            sub_02068BAC(cutIn->animManagerList);
        }
        ov02_0224A32C(cutIn);
    }
    return;
}
