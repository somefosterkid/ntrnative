#include "global.h"

#include "error_handling.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "unk_02005D10.h"

u16 GF_GetBankBySeqNo(int);

s32 sub_020059E0(u8);

void sub_02005150(s32 arg0);

void sub_02005150(s32 seqNo) {
    u16 *v0;

    v0 = GF_SdatGetAttrPtr(0x20U);
    if (GF_GetBankBySeqNo(*v0) == 0x2BC) {
        GF_Snd_LoadSeqEx(seqNo, 4U);
        GF_AssertFail();
    } else {
        GF_Snd_LoadSeqEx((s32)*v0, 6U);
    }
    GF_Snd_SaveState(GF_SdatGetAttrPtr(0x1BU));
    sub_020053A8(1U, 0U);
    GF_SndStartFadeInBGM(0x7FU, 0x28U, 0U);
    sub_020059E0(0);
}

s32 Sound_SetSubScene(u8);

s32 sub_02005328(s32);

void sub_020051A4(s32 arg0, s32 arg1);

void sub_020051A4(s32 seqNo, s32 arg1) {
    u16 *v0;
    u8 *v1;

    v1 = GF_SdatGetAttrPtr(0x13U);
    v0 = GF_SdatGetAttrPtr(0x20U);
    if ((*v1 == 1) || (arg1 == 0)) {
        GF_Snd_LoadState(sub_02005328(1));
        Sound_SetSubScene(0);
        GF_Snd_LoadSeqEx((s32)*v0, 2U);
        GF_Snd_SaveState(GF_SdatGetAttrPtr(0x19U));
        sub_02004B24(4);
        GF_Snd_SaveState(GF_SdatGetAttrPtr(0x1AU));
        if (GF_GetBankBySeqNo(*v0) == 0x2BC) {
            GF_Snd_LoadSeqEx(seqNo, 4U);
            GF_AssertFail();
        } else {
            GF_Snd_LoadSeqEx((s32)*v0, 4U);
        }
        GF_Snd_SaveState(GF_SdatGetAttrPtr(0x1BU));
    }
}

s32 sub_02005B20(void);

void sub_02005228(u16 arg0);

void sub_02005228(u16 seqno) {
    GF_SdatGetAttrPtr(0x18U);
    sub_02005B20();
    GF_Snd_LoadState(sub_02005328(2));
    sub_02004B24(5);
    GF_Snd_SaveState(GF_SdatGetAttrPtr(0x1AU));
    sub_020059E0(1);
    PlayBGM(seqno);
}

s32 Sound_ClearBGMPauseFlags(void);

s32 sub_02005060(s32);

void sub_02005260(u16 arg0);

void sub_02005260(u16 seqno) {
    GF_SdatGetAttrPtr(0x18U);
    Sound_Stop();
    Sound_ClearBGMPauseFlags();
    sub_02005060(4);
    PlayBGM(seqno);
}

void sub_02005280(u16 arg0);

void sub_02005280(u16 seqno) {
    GF_SdatGetAttrPtr(0x18U);
    Sound_Stop();
    sub_02005060(6);
    sub_020059E0(1);
    PlayBGM(seqno);
}

void sub_020052A4(u16 arg0);

void sub_020052A4(u16 seqno) {
    GF_SdatGetAttrPtr(0x18U);
    Sound_Stop();
    sub_02005060(7);
    sub_020059E0(1);
    PlayBGM(seqno);
}

s32 Sound_LoadHeapStateBGM(void);

void sub_020052C8(s32 arg0);

void sub_020052C8(s32 arg0) {
    Sound_LoadHeapStateBGM();
    sub_02004B24(arg0);
    GF_Snd_SaveState(GF_SdatGetAttrPtr(0x1CU));
}

void sub_020052E4(s32 arg0, u16 arg1);

void sub_020052E4(s32 arg0, u16 seqno) {
    GF_SdatGetAttrPtr(0x18U);
    Sound_Stop();
    sub_02005060(arg0);
    PlayBGM(seqno);
}
