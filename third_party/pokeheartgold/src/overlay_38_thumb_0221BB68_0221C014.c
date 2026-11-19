#include "global.h"

#include "overlay_38_thumb.h"

extern s32 _02225040;

s32 ov38_0221C014(void) {
    return *(s32 *)((u8 *)(&_02225040) + 8);
}

s32 ov38_0221C020(void) {
    return *(s32 *)((u8 *)(&_02225040) + 0xC);
}
