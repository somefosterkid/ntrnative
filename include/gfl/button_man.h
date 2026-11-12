// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/button_man.h.
#ifndef BWPORT_GFL_BUTTON_MAN_H
#define BWPORT_GFL_BUTTON_MAN_H

#include "types.h"
#include "gfl/touchpanel.h"
#include "struct_decls.h"

// Buttons of the touch screen (button_man.c): rectangles that report touches, holds and releases to a callback

enum {
    BMN_EVENT_TOUCH,
    BMN_EVENT_RELEASE,
    BMN_EVENT_HOLD,
    // Released after the touch slid off the button
    BMN_EVENT_SLIDEOUT,
};

#define BMN_EVENT_NONE (-1)

typedef void (*ButtonManCallback)(u32 button, u32 event, void *work);

// Creates a manager of a button for each rectangle of a table ending with TOUCH_RECT_END
ButtonMan *func_02051b84(const TouchRect *rects, ButtonManCallback callback, void *work, u32 heapId);
void func_02051c6c(ButtonMan *mgr);
// Reports the events of this frame to the callback, and returns whether a button was touched this frame
BOOL func_02051c88(ButtonMan *mgr);

#endif // BWPORT_GFL_BUTTON_MAN_H
