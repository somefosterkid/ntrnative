#include "global.h"

#include "overlay_27.h"
#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_27_ov27task.h"

s32 ov27_0225B360(s32, s32, Ov27TaskIcon *);

s32 ov27_0225B398(Ov27Task *, s32);

s32 ov27_0225C170(Ov27Task *, s32);

void ov27_0225B4AC(Sprite **arg0, s32 arg1);

void ov27_0225B404(Ov27Task *arg0) {
    s32 v0;
    s32 v1;

    v1 = -1;
    v0 = arg0->unk_14;
    if (0x40 & gSystem.newKeys) {
        v1 = 0;
    } else if (0x80 & gSystem.newKeys) {
        v1 = 1;
    } else if (0x20 & gSystem.newKeys) {
        v1 = 2;
    } else if (gSystem.newKeys & 0x10) {
        v1 = 3;
    }
    if (v1 >= 0) {
        v0 = ov27_0225B360(v0, v1, arg0->unk_470);
    }
    if ((v0 != -1) && (arg0->unk_14 != v0)) {
        arg0->unk_14 = v0;
        PlaySE(0x5E0U);
        if ((u32)v1 <= 1U) {
            Sprite_SetAnimCtrlSeq(arg0->unk_390[arg0->unk_14], 3);
        } else if ((u32)(v1 - 2) <= 1U) {
            Sprite_SetAnimCtrlSeq(arg0->unk_390[arg0->unk_14], 1);
        }
        ov27_0225B398(arg0, arg0->unk_14);
        arg0->fieldSystem->unkD3 = ov27_0225C170(arg0, arg0->unk_14);
    }
}

void ov27_0225B4AC(Sprite **arg0, s32 arg1) {
    Sprite **v1;
    Sprite *sprite;
    s32 v0;

    v1 = arg0;
    v0 = 0;
    do {
        sprite = *v1;
        if (sprite != NULL) {
            if (arg1 != v0) {
                Sprite_SetOamMode(sprite, GX_OAM_MODE_XLU);
            } else {
                Sprite_SetOamMode(sprite, GX_OAM_MODE_NORMAL);
            }
        }
        v0 += 1;
        v1 += 1;
    } while (v0 < 9);
}
