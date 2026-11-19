#include "global.h"

#include "heap.h"
#include "screen_fade.h"
#include "sys_task_api.h"
#include "system.h"
#include "screen_fade_internal.h"
/* unk_0200FA24.h (it declares IsPaletteFadeFinished as s32, screen_fade.h as BOOL): its other prototypes */
void sub_0200FC20(u16 arg0);
void sub_0200FE14(u32 arg0, void *arg1);
void sub_0200FF88(ScreenFadeHBlanks *hblanks, void *data, Callback callback, enum DSScreen screen, enum HeapID heapID);
void sub_0200FFB4(void *arg0, s32 arg1, enum HeapID arg2);

enum ScreenFadeOrder {
    ORDER_SIMULTANEOUS,
    ORDER_MAIN_FIRST,
    ORDER_SUB_FIRST
};

typedef struct ScreenFadeParams {
    enum ScreenFadeOrder order;
    BOOL activeMain;
    BOOL activeSub;
    BOOL existsMain;
    BOOL existsSub;
} ScreenFadeParams;

typedef struct ScreenFadeManager {
    ScreenFadeParams screen;
    ScreenFade mainScreenFade;
    ScreenFade subScreenFade;
    ScreenFadeHBlanks hblanks;
    HardwareWindowSettings hwSettings;
    u16 active;
    u8 dummy_14E;
    u8 dummy_14F;
    u16 savedColor;
} ScreenFadeManager;

typedef struct HBlankStartArgs {
    ScreenFadeHBlanks *hblanks;
    void *data;
    Callback callback;
    enum DSScreen screen;
} HBlankStartArgs;

typedef struct HBlankStopArgs {
    ScreenFadeHBlanks *hblanks;
    enum DSScreen screen;
} HBlankStopArgs;

typedef BOOL (*ScreenFadeFunc)(void *fade);

enum {
    GX_BLEND_BGALL = GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3,
    GX_BLEND_ALL = GX_BLEND_BGALL | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD,
};

BOOL FadeFunc_00(void *fade);
BOOL FadeFunc_01(void *fade);
BOOL FadeFunc_02(void *fade);
BOOL FadeFunc_03(void *fade);
BOOL FadeFunc_04(void *fade);
BOOL FadeFunc_05(void *fade);
BOOL FadeFunc_06(void *fade);
BOOL FadeFunc_07(void *fade);
BOOL FadeFunc_08(void *fade);
BOOL FadeFunc_09(void *fade);
BOOL FadeFunc_10(void *fade);
BOOL FadeFunc_11(void *fade);
BOOL FadeFunc_12(void *fade);
BOOL FadeFunc_13(void *fade);
BOOL FadeFunc_14(void *fade);
BOOL FadeFunc_15(void *fade);
BOOL FadeFunc_16(void *fade);
BOOL FadeFunc_17(void *fade);
BOOL FadeFunc_18(void *fade);
BOOL FadeFunc_19(void *fade);
BOOL FadeFunc_20(void *fade);
BOOL FadeFunc_21(void *fade);
BOOL FadeFunc_22(void *fade);
BOOL FadeFunc_23(void *fade);
BOOL FadeFunc_24(void *fade);
BOOL FadeFunc_25(void *fade);
BOOL FadeFunc_26(void *fade);
BOOL FadeFunc_27(void *fade);
BOOL FadeFunc_28(void *fade);
BOOL FadeFunc_29(void *fade);
BOOL FadeFunc_30(void *fade);
BOOL FadeFunc_31(void *fade);
BOOL FadeFunc_32(void *fade);
BOOL FadeFunc_33(void *fade);
BOOL FadeFunc_34(void *fade);
BOOL FadeFunc_35(void *fade);
BOOL FadeFunc_36(void *fade);
BOOL FadeFunc_37(void *fade);
BOOL FadeFunc_38(void *fade);
BOOL FadeFunc_39(void *fade);
BOOL FadeFunc_40(void *fade);
BOOL FadeFunc_41(void *fade);
BOOL FadeFunc_42(void *fade);

void sub_020131F4(int visible, enum DSScreen screen);
void sub_02013424(HardwareWindowSettings *settings, GXWndMask windowMask, enum DSScreen screen);
void sub_02013440(HardwareWindowSettings *settings, int wnd, BOOL applyColorEffect, enum HardwareWindow windowID, enum DSScreen screen);
void sub_02013468(HardwareWindowSettings *settings, int wnd, BOOL applyColorEffect, enum DSScreen screen);
void sub_02013488(HardwareWindowSettings *settings, int left, int top, int right, int bottom, enum HardwareWindow windowID, enum DSScreen screen);

