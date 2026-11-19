#include "global.h"

#include "bg_window.h"
#include "overlay_31.h"



extern u32 ov31_0225EE88;


void ov31_0225DBD4(MartSubScreen *arg0) {
    Window *v0;
    s32 v1;
    MartSubScreen *v2;
    u8 *v3;
    v2 = arg0;
    AddWindowParameterized(v2->bgConfig, &v2->unk_34, 4U, 2U, 1U, 0x1BU, 2U, 0xBU, 1U);
    AddWindowParameterized(v2->bgConfig, &v2->msgWindow, 4U, 2U, 1U, 0x1BU, 4U, 0xBU, 0x1DU);
    AddWindowParameterized(v2->bgConfig, &v2->moneyWindow, 4U, 1U, 0U, 9U, 4U, 0xBU, 0x89U);
    AddWindowParameterized(v2->bgConfig, &v2->pageWindow, 4U, 0xAU, 0x15U, 7U, 2U, 0xCU, 1U);
    FillWindowPixelBuffer(&v2->pageWindow, 0U);
    AddWindowParameterized(v2->bgConfig, &v2->unk_64, 4U, 0x18U, 0x15U, 7U, 2U, 0xCU, 0xFU);
    FillWindowPixelBuffer(&v2->unk_64, 0U);
    v1 = 0;
    v3 = (u8 *)v2;
    do {
        v0 = (Window *)(v3 + offsetof(MartSubScreen, windows));
        v1 += 1;
        v0->bgConfig = NULL;
        v3 += sizeof(Window);
    } while (v1 < MART_SUBSCREEN_WIN_COUNT);
}

void ov31_0225DCA8(MartSubScreen *param_1) {
    u32 *puVar1;
    Window *window;
    s32 iVar2;

    puVar1 = (u32 *)&ov31_0225EE88;
    iVar2 = 0;
    window = &param_1->windows[0];
    do {
        AddWindowParameterized(param_1->bgConfig, window, 4, (u8)*puVar1, (u8)puVar1[1], 0xb, 4, 0xc, (u16)puVar1[2]);
        iVar2 = iVar2 + 1;
        puVar1 = puVar1 + 3;
        window = window + 1;
    } while (iVar2 < 6);
    return;
}

void ov31_0225DCF4(MartSubScreen *param_1) {
    s32 iVar1;
    Window *window;

    iVar1 = 0;
    window = &param_1->windows[0];
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 6);
    return;
}
