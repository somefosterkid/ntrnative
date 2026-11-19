#include "global.h"

#include "battle/battle_system.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_switch.h"
#include "palette.h"
#include "sys_task_api.h"

s32 ov07_0221BFC0(void *);

void ov07_0221DEC0(BattleBgSwitch *bgSwitch);

void ov07_0221E664(BattleBgSwitch *bgSwitch);

s32 ov07_0221EC7C(void *, s32);

s32 ov07_0221FB04(BattleAnimSystem *, u32);

s32 ov07_0221FB30(void *, s32);

s32 ov07_0221FB58(void *);

BOOL ov07_0221E0B4(SysTask *task, BattleBgSwitch *bgSwitch);

BOOL ov07_0221E0B4(SysTask *task, BattleBgSwitch *bgSwitch) {
    s32 notReady;
    u8 state;
    u8 blendCoeffA;
    u8 blendCoeffB;
    s32 priorityBg3;
    s32 priorityBg2;
    BattleAnimSystem *system;
    state = bgSwitch->state;
    switch ((u32)state) {
    case 0:
        ToggleBgLayer(2U, 1U);
        bgSwitch->state = (u8)(bgSwitch->state + 1);

    case 1:
        priorityBg3 = ov07_0221FB04(bgSwitch->battleAnimSystem, 2);
        priorityBg2 = ov07_0221FB04(bgSwitch->battleAnimSystem, 1);
        SetBgPriority(3U, (u8)priorityBg3);
        SetBgPriority(2U, priorityBg2);
        G2_SetBlendAlpha(4, 8, (s32)bgSwitch->blendCoeffA, (s32)bgSwitch->blendCoeffB);
        ov07_0221DEC0(bgSwitch);
        bgSwitch->state = (u8)(bgSwitch->state + 1);

    case 2:
        blendCoeffA = bgSwitch->blendCoeffA;
        notReady = 0;
        if (((u32)blendCoeffA) < ((u32)bgSwitch->targetBlendCoeffA)) {
            bgSwitch->blendCoeffA = (u8)(blendCoeffA + 2);
        } else {
            notReady += 1;
        }
        blendCoeffB = bgSwitch->blendCoeffB;
        if (((u32)blendCoeffB) > ((u32)bgSwitch->targetBlendCoeffB)) {
            bgSwitch->blendCoeffB = (u8)(blendCoeffB - 2);
        } else {
            notReady += 1;
        }
        if (notReady == 2) {
            bgSwitch->blendCoeffA = (u8)(bgSwitch->targetBlendCoeffA + 2);
            bgSwitch->blendCoeffB = (u8)(bgSwitch->targetBlendCoeffB - 2);
            bgSwitch->state = (u8)(bgSwitch->state + 1);
        }
        reg_G2_BLDALPHA = bgSwitch->blendCoeffA | (bgSwitch->blendCoeffB << 8);
        break;

    case 3:
        ov07_0221E664(bgSwitch);
        BgSetPosTextAndCommit(bgSwitch->battleAnimSystem->bgConfig, 3U, BG_POS_OP_SET_ROT, 0);
        BgSetPosTextAndCommit(bgSwitch->battleAnimSystem->bgConfig, 3U, BG_POS_OP_SET_XSCALE, 0);
        SetBgControlParam(bgSwitch->battleAnimSystem->bgConfig, 3U, GF_BG_CNT_SET_CHAR_BASE, 4U);
        if (ov07_0221BFC0(bgSwitch->battleAnimSystem) == 0) {
            SetBgControlParam(bgSwitch->battleAnimSystem->bgConfig, 3U, GF_BG_CNT_SET_COLOR_MODE, 1U);
            ov07_0221FB30(bgSwitch->battleAnimSystem, 3);
            ov07_0221FB58(bgSwitch->battleAnimSystem);
        } else {
            system = bgSwitch->battleAnimSystem;
            GfGfxLoader_LoadCharData(system->battleBgRefs.narcID, system->battleBgRefs.tilesNarcMemberIdx, system->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, (enum HeapID)system->heapID);
            system = bgSwitch->battleAnimSystem;
            PaletteData_LoadNarc(system->paletteData, system->battleBgRefs.narcID, system->battleBgRefs.paletteNarcMemberIdx, (enum HeapID)system->heapID, PLTTBUF_MAIN_BG, system->battleBgRefs.paletteSrcSize << 5, (u16)((u16)system->battleBgRefs.paletteDestStart));
        }
        system = bgSwitch->battleAnimSystem;
        GfGfxLoader_LoadScrnData(system->battleBgRefs.narcID, system->battleBgRefs.tilemapNarcMemberIdx, system->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 1, (enum HeapID)system->heapID);
        bgSwitch->state = (u8)(bgSwitch->state + 1);
        break;

    case 4:
        BattleSystem_SetDefaultBlend();
        ToggleBgLayer(2U, 0U);
        ov07_0221EC7C(bgSwitch->battleAnimSystem, 2);
        bgSwitch->state = (u8)(bgSwitch->state + 1);
        break;

    default:
        return 0;
    }

    return 1;
}
