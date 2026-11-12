// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/wipe.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "system/wipe.h"
#include "system/wipe_sub.h"
#include "system/wipe_wnd.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
BOOL WipeFunc_CircleIn(WipeScreen *screen);
BOOL WipeFunc_CircleOut(WipeScreen *screen);
BOOL WipeFunc_GrowRightIn(WipeScreen *screen);
BOOL WipeFunc_GrowRightOut(WipeScreen *screen);
BOOL WipeFunc_LinesDownIn(WipeScreen *screen);
BOOL WipeFunc_LinesDownOut(WipeScreen *screen);
BOOL WipeFunc_LinesUpIn(WipeScreen *screen);
BOOL WipeFunc_LinesUpOut(WipeScreen *screen);
BOOL WipeFunc_ShrinkLeftIn(WipeScreen *screen);
BOOL WipeFunc_ShrinkLeftOut(WipeScreen *screen);

// Screen wipes: the wipe of each screen runs its pattern's function from WIPE_FUNCTIONS once a frame until it is done

typedef BOOL (*WipeFunc)(WipeScreen *screen);

// A request to add a wipe's H-blank function at the next VBlank
typedef struct {
    WipeHBlank *hblank;
    void *work;
    WipeHBlankFunc func;
    int slot;
} WipeHBlankAddRequest;

// A request to remove one
typedef struct {
    WipeHBlank *hblank;
    int slot;
} WipeHBlankRemoveRequest;

// The orders the WIPE_MODE_*s run the screens' wipes in
#define WIPE_ORDER_BOTH 0
#define WIPE_ORDER_MAIN_FIRST 1
#define WIPE_ORDER_SUB_FIRST 2

typedef struct {
    u32 order;
    // Whether each screen's wipe is still running, and whether it wipes at all
    BOOL mainActive;
    BOOL subActive;
    BOOL mainOn;
    BOOL subOn;
} WipeControl;

// Black 2's; twinned code confirms the offsets of control, main, sub, hblank, wnd, active, mainCovered, subCovered and
// color in Black
typedef struct {
    WipeControl control;
    WipeScreen main;
    WipeScreen sub;
    WipeHBlank hblank;
    WipeWnd wnd;
    u16 active;
    // Whether each screen was left covered
    u8 mainCovered;
    u8 subCovered;
    // The color the last wipe covered with
    u16 color;
    TCB *hblankTcb;
} WipeSys;

static void func_020222ec(WipeSys *sys);
static BOOL func_02022340(WipeSys *sys, WipeScreen *main, WipeScreen *sub);
static void func_02022390(BOOL *active, WipeScreen *screen);
static BOOL func_020223ac(WipeScreen *screen);
static void func_020223c0(int mode, WipeSys *sys);
static void func_02022424(WipeControl *control, u32 order, BOOL mainOn, BOOL subOn);
static void func_02022430(WipeScreen *screen, u32 type, s32 division, s32 sync, s32 seq, void *work, s32 screenId,
                            WipeWnd *wnd, WipeHBlank *hblank, u32 heapId, u16 color);
static void func_02022458(WipeHBlank *hblank);
static void func_02022478(TCB *tcb, void *data);
static void func_02022490(WipeHBlank *hblank, void *work, WipeHBlankFunc func, int slot);
static void func_020224dc(WipeHBlank *hblank, int slot);
static void func_020225b4(TCB *tcb, void *data);
static void func_020225d4(TCB *tcb, void *data);
static void func_020225f0(void *work);
static u16 func_020225f4(WipeSys *sys, u16 color);
static u16 func_02022608(WipeSys *sys);
static void func_0202262c(TCB *tcb, void *data);
static void func_02022640(WipeScreen *screen);
static void func_02022670(WipeScreen *screen);
static void func_020226a0(WipeSys *sys);

static const WipeFunc WIPE_FUNCTIONS[] = {
    func_020226e0, func_020226fc, func_0202271c, func_02022758, func_02022790, func_020227cc,
    func_02022804, func_02022830, func_0202285c, func_02022888, func_020228b4, func_020228e0,
};

// pokebw2's sWipe
static WipeSys data_021467a4;

