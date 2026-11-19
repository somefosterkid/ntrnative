#include "global.h"

u8 ov96_0221A61C(f32 arg0);

f32 ov96_0221A5D4(u32 param_1, f32 param_2);

u8 ov96_0221A670(s32 param_1);

u8 ov96_0221A670(s32 param_1) {
    return ov96_0221A61C(param_1 * 3);
}

u8 ov96_0221A680(s32 param_1);

u8 ov96_0221A680(s32 param_1) {
    return ov96_0221A61C(param_1 * 3);
}

u8 ov96_0221A690(u32 param_1);

u8 ov96_0221A690(u32 param_1) {
    return ov96_0221A61C(param_1);
}

u8 ov96_0221A69C(u32 param_1);

u8 ov96_0221A69C(u32 param_1) {
    return ov96_0221A61C(ov96_0221A5D4(param_1, 0.08f));
}
