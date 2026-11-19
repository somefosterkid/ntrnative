#include "global.h"
struct SafariAreaCustomizer;

#include "bg_window.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E6850(SafariAreaCustomizer *arg0);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2);

extern TouchscreenHitbox ov108_021EA760;

s32 ov108_021E69A0(SafariAreaCustomizer *arg0);

s32 ov108_021E69A0(SafariAreaCustomizer *arg0) {
    u16 src;
    s32 v0;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&ov108_021EA760);
    if (v0 == -1) {
        return 0;
    }
    if (v0 == 6) {
        src = 0xFFFE;
        if (DoesPixelAtScreenXYMatchPtrVal(arg0->bgConfig, 3U, gSystem.touchX, gSystem.touchY, &src) == 0) {
            return 0;
        }
        arg0->cursor = 6U;
        ov108_021E6850(arg0);
        arg0->unk_C = 2;
        PlaySE(0x5DCU);
        return 4;
    }
    if (arg0->areaIdx == v0) {
        ov108_021E6850(arg0);
        arg0->unk_C = 2;
        PlaySE(0x5DCU);
        return 4;
    }
    arg0->cursor = (u8)v0;
    ov108_021E78F4(arg0, 0, arg0->cursor);
    PlaySE(0x69CU);
    return 3;
}
