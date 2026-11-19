#include "global.h"

#include "sprite_system.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E77D4(SafariAreaCustomizer *param_1);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2);

void ov108_021E7EB0(SafariAreaCustomizer *base);

s32 ov108_021E8270(SafariAreaCustomizer *arg0);

s32 ov108_021E8270(SafariAreaCustomizer *arg0) {
    u32 dx;
    s32 v0;

    if (((u32)(*((u8 *)((u8 *)&arg0->unk_184E1 + 0x1)) << 0x1E) >> 0x1F) == 0) {
        dx = -0x20;
    } else {
        dx = 0x20;
    }
    v0 = 0;
    do {
        ManagedSprite_OffsetPositionXY(arg0->sprites[v0], dx, 0);
        v0 += 1;
    } while (v0 < 0xC);
    arg0->unk_184E1 = (u8)(arg0->unk_184E1 + 1);
    if ((u32) arg0->unk_184E1 >= 8U) {
        arg0->unk_184E1 = 0U;
        ov108_021E78F4(arg0, 1, arg0->cursor);
        ov108_021E7EB0(arg0);
        ov108_021E77D4(arg0);
        return 1;
    }
    return 0;
}
