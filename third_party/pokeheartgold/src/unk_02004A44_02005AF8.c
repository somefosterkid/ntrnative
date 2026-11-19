#include "global.h"

#include "sound.h"
#include "sound_02004A44.h"
#include "unk_02005D10.h"
#include "error_handling.h"

s32 GF_SndSetAllocatableChannelForBGMPlayer(u16);

s32 sub_020058F4(void);

s32 sub_02005910(s32);

void sub_02005AF8(s32 arg0);

void sub_02005B58(u8 param_1);
void sub_02005B68(u8 param_1);

void sub_02005B78(u16 seqID, u32 param_2, s16 *param_3);

void sub_02005AF8(s32 arg0) {
    if (arg0 == 0) {
        GF_SndSetAllocatableChannelForBGMPlayer(0xA7FE);
        sub_02005910(0);
    } else {
        GF_SndSetAllocatableChannelForBGMPlayer(0x3FFF);
    }
    sub_020058F4();
}

s32 GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *handle);

s32 sub_02005FD8(void);

void sub_02005B20(void);

void sub_02005B20(void) {
    if (GF_SndGetFadeTimer() == 0) {
        if (GF_NNS_SndPlayerGetSeqNo(GF_GetSoundHandle(0)) != -1) {
            sub_02005FD8();
            sub_020053A8(1U, 1U);
            return;
        }
    }
    Sound_Stop();
}

void GF_SndHandleSetPlayerVolume(u32 a0, u32 a1) {
    NNS_SndPlayerSetPlayerVolume((s32)a0, (s32)a1);
}

void sub_02005B58(u8 param_1) {
    u8 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(0x35);
    *puVar1 = param_1;
    return;
}

void sub_02005B68(u8 param_1) {
    u8 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(0x36);
    *puVar1 = param_1;
    return;
}

void sub_02005B78(u16 seqID, u32 param_2, s16 *param_3) {
    s32 playerNo;
    enum SoundHandleNo playerNo_00;
    NNSSndHandle *handle;

    if (param_3 == NULL) {
        GF_AssertFail();
    }
    if (0xf < param_2) {
        GF_AssertFail();
    }
    playerNo = GF_GetPlayerNoBySeq(seqID);
    playerNo_00 = GF_GetSndHandleByPlayerNo(playerNo);
    handle = GF_GetSoundHandle(playerNo_00);
    NNS_SndPlayerReadVariable(handle, param_2, param_3);
    return;
}
