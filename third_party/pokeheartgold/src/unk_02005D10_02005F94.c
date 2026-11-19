#include "global.h"

#include "sound.h"

BOOL sub_02005F94(u16 seqID);

BOOL sub_02005F94(u16 seqID) {
    u8 playerNo = GF_GetPlayerNoBySeq(seqID);
    return GF_SndPlayerCountPlayingSeqByPlayerNo(playerNo);
}
