#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_67_app.h"

extern u32 ov67_021E6E98;
extern u32 ov67_021E7020;

void ov67_021E6008(Ov67App *app) {
    WindowTemplate *pWVar1;
    Window *pWVar2;
    s32 iVar3;

    if (app->isRankingsScreen == HEAP_ID_DEFAULT) {
        pWVar1 = (WindowTemplate *)(&ov67_021E6E98);
        iVar3 = 0;
        pWVar2 = app->windows;
        do {
            AddWindow(app->bgConfig, pWVar2, pWVar1);
            iVar3 = iVar3 + 1;
            pWVar1 = pWVar1 + 1;
            pWVar2 = pWVar2 + 1;
        } while (iVar3 < 10);
    } else {
        pWVar1 = (WindowTemplate *)(&ov67_021E7020);
        iVar3 = 0;
        pWVar2 = app->windows;
        do {
            AddWindow(app->bgConfig, pWVar2, pWVar1);
            FillWindowPixelBuffer(pWVar2, 0);
            iVar3 = iVar3 + 1;
            pWVar1 = pWVar1 + 1;
            pWVar2 = pWVar2 + 1;
        } while (iVar3 < 0x23);
    }
    FontID_Alloc(FONT_UNK_4, app->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, app->heapID);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, app->heapID);
    return;
}
