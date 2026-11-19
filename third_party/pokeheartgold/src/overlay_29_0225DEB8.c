#include "global.h"

#include "overlay_29.h"
#include "sprite.h"
#include "overlay_29_number_entry_data.h"

s32 ov29_0225DC50(void *);

s32 ov29_0225DEB8(NumberEntryData *arg0, s32 arg1) {
    s32 v1;
    u32 new_var;
    s32 v2;
    u32 v0;
    v1 = arg0->cursor;
    arg0->cursor = (s32)(v1 + arg1);
    v2 = arg0->cursor;
    if (v2 < 0) {
        arg0->cursor = 0;
    } else {
        v0 = arg0->numDigits;
        if (((u32)v2) >= v0) {
            arg0->cursor = (s32)(v0 - 1);
        }
    }
    new_var = (v0 = arg0->cursor);
    if (new_var == v1) {
        return 0;
    }
    ov29_0225DC50(arg0);
    return 1;
}

void ov29_0225DEF4(NumberEntryData *arg0, s32 arg1) {
    *((s32 *)&arg0->digits[(*((s32 *)&arg0->cursor))]) = arg1;
    Sprite_SetAnimCtrlSeq((&arg0->sprites[5])[arg0->cursor], arg1 + 1);
}
