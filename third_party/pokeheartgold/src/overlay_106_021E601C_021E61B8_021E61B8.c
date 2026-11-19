#include "global.h"

#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

extern const LegendaryCinematicModelDesc ov106_021E6E2C[];
extern const LegendaryCinematicCamera ov106_021E6E9C;

s32 ov106_021E61B8(LegendaryCinematic *arg0);

s32 ov106_021E61B8(LegendaryCinematic *arg0) {
    ov106_021E5BBC(arg0);
    ov106_021E59FC(arg0, &ov106_021E6E9C);
    ov106_021E5AA8(arg0, NARC_a_2_4_3, &ov106_021E6E2C[arg0->args->unk4E], 1);
    ov106_021E5954(arg0, 6, 1, 5);
    return 2;
}
