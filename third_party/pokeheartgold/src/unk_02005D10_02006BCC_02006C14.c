#include "global.h"

#include "heap.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "sound_radio.h"
#include "sys_task.h"
#include "sys_task_api.h"

extern u16 GBSounds_GetGBSeqNoByDSSeqNo(u16 seqNo);

void sub_02004AB8(u16 param_1);

u8 GF_GetPlayerNoBySeq(u16 seqID);

void GBSounds_SetAllocatableChannels(void);

BOOL sub_02006C14(int handleNo, int playerNo, int bankNo, int playerPrio, u16 seqNo, u8 checkGB);

BOOL sub_02006C14(int handleNo, int playerNo, int bankNo, int playerPrio, u16 seqNo, u8 checkGB) {
    NNSSndHandle *handle;
    BOOL result;
    u16 seq;
    u16 gbSeqNo;
    u8 remappedPlayerNo;
    seq = seqNo;
    if (checkGB == 1 && SoundSys_GetGBSoundsState() == 1) {
        gbSeqNo = GBSounds_GetGBSeqNoByDSSeqNo(seqNo);
        if (seqNo != gbSeqNo) {
            seq = gbSeqNo;
            bankNo = 0x2BD;
            sub_02004AB8(gbSeqNo);
            remappedPlayerNo = GF_GetPlayerNoBySeq(gbSeqNo);
            if (playerNo != -1) {
                remappedPlayerNo = (u8)playerNo;
            }
            if (remappedPlayerNo == 7) {
                GF_Snd_LoadSeqEx(gbSeqNo, 1);
            }
        }
    }
    handle = GF_GetSoundHandle(handleNo);
    result = NNS_SndArcPlayerStartSeqEx(handle, playerNo, bankNo, playerPrio, seq);
    GBSounds_SetAllocatableChannels();
    return result;
}
