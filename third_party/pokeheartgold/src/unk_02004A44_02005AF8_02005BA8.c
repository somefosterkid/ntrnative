#include "global.h"

#include "constants/sndseq.h"

#include "heap.h"
#include "sound.h"
#include "sound_02004A44.h"

struct SndPitchVolume;

struct SndUnk37;

typedef struct SndPitchVolume {
    u8 pitch;
    u8 volume;
    u16 pad;
} SndPitchVolume;

typedef struct SndUnk37 {
    u8 idx;
    u8 cnt;
} SndUnk37;

void GF_SndHandleSetTrackPitch(s32 playerNo, u16 trackBitMask, s32 pitch);

void sub_02005BA8(u16 seqNo);

extern const SndPitchVolume _020F5710[8];

void sub_02005BA8(u16 seqNo) {
    SndUnk37 *p = GF_SdatGetAttrPtr(0x37);
    GF_SetVolumeBySeqNo(seqNo, _020F5710[p->idx].volume);
    GF_SndHandleSetTrackPitch(4, 0xFFFF, _020F5710[p->idx].pitch);
    if (p->cnt >= 8) {
        p->cnt = 0;
    }
}
