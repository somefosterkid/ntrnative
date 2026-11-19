#include "global.h"

#include "bg_window.h"
#include "render_window.h"

void ov83_02245094(Window *window);

void ov83_02245094(Window *window) {
    ClearFrameAndWindow2(window, 1);
    ClearWindowTilemapAndScheduleTransfer(window);
}
