#include "global.h"

extern s32 ov65_0221E270;

extern s32 ov65_0221E858;

s32 ov65_0221E8EC(void *arg0);

s32 ov65_0221E8EC(void *arg0) {
    s32 v0;
    s32 v1;

    v1 = *(s32 *)((u8 *)arg0 + 0x6C);
    if (v1 != 0) {
        v0 = *(s32 *)((u8 *)arg0 + 0x70);
        if (v0 != 0) {
            if ((v1 == 1) && (v0 == 1)) {
                *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221E858;
            } else {
                *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221E270;
            }
            *(s32 *)((u8 *)arg0 + 0x6C) = 0;
            *(s32 *)((u8 *)arg0 + 0x70) = 0;
            *(s32 *)((u8 *)arg0 + 0x74) = 0x1D;
        }
    }
    return 0;
}
