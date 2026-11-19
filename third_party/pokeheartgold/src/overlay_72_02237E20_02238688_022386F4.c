#include "global.h"

#include "sprite.h"

void ov72_02238688(Sprite *, int, int);

extern s32 ov72_0223B3A4;

extern s32 ov72_0223B3A6;

void ov72_022386F4(Sprite *arg0, s32 arg1);

void ov72_022386F4(Sprite *arg0, s32 arg1) {
    s32 temp_r3;

    temp_r3 = arg1;
    ov72_02238688(arg0, *(u16 *)(&ov72_0223B3A4 + temp_r3), *(u16 *)(&ov72_0223B3A6 + temp_r3) + 0x100);
    if (arg1 < 0xA) {
        Sprite_SetAnimCtrlSeq(arg0, 1);
        return;
    }
    Sprite_SetAnimCtrlSeq(arg0, 2);
}
