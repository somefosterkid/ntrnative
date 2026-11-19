#include "global.h"

#include "sprite.h"
#include "overlay_43_palpad.h"

void ov43_0222AD40(PalPadGfx *gfx, u32 param_2, s32 param_3);

void ov43_0222AD74(PalPadGfx *arg0, s32 arg1, s32 arg2);
void ov43_0222AD98(PalPadGfx *arg0, u8 arg1, u32 arg2);

void ov43_0222AD40(PalPadGfx *gfx, u32 param_2, s32 param_3) {
    u32 uVar1;
    s32 new_var;
    uVar1 = param_2 & 0x7f;
    new_var = uVar1 * 2;
    if ((1 + 1) <= param_2) {
        param_2 = 0xff & (param_2 - 2);
    }
    Sprite_SetAnimActiveFlag(gfx->arrows[param_2], 1);
    Sprite_SetAnimCtrlSeq(gfx->arrows[param_2], (new_var + 5) + param_3);
    Sprite_ResetAnimCtrlState(gfx->arrows[param_2]);
}

void ov43_0222AD74(PalPadGfx *arg0, s32 flag, s32 seq) {
    if (seq != 0xFF) {
        Sprite_SetAnimCtrlSeq(arg0->cursor, seq);
    }
    Sprite_SetDrawFlag(arg0->cursor, flag);
}

void ov43_0222AD98(PalPadGfx *arg0, u8 priority, u32 arg2) {
    Sprite_SetPriority(arg0->cursor, priority);
    Sprite_SetDrawPriority(arg0->cursor, arg2);
}
