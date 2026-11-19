#include "global.h"

extern u8 *_0221A680;

void ov00_021E6428(int param_1, u8 param_2);

void ov00_021E6428(int param_1, u8 param_2) {
    *(u32 *)(_0221A680 + 0x1078) = 0;
    *(u32 *)(_0221A680 + param_2 * 4 + 0x10ac) = 0;
    return;
}
