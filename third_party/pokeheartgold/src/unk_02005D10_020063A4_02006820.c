#include "global.h"

#include "sound_02004A44.h"

s32 sub_02005464(s32, s32);

void sub_02006820(s32 arg0, s32 arg1, s32 arg2);

void sub_02006820(s32 arg0, s32 a0, s32 a1) {
    GF_SndHandleSetInitialVolume(a0, a1);
    sub_02005464(arg0, a0);
}
