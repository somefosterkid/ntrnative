#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_106.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"
#include "sound_02004A44.h"


s32 ov106_021E6BD4(LegendaryCinematic *arg0, s32 *arg1) {
    if (arg1[0] != 8) {
        GF_AssertFail();
    }
    SetBgPriority((u8)arg1[1], (u8)arg1[2]);
    return 1;
}

u32 ov106_021E6BF8(LegendaryCinematic *param_1, s32 *param_2) {
    ArceusCinematic *iVar1;

    iVar1 = param_1->arceus;
    if (*param_2 != 9) {
        GF_AssertFail();
    }
    if (param_2[1] == 0) {
        iVar1->blendPlane1 = param_2[2];
        iVar1->blendStartEv = param_2[3];
    } else {
        iVar1->blendPlane2 = param_2[2];
        iVar1->blendStartEv2 = param_2[3];
    }
    return 1;
}

s32 ov106_021E6C28(LegendaryCinematic *arg0, s32 *arg1) {
    ArceusCinematic *v0;

    v0 = arg0->arceus;
    if (arg1[0] != 0xA) {
        GF_AssertFail();
    }
    v0->blendTargetEv = arg1[1];
    v0->blendDuration = arg1[2];
    arg0->state = 3;
    arg0->nextState = 4;
    return 2;
}

s32 ov106_021E6C5C(LegendaryCinematic *arg0, s32 *arg1) {
    ArceusCinematic *v0;

    v0 = arg0->arceus;
    if (arg1[0] != 0xB) {
        GF_AssertFail();
    }
    reg_G2_BLDCNT = 0;
    v0->blendPlane1 = 0;
    v0->blendStartEv = 0;
    v0->blendPlane2 = 0;
    v0->blendStartEv2 = 0;
    return 1;
}

s32 ov106_021E6C88(LegendaryCinematic *arg0, s32 *arg1) {
    ArceusCinematic *v0;

    v0 = arg0->arceus;
    if (arg1[0] != 0xC) {
        GF_AssertFail();
    }
    v0->savedBgm = GF_GetCurrentPlayingBGM();
    return 1;
}
