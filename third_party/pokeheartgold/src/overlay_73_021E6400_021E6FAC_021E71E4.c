#include "global.h"

#include "bg_window.h"

extern u8 _021EA51C;

void ov73_021E71E4(u32 *param_1, s32 param_2, s8 param_3);

void ov73_021E71E4(u32 *param_1, s32 param_2, s8 param_3) {
    u16 *puVar1;
    puVar1 = (u16 *)param_1[0xc3];
    CopyToBgTilemapRect((BgConfig *)(*param_1), 1, (&_021EA51C)[param_2], 0x14, 8, 4, puVar1 + 6, 0, param_3 << 2, (u8)((*puVar1) >> 3), (u8)(puVar1[1] >> 3));
    return;
}
