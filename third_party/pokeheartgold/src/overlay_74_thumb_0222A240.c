#include "global.h"

#include "bg_window.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_manager.h"
#include "render_window.h"
#include "unk_0203A3B0.h"

s32 ov74_0222A89C(OverlayManager *, s32 *, s32, Window *, s32);

extern s32 ov74_0223C260;

void ov74_0222A240(OverlayManager *arg0, s32 arg1);

void ov74_02229E28(MysteryGiftAppData *appData, BOOL show);
void ov74_02229F60(Window *window, u8 skipTransfer);
s32 ov74_0222A5CC(OverlayManager *);
void ov74_0222A2A4(OverlayManager *arg0);
s32 ov74_02235230(void);
extern s32 ov74_0223C250;
s32 ov74_0222A2E0(OverlayManager *arg0);
extern s32 ov74_0223C220;
s32 ov74_0222A35C(OverlayManager *arg0);
extern s32 ov74_0223C230;
s32 ov74_0222A3CC(OverlayManager *arg0);
extern s32 ov74_0223C240;
void ov74_0222A43C(OverlayManager *arg0);
s32 ov74_02229DF8(void);
void ov74_0222A494(OverlayManager *arg0);

void ov74_0222A240(OverlayManager *man, s32 arg1) {
    Window *window;
    s32 v1;
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    if (arg1 == 0) {
        v1 = 4;
    } else {
        v1 = -1;
    }
    window = &v0->menuWindows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(v0->bgConfig, window, 0U, 0x17U, 0xAU, 6U, 4U, 0U, 0x1ACU);
    }
    DrawFrameAndWindow1(window, 1, 0x1FU, 3U);
    ov74_0222A89C(man, &ov74_0223C260, 2, window, v1);
}

void ov74_0222A2A4(OverlayManager *man) {
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    ov74_02229F60(&v0->menuWindows[2], 0);
    ov74_02229F60(&v0->wonderCardTitleWindow, 0);
    if (v0->receptionMethod == 1) {
        ov74_02229E28(v0, 0);
    }
    sub_0203A914();
    ov74_0222A5CC(man);
}

s32 ov74_0222A2E0(OverlayManager *man) {
    Window *window;
    s32 v1;
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    if (ov74_02235230() != 0) {
        v1 = 2;
    } else {
        v1 = 1;
    }
    v0->receptionMethod = v1;
    ov74_02229F60(&v0->menuWindows[1], 0);
    window = &v0->menuWindows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(v0->bgConfig, window, 0U, 0x17U, 0xAU, 6U, 4U, 0U, 0x1ACU);
    }
    DrawFrameAndWindow1(window, 1, 0x1FU, 3U);
    ov74_0222A89C(man, &ov74_0223C250, 2, window, 2);
    return 5;
}

s32 ov74_0222A35C(OverlayManager *man) {
    Window *window;
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    v0->receptionMethod = 4;
    ov74_02229F60(&v0->menuWindows[1], 0);
    window = &v0->menuWindows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(v0->bgConfig, window, 0U, 0x17U, 0xAU, 6U, 4U, 0U, 0x1ACU);
    }
    DrawFrameAndWindow1(window, 1, 0x1FU, 3U);
    ov74_0222A89C(man, &ov74_0223C220, 2, window, 0x50);
    return 5;
}

s32 ov74_0222A3CC(OverlayManager *man) {
    Window *window;
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    v0->receptionMethod = 3;
    ov74_02229F60(&v0->menuWindows[1], 0);
    window = &v0->menuWindows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(v0->bgConfig, window, 0U, 0x17U, 0xAU, 6U, 4U, 0U, 0x1ACU);
    }
    DrawFrameAndWindow1(window, 1, 0x1FU, 3U);
    ov74_0222A89C(man, &ov74_0223C230, 2, window, 0x4F);
    return 5;
}

void ov74_0222A43C(OverlayManager *man) {
    Window *window;
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    window = &v0->menuWindows[2];
    if (window->bgConfig == NULL) {
        AddWindowParameterized(v0->bgConfig, window, 0U, 0x17U, 0xAU, 6U, 4U, 0U, 0x1ACU);
    }
    DrawFrameAndWindow1(window, 1, 0x1FU, 3U);
    ov74_0222A89C(man, &ov74_0223C240, 2, window, 4);
}

void ov74_0222A494(OverlayManager *man) {
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    ov74_02229F60(&v0->menuWindows[2], 0);
    ov74_02229F60(&v0->wonderCardTitleWindow, 0);
    sub_0203A914();
    if (v0->receptionMethod == 1) {
        ov74_02229DF8();
    }
    ov74_0222A5CC(man);
}
