// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/wipe.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SYSTEM_WIPE_H
#define BWPORT_SYSTEM_WIPE_H

#include "types.h"
#include "struct_decls.h"

// Starts a wipe of the screens mode picks, with the pattern typeMain or typeSub over division steps of sync frames
void GFL_WipeSet(int mode, int typeMain, int typeSub, u16 color, int division, int sync, u32 heapId);
BOOL GFL_WipeIsFinished(void);
void setBrightnessForEngine(int screen, int brightness);

// Which screens wipe, and in which order
#define WIPE_MODE_BOTH 0

// The patterns: each even one covers the screen and the odd one after it uncovers it
#define WIPE_TYPE_FADE_OUT 0

#define WIPE_TYPE_FADE_IN 1

// The colors a wipe covers with; WIPE_COLOR_LAST is the color the last wipe covered with
#define WIPE_COLOR_BLACK 0

// pokebw2's Wipe_SetScreenCovered
// Covers a screen or both with the master brightness, white for WIPE_COLOR_WHITE and black otherwise
void func_0202223c(int screen, u16 color);

#define WIPE_COLOR_WHITE 0x7fff

// Black 2's, not checked against Black
#define WIPE_MODE_SUB_FIRST 2


// pokebw2's Wipe_HideWindows
void func_02022224(int screen);

// pokebw2's Wipe_HideWindows

// pokebw2's Wipe_HideWindows


// pokebw2's Wipe_SetCovered
void func_02022268(u16 color);


#define WIPE_MODE_SUB 4

// pokebw2's Wipe_HideWindows

// pokebw2's GFL_WipeForceEnd
// Ends the wipe at once
void func_020221b8(void);

// A wipe of one screen
struct WipeScreen {
    u32 type;
    s32 division;
    s32 sync;
    s32 seq;
    s32 screen;
    void *work;
    WipeWnd *wnd;
    WipeHBlank *hblank;
    u32 heapId;
    u16 color;
    // Whether the wipe leaves the screen covered
    BOOL endCovered;
    // Whether the wipe is a master brightness fade
    BOOL useBrightness;
};

// Black 2's, not checked against Black
typedef void (*WipeHBlankFunc)(void *work);

// pokebw2's Wipe_SetBackdropColor
void func_020222a8(u16 color);

// pokebw2's GFL_WipeMain
// Runs a frame of the wipe
void func_02022174(void);

// Black 2's, not checked against Black
#define WIPE_MODE_MAIN_FIRST 1
#define WIPE_MODE_MAIN 3

#define WIPE_COLOR_LAST 0xffff

// The H-blank functions of the wipes of both screens
struct WipeHBlank {
    void *work[2];
    WipeHBlankFunc func[2];
    BOOL active[2];
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void killBrightnessEitherEngine(int screen);
// pokebw2's WipeHBlank_AddAtVBlank
// Adds or removes a wipe's H-blank function at the next VBlank
void func_02022518(WipeHBlank *hblank, void *work, WipeHBlankFunc func, int slot, u32 heapId);
// pokebw2's WipeHBlank_RemoveAtVBlank
void func_02022568(WipeHBlank *hblank, int slot, u32 heapId);

#endif // BWPORT_SYSTEM_WIPE_H
