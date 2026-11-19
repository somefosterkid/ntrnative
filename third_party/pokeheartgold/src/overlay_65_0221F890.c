#include "global.h"

void ov65_0221F890(void);

void ov65_0221F8B4(void);

void ov65_0221F890(void) {
    *((s32 *)REG_DISPCNT_ADDR) = (0x2000) | ((*((s32 *)REG_DISPCNT_ADDR)) & 0xFFFF1FFF);
    G2_SetBlendBrightness(0x1E, 7);
}

void ov65_0221F8B4(void) {
    reg_G2_BLDCNT = 0;
    reg_GX_DISPCNT &= 0xFFFF1FFF;
}
