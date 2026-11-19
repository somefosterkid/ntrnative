#include "global.h"

#include "sprite.h"
#include "error_handling.h"
#include "math_util.h"
#include "overlay_96_ov96_sprite_mgr.h"


void ov96_021F30C4(void *arg0, void *arg1);

u8 ov96_021F30F8(u32 arg0, s32 arg1);

void ov96_021F30C4(void *arg0, void *arg1) {
    Sprite *sprite;
    s32 v0;
    void *v1;

    v1 = arg0;
    v0 = 0;
    do {
        sprite = ov96_021EB5B8(*(struct Ov96Obj **)((u8 *)v1 + 0xC) = ov96_021EB4F4(arg1, 0x65, 2));
        Sprite_SetOamMode(sprite, GX_OAM_MODE_XLU);
        Sprite_SetDrawPriority(sprite, 0x64U);
        v0 += 1;
        v1 += 0x10;
    } while (v0 < 8);
}

u8 ov96_021F30F8(u32 arg0, s32 arg1) {
    s32 v0;
    u32 v1;
    u8 new_var;
    u32 v2;
    if (arg0 == 0xF) {
        return 0U;
    }
    if (arg0 < 1U) {
        v0 = 0;
    } else if (arg0 < 6U) {
        v0 = 1;
    } else if (arg0 < 0xAU) {
        v0 = 2;
    } else {
        v0 = 3;
    }
    if (arg1 == 1) {
        v1 = 1;
        v2 = 5;
    } else if (arg1 == 2) {
        v1 = 6;
        v2 = 4;
    } else {
        v1 = 0xA;
        v2 = 5;
    }
    if (arg1 == v0) {
        if (arg0 < v1) {
            GF_AssertFail();
        }
        new_var = (u8)(((u8)(((u8)((arg0 - v1) + 1)) + (((s32)LCRandom()) % ((s32)(v2 - 1))))) % v2);
        return (u8)(new_var + v1);
    }
    return (u8)(v1 + (((s32)LCRandom()) % ((s32)v2)));
}
