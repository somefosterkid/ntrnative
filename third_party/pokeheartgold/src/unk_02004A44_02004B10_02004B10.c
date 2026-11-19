#include "global.h"

#include "sound.h"
#include "sound_02004A44.h"

s32 Sound_LoadHeapStateBGM(void);

void Sound_LoadHeapStateBGMAndResetSubScene(void) {
    s8 *v0;

    v0 = GF_SdatGetAttrPtr(0x16U);
    Sound_LoadHeapStateBGM();
    *v0 = 0;
}
