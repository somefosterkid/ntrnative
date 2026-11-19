#include "global.h"
struct TradeSendPhase;

#include "sys_task.h"
#include "heap.h"
#include "sys_task_api.h"
#include "overlay_71_trade_send_phase.h"
#include "overlay_71_trade_receive_phase.h"

typedef struct UnkStruct_ov71_0224817C_Heap_Alloc {
    void *unk_0;
    void *unk_4;
    u8 unk_8;
    u8 unk_9[0xB];
    u8 unk_14;
    u8 unk_15[0x7];
    s32 unk_1C;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    s16 unk_2C;
    u8 unk_2E[0x2];
} UnkStruct_ov71_0224817C_Heap_Alloc;

struct TradeReceivePhase;

typedef struct BrightnessAnimState {
    TradeReceivePhase *phase;
    fx32 currentValue;
    fx32 targetValue;
    fx32 step;
    int framesRemaining;
} BrightnessAnimState;

void ov71_02248120(TradeReceivePhase *trPhase);

void ov71_02248140(SysTask *unused, void *param);

s32 ov71_022476B4(void *, void *);
s32 ov71_022476D4(void *, void *);
void ov71_0224820C(SysTask *task, void *data);
void ov71_0224817C(struct TradeSendPhase *arg0);
s32 ov71_022481C8(struct TradeSendPhase *arg0);
s32 ov71_022481D8(struct TradeSendPhase *arg0);
void ov71_022481EC(struct TradeSendPhase *arg0);

void ov71_02248140(SysTask *unused, void *param) {
    BrightnessAnimState *baState = param;
    int brightness;

    if (--(baState->framesRemaining) > 0) {
        baState->currentValue += baState->step;
        brightness = baState->currentValue >> 12;
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, brightness);
    } else {
        brightness = baState->targetValue >> 12;
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, brightness);
        ov71_02248120(baState->phase);
    }
}

void ov71_0224817C(TradeSendPhase *arg0) {
    void *v1;
    UnkStruct_ov71_0224817C_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_0224817C_Heap_Alloc));
    if (v0 != NULL) {
        v0->unk_0 = arg0;
        v1 = *((void **)&arg0->model);
        v0->unk_4 = v1;
        ov71_022476B4(v1, (void *)&v0->unk_8);
        ov71_022476D4(v0->unk_4, (void *)&v0->unk_14);
        v0->unk_1C = 0x2F60;
        v0->unk_20 = 0;
        v0->unk_2C = 0;
        v0->unk_24 = 0;
        v0->unk_28 = 0;
        arg0->task1 = SysTask_CreateOnMainQueue(ov71_0224820C, v0, 0U);
    }
}

s32 ov71_022481C8(TradeSendPhase *arg0) {
    if (*((s32 *)&arg0->task1) == 0) {
        return 1;
    }
    return 0;
}

s32 ov71_022481D8(TradeSendPhase *arg0) {
    SysTask *sysTask;

    sysTask = arg0->task1;
    if (sysTask != NULL) {
        return *(s32 *)((u8 *)(SysTask_GetData(sysTask)) + 0x20);
    }
    return 0;
}

void ov71_022481EC(TradeSendPhase *arg0) {
    SysTask *sysTask;

    sysTask = arg0->task1;
    if (sysTask != NULL) {
        Heap_Free(SysTask_GetData(sysTask));
        SysTask_Destroy(arg0->task1);
        arg0->task1 = NULL;
    }
}
