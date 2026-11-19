#include "global.h"

#include "constants/sndseq.h"

#include "heap.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "unk_02005D10.h"

struct SndMicState;

typedef struct SndMicState {
    u8 micActive;
    int unk4;
    int monoFlag;
    int unkC;
} SndMicState;

extern void sub_02005FD8(void);

u16 sub_02004AAC(void);

void sub_02005150(s32 seqNo, u16 unused);

int sub_02005328(int a0);

int GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *handle);

void sub_020059E0(u8 a0);

u16 GBSounds_GetDSSeqNoByGBSeqNo(u16 seqNo);

extern SndMicState _021D05E8;

void sub_0200508C(u16 seqNo, int unused);

void sub_0200508C(u16 seqNo, int unused) {
    int curSeq;
    u8 *p;
    u16 curDsSeq;

    p = GF_SdatGetAttrPtr(0xC);
    GF_SdatGetAttrPtr(0x18);
    GF_SdatGetAttrPtr(0x20);
    curSeq = GF_NNS_SndPlayerGetSeqNo(GF_GetSoundHandle(0));
    curDsSeq = curSeq;
    if (curSeq > SEQ_GS_P_START) {
        curDsSeq = GBSounds_GetDSSeqNoByGBSeqNo(curDsSeq);
    }
    if (*p == 0 && curDsSeq == seqNo && sub_02004AAC() != SEQ_GS_BICYCLE) {
        return;
    }
    sub_020059E0(1);
    if (_021D05E8.unkC == 0 || _021D05E8.unk4 == 0) {
        sub_02005FD8();
        _021D05E8.unk4 = 1;
    }
    if (curDsSeq != seqNo) {
        sub_020053A8(1, 0);
        Sound_Stop();
    }
    if (*p == 1) {
        GF_Snd_LoadState(sub_02005328(2));
        sub_02004B24(4);
        GF_Snd_SaveState(GF_SdatGetAttrPtr(0x1A));
        if (curDsSeq != seqNo) {
            sub_020053A8(1, 0);
        }
        sub_02005150(seqNo, (u16)curSeq);
        return;
    }
    PlayBGM(seqNo);
}