void SetMasterBrightness(enum DSScreen screen, int brightness);
void sub_0200FC60(enum DSScreen screen, u16 color);
void sub_0200FCDC(u16 color);
void HandleEndFade(ScreenFadeManager *manager);
BOOL DoFadeUpdateFrame(ScreenFadeParams *screen, ScreenFade *fadeMain, ScreenFade *fadeSub);
void FadeWork_UpdateFrame(BOOL *active, ScreenFade *fade);
BOOL CallFadeFunc(ScreenFade *fade);
void sub_0200FE78(ScreenFadeParams *params, enum ScreenFadeOrder order, BOOL fadeMain, BOOL fadeSub);
void sub_0200FE84(ScreenFade *fade, enum FadeType type, int steps, int framesPerStep, enum FadeState state, void *data, enum DSScreen screen, HardwareWindowSettings *hwSettings, ScreenFadeHBlanks *hblanks, enum HeapID heapID, u16 color);
void sub_0200FEB0(ScreenFadeHBlanks *hblanks);
static void sub_0200FECC(void *data);
void sub_0200FEE4(ScreenFadeHBlanks *hblanks, void *data, Callback callback, enum DSScreen screen);
void sub_0200FF5C(ScreenFadeHBlanks *hblanks, enum DSScreen screen);
void sub_0200FFD8(SysTask *task, void *data);
void sub_0200FFF8(SysTask *task, void *data);
void sub_02010014(void *data);
u16 sub_02010018(ScreenFadeManager *manager, u16 color);
u16 sub_0201002C(const ScreenFadeManager *manager);
void sub_02010050(SysTask *task, void *data);
void sub_02010064(ScreenFade *fade);
void sub_02010094(ScreenFade *fade);
void sub_020100C4(ScreenFadeManager *manager);

static const ScreenFadeFunc sFadeFuncPtrs[] = {
    FadeFunc_00,
    FadeFunc_01,
    FadeFunc_02,
    FadeFunc_03,
    FadeFunc_04,
    FadeFunc_05,
    FadeFunc_06,
    FadeFunc_07,
    FadeFunc_08,
    FadeFunc_09,
    FadeFunc_10,
    FadeFunc_11,
    FadeFunc_12,
    FadeFunc_13,
    FadeFunc_14,
    FadeFunc_15,
    FadeFunc_16,
    FadeFunc_17,
    FadeFunc_18,
    FadeFunc_19,
    FadeFunc_20,
    FadeFunc_21,
    FadeFunc_22,
    FadeFunc_23,
    FadeFunc_24,
    FadeFunc_25,
    FadeFunc_26,
    FadeFunc_27,
    FadeFunc_28,
    FadeFunc_29,
    FadeFunc_30,
    FadeFunc_31,
    FadeFunc_32,
    FadeFunc_33,
    FadeFunc_34,
    FadeFunc_35,
    FadeFunc_36,
    FadeFunc_37,
    FadeFunc_38,
    FadeFunc_39,
    FadeFunc_40,
    FadeFunc_41,
    FadeFunc_42,
};

static ScreenFadeManager _021D0EF4;

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID) {
    GF_ASSERT(steps);
    GF_ASSERT(framesPerStep);
    GF_ASSERT(_021D0EF4.active == FALSE);
    ScreenFadeManager *manager = &_021D0EF4;
    sub_020100C4(manager);
    sub_0200FE14(mode, (u8 *)&manager->screen);
    sub_0200FEB0(&manager->hblanks);
    u16 fadeColor = sub_02010018(manager, color);
    sub_0200FE84(&manager->mainScreenFade, typeMain, steps, framesPerStep, FADE_IDLE, NULL, DS_SCREEN_MAIN, &manager->hwSettings, &manager->hblanks, heapID, fadeColor);
    sub_0200FE84(&manager->subScreenFade, typeSub, steps, framesPerStep, FADE_IDLE, NULL, DS_SCREEN_SUB, &manager->hwSettings, &manager->hblanks, heapID, fadeColor);
    manager->active = TRUE;
    FadeWork_UpdateFrame(&manager->screen.activeMain, &manager->mainScreenFade);
    FadeWork_UpdateFrame(&manager->screen.activeSub, &manager->subScreenFade);
    if (manager->screen.existsMain) {
        sub_02010064(&manager->mainScreenFade);
        manager->dummy_14E = 1;
    }
    if (manager->screen.existsSub) {
        sub_02010064(&manager->subScreenFade);
        manager->dummy_14F = 1;
    }
}

