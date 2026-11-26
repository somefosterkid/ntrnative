#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "sprite_system.h"
#include "overlay_108_safari_area_customizer.h"

int ov108_021E6450(SafariAreaCustomizer *data);

void ov108_021E78C0(SafariAreaCustomizer *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2);

void ov108_021E7EB0(SafariAreaCustomizer *base);

extern TouchscreenHitbox ov108_021EA7D0;

s32 ov108_021E66AC(SafariAreaCustomizer *arg0);

u8 ov108_021E7700(void *, s32, s32, s32);
void ov108_021E6804(SafariAreaCustomizer *arg0);
void ov108_021E6850(SafariAreaCustomizer *arg0);

s32 ov108_021E66AC(SafariAreaCustomizer *arg0) {
    u16 src;
    s32 v0;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&ov108_021EA7D0);
    if (v0 == -1) {
        return 0;
    }
    switch (v0) {
    case 6:
        src = 0xFFFE;
        if (DoesPixelAtScreenXYMatchPtrVal(arg0->bgConfig, 3U, gSystem.touchX, gSystem.touchY, &src) == 0) {
            return 0;
        }
        arg0->cursor = 6U;
        PlaySE(0x5DCU);
        return 2;
    case 7:
        if (arg0->page == 0) {
            return 0;
        }
        arg0->unk_184E2_1 = 1;
        arg0->cursor = 5U;
        ov108_021E78C0(arg0, 1, 0, 0);
        Sprite_SetAnimActiveFlag(arg0->unk_364, 1);
        Sprite_ResetAnimCtrlState(arg0->unk_364);
        PlaySE(0x5E1U);
        return 3;
    case 8:
        if (arg0->page == 1) {
            return 0;
        }
        arg0->unk_184E2_1 = 0;
        arg0->cursor = 0U;
        ov108_021E78C0(arg0, 1, 0, 0);
        Sprite_SetAnimActiveFlag(arg0->unk_368, 1);
        Sprite_ResetAnimCtrlState(arg0->unk_368);
        PlaySE(0x5E1U);
        return 3;
    default:
        arg0->cursor = (u8)v0;
        ov108_021E78F4(arg0, 1, arg0->cursor);
        ov108_021E7EB0(arg0);
        if ((arg0->cursor + (6 * arg0->page)) == arg0->areaSet.areas[arg0->areaIdx].area_no) {
            return 0;
        }
        return ov108_021E6450(arg0);
    }
}

void ov108_021E6804(SafariAreaCustomizer *arg0) {
    arg0->unk_184E2_3 = 2;
    ov108_021E78F4(arg0, 0, arg0->cursor);
    thunk_Sprite_SetPriority(arg0->unk_354[0], 0);
    ov108_021E78F4(arg0, 3, arg0->areaIdx);
    ov108_021E7700(arg0, 0, 2, 1);
}

void ov108_021E6850(SafariAreaCustomizer *arg0) {
    arg0->unk_184E2_3 = 0;
    ov108_021E78F4(arg0, 0, arg0->areaIdx);
    thunk_Sprite_SetPriority(arg0->unk_354[0], 2);
    ov108_021E78C0(arg0, 3, 0, 0);
    ov108_021E7700(arg0, 0, 0, 1);
}
