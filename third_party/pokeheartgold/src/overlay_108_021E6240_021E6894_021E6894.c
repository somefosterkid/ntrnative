#include "global.h"
struct SafariAreaCustomizer;

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E6850(SafariAreaCustomizer *arg0);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2);

s32 ov108_021E6894(SafariAreaCustomizer *arg0);

s32 ov108_021E6894(SafariAreaCustomizer *arg0) {
    u8 v0;
    u8 v1;
    u8 v2;
    u8 v3;

    if (2 & gSystem.newKeys) {
        ov108_021E6850(arg0);
        arg0->unk_C = 2;
        PlaySE(0x5DCU);
        return 4;
    }
    if (1 & gSystem.newKeys) {
        v0 = arg0->cursor;
        if (((u32)v0 >= 6U) || (arg0->areaIdx == v0)) {
            ov108_021E6850(arg0);
            arg0->unk_C = 2;
            PlaySE(0x5DCU);
            return 4;
        }
        PlaySE(0x69CU);
        return 3;
    }
    if (!(0xF0 & gSystem.newKeys)) {
        return 0;
    }
    v1 = arg0->cursor;
    v3 = (u8)((s32)v1 % 3);
    v2 = (u8)((s32)v1 / 3);
    if ((u32)v2 < 2U) {
        if (0x10 & gSystem.newKeys) {
            v3 = (u8)((s32)(v3 + 1) % 3);
        } else if (0x20 & gSystem.newKeys) {
            v3 = (u8)((s32)(v3 + 2) % 3);
        }
    }
    if (0x40 & gSystem.newKeys) {
        v2 = (u8)((s32)(v2 + 2) % 3);
    } else if (0x80 & gSystem.newKeys) {
        v2 = (u8)((s32)(v2 + 1) % 3);
    }
    arg0->cursor = (u8)(v3 + (v2 * 3));
    if (((u32)v2 < 2U) || (0xC0 & gSystem.newKeys)) {
        PlaySE(0x5E5U);
    }
    ov108_021E78F4(arg0, 0, arg0->cursor);
    return 0;
}
