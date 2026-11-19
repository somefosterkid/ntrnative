#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "sound.h"
#include "sound_radio.h"
#include "sys_task.h"
#include "sys_task_api.h"

BOOL sub_02006C14(int handleNo, int playerNo, int bankNo, int playerPrio, u16 seqNo, u8 checkGB);

u8 GF_GetPlayerNoBySeq(u16 seqID);

void sub_02004A60(u32 param_1);

BOOL sub_02005E44(u16 seqNo);

BOOL sub_02005E44(u16 seqNo) {
    u8 *p15;
    BOOL result;
    p15 = GF_SdatGetAttrPtr(0x15);
    if (*p15 != 4) {
        GF_AssertFail();
        return 0;
    }
    if (GF_GetPlayerNoBySeq(seqNo) != 7) {
        GF_AssertFail();
        return 0;
    }
    GF_Snd_LoadSeqEx(seqNo, 1);
    result = sub_02006C14(7, -1, -1, -1, seqNo, 1);
    if (!result) {
        sub_02004920(seqNo);
    }
    SndRadio_StopSeq(0);
    sub_02004A60(seqNo);
    GF_SndSetState(1);
    return result;
}
