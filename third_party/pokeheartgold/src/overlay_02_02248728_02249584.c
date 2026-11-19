#include "global.h"

#include "overlay_02_hmcut_in.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"

void ov02_0224A32C(HMCutIn *cutIn);

extern s32 (*ov02_02253550[14])(HMCutIn *);

void ov02_02249584(SysTask *task, void *param);

void ov02_02249CF0(HMCutIn *);
void ov02_02249EC0(HMCutIn *);
s32 ov02_022495B8(HMCutIn *cutIn);
void ov02_02249F6C(HMCutIn *);
s32 ov02_022495D0(HMCutIn *cutIn);

void ov02_02249584(SysTask *task, void *param) {
    HMCutIn *cutIn = param;
    s32 iVar1;

    do {
        iVar1 = ov02_02253550[cutIn->state](cutIn);
    } while (iVar1 == 1);
    if (cutIn->updateSprites == 1) {
        if (cutIn->animManagerList != NULL) {
            sub_02068BAC(cutIn->animManagerList);
        }
        ov02_0224A32C(cutIn);
    }
    return;
}

s32 ov02_022495B8(HMCutIn *cutIn) {
    ov02_02249EC0(cutIn);
    ov02_02249CF0(cutIn);
    cutIn->state += 1;
    return 0;
}

s32 ov02_022495D0(HMCutIn *cutIn) {
    ov02_02249F6C(cutIn);
    ov02_02249CF0(cutIn);
    cutIn->state += 1;
    return 0;
}