void HandleFadeUpdateFrame(void) {
    ScreenFadeManager *manager = &_021D0EF4;
    if (manager->active && DoFadeUpdateFrame(&manager->screen, &manager->mainScreenFade, &manager->subScreenFade) == TRUE) {
        HandleEndFade(manager);
    }
}

BOOL IsPaletteFadeFinished(void) {
    if (_021D0EF4.active == 0) {
        return 1;
    }
    return 0;
}

void sub_0200FB70(void) {
    sub_0200FF5C(&_021D0EF4.hblanks, DS_SCREEN_MAIN);
    sub_0200FF5C(&_021D0EF4.hblanks, DS_SCREEN_SUB);
    if (_021D0EF4.screen.activeMain) {
        _021D0EF4.mainScreenFade.state = FADE_CLEANUP;
    }
    if (_021D0EF4.screen.activeSub) {
        _021D0EF4.subScreenFade.state = FADE_CLEANUP;
    }
    FadeWork_UpdateFrame(&_021D0EF4.screen.activeMain, &_021D0EF4.mainScreenFade);
    FadeWork_UpdateFrame(&_021D0EF4.screen.activeSub, &_021D0EF4.subScreenFade);
    _021D0EF4.active = 0;
    _021D0EF4.dummy_14E = 0;
    _021D0EF4.dummy_14F = 0;
    sub_020100C4(&_021D0EF4);
}

void ResetVisibleHardwareWindows(PMLCDTarget screen) {
    sub_020131F4(0, (enum DSScreen)screen);
}

void SetMasterBrightnessNeutral(PMLCDTarget screen) {
    SetMasterBrightness((enum DSScreen)screen, 0);
}

void sub_0200FBF4(PMLCDTarget screen, u16 color) {
    s32 v1;
    u16 v0;
    v0 = color;
    if (v0 == 0xFFFF) {
        v0 = _021D0EF4.savedColor;
    }
    if (v0 == 0x7FFF) {
        v1 = 0x10;
    } else {
        v1 = -0x10;
    }
    SetMasterBrightness((enum DSScreen)screen, v1);
}

void sub_0200FC20(u16 arg0) {
    s32 v1;
    u16 v0;
    v0 = arg0;
    if (v0 == 0xFFFF) {
        v0 = _021D0EF4.savedColor;
    }
    if (v0 == 0x7FFF) {
        v1 = 0x10;
    } else {
        v1 = -0x10;
    }
    SetMasterBrightness(DS_SCREEN_MAIN, v1);
    SetMasterBrightness(DS_SCREEN_SUB, v1);
    _021D0EF4.savedColor = v0;
}

void sub_0200FC60(enum DSScreen screen, u16 color) {
    if (color == 0xFFFF) {
        color = _021D0EF4.savedColor;
    }
    if (screen == DS_SCREEN_MAIN) {
        GX_LoadBGPltt((void *)&color, 0, sizeof(u16));
    } else {
        GXS_LoadBGPltt((void *)&color, 0, sizeof(u16));
    }
    sub_02013424(&_021D0EF4.hwSettings, GX_WNDMASK_W0, screen);
    sub_02013440(&_021D0EF4.hwSettings, GX_BLEND_ALL, 0, HW_WINDOW_WND0, screen);
    sub_02013488(&_021D0EF4.hwSettings, 0, 0, 0, 0, HW_WINDOW_WND0, screen);
    sub_02013468(&_021D0EF4.hwSettings, GX_BLEND_PLANEMASK_BD, 0, screen);
}

void sub_0200FCDC(u16 color) {
    GX_LoadBGPltt((void *)&color, 0, sizeof(u16));
    GXS_LoadBGPltt((void *)&color, 0, sizeof(u16));
}

void SetMasterBrightness(enum DSScreen screen, int brightness) {
    if (screen == DS_SCREEN_MAIN) {
        GX_SetMasterBrightness(brightness);
    } else {
        GXS_SetMasterBrightness(brightness);
    }
}

