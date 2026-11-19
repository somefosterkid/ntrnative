#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"

void ov96_0220DB3C(UnkStruct_ov96_0220E3F0_HudGroup *arg0, s32 arg1, s32 arg2);

void ov96_0220DB3C(UnkStruct_ov96_0220E3F0_HudGroup *arg0, s32 arg1, s32 flag) {
    s32 v0;
    s32 v1;

    switch (arg1) {
    case 0:
        ManagedSprite_SetDrawFlag(arg0->unk_0[0], flag);
        if (flag == 0) {
            v0 = 1;
        } else {
            v0 = 0;
        }
        ManagedSprite_SetDrawFlag(arg0->unk_0[1], v0);
        ManagedSprite_SetDrawFlag(arg0->unk_0[2], flag);
        ManagedSprite_SetDrawFlag(arg0->unk_0[3], v0);
        return;
    case 1:
        if (flag == 0) {
            v1 = 1;
        } else {
            v1 = 0;
        }
        ManagedSprite_SetDrawFlag(arg0->unk_0[0], v1);
        ManagedSprite_SetDrawFlag(arg0->unk_0[1], flag);
        ManagedSprite_SetDrawFlag(arg0->unk_0[2], v1);
        ManagedSprite_SetDrawFlag(arg0->unk_0[3], flag);
        return;
    case 2:
        ManagedSprite_SetDrawFlag(arg0->unk_0[0], flag);
        ManagedSprite_SetDrawFlag(arg0->unk_0[1], flag);
        ManagedSprite_SetDrawFlag(arg0->unk_0[2], flag);
        ManagedSprite_SetDrawFlag(arg0->unk_0[3], flag);
        return;
    }
}

void ov96_0220DBCC(UnkStruct_ov96_0220E3F0_HudGroup *param_1, f32 param_2);

void ov96_0220DBCC(UnkStruct_ov96_0220E3F0_HudGroup *param_1, f32 param_2) {
    ManagedSprite_SetAffineScale(param_1->unk_0[0], param_2, param_2);
    ManagedSprite_SetAffineScale(param_1->unk_0[1], param_2, param_2);
    return;
}

void ov96_0220DBE8(UnkStruct_ov96_0220E3F0_HudGroup *arg0, s32 arg1, s16 arg2, s16 arg3);

void ov96_0220DBE8(UnkStruct_ov96_0220E3F0_HudGroup *arg0, s32 arg1, s16 x, s16 y) {
    s32 seqno;

    seqno = arg1 - 3;
    if (seqno >= 4) {
        GF_AssertFail();
    }
    ManagedSprite_SetPositionXYWithSubscreenOffset(arg0->unk_10, x, y, 0x1E0000);
    ManagedSprite_SetPositionXYWithSubscreenOffset(arg0->unk_14, x, y, 0x1E0000);
    ManagedSprite_SetAnim(arg0->unk_10, seqno);
    ManagedSprite_ResetSpriteAnimCtrlState(arg0->unk_14);
    ManagedSprite_SetDrawFlag(arg0->unk_10, 1);
    ManagedSprite_SetDrawFlag(arg0->unk_14, 1);
}
