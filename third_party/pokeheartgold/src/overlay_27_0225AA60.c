#include "global.h"

#include "overlay_27.h"
#include "sprite.h"
#include "overlay_27_ov27task.h"



u8 ov27_0225AA60(u8 *arg0, s32 arg1) {
    s32 v1;
    u8 v0;
    u32 v2;

    v2 = 0;
    v1 = 0;
    if (arg1 > 0) {
        do {
            v0 = arg0[v1];
            v1 += 1;
            v2 += v0;
        } while (v1 < arg1);
    }
    return v2;
}

void ov27_0225AA7C(Ov27Task *arg0) {
    s32 v0;

    v0 = 0;
    do {
        Sprite_SetDrawFlag(arg0->unk_390[v0], arg0->unk_514[v0]);
        v0 += 1;
    } while (v0 < 7);
    arg0->unk_50C = 1;
    if (ov27_0225BDC8(arg0) == 0) {
        Sprite_SetDrawFlag(arg0->unk_390[11], 0);
        Sprite_SetDrawFlag(arg0->unk_390[13], 0);
        arg0->unk_50C = 0;
    }
}
