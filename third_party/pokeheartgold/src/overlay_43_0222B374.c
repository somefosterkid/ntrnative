#include "global.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_palpad.h"

s32 ov43_0222B278(PalPadMenu *, PalPadShared *, PalPadGfx *);

s32 ov43_0222B324(PalPadMenu *, PalPadShared *, PalPadGfx *, s32 *);

unsigned int ov43_0222B374(PalPadMenu *arg0, PalPadShared *arg1, PalPadGfx *arg2);

void ov43_0222B3A4(PalPadMenu *arg0, PalPadGfx *arg1);

unsigned int ov43_0222B374(PalPadMenu *arg0, PalPadShared *arg1, PalPadGfx *arg2) {
    s32 v0;
    v0 = 0;
    ov43_0222B324(arg0, arg1, arg2, &v0);
    if (v0 != 0) {
        arg1->inputState = 1;
        return;
    }
    return ov43_0222B278(arg0, arg1, arg2);
}

void ov43_0222B3A4(PalPadMenu *arg0, PalPadGfx *arg1) {
    Sprite_SetPositionXY(arg1->cursor, 0x80, (s16)((0x28 * arg0->selection) + 0x30));
}
