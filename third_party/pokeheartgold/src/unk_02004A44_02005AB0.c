#include "global.h"

#include "sound.h"

u32 GF_GetBankInfoBySeqNo(u32);

void GF_SetCurrentPlayingBGM(u32);

void sub_02004A60(u32);

void sub_020059A0(u32);

void sub_020059E0(u8);

void sub_02005AB0(u32 param_1, u32 param_2, u16 param_3, u32 param_4, u8 param_5);

void GF_SndSetAllocatableChannelForBGMPlayer(u16 channels);

void sub_02005AB0(u32 param_1, u32 param_2, u16 param_3, u32 param_4, u8 param_5) {
    u32 *puVar1;
    u32 uVar2;

    puVar1 = GF_SdatGetAttrPtr(2);
    GF_SndStartFadeOutBGM(0, param_3);
    sub_02004A60(0);
    GF_SetCurrentPlayingBGM(param_2);
    sub_020059A0(param_4);
    uVar2 = GF_GetBankInfoBySeqNo(param_2);
    *puVar1 = uVar2;
    sub_020059E0(param_5);
    return;
}

void GF_SndSetAllocatableChannelForBGMPlayer(u16 channels) {
    NNS_SndPlayerSetAllocatableChannel(7, channels);
}
