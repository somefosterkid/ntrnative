#include "global.h"
struct UnkStruct_ov62_02233310;
struct UnkStruct_0208C06C;

#include "sprite_system.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_02233310.h"

void ov40_0222F858(struct UnkStruct_ov62_02233310 *arg0, s16 arg1, s16 arg2);

void ov40_0222F858(UnkStruct_ov62_02233310 *arg0, s16 y, s16 arg2) {
    ManagedSprite_SetPositionXY(arg0->unk_2C[0], 0x80, y);
    ManagedSprite_SetPositionXY(arg0->unk_2C[1], 0x80, arg2);
}

void ov40_0222F878(struct UnkStruct_ov62_02233310 *arg0);

void ov40_0222F878(UnkStruct_ov62_02233310 *arg0) {
    ManagedSprite_SetDrawFlag(arg0->unk_2C[0], 1);
    ManagedSprite_SetDrawFlag(arg0->unk_2C[1], 1);
    if (arg0->unk_08 == 0) {
        ManagedSprite_SetDrawFlag(arg0->unk_2C[0], 0);
    }
    if ((s32)(arg0->unk_08 + *((s32 *)&arg0->unk_10)) >= (s32) *((s32 *)&arg0->unk_04)) {
        ManagedSprite_SetDrawFlag(arg0->unk_2C[1], 0);
    }
    ManagedSprite_TickTwoFrames(arg0->unk_2C[0]);
    ManagedSprite_TickTwoFrames(arg0->unk_2C[1]);
}

void ov40_0222F8C0(struct UnkStruct_ov62_02233310 *arg0);

void ov40_0222F8C0(UnkStruct_ov62_02233310 *arg0) {
    ManagedSprite_SetDrawFlag(arg0->unk_2C[0], 1);
    ManagedSprite_SetDrawFlag(arg0->unk_2C[1], 1);
    if (arg0->unk_08 == 0) {
        ManagedSprite_SetDrawFlag(arg0->unk_2C[0], 0);
    }
    if ((s32)(arg0->unk_08 + *((s32 *)&arg0->unk_10)) >= (s32) *((s32 *)&arg0->unk_04)) {
        ManagedSprite_SetDrawFlag(arg0->unk_2C[1], 0);
    }
    if ((s32) *((s32 *)&arg0->unk_10) >= (s32) *((s32 *)&arg0->unk_04)) {
        ManagedSprite_SetDrawFlag(arg0->unk_2C[0], 0);
        ManagedSprite_SetDrawFlag(arg0->unk_2C[1], 0);
    }
    ManagedSprite_TickTwoFrames(arg0->unk_2C[0]);
    ManagedSprite_TickTwoFrames(arg0->unk_2C[1]);
}

void ov40_0222F920(struct UnkStruct_ov62_02233310 *arg0, struct UnkStruct_0208C06C *arg1);

void ov40_0222F920(UnkStruct_ov62_02233310 *arg0, UnkStruct_0208C06C *arg1) {
    SpriteManager_UnloadCharObjById(arg1->unk_14.unk_08, 0x30D40U);
    SpriteManager_UnloadCellObjById(arg1->unk_14.unk_08, 0x30D40U);
    SpriteManager_UnloadAnimObjById(arg1->unk_14.unk_08, 0x30D40U);
    Sprite_DeleteAndFreeResources(arg0->unk_2C[0]);
    Sprite_DeleteAndFreeResources(arg0->unk_2C[1]);
}
