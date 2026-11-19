#include "global.h"

void ov00_021E6388(u32);

extern u8 *_0221A680;

void ov00_021E6FBC(s32 param_1, s32 param_2, u32 param_3, u32 param_4, u32 param_5);

void ov00_021E6FBC(s32 param_1, s32 param_2, u32 param_3, u32 param_4, u32 param_5) {
    *(u32 *)(_0221A680 + 0x10a0) = 0;
    if ((param_1 == 0) && (param_2 == 0)) {
        ov00_021E6388(param_5);
    }
    return;
}
