#include "global.h"

#include "bg_window.h"

s32 ov65_0221DE24(void *, s32, s32);

void ov65_0221F0C4(void *arg0);

void ov65_0221F0C4(void *arg0) {
    ov65_0221DE24(arg0, 0x18, 3);
    *(s32 *)((u8 *)arg0 + 0x68) = 1;
}

u8 ov65_0221FB90(void *, s32, s32, void *, void *);

void ov65_0221F0D8(void *arg0, s32 arg1, s32 arg2);

void ov65_0221F0D8(void *arg0, s32 arg1, s32 arg2) {
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x180), 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
    ov65_0221FB90(arg0 + 0x444 + (arg1 * 0x10), arg2, 1, *(void **)((u8 *)arg0 + 0x190), *(void **)((u8 *)arg0 + 0x184));
}
