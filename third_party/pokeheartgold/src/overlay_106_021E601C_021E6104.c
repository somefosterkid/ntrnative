#include "global.h"

#include "screen_fade.h"
#include "unk_02005D10.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

s32 ov106_021E6104(LegendaryCinematic *arg0) {
    ov106_021E5BBC(arg0);
    ov106_021E594C(arg0);
    return 6;
}

s32 ov106_021E6118(LegendaryCinematic *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        return arg0->nextState;
    }
    return 2;
}

u32 ov106_021E6130(LegendaryCinematic *param_1) {
    s32 iVar1;
    BOOL BVar2;

    iVar1 = ov106_021E5D08(param_1);
    if ((iVar1 == 0) && (BVar2 = IsPaletteFadeFinished(), BVar2 == 1)) {
        param_1->frame = 0;
        return 4;
    }
    iVar1 = param_1->frame;
    if (iVar1 == 0) {
        PlaySE(0x930);
    } else if (iVar1 == 0x28) {
        PlaySE(0x92e);
    } else if (iVar1 == 0x4e) {
        sub_02005E44(0x48d);
        PlaySE(0x932);
    } else if (iVar1 == 0x62) {
        PlaySE(0x92f);
    } else if (iVar1 == 0x81) {
        ov106_021E597C(param_1, 6, 1, 3);
    }
    param_1->frame = param_1->frame + 1;
    return 3;
}
