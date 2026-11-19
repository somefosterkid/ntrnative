#include "global.h"

#include "sound_02004A44.h"
#include "unk_02005D10.h"

s32 sub_02006118(u16 arg0, u16 arg1);

s32 sub_02006118(u16 arg0, u16 arg1) {
    s32 temp_r0;

    temp_r0 = PlaySE(arg0);
    if (temp_r0 != 0) {
        GF_SetVolumeBySeqNo(arg0, arg1);
    }
    return temp_r0;
}
