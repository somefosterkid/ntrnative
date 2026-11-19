#include "global.h"

#include "sound.h"
#include "sound_radio.h"

u16 GBSounds_GetGBSeqNoByDSSeqNo(u16);

void sub_02005EEC(void);

void StopBGM(u16 a0, s32 a1);

void StopBGM(u16 a0, s32 a1) {
    s32 seqNo;
    s32 playerNo;

    NNS_SndPlayerStopSeqBySeqNo((s32)a0, a1);
    seqNo = GBSounds_GetGBSeqNoByDSSeqNo(a0);
    if (a0 != seqNo) {
        NNS_SndPlayerStopSeqBySeqNo(seqNo, a1);
    }
    playerNo = GF_GetPlayerNoBySeq(a0);
    if (playerNo != 0xFF) {
        NNS_SndHandleReleaseSeq(GF_GetSoundHandle(GF_GetSndHandleByPlayerNo(playerNo)));
    }
    sub_02005EEC();
}

void GF_SetCurrentPlayingBGM(u16);

void sub_02004A60(u32);

void sub_02004AB8(u16);

void sub_02005EEC(void) {
    SndRadio_StopSeq(0);
    sub_02004A60(0);
    GF_SetCurrentPlayingBGM(0);
    sub_02004AB8(0);
    GF_SndSetState(0);
    return;
}
