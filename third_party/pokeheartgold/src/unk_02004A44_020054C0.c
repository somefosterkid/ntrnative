#include "global.h"

int GF_SndPlayerCountPlayingSeqByPlayerNo(int playerID);
u8 GF_GetPlayerNoBySeq(u16 seqID);

int GF_SndPlayerCountPlayingSeqByPlayerNo(int playerID) {
    if (playerID < 0) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    return NNS_SndPlayerCountPlayingSeqByPlayerNo(playerID);
}

u8 GF_GetPlayerNoBySeq(u16 seqID) {
    if (seqID == 0) {
        return 0xFF;
    }

    const NNSSndSeqParam *param = NNS_SndArcGetSeqParam(seqID);
    if (param == 0L) {
        return 0xff;
    }

    return param->playerNo;
}
