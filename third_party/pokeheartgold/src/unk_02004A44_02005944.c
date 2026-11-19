#include "global.h"

#include "sound.h"

void GF_SndHandleSetTrackPitch(s32 param_1, u16 param_2, s32 param_3);

void sub_02005944(s32 param_1, u16 param_2, s32 param_3);

void GF_SndHandleSetTempoRatio(s32 arg0, s32 arg1);

void sub_02005944(s32 param_1, u16 param_2, s32 param_3) {
    enum SoundHandleNo SVar1;

    SVar1 = GF_GetSndHandleByPlayerNo(param_1);
    GF_SndHandleSetTrackPitch(SVar1, param_2, param_3);
    return;
}

void GF_SndHandleSetTrackPan(s32 arg0, u16 arg1, s32 arg2);

void GF_SndHandleSetTrackPan(s32 playerNo, u16 trackBitMask, s32 pan) {
    NNS_SndPlayerSetTrackPan(GF_GetSoundHandle(playerNo), trackBitMask, pan);
}

void GF_SndHandleSetTempoRatio(s32 playerNo, s32 ratio) {
    NNS_SndPlayerSetTempoRatio(GF_GetSoundHandle(playerNo), ratio);
}
