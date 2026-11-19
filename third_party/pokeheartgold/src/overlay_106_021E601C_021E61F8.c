#include "global.h"

#include "screen_fade.h"
#include "unk_02005D10.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

s32 ov106_021E61F8(LegendaryCinematic *arg0) {
    if ((ov106_021E5D08(arg0) == 0) && (IsPaletteFadeFinished() == 1)) {
        arg0->frame = 0;
        return 1;
    }
    if (!(0x3F & arg0->frame)) {
        PlaySE(0x92DU);
    }
    if (arg0->frame == 0xBE) {
        ov106_021E59D0(arg0, 0xA, 1, 1);
    }
    arg0->frame = (s32)(arg0->frame + 1);
    return 5;
}