void GFL_WipeSet(int mode, int typeMain, int typeSub, u16 color, int division, int sync, u32 heapId) {
    WipeSys *sys;

    sync /= GFL_FadeGetUpdateFreq();
    sys = &data_021467a4;

    func_020226a0(sys);
    func_020223c0(mode, sys);
    func_02022458(&data_021467a4.hblank);
    color = func_020225f4(sys, color);
    func_02022430(&data_021467a4.main, typeMain, division, sync, 0, NULL, 0, &data_021467a4.wnd, &data_021467a4.hblank, heapId, color);
    func_02022430(&data_021467a4.sub, typeSub, division, sync, 0, NULL, 1, &data_021467a4.wnd, &data_021467a4.hblank, heapId, color);
    data_021467a4.active = TRUE;
    func_02022390(&data_021467a4.control.mainActive, &data_021467a4.main);
    func_02022390(&data_021467a4.control.subActive, &data_021467a4.sub);
    if (sys->control.mainOn) {
        func_02022640(&sys->main);
        sys->mainCovered = TRUE;
    }
    if (sys->control.subOn) {
        func_02022640(&sys->sub);
        sys->subCovered = TRUE;
    }
}

// pokebw2's GFL_WipeMain
void func_02022174(void) {
    WipeSys *sys = &data_021467a4;

    if (data_021467a4.active) {
        if (func_02022340(sys, &sys->main, &sys->sub) == TRUE) {
            func_020222ec(sys);
        }
    }
}

