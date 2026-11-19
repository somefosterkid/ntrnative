#include "global.h"

#include "sound_02004A44.h"

s32 ov45_0222BC84(void *);

void ov45_0222A4D0(void *arg0);

void ov45_0222A4D0(void *arg0) {
    if ((((u32)(*(u8 *)((u8 *)arg0 + 0x1FC) << 0x1B) >> 0x1F) == 1) && ((s32) * (s16 *)((u8 *)arg0 + 0x206) <= 0)) {
        Sound_SetSceneAndPlayBGM(0x15U, 0x481U, 0);
        ov45_0222BC84(arg0 + 0x1FC);
        return;
    }
    Sound_SetSceneAndPlayBGM(0x15U, 0x47FU, 0);
    ov45_0222BC84(arg0 + 0x1FC);
}
