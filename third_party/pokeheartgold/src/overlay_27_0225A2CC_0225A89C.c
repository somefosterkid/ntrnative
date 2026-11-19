#include "global.h"
struct Ov27Task;

#include "bag.h"
#include "field_system.h"
#include "overlay_27.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "sprite.h"
#include "overlay_27_ov27task.h"

s32 ov27_0225BD44(FieldSystem *);

s32 sub_02058AA0(void);

s32 ov27_0225A89C(FieldSystem *fieldSystem) {
    if ((IsPaletteFadeFinished() == 0) || (fieldSystem->runningFieldMap == 0) || (fieldSystem->unkD2_0 != 0) || (FieldSystem_IsPlayerMovementAllowed(fieldSystem) == 0) || (ov27_0225BD44(fieldSystem) != 0) || (MapSceneScriptCheck(fieldSystem) != 0) || (sub_02058AA0() != 0)) {
        return 1;
    }
    return 0;
}

s32 ov27_0225B398(Ov27Task *, s32);

s32 ov27_0225B4AC(void *, s32);

void ov27_0225A8E8(Ov27Task *arg0, s32 arg1);

void ov27_0225A8E8(Ov27Task *arg0, s32 arg1) {
    if (arg1 != 0) {
        if (((arg0->unk_51C_1) == 5) && (arg0->fieldSystem->unkD2_0 == 0)) {
            ov27_0225B398(arg0, -1);
        }
        Sprite_SetOamMode(arg0->unk_390[11], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(arg0->unk_390[13], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(arg0->unk_390[7], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(arg0->unk_390[9], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(arg0->unk_390[8], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(arg0->unk_390[10], GX_OAM_MODE_XLU);
        if (arg0->fieldSystem->unkD2_0 != 0) {
            ov27_0225B4AC(arg0->unk_390, arg0->unk_14);
        } else {
            ov27_0225B4AC(arg0->unk_390, -1);
        }
        if ((arg0->unk_51C_1) == 2) {
            Sprite_SetOamMode(arg0->unk_390[6], GX_OAM_MODE_NORMAL);
        }
        G2S_SetBlendAlpha(0, 0x23, 6, 9);
        return;
    }
    reg_G2S_DB_BLDCNT = 0;
}

s32 ov27_0225BDAC(Ov27Task *arg0);

void ov27_0225A9C0(Ov27Task *arg0, s32 arg1);

void ov27_0225A9C0(Ov27Task *arg0, s32 arg1) {
    Bag *bag;
    s32 v0;
    s32 flag;
    s32 v1;

    v0 = arg1;
    if (ov27_0225BDAC(arg0) == 0) {
        v0 = 0;
    }
    if (v0 != 0) {
        bag = Save_Bag_Get(arg0->fieldSystem->saveData);
        if (Bag_GetRegisteredItem1(bag) != 0) {
            v1 = 1;
        } else {
            v1 = 0;
        }
        if (Bag_GetRegisteredItem2(bag) != 0) {
            flag = 1;
        } else {
            flag = 0;
        }
        Sprite_SetDrawFlag(arg0->unk_390[7], v1);
        Sprite_SetDrawFlag(arg0->unk_390[9], v1);
        Sprite_SetDrawFlag(arg0->unk_390[8], flag);
        Sprite_SetDrawFlag(arg0->unk_390[10], flag);
        return;
    }
    Sprite_SetDrawFlag(arg0->unk_390[7], 0);
    Sprite_SetDrawFlag(arg0->unk_390[9], 0);
    Sprite_SetDrawFlag(arg0->unk_390[8], 0);
    Sprite_SetDrawFlag(arg0->unk_390[10], 0);
}
