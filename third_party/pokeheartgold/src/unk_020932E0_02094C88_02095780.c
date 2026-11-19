#include "global.h"

#include "unk_020932E0.h"

void sub_02095780(void) {
    G2_SetBlendBrightness(0x16, -7);
}

void sub_02095794(void) {
    reg_G2_BLDCNT = 0;
    G2_SetBlendAlpha(0, 4, 6, 0xA);
}
