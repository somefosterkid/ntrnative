#include "global.h"

#include "sound_02004A44.h"


void sub_02005748(s32 arg0, u32 arg1);
void sub_02005760(s32 arg0, s32 arg1);

BOOL sub_02005738(int a0) {
    return NNS_SndWaveOutIsPlaying(*sub_020055AC(a0));
}

void sub_02005748(s32 arg0, u32 arg1) {
    u32 pan;

    pan = arg1;
    if (pan > 0x7FU) {
        pan = 0x7F;
    }
    NNS_SndWaveOutSetPan(*sub_020055AC(arg0), (s32)pan);
}

void sub_02005760(s32 arg0, s32 speed) {
    NNS_SndWaveOutSetSpeed(*sub_020055AC(arg0), speed);
}
