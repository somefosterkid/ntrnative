#include "global.h"

struct SoundSystemFlags;

typedef struct SoundSystemFlags {
    u8 micSampling;
    BOOL fieldBGMStopped;
    BOOL mono;
    BOOL fieldBGMStopOnce;
} SoundSystemFlags;

extern SoundSystemFlags _021D05E8;

extern MICAutoParam _021D05F8;

BOOL GF_MIC_StartAutoSampling(MICAutoParam *param);

BOOL GF_MIC_StartAutoSampling(MICAutoParam *param) {
    MICResult result = MIC_StartAutoSampling(param);
    _021D05E8.micSampling = 1;
    _021D05F8 = *param;
    return result;
}
