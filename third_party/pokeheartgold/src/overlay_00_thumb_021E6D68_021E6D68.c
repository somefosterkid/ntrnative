#include "global.h"

#include "overlay_00_thumb.h"

extern u8 *_0221A680;

s32 ov00_021E6D68(s32 arg0) {
    return (s32) * (u8 *)((u8 *)(_0221A680 + arg0) + 0x1044);
}
