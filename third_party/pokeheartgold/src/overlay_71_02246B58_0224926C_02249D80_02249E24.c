#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "sys_task_api.h"
#include "overlay_71_trade_sequence_data.h"

typedef struct UnkStruct_ov71_02249E6C {
    struct TradeSequenceData *unk_0;
    u8 unk_4[0x8];
    BgConfig *unk_C;
} UnkStruct_ov71_02249E6C;

struct TradeSequenceData;

typedef struct TradeWormholePhase {
    TradeSequenceData *sequenceData;
    int subStepCounter;
    int framesRemaining;
    BgConfig *bgConfig;
    fx32 bg1Scroll;
    fx32 bg2Scroll;
    fx32 bg1ScrollStep;
    fx32 bg2ScrollStep;
    BOOL scrollDone;
} TradeWormholePhase;

void ov71_0224A0B8(BgConfig *bgConfig, int bg1Scroll, int bg2Scroll);

void ov71_02249E24(SysTask *task, void *param);

void ov71_02249E24(SysTask *task, void *param) {
    TradeWormholePhase *twPhase = param;

    if (twPhase->framesRemaining) {
        twPhase->framesRemaining--;
        twPhase->bg1Scroll += twPhase->bg1ScrollStep;
        twPhase->bg2Scroll += twPhase->bg2ScrollStep;

        ov71_0224A0B8(twPhase->bgConfig, twPhase->bg1Scroll >> 12, twPhase->bg2Scroll >> 12);
    } else {
        ov71_0224A0B8(twPhase->bgConfig, -48, -48);
        twPhase->scrollDone = 1;
        SysTask_Destroy(task);
    }
}

void ov71_02247124(TradeSequenceData *, s32, s32, s32, s32);

s32 ov71_02247230(TradeSequenceData *, s32, s32, s32, s32, s32);

extern GraphicsModes ov71_0224BDF0;

extern BgTemplate ov71_0224BE00;

extern BgTemplate ov71_0224BE1C;

extern BgTemplate ov71_0224BE38;

extern GraphicsBanks ov71_0224BE54;

void ov71_02249E6C(UnkStruct_ov71_02249E6C *arg0);

void ov71_02249E6C(UnkStruct_ov71_02249E6C *arg0) {
    GfGfx_SetBanks(&ov71_0224BE54);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&ov71_0224BDF0);
    InitBgFromTemplate(arg0->unk_C, 1U, &ov71_0224BE00, 0U);
    InitBgFromTemplate(arg0->unk_C, 5U, &ov71_0224BE00, 0U);
    InitBgFromTemplate(arg0->unk_C, 2U, &ov71_0224BE1C, 0U);
    InitBgFromTemplate(arg0->unk_C, 3U, &ov71_0224BE38, 0U);
    InitBgFromTemplate(arg0->unk_C, 7U, &ov71_0224BE38, 0U);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 0x13, arg0->unk_C, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 0x13, arg0->unk_C, GF_BG_LYR_SUB_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0x12, arg0->unk_C, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0x12, arg0->unk_C, GF_BG_LYR_SUB_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 0x14, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 0x14, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    BG_FillCharDataRange(arg0->unk_C, GF_BG_LYR_SUB_0_F, 0U, 1U, 0xC8U);
    BG_FillCharDataRange(arg0->unk_C, GF_BG_LYR_SUB_1, 0U, 1U, 0xC8U);
    BG_FillCharDataRange(arg0->unk_C, GF_BG_LYR_SUB_1_F, 0U, 1U, 0xC8U);
    ov71_02247124(arg0->unk_0, 0, 1, 0xE, 0);
    ov71_02247124(arg0->unk_0, 0, 5, 0xE, 0);
    ov71_02247124(arg0->unk_0, 1, 2, 0xF, 0);
    FillBgTilemapRect(arg0->unk_C, 1U, 0xC8U, 0U, 0U, 0x20U, 0x40U, 0U);
    FillBgTilemapRect(arg0->unk_C, 5U, 0xC8U, 0U, 0U, 0x20U, 0x40U, 0U);
    FillBgTilemapRect(arg0->unk_C, 2U, 0xC8U, 0U, 0U, 0x20U, 0x40U, 0U);
    ov71_02247230(arg0->unk_0, 0, 1, 0xE, 0x14, 0);
    ov71_02247230(arg0->unk_0, 0, 5, 0xE, 0x14, 0);
    ov71_02247230(arg0->unk_0, 1, 2, 0xF, 2, 0);
    BgCommitTilemapBufferToVram(arg0->unk_C, 1U);
    BgCommitTilemapBufferToVram(arg0->unk_C, 5U);
    BgCommitTilemapBufferToVram(arg0->unk_C, 2U);
    ov71_0224A0B8(arg0->unk_C, 0x50, 0xFFFFFE80);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}

void ov71_0224A080(TradeWormholePhase *twPhase);

void ov71_0224A080(TradeWormholePhase *twPhase) {
    FreeBgTilemapBuffer(twPhase->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(twPhase->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(twPhase->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(twPhase->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(twPhase->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(twPhase->bgConfig, GF_BG_LYR_SUB_1);
}
