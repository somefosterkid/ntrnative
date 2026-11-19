#include "global.h"
struct SafariAreaCustomizer;

#include "system.h"
#include "unk_02005D10.h"
#include "bg_window.h"
#include "touchscreen.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E6A58(SafariAreaCustomizer *arg0);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2);

s32 ov108_021E7CD8(void *, u8);

s32 ov108_021E62B4(SafariAreaCustomizer *arg0);

extern TouchscreenHitbox ov108_021EA760;
s32 ov108_021E63B8(SafariAreaCustomizer *arg0);

s32 ov108_021E62B4(SafariAreaCustomizer *arg0) {
    u8 v0;
    u8 v1;
    u8 v2;

    if (2 & gSystem.newKeys) {
        PlaySE(0x5DCU);
        arg0->unk_C = 6;
        return 4;
    }
    if (1 & gSystem.newKeys) {
        if ((u32) arg0->areaIdx >= 6U) {
            PlaySE(0x5DCU);
            arg0->unk_C = 6;
            return 4;
        }
        PlaySE(0x5DCU);
        ov108_021E6A58(arg0);
        return 1;
    }
    if (!(0xF0 & gSystem.newKeys)) {
        return 0;
    }
    v0 = arg0->areaIdx;
    v2 = (u8)((s32)v0 % 3);
    v1 = (u8)((s32)v0 / 3);
    if ((u32)v1 < 2U) {
        if (0x10 & gSystem.newKeys) {
            v2 = (u8)((s32)(v2 + 1) % 3);
        } else if (0x20 & gSystem.newKeys) {
            v2 = (u8)((s32)(v2 + 2) % 3);
        }
    }
    if (0x40 & gSystem.newKeys) {
        v1 = (u8)((s32)(v1 + 2) % 3);
    } else if (0x80 & gSystem.newKeys) {
        v1 = (u8)((s32)(v1 + 1) % 3);
    }
    arg0->areaIdx = (u8)(v2 + (v1 * 3));
    if (((u32)v1 < 2U) || (0xC0 & gSystem.newKeys)) {
        PlaySE(0x5E5U);
    }
    ov108_021E78F4(arg0, 0, arg0->areaIdx);
    ov108_021E7CD8(arg0, arg0->areaIdx);
    return 0;
}

s32 ov108_021E63B8(SafariAreaCustomizer *arg0) {
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
        PlaySE(0x5DCU);
        arg0->unk_C = 6;
        return 4;
    }
    arg0->areaIdx = (u8)v0;
    arg0->cursor = 0;
    ov108_021E78F4(arg0, 0, arg0->areaIdx);
    ov108_021E7CD8(arg0, arg0->areaIdx);
    PlaySE(0x5DCU);
    ov108_021E6A58(arg0);
    return 1;
}
