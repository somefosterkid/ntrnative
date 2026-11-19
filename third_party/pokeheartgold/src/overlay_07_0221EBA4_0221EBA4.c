#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "overlay_07_battle_anim_system.h"

BOOL ov07_0221BFC0(BattleAnimSystem *system);

s32 ov07_0221FB04(BattleAnimSystem *system, u32 bg);

void ov07_0221FB30(BattleAnimSystem *system, GFBgLayer layer);

void ov07_0221EBA4(BattleAnimSystem *system, GFBgLayer layer, u16 colorMode);

void ov07_0221EBA4(BattleAnimSystem *system, GFBgLayer layer, u16 colorMode) {
    SetBgPriority(layer, ov07_0221FB04(system, 2));
    ToggleBgLayer(layer, 0U);
    if (ov07_0221BFC0(system) == 1) {
        SetBgControlParam(system->bgConfig, layer, GF_BG_CNT_SET_CHAR_BASE, 4U);
    } else {
        SetBgControlParam(system->bgConfig, layer, GF_BG_CNT_SET_COLOR_MODE, colorMode);
        SetBgControlParam(system->bgConfig, layer, GF_BG_CNT_SET_CHAR_BASE, 4U);
    }
    BgClearTilemapBufferAndCommit(system->bgConfig, layer);
    if (ov07_0221BFC0(system) == 1) {
        GfGfxLoader_LoadCharData(system->battleBgRefs.narcID, system->battleBgRefs.tilesNarcMemberIdx, system->bgConfig, layer, 0U, 0U, 1, (enum HeapID)system->heapID);
    } else {
        ov07_0221FB30(system, layer);
    }
    GfGfxLoader_LoadScrnData(system->battleBgRefs.narcID, system->battleBgRefs.tilemapNarcMemberIdx, system->bgConfig, layer, 0U, 0U, 1, (enum HeapID)system->heapID);
}
