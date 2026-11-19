#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "overlay_43_unk_struct_ov64_0222_f0_c4.h"
#include "overlay_43_palpad.h"

s32 ov43_0222AD74(void *, s32, s32);

extern s16 ov43_0222EFA0;

extern s16 ov43_0222EFA2;

void ov43_0222C65C(struct PalPadRoster *arg0, void *arg1);

void ov43_0222C65C(PalPadRoster *arg0, void *arg1) {
    VecFx32 sp0;
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r3;

    Sprite_SetDrawFlag(arg0->deleteEffect, 1);
    ov43_0222AD74(arg1, 0, 0xFF);
    Sprite_SetAnimCtrlSeq(arg0->deleteEffect, 4);
    *((s32 *)&arg0->lastFrame) = 0;
    Sprite_SetAnimActiveFlag(arg0->deleteEffect, 1);
    temp_r3 = arg0->cursor * 0xA;
    temp_r2 = *(s16 *)((u8 *)&ov43_0222EFA0 + temp_r3) * 8;
    sp0.x = temp_r2;
    temp_r1 = (*(s16 *)((u8 *)&ov43_0222EFA2 + temp_r3) * 8) + 0xFC;
    sp0.y = temp_r1;
    sp0.x = temp_r2 << 0xC;
    sp0.y = temp_r1 << 0xC;
    Sprite_SetMatrix(arg0->deleteEffect, (VecFx32 *)&sp0);
    reg_G2_WINOUT = (reg_G2_WINOUT & 0xFFFFC0FF) | 0x700;
    {
        u32 w = (reg_G2_WINOUT & ~0x3F) | 0x1F;
        w |= 0x20;
        reg_G2_WINOUT = w;
    }
    *(s32 *)REG_DISPCNT_ADDR = 0x8000 | (*(s32 *)REG_DISPCNT_ADDR & 0xFFFF1FFF);
    PlaySE(0x628U);
}
