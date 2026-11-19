#include "global.h"

#include "sound.h"

BOOL IsSEPlaying(u16 seqID);

BOOL IsSEPlaying(u16 seqID) {
    u8 playerNo = GF_GetPlayerNoBySeq(seqID);
    return GF_SndPlayerCountPlayingSeqByPlayerNo(playerNo);
}
