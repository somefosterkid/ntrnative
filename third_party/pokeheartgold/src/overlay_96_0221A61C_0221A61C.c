#include "global.h"

u8 ov96_0221A61C(f32 arg0);

u8 ov96_0221A61C(f32 arg0) {
    u32 v0;
    u32 v1;

    v0 = (u32)arg0;
    v1 = v0;
    if ((double)v0 > 200.0) {
        v1 = 0xC8;
    }
    return (u8)v1;
}

void ov96_0221A640(u32 arg0);

void ov96_0221A640(u32 arg0) {
    ov96_0221A61C((f32)(11500.0 / (double)((f32)arg0 / 30.0f)));
}