void HandleEndFade(ScreenFadeManager *manager) {
    manager->active = FALSE;
    manager->savedColor = sub_0201002C(manager);
    if (manager->screen.existsMain) {
        sub_02010094(&manager->mainScreenFade);
        if (manager->mainScreenFade.direction == FADE_IN) {
            _021D0EF4.dummy_14E = 0;
        }
    }
    if (manager->screen.existsSub) {
        sub_02010094(&manager->subScreenFade);
        if (manager->mainScreenFade.direction == FADE_IN) {
            _021D0EF4.dummy_14F = 0;
        }
    }
    sub_020100C4(manager);
}

BOOL DoFadeUpdateFrame(ScreenFadeParams *screen, ScreenFade *fadeMain, ScreenFade *fadeSub) {
    switch (screen->order) {
    case ORDER_SIMULTANEOUS:
        FadeWork_UpdateFrame(&screen->activeMain, fadeMain);
        FadeWork_UpdateFrame(&screen->activeSub, fadeSub);
        break;
    case ORDER_MAIN_FIRST:
        if (screen->activeMain) {
            FadeWork_UpdateFrame(&screen->activeMain, fadeMain);
        } else {
            FadeWork_UpdateFrame(&screen->activeSub, fadeSub);
        }
        break;
    case ORDER_SUB_FIRST:
        if (screen->activeSub) {
            FadeWork_UpdateFrame(&screen->activeSub, fadeSub);
        } else {
            FadeWork_UpdateFrame(&screen->activeMain, fadeMain);
        }
        break;
    }
    if (screen->activeMain == 0 && screen->activeSub == 0) {
        return 1;
    }
    return 0;
}

void FadeWork_UpdateFrame(BOOL *active, ScreenFade *fade) {
    if ((*active != 0) && (CallFadeFunc(fade) == 1)) {
        *active = 0;
    }
}

BOOL CallFadeFunc(ScreenFade *fade) {
    return sFadeFuncPtrs[fade->type](fade);
}

void sub_0200FE14(u32 mode, void *params) {
    switch (mode) {
    case 0:
        sub_0200FE78((ScreenFadeParams *)params, ORDER_SIMULTANEOUS, 1, 1);
        return;
    case 1:
        sub_0200FE78((ScreenFadeParams *)params, ORDER_MAIN_FIRST, 1, 1);
        return;
    case 2:
        sub_0200FE78((ScreenFadeParams *)params, ORDER_SUB_FIRST, 1, 1);
        return;
    case 3:
        sub_0200FE78((ScreenFadeParams *)params, ORDER_MAIN_FIRST, 1, 0);
        return;
    case 4:
        sub_0200FE78((ScreenFadeParams *)params, ORDER_SUB_FIRST, 0, 1);
    default:
        return;
    }
}

void sub_0200FE78(ScreenFadeParams *params, enum ScreenFadeOrder order, BOOL fadeMain, BOOL fadeSub) {
    params->order = order;
    params->activeMain = fadeMain;
    params->activeSub = fadeSub;
    params->existsMain = fadeMain;
    params->existsSub = fadeSub;
}

void sub_0200FE84(ScreenFade *fade, enum FadeType type, int steps, int framesPerStep, enum FadeState state, void *data, enum DSScreen screen, HardwareWindowSettings *hwSettings, ScreenFadeHBlanks *hblanks, enum HeapID heapID, u16 color) {
    fade->type = type;
    fade->steps = steps;
    fade->framesPerStep = framesPerStep;
    fade->state = state;
    fade->data = data;
    fade->screen = screen;
    fade->hwSettings = hwSettings;
    fade->hblanks = hblanks;
    fade->heapID = heapID;
    fade->color = color;
}

void sub_0200FEB0(ScreenFadeHBlanks *hblanks) {
    for (int screen = 0; screen < DS_SCREEN_MAX; screen++) {
        hblanks->data[screen] = NULL;
        hblanks->callback[screen] = sub_02010014;
        hblanks->running[screen] = 0;
    }
}

static void sub_0200FECC(void *data) {
    ScreenFadeHBlanks *hblanks = data;
    for (int screen = 0; screen < DS_SCREEN_MAX; screen++) {
        hblanks->callback[screen](hblanks->data[screen]);
    }
}

