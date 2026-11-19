#include "global.h"

struct SndGBSeqPair;

typedef struct SndGBSeqPair {
    u16 ds;
    u16 gb;
} SndGBSeqPair;

u16 GBSounds_GetGBSeqNoByDSSeqNo(u16 seqNo);

extern const SndGBSeqPair _020F5730[0x88];

u16 GBSounds_GetGBSeqNoByDSSeqNo(u16 seqNo) {
    u16 i;
    for (i = 0; i < 0x88; i++) {
        if (seqNo == _020F5730[i].ds) {
            return _020F5730[i].gb;
        }
    }
    return seqNo;
}
