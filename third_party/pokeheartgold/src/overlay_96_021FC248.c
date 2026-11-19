#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_icon_work.h"
#include "sprite.h"

void ov96_021FC248(Ov96IconWork *work, s32 idx, s32 x) {
    VecFx32 pos;

    pos.z = 0;
    pos.x = x << 0xc;
    pos.y = 0xe000;
    Sprite_SetMatrix(work->icons[idx].monIcon, &pos);
    Sprite_SetMatrix(work->icons[idx].teamMark, &pos);
    Sprite_SetMatrix(work->icons[idx].rank, &pos);
}

void ov96_021FC28C(Ov96IconWork *work, s32 idx, u32 rank) {
    if (rank == 0 || 8 < rank) {
        GF_AssertFail();
    }
    Sprite_SetAnimCtrlSeq(work->icons[idx].rank, rank + 0xf);
}

void ov96_021FC2B4(Ov96IconWork *work, BOOL visible) {
    s32 i;

    for (i = 0; i < 12; i++) {
        Sprite_SetDrawFlag(work->icons[i].monIcon, visible);
        Sprite_SetDrawFlag(work->icons[i].teamMark, visible);
    }
}
