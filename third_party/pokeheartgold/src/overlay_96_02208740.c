#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"
#include "sprite.h"

void ov96_02208740(Ov96HurdleHud *hud, s32 arg1);

void ov96_02208740(Ov96HurdleHud *hud, s32 arg1) {
    u8 v0;

    v0 = (u8)(arg1 / 30);
    Sprite_SetAnimCtrlSeq(hud->digits[0], (u8)((s32)v0 / 10) + 1);
    Sprite_SetAnimCtrlSeq(hud->digits[1], (u8)((s32)v0 % 10) + 1);
}
