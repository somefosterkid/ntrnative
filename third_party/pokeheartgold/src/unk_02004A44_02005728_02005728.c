#include "global.h"

#include "sound_02004A44.h"

void sub_02005728(s32 arg0);

void sub_02005728(s32 arg0) {
    NNS_SndWaveOutStop(*sub_020055AC(arg0));
}
