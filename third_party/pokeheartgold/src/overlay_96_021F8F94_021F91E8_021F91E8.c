#include "global.h"

#include "sprite.h"

Sprite *ov96_021EB5B8();

void ov96_021F91E8(void *arg0, s32 arg1);

void ov96_021F91E8(void *arg0, s32 arg1) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r6;

    temp_r0 = arg1 / 100;
    temp_r6 = arg1 - (0x64 * temp_r0);
    temp_r0_2 = temp_r6 / 10;
    Sprite_SetAnimCtrlSeq(ov96_021EB5B8(*(s32 *)((u8 *)arg0 + 4)), temp_r0 + 1);
    Sprite_SetAnimCtrlSeq(ov96_021EB5B8(*(s32 *)((u8 *)arg0 + 8)), temp_r0_2 + 1);
    Sprite_SetAnimCtrlSeq(ov96_021EB5B8(*(s32 *)((u8 *)arg0 + 0xC)), (temp_r6 - (0xA * temp_r0_2)) + 1);
}
