#include "global.h"

#include "sound.h"
#include "sound_02004A44.h"

u16 GBSounds_GetGBSeqNoByDSSeqNo(u16);

u16 sub_02004AAC(void);

s32 sub_02005C08(u8);

void SoundSys_ToggleGBSounds(void) {
    u16 seqno;
    u8 v0;

    if (SoundSys_GetGBSoundsState() == 0) {
        sub_02005C08(1);
    } else {
        sub_02005C08(0);
    }
    if (sub_02004AAC() == 0) {
        seqno = GF_GetCurrentPlayingBGM();
        v0 = GF_SndWorkGetGbSoundsVolume();
        if (seqno != GBSounds_GetGBSeqNoByDSSeqNo(seqno)) {
            PlayBGM(seqno);
        }
        GF_SndHandleMoveVolume(0, (s32)v0, 0);
    }
}
