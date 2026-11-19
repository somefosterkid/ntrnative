#include "global.h"

#include "overlay_38_thumb.h"


extern s32 _02225040;

u32 ov38_0221BB44(u32 decodedSize) {
    u32 padding = (decodedSize % 3 != 0) ? 1 : 0;
    return (decodedSize / 3 + padding) * 4;
}

void ov38_0221BB68(s32 arg0) {
    u32 v0;

    v0 = arg0 + 8;
    switch (v0) {
    case 0:
        *(s32 *)((u8 *)(&_02225040) + 8) = 0;
        return;
    case 2:
        *(s32 *)((u8 *)(&_02225040) + 8) = 1;
        return;
    case 3:
        *(s32 *)((u8 *)(&_02225040) + 8) = 2;
        return;
    case 4:
        *(s32 *)((u8 *)(&_02225040) + 8) = 3;
        return;
    case 5:
        *(s32 *)((u8 *)(&_02225040) + 8) = 4;
        return;
    case 6:
        *(s32 *)((u8 *)(&_02225040) + 8) = 5;
        return;
    case 7:
        *(s32 *)((u8 *)(&_02225040) + 8) = 6;
        return;
    default:
        *(s32 *)((u8 *)(&_02225040) + 8) = 0;
        return;
    }
}
