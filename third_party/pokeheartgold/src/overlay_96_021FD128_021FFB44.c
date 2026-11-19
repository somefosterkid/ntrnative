#include "global.h"

#include "overlay_96_ov96_rower_work.h"

s32 ov96_021EB630(void *, s32);

void ov96_021FFB44(Ov96Obstacle *arg0);

void ov96_021FFB44(Ov96Obstacle *arg0) {
    s32 v0;

    if (arg0->unkB == 3) {
        ov96_021EB630(arg0->unk4, 0x258);
        return;
    }
    v0 = 0xA0 - (arg0->unkA + 0x20);
    if (v0 < 0) {
        v0 = 0;
    }
    ov96_021EB630(arg0->unk4, (0x1E * (((s32)(v0 + ((u32)(v0 >> 2) >> 0x1D)) >> 3) + 1)) + 3);
}
