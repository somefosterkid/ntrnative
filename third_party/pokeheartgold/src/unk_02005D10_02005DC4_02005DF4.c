#include "global.h"

#include "sound.h"
#include "sound_radio.h"

u16 GF_GetBankBySeqNo();

void GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *handle);

void sub_020051A4(u32, u32);

void sub_02006C14(u32, u32, u32, u32, u32, u32);

void sub_02005DF4(u32 param_1, u32 param_2, u32 param_3);

void sub_02005DF4(u32 param_1, u32 param_2, u32 param_3) {
    u16 *puVar1;
    u32 uVar2;

    GF_SdatGetAttrPtr(0x13);
    puVar1 = GF_SdatGetAttrPtr(0x20);
    GF_NNS_SndPlayerGetSeqNo(GF_GetSoundHandle(0));
    uVar2 = GF_GetBankBySeqNo();
    sub_020051A4(param_1, uVar2);
    SndRadio_StopSeq(0);
    uVar2 = GF_GetBankBySeqNo(*puVar1);
    sub_02006C14(param_3, 0xffffffff, uVar2, 0xffffffff, param_1, 1);
    return;
}
