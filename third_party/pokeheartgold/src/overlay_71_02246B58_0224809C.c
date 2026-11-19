#include "global.h"
struct TradeReceivePhase;
struct TradeSendPhase;

#include "heap.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_71_trade_send_phase.h"
#include "overlay_71_trade_receive_phase.h"

typedef struct UnkStruct_ov71_022480C0_Heap_Alloc {
    void *unk_0;
    s32 unk_4;
    s32 unk_8;
    s32 unk_C;
    s32 unk_10;
} UnkStruct_ov71_022480C0_Heap_Alloc;

s32 ov71_022472FC(void *);

void ov71_0224809C(void *arg0);

void ov71_0224809C(void *arg0) {
    Sprite *sprite;
    s32 v0;

    v0 = 0;
    do {
        sprite = ((TradeSendPhase *)arg0)->sprites[v0];
        if (sprite != NULL) {
            Sprite_Delete(sprite);
        }
        v0 += 1;
    } while (v0 < 2);
    ov71_022472FC(arg0 + 0x44);
}

void ov71_02248140(SysTask *task, void *data);

void ov71_022480C0(struct TradeSendPhase *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov71_022480C0(TradeSendPhase *arg0, s32 brightness, s32 arg2, s32 arg3) {
    s32 v0;
    s32 v2;
    UnkStruct_ov71_022480C0_Heap_Alloc *v1;

    v0 = arg2;
    v1 = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_022480C0_Heap_Alloc));
    if (v1 != NULL) {
        v1->unk_0 = arg0;
        v1->unk_4 = (s32)(brightness << 0xC);
        v2 = v0 << 0xC;
        v1->unk_8 = v2;
        v1->unk_C = (s32)((s32)(v2 - v1->unk_4) / arg3);
        v1->unk_10 = arg3;
        arg0->task0 = SysTask_CreateOnVBlankQueue(ov71_02248140, v1, 0U);
        G2_SetBlendBrightness(1, brightness);
    }
}

s32 ov71_02248110(struct TradeSendPhase *arg0);

s32 ov71_02248110(TradeSendPhase *arg0) {
    if (*((s32 *)&arg0->task0) == 0) {
        return 1;
    }
    return 0;
}

s32 ov71_02247424();

void ov71_02248120(struct TradeReceivePhase *arg0);

void ov71_02248120(TradeReceivePhase *arg0) {
    SysTask *sysTask;

    sysTask = *((SysTask * *)&arg0->model);
    if (sysTask != NULL) {
        SysTask_GetData(sysTask);
        ov71_02247424();
        SysTask_Destroy(*((SysTask * *)&arg0->model));
        *((SysTask * *)&arg0->model) = NULL;
    }
}
