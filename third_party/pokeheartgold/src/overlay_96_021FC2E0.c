#include "global.h"

#include "overlay_96_ov96_icon_work.h"
#include "sprite.h"
#include "unk_02009D48.h"

void ov96_021FC2E0(Ov96IconWork *work, s32 idx, BOOL visible) {
    Sprite_SetDrawFlag(work->icons[idx].monIcon, visible);
    Sprite_SetDrawFlag(work->icons[idx].teamMark, visible);
    Sprite_SetDrawFlag(work->icons[idx].rank, visible);
}

void ov96_021FC314(Ov96IconWork *work) {
    SpriteList_RenderAndAnimateSprites(work->spriteList);
}
