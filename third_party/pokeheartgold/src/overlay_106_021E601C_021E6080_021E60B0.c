#include "global.h"

#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

extern GXRgb ov106_021E6E04[8];
extern const LegendaryCinematicModelDesc ov106_021E6E50[];
extern const LegendaryCinematicCamera ov106_021E6E74;

s32 ov106_021E60B0(LegendaryCinematic *arg0);

s32 ov106_021E60B0(LegendaryCinematic *arg0) {
    ov106_021E5900(arg0);
    ov106_021E5A44(&arg0->args->unk4);
    ov106_021E5A74(ov106_021E6E04);
    ov106_021E59FC(arg0, &ov106_021E6E74);
    ov106_021E5AA8(arg0, NARC_a_2_4_3, &ov106_021E6E50[arg0->args->unk4E], 1);
    ov106_021E59A4(arg0, 6, 1, 3);
    return 2;
}
