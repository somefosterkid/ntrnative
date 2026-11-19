#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_90.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"

typedef struct UnkStruct_ov90_02258BD4_Heap_Alloc {
    u8 unk_0[0x8];
    s32 unk_8;
    NARC *unk_C;
    u8 unk_10;
    u8 unk_11[0x17];
    void *unk_28;
    u8 unk_2C[0x1A0];
} UnkStruct_ov90_02258BD4_Heap_Alloc;

struct UnkStruct_ov90_02258C38_ptr {
    u8 unk_0[0xC];
    NARC *unk_C;
    u8 unk_10;
    u8 unk_11[0x1B];
    SysTask *unk_2C;
    Sprite *unk_30;
    u8 unk_34;
};

s32 ov90_02258DD0(void *, s32, enum HeapID);

void *ov90_02258E54(void *, NARC *, s32, s32, s32, s32, s32, s32, enum HeapID);

void ov90_02258CF0(UnkStruct_ov114_0225CBF0 *param0, fx32 param1, fx32 param2, fx32 param3, int param4);
BOOL ov90_02258D4C(UnkStruct_ov114_0225CBF0 *param0, s32 param1);
void ov90_02258DB0(NNSG2dScreenData *param0, u32 param1);

void *ov90_02258BD4(s32 arg0, enum HeapID heapID) {
    UnkStruct_ov90_02258BD4_Heap_Alloc *v0;

    v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov90_02258BD4_Heap_Alloc));
    memset(v0, 0, sizeof(UnkStruct_ov90_02258BD4_Heap_Alloc));
    v0->unk_8 = arg0;
    v0->unk_C = NARC_New(NARC_a_1_9_8, heapID);
    ov90_02258DD0((void *)&v0->unk_10, 1, heapID);
    v0->unk_28 = ov90_02258E54((void *)&v0->unk_10, v0->unk_C, 3, 3, 2, 1, 0, 0x1388, heapID);
    return v0;
}

s32 ov90_02258E10(void *);

s32 ov90_0225BD08(void *);

void ov90_02258C38(UnkStruct_ov90_02258C38_ptr *ptr) {
    Sprite *sprite;
    SysTask *sysTask;

    sysTask = ptr->unk_2C;
    if (sysTask != NULL) {
        SysTask_Destroy(sysTask);
        ptr->unk_2C = NULL;
    }
    sprite = ptr->unk_30;
    if (sprite != NULL) {
        Sprite_Delete(sprite);
    }
    ov90_0225BD08((void *)&ptr->unk_34);
    ov90_02258E10((void *)&ptr->unk_10);
    NARC_Delete(ptr->unk_C);
    Heap_Free(ptr);
}

void ov90_02258C74(void *arg0) {
    SpriteTransfer_GetPlttOffset(SpriteResourceCollection_Find(*(GF_2DGfxResMan **)((u8 *)arg0 + 0x14), 0x1388), NNS_G2D_VRAM_TYPE_2DMAIN);
}

void ov90_0225BEE0(SysTask *task, void *data);

void ov90_02258C8C(void *arg0) {
    if (*(SysTask **)((u8 *)arg0 + 0x2C) != NULL) {
        GF_AssertFail();
    }
    *(SysTask **)((u8 *)arg0 + 0x2C) = SysTask_CreateOnMainQueue(ov90_0225BEE0, arg0, 0U);
    *(s16 *)((u8 *)arg0 + 4) = 1;
}

void ov90_0225C06C(SysTask *task, void *data);

void ov90_02258CB0(void *arg0) {
    if (*(SysTask **)((u8 *)arg0 + 0x2C) != NULL) {
        GF_AssertFail();
    }
    *(SysTask **)((u8 *)arg0 + 0x2C) = SysTask_CreateOnMainQueue(ov90_0225C06C, arg0, 0U);
    *(s16 *)((u8 *)arg0 + 4) = 1;
    PlaySE(0x5F1U);
}

s32 ov90_02258CE0(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 4) == 0) {
        return 1;
    }
    return 0;
}

void ov90_02258CF0(UnkStruct_ov114_0225CBF0 *param0, fx32 param1, fx32 param2, fx32 param3, int param4) {
    fx32 v0;
    fx32 v1;
    fx32 v2;
    fx32 v3;

    v2 = param2 - param1;
    v0 = (param4 * param4) << FX32_SHIFT;
    v1 = FX_Mul(param3, param4 * FX32_ONE);
    v1 = v2 - v1;
    v1 = FX_Mul(v1, 2 * FX32_ONE);
    v3 = FX_Div(v1, v0);

    param0->unk_00 = param1;
    param0->unk_04 = param1;
    param0->unk_08 = param3;
    param0->unk_0C = v3;
    param0->unk_10 = param4;
}

BOOL ov90_02258D4C(UnkStruct_ov114_0225CBF0 *param0, s32 param1) {
    fx32 v0;
    fx32 v1;
    fx32 v2;
    fx32 v3;
    BOOL v4;

    if (param1 >= param0->unk_10) {
        param1 = param0->unk_10;
        v4 = 1;
    } else {
        v4 = 0;
    }

    v3 = FX_Mul(param0->unk_08, param1 << FX32_SHIFT);
    v1 = (param1 * param1) << FX32_SHIFT;
    v2 = FX_Mul(param0->unk_0C, v1);
    v2 = FX_Div(v2, 2 * FX32_ONE);
    v0 = v3 + v2;

    param0->unk_00 = param0->unk_04 + v0;

    return v4;
}

void ov90_02258DB0(NNSG2dScreenData *param0, u32 param1) {
    int v0;
    u16 *v1;
    int v2 = param0->szByte / 2;
    v1 = (u16 *)param0->rawData;

    for (v0 = 0; v0 < v2; v0++) {
        v1[v0] += param1;
    }
}