BOOL GFL_WipeIsFinished(void) {
    if (data_021467a4.active == FALSE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GFL_WipeForceEnd
void func_020221b8(void) {
    func_020224dc(&data_021467a4.hblank, 0);
    func_020224dc(&data_021467a4.hblank, 1);
    if (data_021467a4.control.mainActive) {
        data_021467a4.main.seq = 2;
    }
    if (data_021467a4.control.subActive) {
        data_021467a4.sub.seq = 2;
    }
    func_02022390(&data_021467a4.control.mainActive, &data_021467a4.main);
    func_02022390(&data_021467a4.control.subActive, &data_021467a4.sub);
    data_021467a4.active = FALSE;
    data_021467a4.mainCovered = FALSE;
    data_021467a4.subCovered = FALSE;
    func_020226a0(&data_021467a4);
}

// pokebw2's Wipe_HideWindows
void func_02022224(int screen) {
    func_02023b48(0, screen);
}

void killBrightnessEitherEngine(int screen) {
    setBrightnessForEngine(screen, 0);
}

// pokebw2's Wipe_SetScreenCovered
void func_0202223c(int screen, u16 color) {
    if (color == WIPE_COLOR_LAST) {
        color = data_021467a4.color;
    }
    setBrightnessForEngine(screen, color == WIPE_COLOR_WHITE ? 16 : -16);
}

// pokebw2's Wipe_SetCovered
void func_02022268(u16 color) {
    int brightness;

    if (color == WIPE_COLOR_LAST) {
        color = data_021467a4.color;
    }
    if (color == WIPE_COLOR_WHITE) {
        brightness = 16;
    } else {
        brightness = -16;
    }
    setBrightnessForEngine(0, brightness);
    setBrightnessForEngine(1, brightness);
    data_021467a4.color = color;
}

// pokebw2's Wipe_SetBackdropColor
void func_020222a8(u16 color) {
    GX_LoadBGPltt(&color, 0, sizeof(color));
    GXS_LoadBGPltt(&color, 0, sizeof(color));
}

void setBrightnessForEngine(int screen, int brightness) {
    if (screen == 0) {
        GX_SetMasterBrightness(brightness);
    } else {
        GXS_SetMasterBrightness(brightness);
    }
}

// pokebw2's Wipe_End
static void func_020222ec(WipeSys *sys) {
    sys->active = FALSE;
    sys->color = func_02022608(sys);
    if (sys->control.mainOn) {
        func_02022670(&sys->main);
        if (sys->main.endCovered == FALSE) {
            data_021467a4.mainCovered = FALSE;
        }
    }
    if (sys->control.subOn) {
        func_02022670(&sys->sub);
        // BUG: The sub screen's flag is cleared by whether the main screen's wipe ended covered. Nothing reads the
        // flags, so it has no effect
#ifdef BUGFIX
        if (sys->sub.endCovered == FALSE) {
#else
        if (sys->main.endCovered == FALSE) {
#endif
            data_021467a4.subCovered = FALSE;
        }
    }
    func_020226a0(sys);
}

// Runs a frame of each screen's wipe in the control's order; TRUE once both are done
// pokebw2's Wipe_Update
static BOOL func_02022340(WipeSys *sys, WipeScreen *main, WipeScreen *sub) {
    switch (sys->control.order) {
    case WIPE_ORDER_BOTH:
        func_02022390(&sys->control.mainActive, main);
        func_02022390(&sys->control.subActive, sub);
        break;
    case WIPE_ORDER_MAIN_FIRST:
        if (sys->control.mainActive) {
            func_02022390(&sys->control.mainActive, main);
        } else {
            func_02022390(&sys->control.subActive, sub);
        }
        break;
    case WIPE_ORDER_SUB_FIRST:
        if (sys->control.subActive) {
            func_02022390(&sys->control.subActive, sub);
        } else {
            func_02022390(&sys->control.mainActive, main);
        }
        break;
    }
    if (sys->control.mainActive == FALSE && sys->control.subActive == FALSE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WipeScreen_Run
static void func_02022390(BOOL *active, WipeScreen *screen) {
    if (*active) {
        if (func_020223ac(screen) == TRUE) {
            *active = FALSE;
        }
    }
}

// pokebw2's WipeScreen_Call
static BOOL func_020223ac(WipeScreen *screen) {
    return WIPE_FUNCTIONS[screen->type](screen);
}

// pokebw2's Wipe_SetMode
static void func_020223c0(int mode, WipeSys *sys) {
    switch (mode) {
    case WIPE_MODE_BOTH:
        func_02022424(&sys->control, WIPE_ORDER_BOTH, TRUE, TRUE);
        break;
    case WIPE_MODE_MAIN_FIRST:
        func_02022424(&sys->control, WIPE_ORDER_MAIN_FIRST, TRUE, TRUE);
        break;
    case WIPE_MODE_SUB_FIRST:
        func_02022424(&sys->control, WIPE_ORDER_SUB_FIRST, TRUE, TRUE);
        break;
    case WIPE_MODE_MAIN:
        func_02022424(&sys->control, WIPE_ORDER_MAIN_FIRST, TRUE, FALSE);
        break;
    case WIPE_MODE_SUB:
        func_02022424(&sys->control, WIPE_ORDER_SUB_FIRST, FALSE, TRUE);
        break;
    }
}

// pokebw2's Wipe_SetControl
static void func_02022424(WipeControl *control, u32 order, BOOL mainOn, BOOL subOn) {
    control->order = order;
    control->mainActive = mainOn;
    control->subActive = subOn;
    control->mainOn = mainOn;
    control->subOn = subOn;
}

static void func_02022430(WipeScreen *screen, u32 type, s32 division, s32 sync, s32 seq, void *work, s32 screenId,
                            WipeWnd *wnd, WipeHBlank *hblank, u32 heapId, u16 color) {
    screen->type = type;
    screen->division = division;
    screen->sync = sync;
    screen->seq = seq;
    screen->work = work;
    screen->screen = screenId;
    screen->wnd = wnd;
    screen->hblank = hblank;
    screen->heapId = heapId;
    screen->color = color;
}

// pokebw2's WipeHBlank_Init
static void func_02022458(WipeHBlank *hblank) {
    int i;

    for (i = 0; i < 2; i++) {
        hblank->work[i] = NULL;
        hblank->func[i] = func_020225f0;
        hblank->active[i] = FALSE;
    }
}

// pokebw2's WipeHBlank_Run
static void func_02022478(TCB *tcb, void *data) {
    WipeHBlank *hblank = data;
    int i;

    for (i = 0; i < 2; i++) {
        hblank->func[i](hblank->work[i]);
    }
}

// pokebw2's WipeHBlank_Add
static void func_02022490(WipeHBlank *hblank, void *work, WipeHBlankFunc func, int slot) {
    if (hblank->active[0] == FALSE && hblank->active[1] == FALSE) {
        data_021467a4.hblankTcb = GFL_HBlankTCBAdd(func_02022478, hblank, 0);
    }
    hblank->work[slot] = work;
    if (func != NULL) {
        hblank->func[slot] = func;
    } else {
        hblank->func[slot] = func_020225f0;
    }
    hblank->active[slot] = TRUE;
}

// pokebw2's WipeHBlank_Remove
static void func_020224dc(WipeHBlank *hblank, int slot) {
    hblank->active[slot] = FALSE;
    if (hblank->active[0] == FALSE && hblank->active[1] == FALSE) {
        if (data_021467a4.hblankTcb != NULL) {
            GFL_TCBRemove(data_021467a4.hblankTcb);
            data_021467a4.hblankTcb = NULL;
        }
    }
    hblank->func[slot] = func_020225f0;
    hblank->work[slot] = NULL;
}

// pokebw2's WipeHBlank_AddAtVBlank
void func_02022518(WipeHBlank *hblank, void *work, WipeHBlankFunc func, int slot, u32 heapId) {
    WipeHBlankAddRequest *request =
        GFL_HeapAllocate(HEAPID_TAIL((HeapID)heapId), sizeof(WipeHBlankAddRequest), FALSE, "wipe.c", 1000);

    request->hblank = hblank;
    request->work = work;
    request->func = func;
    request->slot = slot;
    GFL_VBlankTCBAdd(func_020225b4, request, 0x400);
}

// pokebw2's WipeHBlank_RemoveAtVBlank
void func_02022568(WipeHBlank *hblank, int slot, u32 heapId) {
    u16 size = sizeof(WipeHBlankRemoveRequest);
    WipeHBlankRemoveRequest *request =
        GFL_HeapAllocate(HEAPID_TAIL((HeapID)heapId), size, FALSE, "wipe.c", 1025);

    request->hblank = hblank;
    request->slot = slot;
    GFL_VBlankTCBAdd(func_020225d4, request, size << 7);
}

// pokebw2's WipeHBlank_AddTask
static void func_020225b4(TCB *tcb, void *data) {
    WipeHBlankAddRequest *request = data;

    func_02022490(request->hblank, request->work, request->func, request->slot);
    GFL_TCBRemove(tcb);
    GFL_HeapFree(request);
}

// pokebw2's WipeHBlank_RemoveTask
static void func_020225d4(TCB *tcb, void *data) {
    WipeHBlankRemoveRequest *request = data;

    func_020224dc(request->hblank, request->slot);
    GFL_TCBRemove(tcb);
    GFL_HeapFree(request);
}

// pokebw2's WipeHBlank_Nop
static void func_020225f0(void *work) {
}

// pokebw2's Wipe_GetColor
static u16 func_020225f4(WipeSys *sys, u16 color) {
    if (color == WIPE_COLOR_LAST) {
        color = sys->color;
    }
    return color;
}

// The color the screens were left covered with, or the last one if the wipe uncovered them
// pokebw2's Wipe_GetEndColor
static u16 func_02022608(WipeSys *sys) {
    WipeScreen *screen;

    if (sys->control.mainOn == TRUE) {
        screen = &sys->main;
    } else {
        screen = &sys->sub;
    }
    if (screen->endCovered == TRUE) {
        return screen->color;
    }
    return sys->color;
}

// pokebw2's WipeScreen_ClearBrightnessTask
static void func_0202262c(TCB *tcb, void *data) {
    WipeScreen *screen = data;

    setBrightnessForEngine(screen->screen, 0);
    GFL_TCBRemove(tcb);
}

// A wipe that uncovers a screen in black or white with a pattern clears the master brightness at the next VBlank
// pokebw2's WipeScreen_Start
static void func_02022640(WipeScreen *screen) {
    if (screen->endCovered == FALSE && (screen->color == WIPE_COLOR_WHITE || screen->color == WIPE_COLOR_BLACK) &&
        screen->useBrightness == FALSE) {
        GFL_VBlankTCBAdd(func_0202262c, screen, 0x400);
    }
}

// A wipe that covers a screen in black or white with a pattern leaves it covered with the master brightness, and its
// windows hidden
// pokebw2's WipeScreen_Finish
static void func_02022670(WipeScreen *screen) {
    if (screen->endCovered == TRUE && (screen->color == WIPE_COLOR_WHITE || screen->color == WIPE_COLOR_BLACK) &&
        screen->useBrightness == FALSE) {
        func_0202223c(screen->screen, screen->color);
        func_02022224(screen->screen);
    }
}

// pokebw2's Wipe_Clear
static void func_020226a0(WipeSys *sys) {
    MI_CpuFill8(&sys->control, 0, sizeof(WipeControl));
    MI_CpuFill8(&sys->main, 0, sizeof(WipeScreen));
    MI_CpuFill8(&sys->sub, 0, sizeof(WipeScreen));
    MI_CpuFill8(&sys->hblank, 0, sizeof(WipeHBlank));
    MI_CpuFill8(&sys->wnd, 0, sizeof(WipeWnd));
}
