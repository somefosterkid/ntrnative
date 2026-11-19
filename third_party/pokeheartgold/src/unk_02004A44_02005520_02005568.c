#include "global.h"

#include "error_handling.h"
#include "sound.h"
#include "sound_02004A44.h"

extern u8 _021D05E8;

void GF_MicPauseOnLidClose(void) {
    if ((_021D05E8 != 0) && (MIC_StopAutoSampling() != MIC_RESULT_SUCCESS)) {
        GF_AssertFail();
    }
}

extern MICAutoParam _021D05F8;

void GF_MicResumeOnLidOpen(void) {
    if ((_021D05E8 != 0) && (MIC_StartAutoSampling(&_021D05F8) != MIC_RESULT_SUCCESS)) {
        GF_AssertFail();
    }
    GF_SndWorkMicCounterReset();
}
