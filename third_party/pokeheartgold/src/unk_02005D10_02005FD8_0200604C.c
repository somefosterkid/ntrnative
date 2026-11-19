#include "global.h"

#include "sound.h"
#include "unk_02005D10.h"

static s32 sub_020060BC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

BOOL PlaySE(u16 sndseq) {
    return sub_020060BC(GF_GetSndHandleByPlayerNo(GF_GetPlayerNoBySeq(sndseq)), -1, -1, -1, (s32)sndseq);
}

void sub_0200606C(u32 param_1, s32 param_2);

void sub_0200606C(u32 param_1, s32 param_2) {
    enum SoundHandleNo SVar1;

    SVar1 = GF_GetSndHandleByPlayerNo(param_2);
    sub_020060BC(SVar1, param_2, 0xffffffff, 0xffffffff, param_1);
    return;
}

u16 GF_GetBankBySeqNo(int);

void sub_02006088(u16 sndseq) {
    u16 *puVar1;
    s32 playerNo;
    enum SoundHandleNo SVar2;
    u32 uVar3;

    puVar1 = GF_SdatGetAttrPtr(0x20);
    playerNo = GF_GetPlayerNoBySeq(sndseq);
    SVar2 = GF_GetSndHandleByPlayerNo(playerNo);
    uVar3 = GF_GetBankBySeqNo(*puVar1);
    sub_020060BC(SVar2, 0xffffffff, uVar3, 0xffffffff, (u32)sndseq);
    return;
}

s32 sub_02005464(s32, s32);

u8 sub_02005BFC(void);

s32 sub_02006C14(s32, s32, s32, s32, s32, s32);

static s32 sub_020060BC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 unk) {
    s32 v0;
    s32 v1;

    v0 = arg3;
    if ((sub_02005BFC() == 1) && ((u32)(unk - 0x5DC) <= 1U)) {
        return 0;
    }
    v1 = sub_02006C14(arg0, arg1, arg2, v0, (s32)(u16)unk, 0);
    sub_02005464(unk, arg0);
    if (v1 == 0) {
        sub_02004920((u16)unk);
    }
    return v1;
}
