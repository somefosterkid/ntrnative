#include "global.h"
struct Ov96_02203468Work;

#include "sprite.h"
#include "overlay_96_ov96_02203468work.h"

void ov96_022038A0(struct Ov96_02203468Work *arg0, u16 arg1);

void ov96_022038A0(Ov96_02203468Work *arg0, u16 arg1) {
    u8 v0;

    v0 = arg1;
    Sprite_SetAnimCtrlSeq(arg0->sprites[0], (u8)((s32)v0 / 10) + 1);
    Sprite_SetAnimCtrlSeq(arg0->sprites[1], (u8)((s32)v0 % 10) + 1);
}
