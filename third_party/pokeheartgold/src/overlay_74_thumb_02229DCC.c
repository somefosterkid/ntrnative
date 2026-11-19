#include "global.h"

s32 ov74_02229DCC(void);

void ov74_02229DE0(s32 arg0);

s32 ov74_02229DCC(void) {
    return 0x358;
}

extern u8 *ov74_0223D0A4;

void *ov74_02229DD4(void);

void *ov74_02229DD4(void) {
    return ov74_0223D0A4 + 0x68;
}

void ov74_02229DE0(s32 arg0) {
    *(s8 *)((u8 *)(ov74_0223D0A4 + arg0) + 0x3C1) = 1;
}
