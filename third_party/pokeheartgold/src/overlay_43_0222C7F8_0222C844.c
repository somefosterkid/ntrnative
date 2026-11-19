#include "global.h"

#include "sprite_system.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_palpad.h"

s32 ov43_0222AD74(void *, s32, s32);

extern u8 ov43_0222EFA0;

void ov43_0222C844(void *arg0, struct PalPadGfx *arg1, s32 arg2);

void ov43_0222C844(void *arg0, PalPadGfx *arg1, s32 arg2) {
    u32 v0;

    if (arg2 == 0xA) {
        ov43_0222AD74(arg1, 0, 0xFF);
        return;
    }
    v0 = arg2 * 0xA;
    Sprite_SetPositionXY(*((Sprite * *)&arg1->cursor), (s16)((*(s16 *)(&ov43_0222EFA0 + v0) + 7) * 8), (s16)((*(s16 *)((u8 *)(&ov43_0222EFA0 + v0) + 2) + 2) * 8));
    ov43_0222AD74(arg1, 1, 0xFF);
}
