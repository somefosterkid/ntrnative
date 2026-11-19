#include "global.h"

#include "overlay_74_app_templates.h"

#include "bg_window.h"
#include "heap.h"
#include "main.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokedex.h"
#include "save_arrays.h"
#include "sound_02004A44.h"
#include "overlay_74_ranger_link_manager.h"

s32 ov74_022359BC(void);

void ov74_02229190(OverlayManager *arg0);

void ov74_02229190(OverlayManager *man) {
    RangerLinkManager *rangerLinkMan;

    rangerLinkMan = OverlayManager_GetData(man);
    ov74_022359BC();
    if (WindowIsInUse(&rangerLinkMan->stdWindow) == 1) {
        ClearWindowTilemapAndCopyToVram(&rangerLinkMan->stdWindow);
        RemoveWindow(&rangerLinkMan->stdWindow);
        ClearWindowTilemapAndCopyToVram(&rangerLinkMan->messageBoxWindow);
        RemoveWindow(&rangerLinkMan->messageBoxWindow);
        ClearWindowTilemapAndCopyToVram(&rangerLinkMan->appNumberStdWindow);
        RemoveWindow(&rangerLinkMan->appNumberStdWindow);
    }
    FreeBgTilemapBuffer(rangerLinkMan->bgConfig, 0U);
    FreeBgTilemapBuffer(rangerLinkMan->bgConfig, 1U);
    FreeBgTilemapBuffer(rangerLinkMan->bgConfig, 4U);
    FreeBgTilemapBuffer(rangerLinkMan->bgConfig, 5U);
    Heap_Free(rangerLinkMan->bgConfig);
}

s32 ov74_022352A0(s32);

void *ov74_022352D0(OverlayManager *, s32, s32, s32);

BOOL ov74_02229200(OverlayManager *man, int *state) {
    SaveData *saveData;
    RangerLinkManager *rangerLinkMan;

    rangerLinkMan = ov74_022352D0(man, HEAP_ID_83, sizeof(RangerLinkManager), 0x20000);
    rangerLinkMan->heapID = HEAP_ID_83;
    rangerLinkMan->bgConfig = BgConfig_Alloc(HEAP_ID_83);
    saveData = ((UnkStruct_02111868_sub *)OverlayManager_GetArgs(man))->saveData;
    rangerLinkMan->saveData = saveData;
    rangerLinkMan->trainerInfo = Save_PlayerData_GetProfile(saveData);
    rangerLinkMan->pokedex = Save_Pokedex_Get(rangerLinkMan->saveData);
    rangerLinkMan->options = Save_PlayerData_GetOptionsAddr(rangerLinkMan->saveData);
    ov74_022352A0(rangerLinkMan->heapID);
    rangerLinkMan->mysteryGift = Save_MysteryGift_Get(rangerLinkMan->saveData);
    rangerLinkMan->unused_14C = 9;
    rangerLinkMan->nextWindowBasetile = 0x28;
    rangerLinkMan->unused_154 = 0;
    rangerLinkMan->wirelessState = 0;
    rangerLinkMan->wirelessActive = 0;
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570U);
    Sound_SetSceneAndPlayBGM(9U, 0x47EU, 1);
    return 1;
}
