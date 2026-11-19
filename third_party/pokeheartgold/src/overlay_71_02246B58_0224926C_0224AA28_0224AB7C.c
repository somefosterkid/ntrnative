#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "overlay_71_trade_pokemon_reveal_phase.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_palette_shimmer_effect.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_71_trade_sequence_data.h"
#include "msgdata.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct Trade3DScene Trade3DScene;

typedef struct BgScaleAnimation BgScaleAnimation;

typedef u16 charcode_t;

enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};

enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
};

struct BgScaleAnimation {
    BgConfig *bgConfig;
    volatile BOOL *done;
    fx32 currentScale;
    fx32 targetScale;
    fx32 rate;
    fx32 acceleration;
    int direction;
    int centerIndex;
    SysTask *task;
};

u32 ov71_022473A0(TradeSequenceData *);

extern const u16 ov71_0224BEE8[];

extern GraphicsModes ov71_0224BEFC;

extern BgTemplate ov71_0224BF1C;

extern BgTemplate ov71_0224BF38;

extern GraphicsBanks ov71_0224BF54;

void ov71_0224AB7C(TradePokemonRevealPhase *tprPhase);

void ov71_0224AB7C(TradePokemonRevealPhase *tprPhase) {
    TradePokemonRevealPhase *var_r4;
    u32 var_r5;
    void *var_r5_p;

    var_r4 = tprPhase;
    GfGfx_SetBanks(&ov71_0224BF54);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&ov71_0224BEFC);
    InitBgFromTemplate(var_r4->bgConfig, 2U, &ov71_0224BF1C, 1U);
    InitBgFromTemplate(var_r4->bgConfig, 6U, &ov71_0224BF1C, 1U);
    var_r5 = OS_DisableInterrupts();
    InitBgFromTemplate(var_r4->bgConfig, 3U, &ov71_0224BF38, 2U);
    InitBgFromTemplate(var_r4->bgConfig, 7U, &ov71_0224BF38, 2U);
    OS_RestoreInterrupts(var_r5);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, var_r4->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, var_r4->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 1, var_r4->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 1, var_r4->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_57);
    var_r5 = ov71_022473A0(var_r4->sequenceData);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, ov71_0224BEE8[var_r5], GF_PAL_SLOT_1_OFFSET, 0x40U, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, ov71_0224BEE8[var_r5], GF_PAL_SLOT_1_OFFSET, 0x40U, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, var_r4->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, var_r4->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, var_r4->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, var_r4->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 1, HEAP_ID_57);
    var_r5_p = Heap_Alloc(HEAP_ID_57, 0x60U);
    if (var_r5_p != 0) {
        MIi_CpuClear32(0U, (void *)var_r5_p, 0x60U);
        DC_FlushRange((void *)var_r5_p, 0x60U);
        GX_BeginLoadBGExtPltt();
        GXS_BeginLoadBGExtPltt();
        GX_LoadBGExtPltt((void *)var_r5_p, 0x6000U, 0x60U);
        GXS_LoadBGExtPltt((void *)var_r5_p, 0x6000U, 0x60U);
        GX_EndLoadBGExtPltt();
        GXS_EndLoadBGExtPltt();
        Heap_Free((void *)var_r5_p);
    }
    BgSetPosTextAndCommit(var_r4->bgConfig, 2U, BG_POS_OP_SET_XSCALE, -0x180);
    BgSetPosTextAndCommit(var_r4->bgConfig, 6U, BG_POS_OP_SET_XSCALE, -0xC0);
    BgSetPosTextAndCommit(var_r4->bgConfig, 3U, BG_POS_OP_SET_XSCALE, 0x43);
    ToggleBgLayer(7U, 0U);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}
