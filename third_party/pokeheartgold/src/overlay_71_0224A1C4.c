#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_0203A3B0.h"
#include "overlay_71_trade_ball_arrival_phase.h"

void ov71_02247704(void *, u32);

void ov71_0224A5C4(void *, void *);

s32 ov71_0224A684(void *);

u32 ov71_0224A1C4(u8 *param_1, s32 *param_2);

void ov71_0224A3D8(void *arg0);

u32 ov71_0224A1C4(u8 *param_1, s32 *param_2) {
    s32 iVar1;
    BOOL BVar2;

    switch (*param_2) {
    case 0:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            Sprite_SetAnimCtrlSeq(((TradeBallArrivalPhase *)param_1)->sprite, 3);
            Sprite_SetDrawFlag(((TradeBallArrivalPhase *)param_1)->sprite, 1);
            *param_2 = *param_2 + 1;
        }
        break;
    case 1:
        iVar1 = Sprite_IsAnimated(((TradeBallArrivalPhase *)param_1)->sprite);
        if (iVar1 == 0) {
            PlaySE(0x6aa);
            ov71_0224A5C4(param_1, &((TradeBallArrivalPhase *)param_1)->bapState);
            ov71_02247704(((void *)((TradeBallArrivalPhase *)param_1)->model), 1);
            *param_2 = *param_2 + 1;
        }
        break;
    case 2:
        iVar1 = ov71_0224A684(&((TradeBallArrivalPhase *)param_1)->bapState);
        if (iVar1 != 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7fff, 0x10, 1, HEAP_ID_57);
            *param_2 = *param_2 + 1;
        }
        break;
    case 3:
        BVar2 = IsPaletteFadeFinished();
        if (BVar2 != 0) {
            return 1;
        }
    }
    return 0;
}

u32 ov71_022473A0(void **);

s32 sub_0203A948(s32, s32);

extern GraphicsModes ov71_0224BE84;

extern BgTemplate ov71_0224BEA4;

extern GraphicsBanks ov71_0224BEC0;

void ov71_0224A278(void *arg0);

void ov71_0224A278(void *arg0) {
    s32 memberNo;
    s32 v2;
    s32 v1;
    u32 v0;
    u32 srcOffset;

    GfGfx_SetBanks(&ov71_0224BEC0);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&ov71_0224BE84);
    InitBgFromTemplate(((TradeBallArrivalPhase *)arg0)->bgConfig, 3U, &ov71_0224BEA4, 0U);
    InitBgFromTemplate(((TradeBallArrivalPhase *)arg0)->bgConfig, 7U, &ov71_0224BEA4, 0U);
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
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, v2, ((TradeBallArrivalPhase *)arg0)->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, v2, ((TradeBallArrivalPhase *)arg0)->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, memberNo, ((TradeBallArrivalPhase *)arg0)->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, memberNo, ((TradeBallArrivalPhase *)arg0)->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, v1, GF_PAL_LOCATION_MAIN_BG, srcOffset, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, v1, GF_PAL_LOCATION_SUB_BG, srcOffset, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    BgSetPosTextAndCommit(((TradeBallArrivalPhase *)arg0)->bgConfig, 3U, BG_POS_OP_SET_XSCALE, 0);
    BgSetPosTextAndCommit(((TradeBallArrivalPhase *)arg0)->bgConfig, 7U, BG_POS_OP_SET_XSCALE, 0x100);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    sub_0203A880();
    sub_0203A948(1, 0x38);
}

void ov71_0224A3D8(void *arg0) {
    FreeBgTilemapBuffer(*(BgConfig **)((u8 *)arg0 + 0x24), 3U);
    FreeBgTilemapBuffer(*(BgConfig **)((u8 *)arg0 + 0x24), 7U);
}
