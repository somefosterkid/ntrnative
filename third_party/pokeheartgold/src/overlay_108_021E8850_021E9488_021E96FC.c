#include "global.h"

#include "touchscreen.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

extern TouchscreenHitbox ov108_021EABF0;

s32 ov108_021E96FC(SafariDecorationApp *arg0, s32 *arg1) {
    s32 v0;
    u8 v1;
    u8 v2;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&ov108_021EABF0);
    if (v0 == -1) {
        *arg1 = 0;
        return 0;
    } else if (v0 == 7) {
        v1 = arg0->page;
        if (v1 != 0) {
            arg0->page = (u8)(v1 - 1);
            ov108_021EA584(arg0, 0, &arg0->pendingTasks, 0);
            *arg1 = 1;
        }
        return 0;
    } else if (v0 == 8) {
        v2 = arg0->page;
        if ((s32)v2 < (s32)(arg0->numPages - 1)) {
            arg0->page = (u8)(v2 + 1);
            ov108_021EA584(arg0, 1, &arg0->pendingTasks, 0);
            *arg1 = 1;
        }
        return 0;
    } else {
        if (v0 != arg0->cursor) {
            arg0->cursor = (u8)v0;
            ov108_021EA2EC(arg0, arg0->cursor);
            ov108_021EA47C(arg0);
        }
        *arg1 = 1;
        return ov108_021E9528(arg0, arg0->cursor);
    }
}
