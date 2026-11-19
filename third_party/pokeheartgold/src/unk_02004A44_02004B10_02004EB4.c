#include "global.h"

#include "sound_02004A44.h"

void sub_02004EB4(u16 arg0);

void sub_02004EB4(u16 seqNo) {
    Sound_SetSceneAndPlayBGM(4U, seqNo, 1);
}
