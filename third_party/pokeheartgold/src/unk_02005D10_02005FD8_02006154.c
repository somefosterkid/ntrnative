#include "global.h"

#include "sound.h"

void StopSE(u16 sndseq, s32 a1);

void StopSE(u16 sndseq, s32 a1) {
    NNS_SndPlayerStopSeqBySeqNo((s32)sndseq, a1);
}

void sub_0200615C(s32 arg0, s32 arg1);

void sub_0200615C(s32 playerNo, s32 fadeFrame) {
    NNS_SndPlayerStopSeq(GF_GetSoundHandle(playerNo), fadeFrame);
}
