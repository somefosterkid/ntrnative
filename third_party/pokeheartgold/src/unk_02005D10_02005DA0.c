#include "global.h"

#include "sound.h"

s32 sub_02004A60(u32 param_1);

s32 sub_02005464(s32, s32);

void sub_02005DA0(s32 arg0, s32 arg1);

s32 sub_02005328(s32);
s32 sub_02006C14(s32, s32, s32, s32, s32, s32);
void sub_02005DC4(s32 arg0, s32 arg1, s32 arg2);

void sub_02005DA0(s32 arg0, s32 arg1) {
    sub_02004A60(arg0);
    sub_02005464(arg0, arg1);
    GF_SndSetState(1U);
    if (arg1 == 0) {
        GF_SndWorkSetGbSoundsVolume(0x80U);
    }
}

void sub_02005DC4(s32 seqNo, s32 arg1, s32 arg2) {
    GF_Snd_LoadState(sub_02005328(3));
    GF_Snd_LoadSeq(seqNo);
    sub_02006C14(arg2, -1, -1, -1, seqNo, 1);
}
