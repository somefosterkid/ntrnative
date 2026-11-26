#ifndef POKEHEARTGOLD_OVERLAY_97_RECEPTION_H
#define POKEHEARTGOLD_OVERLAY_97_RECEPTION_H

#include "bg_window.h"
#include "overlay_97_course_sub_screen.h"
#include "save.h"

// Ov97Reception: overlay 97 reception screen state (OverlayManager data), DS size 0x34.
// Passed as the callback argument of sub_02093440, so ov97_0221EC14 reads it too.
typedef struct Ov97Reception {
    BgConfig *unk_0;
    s32 unk_4;
    void *unk_8;
    Ov97CourseSubScreen *unk_C;
    u8 unk_10;
    u8 unk_11[0x1B];
    SaveData *unk_2C;
    s32 unk_30;
} Ov97Reception;

#endif
