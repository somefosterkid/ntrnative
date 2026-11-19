#include "global.h"

#include "touchscreen.h"
#include "field_types_def.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6A24(void);
s32 ov14_021F6A34(FieldSystem *fieldSystem);

extern TouchscreenHitbox ov14_021F8614;

extern TouchscreenHitbox ov14_021F8630;

void ov14_021F6A24(void) {
    TouchscreenHitbox_FindRectAtTouchNew(&ov14_021F8614);
}

s32 ov14_021F6A34(FieldSystem *fieldSystem) {
    return TouchscreenHitbox_FindRectAtTouchNew(&ov14_021F8630);
}
