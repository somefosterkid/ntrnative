#include "global.h"

#include "bg_window.h"
#include "overlay_98_ov98text_sys.h"

void ov98_0221EF14(Ov98TextSys *textSys, s32 windowIdx) {
    FillWindowPixelBuffer(&textSys->windows[windowIdx], 0);
}
