#include "global.h"

#include "sound.h"
#include "sound_02004A44.h"

extern s8 _021D05E8;

void GF_MIC_StopAutoSampling(void) {
    GetSoundDataPointer();
    _021D05E8 = 0;
    MIC_StopAutoSampling();
}
