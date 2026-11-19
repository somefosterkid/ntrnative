#include "global.h"

#include "bg_window.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "overlay_81.h"
#include "unk_02019BA4.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "render_text.h"
#include "unk_02096910.h"
#include "overlay_81_battle_factory_app.h"

s32 ov80_02237254(u8);

s32 ov81_02240D2C(void);

s32 ov81_02240D64(BattleFactoryApp *);

s32 ov81_02240F08(BattleFactoryApp *, s32);

s32 ov81_02241BB8(void *);

void ov81_02242BC8(void);

GridInputHandler *ov81_02243240(void *, u8);

GridInputHandler *ov81_022432AC(void *, u8);

GridInputHandler *ov81_022432DC(void *, u8);

FS_EXTERN_OVERLAY(OVY_80);

s32 ov81_0223DD60(OverlayManager *man, s32 *arg1) {
    s32 v1;
    BattleFactoryApp *data;
    BattleFactoryAppArgs *args;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    ov81_02240D2C();
    Heap_Create(HEAP_ID_3, HEAP_ID_100, 0x30000U);
    data = OverlayManager_CreateAndGetData(man, sizeof(BattleFactoryApp), HEAP_ID_100);
    memset(data, 0, sizeof(BattleFactoryApp));
    data->g3dPipeline = GF_3DVramMan_Create(HEAP_ID_100, 0, 2, 0, 2, ov81_02242BC8);
    data->bgConfig = BgConfig_Alloc(HEAP_ID_100);
    data->appMan = man;
    args = OverlayManager_GetArgs(man);
    data->saveData = args->saveData;
    data->challengeType = args->challengeType;
    data->isOpenLevel = args->isOpenLevel;
    data->isExchangeMode = args->isExchangeMode;
    data->personalParty = args->personalParty;
    data->receivableParty = args->receivableParty;
    data->selectedIndicesFinal = args->selectedIndices;
    data->options = Save_PlayerData_GetOptionsAddr(data->saveData);
    data->wheelPaletteCounter = 8;
    if (ov81_02240F08(data, 0) == 1) {
        data->numPokeballs = 6;
        data->inputHandler = ov81_02243240(data, 0);
    } else if (ov80_02237254(data->challengeType) == 1) {
        data->numPokeballs = 2;
        data->exchangeSelectCursorPositions = 3;
        data->numReceiveSelectCursorPositions = 6;
        data->inputHandler = ov81_022432DC(data, 0);
    } else {
        data->numPokeballs = 3;
        data->exchangeSelectCursorPositions = 4;
        data->numReceiveSelectCursorPositions = 5;
        data->inputHandler = ov81_022432AC(data, 0);
    }
    if (ov80_02237254(data->challengeType) == 1) {
        v1 = 2;
        data->numMonsToFade = v1;
    } else {
        v1 = 3;
        data->numMonsToFade = v1;
    }
    ov81_02241BB8(&data->yesNoPrompt);
    ov81_02240D64(data);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    if (ov80_02237254(data->challengeType) == 1) {
        sub_02096910(data);
    }
    *arg1 = 0;
    return 1;
}
