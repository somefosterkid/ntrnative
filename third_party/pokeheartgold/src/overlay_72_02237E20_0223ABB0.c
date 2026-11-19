#include "global.h"

#include "screen_fade.h"
#include "sprite.h"
#include "overlay_72_wfc_connect.h"
struct NintendoWfcConnect;

void ov72_0223ABB0(NintendoWfcConnect *arg0);

void ov72_0223ABB0(NintendoWfcConnect *arg0) {
    s32 v1;
    s8 v0;

    v1 = 0;
    do {
        v0 = arg0->numberInput.digits[v1];
        if (v0 == -1) {
            Sprite_SetAnimCtrlSeq(arg0->sprites[v1], 0);
        } else if (((s32)v0 >= 0) && ((s32)v0 <= 9)) {
            Sprite_SetAnimCtrlSeq(arg0->sprites[v1], v0 + 1);
        }
        v1 += 1;
    } while (v1 < 3);
}

s32 ov72_0223ABF4(void);

s32 ov72_0223ABF4(void) {
    return 0;
}

s32 ov72_0223ABF8(void);

s32 ov72_0223ABF8(void) {
    return 0;
}

s32 ov72_0223ABFC(void);

s32 ov72_0223ABFC(void) {
    return 0;
}

s32 ov72_0223AC00(NintendoWfcConnect *arg0);

s32 ov72_0223AC00(NintendoWfcConnect *arg0) {
    BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 4, 1, HEAP_ID_67);
    arg0->numberInput.state = 8;
    return 0;
}

s32 ov72_02238B18(struct NintendoWfcConnect *, s32);

s32 ov72_0223AD94(NintendoWfcConnect *);

s32 ov72_0223AC2C(NintendoWfcConnect *arg0);

s32 ov72_0223AC2C(NintendoWfcConnect *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        ov72_0223AD94(arg0);
        ov72_02238B18(arg0, 0);
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 4, 1, HEAP_ID_67);
        arg0->numberInput.state = 9;
    }
    return 0;
}

s32 ov72_0223AE6C();

s32 ov72_0223AC6C(NintendoWfcConnect *arg0);

s32 ov72_0223AC6C(NintendoWfcConnect *arg0) {
    if (ov72_0223AE6C() == 0) {
        arg0->numberInput.state = (u8) arg0->numberInput.nextState;
    }
    return 0;
}

s32 ov72_0223AC88(NintendoWfcConnect *arg0);

s32 ov72_0223AC88(NintendoWfcConnect *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        arg0->numberInput.state = 0xB;
    }
    return 0;
}

s32 ov72_0223ACA4(void);

s32 ov72_0223ACA4(void) {
    return 1;
}
