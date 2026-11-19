#include "global.h"
#include "sound.h"
#include "sound_02004A44.h"

u16 GF_GetVolumeBySeqNo(s32 arg0);

u16 GF_GetVolumeBySeqNo(s32 seqNo) {
    NNSSndSeqParam *nNSSndSeqParam;

    nNSSndSeqParam = (struct NNSSndSeqParam *)(NNS_SndArcGetSeqParam(seqNo));
    if (nNSSndSeqParam == NULL) {
        return 0U;
    }
    return (u16)nNSSndSeqParam->volume;
}

void GF_SetVolumeBySeqNo(u16 seqNo, int volume) {
    GF_SndHandleSetInitialVolume(GF_GetSndHandleByPlayerNo(GF_GetPlayerNoBySeq(seqNo)), volume);
}
