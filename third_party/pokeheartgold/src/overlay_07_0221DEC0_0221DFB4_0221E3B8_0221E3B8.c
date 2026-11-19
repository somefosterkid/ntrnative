#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_switch.h"
#include "palette.h"
#include "sys_task_api.h"

BOOL ov07_0221BFC0(BattleAnimSystem *system);

void ov07_0221DEC0(BattleBgSwitch *bgSwitch);

void ov07_0221E664(BattleBgSwitch *bgSwitch);

void ov07_0221FB30(BattleAnimSystem *system, enum GFBgLayer layer);

void ov07_0221FB58(BattleAnimSystem *system);

BOOL ov07_0221E3B8(SysTask *task, BattleBgSwitch *bgSwitch);

BOOL ov07_0221E3B8(SysTask *task, BattleBgSwitch *bgSwitch) {
    u8 state;
    BattleAnimSystem *system;

    state = bgSwitch->state;
    switch (state) {
    case 0:
        ov07_0221DEC0(bgSwitch);
        bgSwitch->state = (u8)(bgSwitch->state + 1);
    case 1:
        if (bgSwitch->fadeType == 0) {
            PaletteData_ForceBeginPaletteFade(bgSwitch->battleAnimSystem->paletteData, 1U, 0x200U, bgSwitch->unk_0F, 0U, 0x10U, 0U);
            system = bgSwitch->battleAnimSystem;
            PaletteData_BlendPalettes(system->paletteData, PLTTBUF_MAIN_BG, (u16)system->baseBgPalettes, 0x10U, 0U);
        } else {
            PaletteData_ForceBeginPaletteFade(bgSwitch->battleAnimSystem->paletteData, 1U, 0x200U, bgSwitch->unk_0F, 0U, 0x10U, 0xFFFFU);
            system = bgSwitch->battleAnimSystem;
            PaletteData_BlendPalettes(system->paletteData, PLTTBUF_MAIN_BG, (u16)system->baseBgPalettes, 0x10U, 0xFFFFU);
        }
        bgSwitch->state = (u8)(bgSwitch->state + 1);
    case 2:
        if (PaletteData_GetSelectedBuffersBitmask(bgSwitch->battleAnimSystem->paletteData) != 0) {
            break;
        }
        ov07_0221E664(bgSwitch);
        ToggleBgLayer(3U, 0U);
        if (ov07_0221BFC0(bgSwitch->battleAnimSystem) == 0) {
            SetBgControlParam(bgSwitch->battleAnimSystem->bgConfig, 3U, GF_BG_CNT_SET_COLOR_MODE, 1U);
            ov07_0221FB30(bgSwitch->battleAnimSystem, GF_BG_LYR_MAIN_3);
            ov07_0221FB58(bgSwitch->battleAnimSystem);
        } else {
            system = bgSwitch->battleAnimSystem;
            GfGfxLoader_LoadCharData(system->battleBgRefs.narcID, system->battleBgRefs.tilesNarcMemberIdx, system->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, (enum HeapID)system->heapID);
            system = bgSwitch->battleAnimSystem;
            PaletteData_LoadNarc(system->paletteData, system->battleBgRefs.narcID, system->battleBgRefs.paletteNarcMemberIdx, (enum HeapID)system->heapID, PLTTBUF_MAIN_BG, system->battleBgRefs.paletteSrcSize << 5, (s32)system->battleBgRefs.paletteDestStart);
        }
        system = bgSwitch->battleAnimSystem;
        GfGfxLoader_LoadScrnData(system->battleBgRefs.narcID, system->battleBgRefs.tilemapNarcMemberIdx, system->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, (enum HeapID)system->heapID);
        ToggleBgLayer(3U, 1U);
        bgSwitch->state = (u8)(bgSwitch->state + 1);
    case 3:
        BgSetPosTextAndCommit(bgSwitch->battleAnimSystem->bgConfig, 3U, BG_POS_OP_SET_ROT, 0);
        BgSetPosTextAndCommit(bgSwitch->battleAnimSystem->bgConfig, 3U, BG_POS_OP_SET_XSCALE, 0);
        system = bgSwitch->battleAnimSystem;
        if (bgSwitch->fadeType == 0) {
            PaletteData_ForceBeginPaletteFade(system->paletteData, 1U, (u16)system->baseBgPalettes, bgSwitch->unk_0F, 0x10U, 0U, 0U);
        } else {
            PaletteData_ForceBeginPaletteFade(system->paletteData, 1U, (u16)system->baseBgPalettes, bgSwitch->unk_0F, 0x10U, 0U, 0xFFFFU);
        }
        bgSwitch->state = (u8)(bgSwitch->state + 1);
    default:
        if (PaletteData_GetSelectedBuffersBitmask(bgSwitch->battleAnimSystem->paletteData) == 0) {
            bgSwitch->battleAnimSystem->bgSwitchState = 2;
            bgSwitch->state = (u8)(bgSwitch->state + 1);
            return 0;
        }
        break;
    }
    return 1;
}
