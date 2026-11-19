#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_71_bg_slide_state.h"
#include "overlay_71_trade_pokemon_reveal_phase.h"
#include "overlay_71_trade3dmodel.h"

typedef struct UnkStruct_ov71_0224AFD4_Heap_Alloc {
    SysTask **unk_0;
    s32 unk_4;
    void *unk_8;
    u8 unk_C;
    u8 unk_D[0x3];
    s32 unk_10;
    u8 unk_14[0x4];
    s32 unk_18;
} UnkStruct_ov71_0224AFD4_Heap_Alloc;

struct TradePokemonRevealPhase;

void ov71_02247424(void *param);


void ov71_0224AFB8(SysTask *task);

s32 ov71_02247708(void *, s32);
void ov71_0224B0E8(SysTask *task, void *data);
void ov71_0224B084(void *arg0, s32 arg1, s32 arg2, s32 arg3, SysTask **arg4);

void ov71_0224AFB8(SysTask *task) {
    if (task) {
        BgSlideState *bgSlideState = SysTask_GetData(task);

        *(bgSlideState->taskHandle) = NULL;
        ov71_02247424(bgSlideState);
        SysTask_Destroy(task);
    }
}

s32 ov71_022476B4(void *, void *);

void ov71_0224B028(SysTask *task, void *param);

void ov71_0224AFD4(void *arg0, SysTask **arg1);

void ov71_0224AFD4(void *arg0, SysTask **arg1) {
    SysTask *sysTask;
    void *v1;
    s32 v2;
    UnkStruct_ov71_0224AFD4_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_0224AFD4_Heap_Alloc));
    if (v0 != NULL) {
        v0->unk_0 = arg1;
        v1 = *((void **)&((TradePokemonRevealPhase *)arg0)->model);
        v0->unk_8 = v1;
        ov71_022476B4(v1, (void *)&v0->unk_C);
        v2 = 0x32000 - v0->unk_10;
        v0->unk_18 = (s32)((s32)(v2 + ((u32)(v2 >> 3) >> 0x1C)) >> 4);
        v0->unk_4 = 0x10;
        sysTask = SysTask_CreateOnMainQueue(ov71_0224B028, v0, 0U);
        *arg1 = sysTask;
        if (sysTask == NULL) {
            Heap_Free(v0);
        }
    }
}

struct Trade3DModel;

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);

typedef struct ModelSlideState {
    SysTask **taskHandle;
    int framesRemaining;
    Trade3DModel *model;
    VecFx32 position;
    fx32 yStep;
} ModelSlideState;

void ov71_0224B068(SysTask *task);

void ov71_0224B028(SysTask *task, void *param) {
    ModelSlideState *msState = param;

    if (msState->framesRemaining) {
        msState->position.y += msState->yStep;
        ov71_022476C4(msState->model, &msState->position);
        msState->framesRemaining--;
    } else {
        msState->position.y = 204800;
        ov71_022476C4(msState->model, &msState->position);
        ov71_0224B068(task);
    }
}

void ov71_0224B068(SysTask *task) {
    if (task) {
        ModelSlideState *msState = SysTask_GetData(task);

        *(msState->taskHandle) = NULL;

        Heap_Free(msState);
        SysTask_Destroy(task);
    }
}

void ov71_0224B084(void *arg0, s32 arg1, s32 arg2, s32 arg3, SysTask **arg4) {
    s32 v1;
    void *v0;
    SysTask *sysTask;
    s32 v3;
    void *v2;

    v0 = arg0;
    v1 = arg2;
    v2 = Heap_Alloc(HEAP_ID_57, 0x18U);
    if (v2 != NULL) {
        *(SysTask ***)v2 = arg4;
        *(s32 *)((u8 *)v2 + 4) = (s32)(arg1 << 0xC);
        v3 = v1 << 0xC;
        *(s32 *)((u8 *)v2 + 0xC) = v3;
        *(s32 *)((u8 *)v2 + 8) = (s32)((s32)(v3 - *(s32 *)((u8 *)v2 + 4)) / arg3);
        *(s32 *)((u8 *)v2 + 0x10) = arg3;
        *(void **)((u8 *)v2 + 0x14) = v0;
        ov71_02247708(v0, arg1);
        sysTask = SysTask_CreateOnMainQueue(ov71_0224B0E8, v2, 0U);
        *arg4 = sysTask;
        if (sysTask == NULL) {
            Heap_Free(v2);
        }
    } else {
        *arg4 = NULL;
    }
}
