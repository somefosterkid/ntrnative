#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"
#include "main.h"
#include "gf_gfx_planes.h"

FS_EXTERN_OVERLAY(OVY_74);
extern WonderCardsAppWindowTemplate sWonderCardsAppWindows[NUM_WC_APP_WINDOWS];
extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_MysteryGift;
BOOL ov74_0222EB44(OverlayManager *appMan, int *unused);
BOOL ov74_0222EB44(OverlayManager *appMan, int *unused) {
    WonderCardsAppData *appData = OverlayManager_GetData(appMan);

    for (int i = 0; i < (sizeof(sWonderCardsAppWindows) / sizeof(*(sWonderCardsAppWindows))); i++) {
        if (appData->windows[i].bgConfig) {
            ClearWindowTilemapAndCopyToVram(&appData->windows[i]);
            RemoveWindow(&appData->windows[i]);
        }
    }

    if (appData->messageBox.bgConfig) {
        ClearWindowTilemapAndCopyToVram(&appData->messageBox);
        RemoveWindow(&appData->messageBox);
    }

    if (appData->standardWindow.bgConfig) {
        ClearWindowTilemapAndCopyToVram(&appData->standardWindow);
        RemoveWindow(&appData->standardWindow);
    }

    FreeBgTilemapBuffer(appData->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(appData->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(appData->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(appData->bgConfig, GF_BG_LYR_MAIN_3);
    Heap_Free(appData->bgConfig);
    RegisterMainOverlay(FS_OVERLAY_ID(OVY_74), &gApp_MainMenu_SelectOption_MysteryGift);
    Heap_Destroy(HEAP_ID_89);
    OverlayManager_FreeData(appMan);
    Heap_Destroy((enum HeapID)85);

    return 1;
}
