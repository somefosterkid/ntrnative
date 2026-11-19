#include "global.h"

#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "unk_0202D230.h"
#include "unk_02037C94.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov73_021E83EC(void *, s32, s32);

s32 ov73_021E8628(BgConfig *);

s32 ov73_021E8730(void *);

s32 ov73_021E8A08(void *);

s32 ov73_021E8B64(WifiBattleTowerAppState *arg0);

s32 ov73_021E9FF8(void *, MsgData *, s32, s32, s32);

SysTask *ov73_021EA12C(void *);

s32 ov73_021EA15C(void *);

s32 ov73_021E84D0(WifiBattleTowerAppState *arg0);

s32 ov73_021E84D0(WifiBattleTowerAppState *arg0) {
    ov73_021E8B64(arg0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 8, 1, HEAP_ID_150);
    ov73_021E8628(arg0->bgConfig);
    ov73_021E8730(arg0);
    ov73_021E8A08(arg0);
    GfGfx_EngineATogglePlanes(1U, 1U);
    GfGfx_EngineATogglePlanes(2U, 1U);
    GfGfx_EngineBTogglePlanes(1U, 1U);
    GfGfx_EngineBTogglePlanes(2U, 1U);
    sub_02039418(arg0->args->saveData);
    ov73_021EA12C(arg0);
    arg0->playerRank = sub_0202D488(arg0->args->frontierData, 0U);
    arg0->selectedRank = arg0->playerRank;
    arg0->selectedOpponentIdx = 1;
    arg0->waitDial = NULL;
    if (DWC_CheckInet() == 0) {
        if (arg0->args->unk24 != 0) {
            ov73_021E9FF8(arg0, arg0->msgLoader2, 1, 1, 0xF0F);
            ov73_021E83EC(arg0, 0x2F, 2);
            ov73_021EA15C(arg0);
        } else {
            arg0->state = NINTENDO_WFC_STATE_ASK_CONNECT;
        }
    } else {
        arg0->state = NINTENDO_WFC_STATE_ASK_LOG_OUT;
    }
    return 2;
}
