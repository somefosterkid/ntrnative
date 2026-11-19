#include "global.h"

#include "error_handling.h"
#include "overlay_106.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"
#include "sound_02004A44.h"
#include "unk_02005D10.h"
#include "sound.h"
#include "sprite_system.h"
#include "overlay_manager.h"


struct OverlayManager;
BOOL ov106_021E6D6C(OverlayManager *appMan, int *state);

u32 ov106_021E6CA8(LegendaryCinematic *param_1, s32 *param_2) {
    u16 sndseq;

    if (*param_2 != 0xd) {
        GF_AssertFail();
    }
    sndseq = GF_GetCurrentPlayingBGM();
    StopBGM(sndseq, 0);
    return 1;
}

s32 ov106_021E6CC4(LegendaryCinematic *arg0, s32 *arg1) {
    s32 seqno;
    ArceusCinematic *v0;

    v0 = arg0->arceus;
    if (arg1[0] != 0xE) {
        GF_AssertFail();
    }
    seqno = arg1[1];
    if (seqno == -1) {
        PlayBGM(v0->savedBgm);
    } else {
        PlayBGM((u16)seqno);
    }
    return 1;
}

s32 ov106_021E6CF8(LegendaryCinematic *arg0, s32 *arg1) {
    ArceusCinematic *v0;

    v0 = arg0->arceus;
    if (arg1[0] != 0xF) {
        GF_AssertFail();
    }
    ManagedSprite_SetDrawFlag(v0->sprites[0], arg1[1]);
    return 1;
}

s32 ov106_021E6D1C(LegendaryCinematic *arg0, s32 *arg1) {
    ArceusCinematic *v0;

    v0 = arg0->arceus;
    if (arg1[0] != 0x10) {
        GF_AssertFail();
    }
    ManagedSprite_SetPositionXY(v0->sprites[0], (s16)arg1[1], (s16)arg1[2]);
    return 1;
}

s32 ov106_021E6D48(LegendaryCinematic *arg0, s32 *arg1) {
    ArceusCinematic *v0;

    v0 = arg0->arceus;
    if (arg1[0] != 0x11) {
        GF_AssertFail();
    }
    ManagedSprite_SetPriority(v0->sprites[0], arg1[1]);
    return 1;
}

BOOL ov106_021E6D6C(OverlayManager *appMan, int *state) {
    return 1;
}
