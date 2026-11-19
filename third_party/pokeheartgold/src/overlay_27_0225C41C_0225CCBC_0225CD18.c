#include "global.h"

#include "overlay_27.h"
#include "sprite.h"

extern s32 ov27_0225D3C4;

extern s32 ov27_0225D3C5;

extern s32 ov27_0225D3C6;

void ov27_0225CD18(Ov27TaskData *data, s32 arg1, s32 arg2) {
    VecFx32 sp0;
    s32 temp_r1;
    s32 temp_r2;

    temp_r2 = arg2 * 3;
    temp_r1 = (arg1 - 2) * 0x18;
    (*(s32 *)&sp0) = *(u8 *)(temp_r2 + (((u8 *)&ov27_0225D3C4) + temp_r1)) << 0xC;
    (*(s32 *)((u8 *)&sp0 + 4)) = (*(u8 *)(temp_r2 + (((u8 *)&ov27_0225D3C5) + temp_r1)) + 0x100) << 0xC;
    (*(s32 *)((u8 *)&sp0 + 8)) = 0;
    Sprite_SetAnimCtrlSeq(data->cursor, (s32) * (u8 *)(temp_r2 + (((u8 *)&ov27_0225D3C6) + temp_r1)));
    Sprite_SetMatrix(data->cursor, (VecFx32 *)&sp0);
}
