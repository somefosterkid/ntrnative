#include "global.h"

#include "bg_window.h"
#include "sprite.h"

void ov65_0221D8C4(u8 *param_1, s32 param_2, u8 *param_3);

void ov65_0221D8C4(u8 *param_1, s32 param_2, u8 *param_3) {
    Sprite_SetDrawFlag(*(Sprite **)(param_3 + param_2 * 4 + 0x40c), 0);
    Sprite_SetDrawFlag(*(Sprite **)(param_3 + (param_2 + 2) * 4 + 0x40c), 0);
    Sprite_SetDrawFlag(*(Sprite **)(param_3 + (param_2 + 4) * 4 + 0x40c), 0);
    ClearWindowTilemapAndCopyToVram((Window *)(param_1 + (param_2 + 0x1a) * sizeof(Window)));
    ClearWindowTilemapAndCopyToVram((Window *)(param_1 + (param_2 + 0x1c) * sizeof(Window)));
    ClearWindowTilemapAndCopyToVram((Window *)(param_1 + (param_2 + 0x1e) * sizeof(Window)));
    ClearWindowTilemapAndCopyToVram((Window *)(param_1 + (param_2 + 0x20) * sizeof(Window)));
    return;
}
