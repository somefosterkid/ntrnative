#include "global.h"

#include "sound.h"
#include "sound_02004A44.h"

void sub_02005BEC(BOOL param_1) {
    u8 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(0x38);
    *puVar1 = (s8)param_1;
    return;
}

u8 sub_02005BFC(void);

u8 sub_02005BFC(void) {
    u8 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(0x38);
    return *puVar1;
}

void sub_02005C08(u8 param_1);

void sub_02005C08(u8 param_1) {
    u8 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(0x39);
    *puVar1 = param_1;
    return;
}

BOOL SoundSys_GetGBSoundsState(void) {
    u8 *pbVar1;

    pbVar1 = GF_SdatGetAttrPtr(0x39);
    return (u32)*pbVar1;
}
