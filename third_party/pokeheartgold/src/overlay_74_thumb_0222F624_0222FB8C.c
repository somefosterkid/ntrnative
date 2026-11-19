#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "application/main_menu/main_menu.h"
#include "main.h"
#include "overlay_74_app_templates.h"
#include "overlay_74_wonder_cards_app2_data.h"
#include "overlay_manager.h"

extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_MysteryGift;

BOOL ov74_0222FB8C(OverlayManager *man, int *state) {
    WonderCardsApp2Data *appData = OverlayManager_GetData(man);
    u32 i;

    for (i = 0; i < 11; i++) {
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
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_85);
    return TRUE;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// asm/overlay_74_thumb_0222B3A4_0223B410.s keeps the DS copy (three .word SYMBOL then the literal overlay id), which the
// 64-bit port emits as 8-byte pointers + a 4-byte word (wordmix.py): the PC build uses this C definition instead.
const OverlayManagerTemplate _0223B420 = {
    ov74_0222F598,
    ov74_0222F7D4,
    ov74_0222FB8C,
    FS_OVERLAY_ID_NONE,
};
#endif
