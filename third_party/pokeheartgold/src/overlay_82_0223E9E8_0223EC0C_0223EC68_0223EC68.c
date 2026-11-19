#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "overlay_82_battle_hall_app.h"

struct BattleHallApp;

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

enum BattleHallAppState {
    STATE_FADE_IN = 0,
    STATE_MON_SUMMARY,
    STATE_SELECT_BATTLE,
    STATE_MULTI_CONFIRM,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT,
    STATE_CLOSE_MESSAGE,
};

void ov82_0223EDF0(BattleHallApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleHallApp *app, enum GFBgLayer bgLayer);

void ChangeState(BattleHallApp *app, int *state, enum BattleHallAppState nextState);

void ov82_0223EC68(BgConfig *bgConfig);

extern const GraphicsModes ov82_0223FE28;

extern const BgTemplate ov82_0223FE54;

extern const BgTemplate ov82_0223FE70;

extern const BgTemplate ov82_0223FE8C;

extern const BgTemplate ov82_0223FEA8;

extern const BgTemplate ov82_0223FE38;

void ov82_0223EC68(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = ov82_0223FE28;

    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate bgTemplate0 = ov82_0223FE54;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bgTemplate0, BG_TYPE_STATIC);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_105);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);

    BgTemplate bgTemplate1 = ov82_0223FE70;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bgTemplate1, BG_TYPE_STATIC);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_105);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);

    BgTemplate bgTemplate3 = ov82_0223FE8C;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bgTemplate3, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    BgTemplate bgTemplateSub1 = ov82_0223FEA8;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bgTemplateSub1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);

    BgTemplate bgTemplateSub0 = ov82_0223FE38;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bgTemplateSub0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
}
