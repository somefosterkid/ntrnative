#include "global.h"

s32 ov65_0221F79C(void *);

extern s32 ov65_0221E270;

extern s32 ov65_0221F3B0;

s32 ov65_0221F354(void *arg0);

s32 ov65_0221F354(void *arg0) {
    s32 v0;
    s32 v1;

    v1 = *(s32 *)((u8 *)arg0 + 0x6C);
    if (v1 != 0) {
        v0 = *(s32 *)((u8 *)arg0 + 0x70);
        if (v0 != 0) {
            if ((v1 == 3) && (v0 == 3)) {
                *(s32 *)((u8 *)arg0 + 0x36A4) = (s32)(*(s32 *)((u8 *)arg0 + 0x36A4) + 1);
                *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221F3B0;
            } else {
                *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221E270;
            }
            *(s32 *)((u8 *)arg0 + 0x6C) = 0;
            *(s32 *)((u8 *)arg0 + 0x70) = 0;
            if (ov65_0221F79C(arg0) != 0) {
                *(s32 *)((u8 *)arg0 + 0x74) = 0x18;
            } else {
                *(s32 *)((u8 *)arg0 + 0x74) = 0x22;
            }
        }
    }
    return 0;
}
