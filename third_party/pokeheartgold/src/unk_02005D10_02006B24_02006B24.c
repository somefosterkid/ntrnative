#include "global.h"

void SndRadio_PausePlayer(BOOL flag);

extern void *GF_SdatGetAttrPtr(u32 attr);

extern void GF_Snd_SaveState(int *level_p);

extern void GF_Snd_LoadSeqEx(int seqNo, u32 loadFlag);

extern u16 GF_GetCurrentPlayingBGM(void);

extern void sub_020053A8(u8 a0, BOOL a1);

extern u8 GF_GetPlayerNoBySeq(u16 seqNo);

extern void GF_GetBankInfoBySeqNo(u16 seqNo);

extern void sub_02005464(int seqNo, int a1);

void sub_02006C04(u16 unused);

BOOL sub_02006C14(int handleNo, int playerNo, int bankNo, int playerPrio, u16 seqNo, u8 checkGB);

BOOL PlayFanfare(u16 seqNo);

BOOL PlayFanfare(u16 seqNo) {
    u8 playerNo;
    BOOL result;
    GF_GetBankInfoBySeqNo(seqNo);
    sub_02006C04(seqNo);
    playerNo = GF_GetPlayerNoBySeq(GF_GetCurrentPlayingBGM());
    if (playerNo != 0xFF) {
        sub_020053A8(playerNo, 1);
    } else {
        SndRadio_PausePlayer(1);
    }
    GF_Snd_SaveState(GF_SdatGetAttrPtr(0x1D));
    GF_Snd_LoadSeqEx(seqNo, 3);
    result = sub_02006C14(2, -1, -1, -1, seqNo, 0);
    sub_02005464(seqNo, 2);
    return result;
}
