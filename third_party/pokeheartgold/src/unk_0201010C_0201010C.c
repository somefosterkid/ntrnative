#include "global.h"

#include "screen_fade_fx.h"
#include "screen_fade_shape_globals.h"
#include "unk_0201010C.h"

void sub_02010B14(ScreenFade *param0, int param1);

BOOL sub_02010BB4(ScreenFade *param0);

// Was .data in asm/unk_0201010C.s (_0210F64C.._0210F6B4). The shape pointers start NULL: each FadeFunc fills its own.
ScreenFadeShapeGlobals _0210F64C = {
    .shapes = {
        { NULL, 1, 1 }, // FadeFunc_02
        { NULL, 2, 1 }, // _08
        { NULL, 1, 0 }, // _03
        { NULL, 3, 0 }, // _31
        { NULL, 1, 1 }, // _04
        { NULL, 1, 0 }, // _05
        { NULL, 3, 1 }, // _30
        { NULL, 2, 0 }, // _09
        { NULL, 2, 0 }, // _11
        { NULL, 2, 1 }, // _10
    },
    .windowFadeOut = {
        { { 0x00, 0x5E, 0xFF, 0x62 }, { 0x80, 0x60, 0x80, 0x60 }, 0x00, 0x3F, 0x20, 0x01 },
        { NULL, 2, 1 },
        0xB33,
    },
    .windowFadeIn = {
        { { 0x80, 0x60, 0x80, 0x60 }, { 0x00, 0x5E, 0xFF, 0x62 }, 0x00, 0x3F, 0x20, 0x00 },
        { NULL, 2, 0 },
        0xB33,
    },
};

BOOL FadeFunc_00(void *arg0) {
    ScreenFade *fade = arg0;

    if (fade->state == FADE_IDLE) {
        fade->direction = FADE_OUT;
        fade->method = FADE_BY_BRIGHTNESS;
        sub_02010B14(fade, 1);
        return 0;
    }
    return sub_02010BB4(fade);
}
