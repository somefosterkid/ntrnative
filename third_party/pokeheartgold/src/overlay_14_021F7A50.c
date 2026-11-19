#include "global.h"

#include "heap.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "field_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F7A50(FieldSystem *arg0);
void ov14_021F7AC4(UnkStruct_ov14_boxsys *arg0, s32 arg1, s32 arg2);

s32 ov14_021F7A50(FieldSystem *arg0) {
    s32 v0;

    GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x2BU);
    GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x2CU);
    GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x2DU);
    GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x25U);
    GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x26U);
    GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x27U);
    GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x28U);
    GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x29U);
    GridInputHandler_ClearEnabledFlag(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), 0x2AU);
    v0 = GridInputHandler_HandleInput_AllowHold(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C));
    GridInputHandler_SetAllEnabled(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C));
    return v0;
}

void ov14_021F7AC4(UnkStruct_ov14_boxsys *arg0, s32 target, s32 arg2) {
    u8 py[4];
    u8 v1;
    u8 v2;
    u8 v3;
    u8 v4;
    void *v0;

    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->unk_2C, target), &py[3], &py[2]);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->unk_2C, arg2), &py[1], &py[0]);
    v0 = Heap_Alloc(HEAP_ID_10, 8U);
    *(u32 *)((u8 *)v0 + 4) = (u32)(0x10 | (*(u32 *)((u8 *)v0 + 4) & 3));
    *(u8 *)v0 = (u8) py[3];
    *(u8 *)((u8 *)v0 + 1) = (u8) py[2];
    v3 = py[1];
    v1 = py[3];
    if ((u32)v1 >= (u32)v3) {
        *(u8 *)((u8 *)v0 + 2) = (u8)(v1 - v3);
        *(u32 *)((u8 *)v0 + 4) = (u32)(*(u32 *)((u8 *)v0 + 4) & ~1);
    } else {
        *(u8 *)((u8 *)v0 + 2) = (u8)(v3 - v1);
        *(u32 *)((u8 *)v0 + 4) = (u32)(1 | (*(u32 *)((u8 *)v0 + 4) & ~1));
    }
    v4 = py[0];
    v2 = py[2];
    if ((u32)v2 >= (u32)v4) {
        *(u8 *)((u8 *)v0 + 3) = (u8)(v2 - v4);
        *(u32 *)((u8 *)v0 + 4) = (u32)(*(u32 *)((u8 *)v0 + 4) & ~2);
    } else {
        *(u8 *)((u8 *)v0 + 3) = (u8)(v4 - v2);
        *(u32 *)((u8 *)v0 + 4) = (u32)(2 | *(u32 *)((u8 *)v0 + 4));
    }
    *(u8 *)((u8 *)v0 + 2) = (u8)((u32)((u32)(*(u8 *)((u8 *)v0 + 2) << 8) / (u32)((u32) * (u32 *)((u8 *)v0 + 4) >> 2)) >> 8);
    *(u8 *)((u8 *)v0 + 3) = (u8)((u32)((u32)(*(u8 *)((u8 *)v0 + 3) << 8) / (u32)((u32) * (u32 *)((u8 *)v0 + 4) >> 2)) >> 8);
    arg0->unk_C = v0;
}
