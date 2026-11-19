#include "global.h"

u8 ov96_0221A61C(f32);

void ov96_0221A6B0(u32 arg0);

void ov96_0221A6B0(u32 arg0) {
    ov96_0221A61C((f32)((double)(f32)arg0 / 3.5));
}

void ov96_0221A6D0(u32 arg0);

void ov96_0221A6D0(u32 arg0) {
    ov96_0221A61C((f32)(10.0 * ((double)(f32)arg0 / 1024.0)));
}
