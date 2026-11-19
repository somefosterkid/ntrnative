#include "global.h"

#include "bg_window.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_switch.h"
#include "palette.h"
#include "sys_task_api.h"

void ov07_0221DE04(BattleBgSwitch *bgSwitch, BattleAnimSystem *system, enum GFBgLayer layer, s32 bgID);

void ov07_0221DEC0(BattleBgSwitch *bgSwitch);

BOOL ov07_0221E280(SysTask *task, BattleBgSwitch *bgSwitch);

BOOL ov07_0221E280(SysTask *task, BattleBgSwitch *bgSwitch) {
    u8 state;
    BattleAnimSystem *system;

    state = bgSwitch->state;
    switch (state) {
    case 0:
        system = bgSwitch->battleAnimSystem;
        if (bgSwitch->fadeType == 0) {
            PaletteData_ForceBeginPaletteFade(system->paletteData, 1U, (u16)system->baseBgPalettes, bgSwitch->unk_0E, 0U, 0x10U, 0U);
            PaletteData_BlendPalettes(bgSwitch->battleAnimSystem->paletteData, PLTTBUF_MAIN_BG, 0x200U, 0x10U, 0U);
        } else {
            PaletteData_ForceBeginPaletteFade(system->paletteData, 1U, (u16)system->baseBgPalettes, bgSwitch->unk_0E, 0U, 0x10U, 0xFFFFU);
            PaletteData_BlendPalettes(bgSwitch->battleAnimSystem->paletteData, PLTTBUF_MAIN_BG, 0x200U, 0x10U, 0xFFFFU);
        }
        bgSwitch->state = (u8)(bgSwitch->state + 1);
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(bgSwitch->battleAnimSystem->paletteData) == 0) {
            SetBgControlParam(bgSwitch->battleAnimSystem->bgConfig, 3U, GF_BG_CNT_SET_COLOR_MODE, 0U);
            ov07_0221DE04(bgSwitch, bgSwitch->battleAnimSystem, GF_BG_LYR_MAIN_3, bgSwitch->bgID);
            if (bgSwitch->fadeType == 0) {
                PaletteData_ForceBeginPaletteFade(bgSwitch->battleAnimSystem->paletteData, 1U, 0x200U, bgSwitch->unk_0E, 0x10U, 0U, 0U);
            } else {
                PaletteData_ForceBeginPaletteFade(bgSwitch->battleAnimSystem->paletteData, 1U, 0x200U, bgSwitch->unk_0E, 0x10U, 0U, 0xFFFFU);
            }
            ov07_0221DEC0(bgSwitch);
            bgSwitch->battleAnimSystem->bgSwitchState = 2;
            bgSwitch->state = (u8)(bgSwitch->state + 1);
        }
        break;
    default:
        if (PaletteData_GetSelectedBuffersBitmask(bgSwitch->battleAnimSystem->paletteData) == 0) {
            return 0;
        }
        break;
    }
    return 1;
}
