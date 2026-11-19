#include "global.h"

#include "battle/battle_022378C0.h"
#include "battle/battle_input.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "battle/battle.h"

void ov12_02238A30(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);

    BgConfig_CleanupBattleMenuBackgrounds(bgConfig);
}

void ov12_02238A64(BattleSystem *battleSystem) {
}
