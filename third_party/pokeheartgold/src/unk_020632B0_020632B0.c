#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "heap.h"
#include "map_object.h"
#include "unk_02005D10.h"
#include "unk_0205FD20.h"

void _fadd(u32, u32);

fx32 _ffix(void);

u32 _fflt(s32);

void _fsub(u32, u32);

u8 _s32_div_f(u32, s32);

void sub_020632B0(LocalMapObject *param_1, s32 param_2, s32 param_3, s16 param_4, s8 param_5, s16 param_6, u16 param_7);

void sub_020632B0(LocalMapObject *param_1, s32 param_2, s32 param_3, s16 param_4, s8 param_5, s16 param_6, u16 param_7) {
    u8 uVar1;
    u8 *puVar2;
    void *dest;
    float fv;
    fx32 fVar4;
    fx32 fVar5;
    BOOL BVar6;
    puVar2 = sub_0205F3C0(param_1, 0xc);
    dest = Heap_AllocAtEnd(HEAP_ID_FIELD2, 0x1c);
    *((void **)(8 + puVar2)) = dest;
    MI_CpuFill8(dest, 0, 0x1c);
    *puVar2 = param_5;
    puVar2[1] = (u8)param_6;
    puVar2[3] = (u8)param_7;
    if (param_6 > 0) {
        fv = 0.5f + (float)(param_6 << 0xc);
    } else {
        fv = (float)(param_6 << 0xc) - 0.5f;
    }
    fVar4 = (fx32)fv;
    *((s8 *)((*((u8 **)(puVar2 + 8))) + 1)) = (s8)param_2;
    *((s8 *)((*((u8 **)(puVar2 + 8))) + 2)) = (s8)param_3;
    *((s8 *)((*((u8 **)(puVar2 + 8))) + 3)) = (s8)param_4;
    if ((param_2 * 0x10) > 0) {
        fv = 0.5f + (float)(param_2 << 0x10);
    } else {
        fv = (float)(param_2 << 0x10) - 0.5f;
    }
    fVar5 = (fx32)fv;
    fVar5 = FX_Div(fVar5, fVar4);
    *((fx32 *)((*((u8 **)(puVar2 + 8))) + 4)) = fVar5;
    if ((param_3 * 0x10) > 0) {
        fv = 0.5f + (float)(param_3 << 0x10);
    } else {
        fv = (float)(param_3 << 0x10) - 0.5f;
    }
    fVar5 = (fx32)fv;
    fVar5 = FX_Div(fVar5, fVar4);
    *((fx32 *)((*((u8 **)(puVar2 + 8))) + 8)) = fVar5;
    if ((param_4 * 0x10) > 0) {
        fv = 0.5f + (float)(param_4 << 0x10);
    } else {
        fv = (float)(param_4 << 0x10) - 0.5f;
    }
    fVar5 = (fx32)fv;
    fVar4 = FX_Div(fVar5, fVar4);
    *((fx32 *)((*((u8 **)(puVar2 + 8))) + 0xc)) = fVar4;
    uVar1 = 0xb4 / param_6;
    *(*((u8 **)(puVar2 + 8))) = uVar1;
    MapObject_CopyPositionVector(param_1, (VecFx32 *)((*((u8 **)(puVar2 + 8))) + 0x10));
    *((u32 *)(puVar2 + 4)) = *((u32 *)((*((u8 **)(puVar2 + 8))) + 0x14));
    sub_02060F78(param_1);
    MapObject_SetFlagsBits(param_1, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK16 | MAPOBJECTFLAG_START_MOVEMENT));
    MapObject_SetOrQueueFacing(param_1, (s32)((s8)param_5));
    sub_0205F328(param_1, (u32)param_7);
    MapObject_IncrementMovementStep(param_1);
    BVar6 = MapObject_CheckVisible(param_1);
    if (BVar6 == 0) {
        PlaySE(0x60a);
    }
    return;
}