void sub_0200FEE4(ScreenFadeHBlanks *hblanks, void *data, Callback callback, enum DSScreen screen) {
    u8 validCallback = 1;
    GF_ASSERT(hblanks->running[screen] == 0);
    GF_ASSERT(hblanks->callback[screen] != NULL);
    if (hblanks->running[DS_SCREEN_MAIN] == 0 && hblanks->running[DS_SCREEN_SUB] == 0) {
        validCallback = Main_SetHBlankIntrCB(sub_0200FECC, hblanks);
    }
    GF_ASSERT(validCallback == 1);
    hblanks->data[screen] = data;
    if (callback) {
        hblanks->callback[screen] = callback;
    } else {
        hblanks->callback[screen] = sub_02010014;
    }
    hblanks->running[screen] = 1;
}

void sub_0200FF5C(ScreenFadeHBlanks *hblanks, enum DSScreen screen) {
    hblanks->running[screen] = 0;
    if (hblanks->running[DS_SCREEN_MAIN] == 0 && hblanks->running[DS_SCREEN_SUB] == 0) {
        HBlankInterruptDisable();
    }
    hblanks->callback[screen] = sub_02010014;
    hblanks->data[screen] = NULL;
}

void sub_0200FF88(ScreenFadeHBlanks *hblanks, void *data, Callback callback, enum DSScreen screen, enum HeapID heapID) {
    HBlankStartArgs *args = Heap_AllocAtEnd(heapID, sizeof(HBlankStartArgs));
    args->hblanks = hblanks;
    args->data = data;
    args->callback = callback;
    args->screen = screen;
    SysTask_CreateOnVWaitQueue(sub_0200FFD8, args, 0x400);
}

void sub_0200FFB4(void *arg0, s32 arg1, enum HeapID heapID) {
    HBlankStopArgs *args = Heap_AllocAtEnd(heapID, sizeof(HBlankStopArgs));
    args->hblanks = (ScreenFadeHBlanks *)arg0;
    args->screen = (enum DSScreen)arg1;
    SysTask_CreateOnVWaitQueue(sub_0200FFF8, args, 0x400);
}

void sub_0200FFD8(SysTask *task, void *data) {
    HBlankStartArgs *args = data;
    sub_0200FEE4(args->hblanks, args->data, args->callback, args->screen);
    SysTask_Destroy(task);
    Heap_Free(data);
}

void sub_0200FFF8(SysTask *task, void *data) {
    HBlankStopArgs *args = data;
    sub_0200FF5C(args->hblanks, args->screen);
    SysTask_Destroy(task);
    Heap_Free(data);
}

void sub_02010014(void *data) {
}

u16 sub_02010018(ScreenFadeManager *manager, u16 color) {
    if (color == 0xFFFF) {
        return manager->savedColor;
    }
    return color;
}

u16 sub_0201002C(const ScreenFadeManager *manager) {
    const ScreenFade *fade;
    if (manager->screen.existsMain == 1) {
        fade = &manager->mainScreenFade;
    } else {
        fade = &manager->subScreenFade;
    }
    if (fade->direction == FADE_OUT) {
        return fade->color;
    }
    return manager->savedColor;
}

void sub_02010050(SysTask *task, void *data) {
    ScreenFade *fade = data;
    SetMasterBrightness(fade->screen, 0);
    SysTask_Destroy(task);
}

void sub_02010064(ScreenFade *fade) {
    if (fade->direction == FADE_IN
        && (fade->color == RGB(31, 31, 31) || fade->color == RGB(0, 0, 0))
        && fade->method == FADE_BY_WINDOW) {
        SysTask_CreateOnVWaitQueue(sub_02010050, fade, 1024);
    }
}

void sub_02010094(ScreenFade *fade) {
    if (fade->direction == FADE_OUT
        && (fade->color == RGB(31, 31, 31) || fade->color == RGB(0, 0, 0))
        && fade->method == FADE_BY_WINDOW) {
        sub_0200FBF4((PMLCDTarget)fade->screen, fade->color);
        ResetVisibleHardwareWindows((PMLCDTarget)fade->screen);
    }
}

void sub_020100C4(ScreenFadeManager *manager) {
    memset(&manager->screen, 0, sizeof(ScreenFadeParams));
    memset(&manager->mainScreenFade, 0, sizeof(ScreenFade));
    memset(&manager->subScreenFade, 0, sizeof(ScreenFade));
    memset(&manager->hblanks, 0, sizeof(ScreenFadeHBlanks));
    memset(&manager->hwSettings, 0, sizeof(HardwareWindowSettings));
}
