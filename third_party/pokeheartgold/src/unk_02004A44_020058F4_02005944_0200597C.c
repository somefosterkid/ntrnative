#include "global.h"

extern s32 _021D05E8;

void GF_SndSetMonoFlag(s32 flag);

void GF_SndSetMonoFlag(s32 flag) {
    NNS_SndSetMonoFlag(flag);
    *(s32 *)((u8 *)(&_021D05E8) + 8) = flag;
}
