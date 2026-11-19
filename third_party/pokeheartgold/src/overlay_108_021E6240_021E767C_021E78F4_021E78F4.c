#include "global.h"

#include "sprite_system.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E78C0(SafariAreaCustomizer *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2) {
    s16 x;
    s16 y;
    s32 v0;

    v0 = 0;
    if (arg2 >= 6U) {
        x = 0xE0;
        y = 0xB4;
        v0 = 1;
    } else if (arg1 != 1) {
        x = (s32)((0x48 * ((s32)arg2 % 3)) + 0x38);
        y = (0x48 * ((s32)arg2 / 3)) + 0x38;
        if (arg1 == 2) {
            y += 0xC0;
        } else if (arg1 == 3) {
            v0 = 2;
        } else if (arg1 == 0) {
            if (arg0->unk_184E2_3 == 2) {
                v0 = 3;
            }
        }
    } else {
        x = (s32)((0x50 * ((s32)arg2 % 3)) + 0x30);
        y = (0x48 * ((s32)arg2 / 3)) + 0x38;
    }
    Sprite_SetPositionXY(arg0->unk_354[arg1], (s16)x, y);
    ov108_021E78C0(arg0, arg1, v0, 1);
}
