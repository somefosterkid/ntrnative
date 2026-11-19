#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "constants/heap.h"
#include "touchscreen.h"

Sprite *sub_0209417C(UnkStruct_020932E0 *param_1, s32 param_2, s32 param_3, s32 param_4, BOOL param_5) {
    Sprite *sprite;
    SpriteTemplate SStack_48;

    sub_02094150(&SStack_48, param_1, NNS_G2D_VRAM_TYPE_2DMAIN);
    SStack_48.position.x = param_2 << 0xc;
    SStack_48.position.y = param_3 << 0xc;
    SStack_48.drawPriority = 10;
    sprite = Sprite_CreateAffine(&SStack_48);
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, param_4);
    Sprite_SetPriority(sprite, 1);
    Sprite_SetDrawFlag(sprite, param_5);
    return sprite;
}

void sub_020941CC(UnkStruct_020932E0 *arg0, BgConfig *bgConfig, enum HeapID heapID) {
    GfGfxLoader_LoadScrnData(NARC_a_1_5_1, 1, bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0x600U, 1, heapID);
    BgTilemapRectChangePalette(bgConfig, 2U, 0U, 0U, 0x20U, 0x18U, 1U);
    BgCommitTilemapBufferToVram(bgConfig, 2U);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_1, 4, bgConfig, GF_BG_LYR_MAIN_3, 0U, 0x600U, 1, heapID);
    BgTilemapRectChangePalette(bgConfig, 3U, 0U, 0U, 0x20U, 0x18U, 1U);
    BgCommitTilemapBufferToVram(bgConfig, 3U);
    arg0->unk_46C8 = GfGfxLoader_GetScrnData(NARC_a_1_5_1, 3, 1, &arg0->unk_46D0, heapID);
    arg0->unk_46CC = GfGfxLoader_GetScrnData(NARC_a_1_5_1, 1, 1, &arg0->unk_46D4, heapID);
    GfGfx_EngineATogglePlanes(8U, 0U);
}

void sub_0209428C(TouchscreenHitbox *rect, int param1, int param2, int param3, int param4) {
    rect->rect.top = param2 - param4 / 2;
    rect->rect.left = param1 - param3 / 2;
    rect->rect.bottom = param2 + param4 / 2;
    rect->rect.right = param1 + param3 / 2;
}
