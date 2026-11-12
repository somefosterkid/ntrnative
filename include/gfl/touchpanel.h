// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/touchpanel.h, the lines in use so far.
#ifndef BWPORT_GFL_TOUCHPANEL_H
#define BWPORT_GFL_TOUCHPANEL_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The touch panel (touchpanel.c)

// A rectangle of the touch screen, in pixels. A table of them ends with top TOUCH_RECT_END
typedef struct {
    u8 top;
    u8 bottom;
    u8 left;
    u8 right;
} TouchRect;

#define TOUCH_RECT_END 0xff
#define TOUCH_RECT_NONE (-1)

// The rectangle being touched, or TOUCH_RECT_NONE (pokebw2's func_0203d9c8)
s32 func_02035ba4(const TouchRect *rects);
// The rectangle touched this frame, or TOUCH_RECT_NONE (pokebw2's func_0203da0c)
s32 func_02035c08(const TouchRect *rects);
// Whether the touch screen is touched (pokebw2's func_0203da2c)
BOOL func_02035c38(void);
// Whether the touch screen was touched this frame (pokebw2's func_0203da48)
BOOL func_02035c60(void);
// The position of a touch this frame, and whether there was one (pokebw2's func_0203dac8)
BOOL func_02035d20(u32 *x, u32 *y);

// Rectangles whose top is one of these are circles, or are skipped
#define TOUCH_RECT_CIRCLE 0xfe

#define TOUCH_RECT_SKIP 0xfd

// The rectangle at a point, or TOUCH_RECT_NONE
s32 func_02035d40(const TouchRect *rects, u32 x, u32 y);

void GFL_HIDClearTouchState(SystemUI *ui);
// Set the touch input of a frame
void func_02035e84(SystemUI *ui, u16 x, u16 y, u16 x30, u16 y30);
void func_02035eb0(SystemUI *ui, u16 pressed, u16 pressed30);
void func_02035ecc(SystemUI *ui, u16 held, u16 held30);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Whether a point is held, and whether it was pressed this frame, and where
BOOL func_02035cb8(u32 *x, u32 *y);
TouchpadManager *initTouchpad(HeapID heapId);
// Reads the touch panel; called every frame
void touchpadData(void);
void func_02035dc8(void);
// Stop and restart sampling around sleep
void func_02035e08(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Whether a point is held, and whether it was pressed this frame, and where

#endif // BWPORT_GFL_TOUCHPANEL_H
