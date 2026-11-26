#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "overlay_manager.h"
#include "overlay_74_mystery_gift_app_data.h"

void ov74_022359BC(void);

void ov74_02229E68(OverlayManager *param_1);

s32 ov74_0223563C(void);
s32 ov74_02235690();
s32 ov74_02235728(s32, s32, s32, s32, s32, s32);
void ov74_02229F04(void);

void ov74_02229E68(OverlayManager *param_1) {
    MysteryGiftAppData *appData;
    BOOL BVar2;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    u32 *puVar3;
#endif
    Window *window;
    s32 iVar4;

    appData = OverlayManager_GetData(param_1);
    ov74_022359BC();
    if (appData->listMenuOptions != NULL) {
        ListMenuItems_Delete(appData->listMenuOptions);
    }
    if (appData->listMenu != NULL) {
        DestroyListMenu(appData->listMenu, (u16 *)0x0, (u16 *)0x0);
    }
    ClearWindowTilemapAndCopyToVram(&appData->messageBox);
    RemoveWindow(&appData->messageBox);
    BVar2 = WindowIsInUse(&appData->wonderCardTitleWindow);
    if (BVar2 != 0) {
        ClearWindowTilemapAndCopyToVram(&appData->wonderCardTitleWindow);
        RemoveWindow(&appData->wonderCardTitleWindow);
    }
    iVar4 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    window = appData->menuWindows;
    do {
        if (window->bgConfig != NULL) {
            ClearWindowTilemapAndCopyToVram(window);
            RemoveWindow(window);
        }
        iVar4 = iVar4 + 1;
        window = window + 1;
    } while (iVar4 < 3);
#else
    puVar3 = (u32 *)appData;
    window = appData->menuWindows;
    do {
        if (puVar3[10] != 0) {
            ClearWindowTilemapAndCopyToVram(window);
            RemoveWindow(window);
        }
        iVar4 = iVar4 + 1;
        puVar3 = puVar3 + 4;
        window = window + 1;
    } while (iVar4 < 3);
#endif
    FreeBgTilemapBuffer(appData->bgConfig, 0);
    FreeBgTilemapBuffer(appData->bgConfig, 1);
    FreeBgTilemapBuffer(appData->bgConfig, 4);
    FreeBgTilemapBuffer(appData->bgConfig, 5);
    Heap_Free(appData->bgConfig);
    return;
}

void ov74_02229F04(void) {
    ov74_0223563C();
    ov74_02235690();
    ov74_02235728(0x71, 0xE, 0xB, 0xD, 0xC, 0);
}
