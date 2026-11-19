#include "global.h"

#include "sound.h"

void sub_02005908(void);

BOOL sub_020058F4(void);

void GF_SndHandleSetTrackPitch(s32 param_1, u16 param_2, s32 param_3);
void sub_0200592C(u16 seqID, u16 param_2, s32 param_3);

BOOL sub_020058F4(void) {
    BOOL BVar1;

    BVar1 = NNS_SndCaptureIsActive();
    if (BVar1 == 1) {
        sub_02005908();
    }
    return BVar1;
}

void sub_02005908(void) {
    NNS_SndCaptureGetCaptureType();
}

void sub_02005910(s32 arg0);

void sub_02005910(s32 frames) {
    NNS_SndCaptureStopReverb(frames);
}

void GF_SndHandleSetTrackPitch(s32 arg0, u16 arg1, s32 arg2);

void GF_SndHandleSetTrackPitch(s32 playerNo, u16 trackBitMask, s32 pitch) {
    NNS_SndPlayerSetTrackPitch(GF_GetSoundHandle(playerNo), trackBitMask, pitch);
}

void sub_0200592C(u16 seqID, u16 param_2, s32 param_3) {
    s32 playerNo;
    enum SoundHandleNo SVar1;

    playerNo = GF_GetPlayerNoBySeq(seqID);
    SVar1 = GF_GetSndHandleByPlayerNo(playerNo);
    GF_SndHandleSetTrackPitch(SVar1, param_2, param_3);
    return;
}
