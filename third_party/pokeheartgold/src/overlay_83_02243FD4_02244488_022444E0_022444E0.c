#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "overlay_83_battle_castle_opponent_app.h"

struct BattleCastleOpponentApp;

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

struct Window;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

enum TextAlignment {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

void LoadMainBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadSummaryBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleOpponentApp *app, enum GFBgLayer bgLayer);


u8 PrintMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void ChangeState(BattleCastleOpponentApp *app, int *state, enum BattleCastleAppState newState);

void ov83_022444E0(BgConfig *bgConfig);

extern const GraphicsModes ov83_02248044;

extern const BgTemplate ov83_02248068;

extern const BgTemplate ov83_02248084;

extern const BgTemplate ov83_022480A0;

extern const BgTemplate ov83_022480BC;

extern const BgTemplate ov83_022480D8;

extern const BgTemplate ov83_022480F4;

void ov83_022444E0(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov83_02248044;

    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate bgTemplate1 = ov83_02248068;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bgTemplate1, BG_TYPE_STATIC);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate bgTemplate0 = ov83_02248084;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bgTemplate0, BG_TYPE_STATIC);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate bgTemplate2 = ov83_022480A0;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bgTemplate2, BG_TYPE_STATIC);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);

    BgTemplate bgTemplate3 = ov83_022480BC;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bgTemplate3, BG_TYPE_STATIC);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    BgTemplate bgTemplateSub0 = ov83_022480D8;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bgTemplateSub0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgTemplate bgTemplateSub1 = ov83_022480F4;

    InitBgFromTemplate(bgConfig, 5, &bgTemplateSub1, BG_TYPE_STATIC);
    BG_ClearCharDataRange(5, 32, BG_TYPE_STATIC, HEAP_ID_107);
    BgClearTilemapBufferAndCommit(bgConfig, 5);

    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, TRUE);
}
