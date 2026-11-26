#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "overlay_71_trade_ball_flyby_phase.h"

void ov71_02247704(void *, u32);

void ov71_022499A4(void *, void *);

s32 ov71_02249A50(void *);

u32 ov71_022495A0(TradeBallFlybyPhase *param_1, s32 *param_2);

void ov71_022497C8(void *arg0);

u32 ov71_022495A0(TradeBallFlybyPhase *param_1, s32 *param_2) {
    s32 iVar1;
    BOOL BVar2;

    switch (*param_2) {
    case 0:
        *((u32 *)&param_1->timer) = 0;
        *param_2 = *param_2 + 1;
        break;
    case 1:
        iVar1 = *((s32 *)&param_1->timer) + 1;
        *((s32 *)&param_1->timer) = iVar1;
        if (10 < iVar1) {
            PlaySE(0x6ab);
            ov71_022499A4(param_1, &param_1->ballOrbit);
            ov71_02247704(((void *)param_1->model), 1);
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        iVar1 = ov71_02249A50(&param_1->ballOrbit);
        if (iVar1 != 0) {
            PlaySE(0x6ac);
            Sprite_SetAnimCtrlSeq(param_1->sprite, 3);
            Sprite_SetDrawFlag(param_1->sprite, 1);
            *param_2 = *param_2 + 1;
        }
        break;
    case 3:
        iVar1 = Sprite_IsAnimated(param_1->sprite);
        if (iVar1 == 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7fff, 0x10, 1, HEAP_ID_57);
            *param_2 = *param_2 + 1;
        }
        break;
    case 4:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            return 1;
        }
    }
    return 0;
}

u32 ov71_022473A0(void **);

extern GraphicsModes ov71_0224BD84;

extern BgTemplate ov71_0224BDA4;

extern GraphicsBanks ov71_0224BDC0;

void ov71_02249670(void *arg0);

void ov71_02249670(void *arg0) {
    s32 memberNo;
    s32 v2;
    s32 v1;
    u32 v0;
    u32 srcOffset;

    GfGfx_SetBanks(&ov71_0224BDC0);
    reg_GX_POWCNT &= 0xFFFF7FFF;
    SetBothScreensModesAndDisable(&ov71_0224BD84);
    InitBgFromTemplate(((TradeBallFlybyPhase *)arg0)->bgConfig, 3U, &ov71_0224BDA4, 0U);
    InitBgFromTemplate(((TradeBallFlybyPhase *)arg0)->bgConfig, 7U, &ov71_0224BDA4, 0U);
    v0 = ov71_022473A0(*(void **)arg0);
    switch (v0) {
    default:
    case 0:
        memberNo = 0xF;
        v2 = 0x10;
        v1 = 0x11;
        srcOffset = 0;
        break;
    case 1:
        memberNo = 0xF;
        v2 = 0x10;
        v1 = 0x11;
        srcOffset = 0x20;
        break;
    case 2:
        memberNo = 0xF;
        v2 = 0x10;
        v1 = 0x11;
        srcOffset = 0x40;
        break;
    case 3:
        memberNo = 0x18;
        v2 = 0x19;
        v1 = 0x1A;
        srcOffset = 0;
        break;
    }
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, v2, ((TradeBallFlybyPhase *)arg0)->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, v2, ((TradeBallFlybyPhase *)arg0)->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, memberNo, ((TradeBallFlybyPhase *)arg0)->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, memberNo, ((TradeBallFlybyPhase *)arg0)->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, v1, GF_PAL_LOCATION_MAIN_BG, srcOffset, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, v1, GF_PAL_LOCATION_SUB_BG, srcOffset, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    BgSetPosTextAndCommit(((TradeBallFlybyPhase *)arg0)->bgConfig, 3U, BG_POS_OP_SET_XSCALE, 0x100);
    BgSetPosTextAndCommit(((TradeBallFlybyPhase *)arg0)->bgConfig, 7U, BG_POS_OP_SET_XSCALE, 0);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}

void ov71_022497C8(void *arg0) {
    FreeBgTilemapBuffer(*(BgConfig **)((u8 *)arg0 + 0x24), 3U);
    FreeBgTilemapBuffer(*(BgConfig **)((u8 *)arg0 + 0x24), 7U);
}
