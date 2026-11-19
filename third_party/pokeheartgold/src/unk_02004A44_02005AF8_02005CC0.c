#include "global.h"

#include "sound.h"
#include "sound_02004A44.h"

void GBSounds_SetAllocatableChannels(void);

void GBSounds_SetAllocatableChannels(void) {
    NNS_SndPlayerSetTrackAllocatableChannel(GF_GetSoundHandle(0), 0xFU, 0xA7FEU);
    NNS_SndPlayerSetTrackAllocatableChannel(GF_GetSoundHandle(7), 0xFU, 0xA7FEU);
    NNS_SndPlayerSetTrackAllocatableChannel(GF_GetSoundHandle(2), 0xFU, 0xA7FEU);
}

extern s32 _021D05E8;

void sub_02005CF4(s32 arg0);

void sub_02005CF4(s32 arg0) {
    *(s32 *)((u8 *)(&_021D05E8) + 0xC) = arg0;
}

void sub_02005D00(void) {
    *(s32 *)((u8 *)(&_021D05E8) + 0xC) = 0;
    *(s32 *)((u8 *)(&_021D05E8) + 4) = 0;
}
