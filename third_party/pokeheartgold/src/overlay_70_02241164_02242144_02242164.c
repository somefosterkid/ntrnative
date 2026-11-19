#include "global.h"

#include "touchscreen.h"
#include "unk_struct_ov70_02242e58.h"

extern TouchscreenHitbox ov70_02245DD0;

extern TouchscreenHitbox ov70_02245DE4;

extern TouchscreenHitbox ov70_02245E3E;

extern TouchscreenHitbox ov70_02245E5E;

extern TouchscreenHitbox ov70_02245ED0;

extern TouchscreenHitbox ov70_02245F28;

unsigned int ov70_02242164(UnkStruct_ov70_02242E58 *arg0, s32 arg1);

unsigned int ov70_02242164(UnkStruct_ov70_02242E58 *arg0, s32 arg1) {
    switch (arg1) {
    case 4:
        TouchscreenHitbox_FindRectAtTouchNew(&ov70_02245ED0);
        return;

    case 6:
        TouchscreenHitbox_FindRectAtTouchNew(&ov70_02245F28);
        return;

    case 0:

    case 2:
        TouchscreenHitbox_FindRectAtTouchNew(&ov70_02245E3E);
        return;

    case 1:
        TouchscreenHitbox_FindRectAtTouchNew(&ov70_02245DD0);
        return;

    case 3:
        TouchscreenHitbox_FindRectAtTouchNew(&ov70_02245E5E);
        return;

    case 5:

    case 7:
        TouchscreenHitbox_FindRectAtTouchNew(&ov70_02245DE4);

    default:
        return;
    }
}
