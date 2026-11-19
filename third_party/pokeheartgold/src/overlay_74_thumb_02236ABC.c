#include "global.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"
#include "unk_020915B0.h"

extern s32 ov74_0223E304;

void ov74_02236ABC(void);

void sub_0203A914(void);
void ov74_02236AC8(void);

void ov74_02236ABC(void) {
    *(s32 *)((u8 *)(&ov74_0223E304) + 4) = 1;
}

void ov74_02236AC8(void) {
    sub_0203A914();
    sub_020394F4();
    UnloadOVY38();
    UnloadDwcOverlay();
    sub_02034DE0();
}
