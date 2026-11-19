#include "global.h"

#include "bg_window.h"

s32 ov102_021EC37C(void *, s32, s32, s32, s32, s32);

void ov102_021EBB50(u8 *arg0);

void ov102_021EBB50(u8 *arg0) {
    ov102_021EC37C(arg0 + 0x1C, 2, 0x3F, 0x7C, 0, 0xC);
}

s32 ov102_021EC3AC(void *);

s32 ov102_021EBB6C(u8 *arg0);

s32 ov102_021EBB6C(u8 *arg0) {
    if (ov102_021EC3AC(arg0 + 0x1C) != 0) {
        ToggleBgLayer(1U, 0U);
        return 1;
    }
    return 0;
}

void ov102_021EBB88(u8 *arg0);

void ov102_021EBB88(u8 *arg0) {
    G2_SetBlendAlpha(2, 0x3F, 0, 0x10);
    ToggleBgLayer(1U, 1U);
    ov102_021EC37C(arg0 + 0x1C, 2, 0x3F, 0, 0x7C, 0xC);
}
