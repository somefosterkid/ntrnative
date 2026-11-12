// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual_touchbar.h: the lines in use so
// far.
#ifndef BWPORT_APP_MANUAL_TOUCHBAR_H
#define BWPORT_APP_MANUAL_TOUCHBAR_H

#include "types.h"
#include "app/manual_common.h"

// What the bar shows, by the screen it is under
enum {
    MANUAL_TOUCHBAR_MODE_TOP,
    MANUAL_TOUCHBAR_MODE_CATEGORY,
    MANUAL_TOUCHBAR_MODE_TITLE,
    MANUAL_TOUCHBAR_MODE_EXPLAIN,
    MANUAL_TOUCHBAR_MODE_NONE,
};

#define MANUAL_TOUCHBAR_ICON_RETURN 1

ManualTouchBar *func_ov225_021b9f24(ManualCommon *common);
void func_ov225_021b9f8c(ManualTouchBar *bar);

// pokebw2's ManualTouchBar_SetMode
void func_ov225_021ba068(ManualTouchBar *bar, u32 mode);
// pokebw2's ManualTouchBar_Main
void func_ov225_021b9fbc(ManualTouchBar *bar);

#define MANUAL_TOUCHBAR_ICON_PREV 4
#define MANUAL_TOUCHBAR_ICON_NEXT 5

// pokebw2's ManualTouchBar_SetPage
// Shows the page number and the arrows of MANUAL_TOUCHBAR_MODE_EXPLAIN
void func_ov225_021ba0f0(ManualTouchBar *bar, u8 pageCount, u8 page);

// The icons ManualTouchBar_GetDecided and ManualTouchBar_GetTouched return
#define MANUAL_TOUCHBAR_ICON_CLOSE 0

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ManualTouchBar_GetTouched
u32 func_ov225_021ba158(ManualTouchBar *bar);
// pokebw2's ManualTouchBar_GetDecided
// The icon whose animation has ended, and the icon being touched, or -1
u32 func_ov225_021ba154(ManualTouchBar *bar);
// pokebw2's ManualTouchBar_Create
// pokebw2's ManualTouchBar_Free

#endif // BWPORT_APP_MANUAL_TOUCHBAR_H
