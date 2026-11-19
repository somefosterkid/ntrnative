#include "global.h"

#include "bg_window.h"

void ov102_021EB570(u8 *param_1, s8 param_2, s8 param_3);

void ov102_021EB570(u8 *param_1, s8 param_2, s8 param_3) {
    u16 *puVar1;
    puVar1 = *((u16 **)(param_1 + 0x60));
    CopyToBgTilemapRect(*((BgConfig **)(param_1 + 0x10)), 3, (u8)((param_2 * 0x11) + 2), 0, 0xb, 6, puVar1 + 6, 0, (u8)(param_3 * 6), (u8)((*puVar1) >> 3), (u8)(puVar1[1] >> 3));
}
