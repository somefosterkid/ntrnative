#include "global.h"

#include "touchscreen.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6A24(void);

extern TouchscreenHitbox ov14_021F8614;

extern TouchscreenHitbox ov14_021F8630;

void ov14_021F6A24(void) {
    TouchscreenHitbox_FindRectAtTouchNew(&ov14_021F8614);
}

s32 ov14_021F6A34(UnkStruct_ov14_boxapp *app) {
    return TouchscreenHitbox_FindRectAtTouchNew(&ov14_021F8630);
}
