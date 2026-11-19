#include "global.h"

#include "error_handling.h"
#include "overlay_07_foresight_sprite_context.h"

s32 ov07_02222268(void *, s16, s16, s16, s16, u16);

void ov07_0222DE30(void *arg0);

void ov07_0222DE30(void *arg0) {
    u32 v0;

    v0 = *((u32 *)&((ForesightSpriteContext *)arg0)->segment);
    switch (v0) {
    case 0:
        ov07_02222268(arg0 + 0x18, 0, 0x28, 0, 0x28, 8);
        return;
    case 1:
        ov07_02222268(arg0 + 0x18, 0, 0, 0, -0x50, 8);
        return;
    case 2:
        ov07_02222268(arg0 + 0x18, 0, -0x50, 0, 0x50, 8);
        return;
    case 3:
        ov07_02222268(arg0 + 0x18, 0, 0, 0, -0x50, 8);
        return;
    case 4:
        ov07_02222268(arg0 + 0x18, 0, 0x50, 0, 0x50, 8);
        return;
    case 5:
        ov07_02222268(arg0 + 0x18, 0, -0x28, 0, -0x28, 8);
        return;
    default:
        GF_AssertFail();
        return;
    }
}
