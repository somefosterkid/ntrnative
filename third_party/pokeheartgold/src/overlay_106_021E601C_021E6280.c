#include "global.h"

#include "screen_fade.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

extern GXRgb ov106_021E6EC4[8];
extern const LegendaryCinematicModelDesc ov106_021E6EE4[];
extern const LegendaryCinematicCamera ov106_021E6EFC;

s32 ov106_021E6280(LegendaryCinematic *arg0) {
    ov106_021E5900(arg0);
    ov106_021E59FC(arg0, &ov106_021E6EFC);
    ov106_021E5A44(&arg0->args->unk4);
    ov106_021E5A74(ov106_021E6EC4);
    ov106_021E5AA8(arg0, NARC_a_2_4_4, ov106_021E6EE4, 2);
    ov106_021E5954(arg0, 6, 1, 3);
    return 2;
}

s32 ov106_021E62C8(LegendaryCinematic *arg0) {
    ov106_021E5BBC(arg0);
    ov106_021E594C(arg0);
    return 4;
}

s32 ov106_021E62DC(LegendaryCinematic *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        return arg0->nextState;
    }
    return 2;
}
