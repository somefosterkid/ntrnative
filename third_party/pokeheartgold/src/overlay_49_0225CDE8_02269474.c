#include "global.h"

#include "math_util.h"
#include "overlay_49.h"

void ov49_02269474(void *arg0) {
    *(s16 *)((u8 *)arg0 + 4) = 0;
    *(s16 *)((u8 *)arg0 + 6) = 8;
    *(u16 *)((u8 *)arg0 + 8) = GF_DegreeToSinCosIdx(5U);
    *(s16 *)((u8 *)arg0 + 0xA) = 2;
    *(s16 *)((u8 *)arg0 + 0xC) = 4;
}

void ov49_02269494(void *arg0) {
    *(s16 *)((u8 *)arg0 + 4) = 0;
    *(s16 *)((u8 *)arg0 + 6) = 0x10;
    *(u16 *)((u8 *)arg0 + 8) = GF_DegreeToSinCosIdx(0xAU);
    *(s16 *)((u8 *)arg0 + 0xA) = 4;
    *(s16 *)((u8 *)arg0 + 0xC) = 2;
}
