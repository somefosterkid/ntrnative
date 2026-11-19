#include "global.h"

#include "sprite_system.h"
#include "overlay_43_unk_struct_ov64_02230904.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_palpad.h"

s32 ov43_0222AD74(void *, s32, s32);

void ov43_0222DB28(struct PalPadDetails *arg0, struct PalPadGfx *arg1, s32 arg2);

void ov43_0222DB28(PalPadDetails *arg0, PalPadGfx *arg1, s32 arg2) {
    s32 v0;

    if (*((s32 *)&arg0->hasFrontierPages) == 1) {
        v0 = 2;
        Sprite_SetPositionXY(*((Sprite * *)&arg1->cursor), (s16)((arg2 << 5) + 0x10), 0xB0);
    } else if (arg2 == 2) {
        v0 = 2;
        Sprite_SetPositionXY(*((Sprite * *)&arg1->cursor), 0xF0, 0xB0);
    } else {
        v0 = 1;
        Sprite_SetPositionXY(*((Sprite * *)&arg1->cursor), (s16)((0x70 * arg2) + 0x38), 0xB0);
    }
    ov43_0222AD74(arg1, 1, v0);
}
