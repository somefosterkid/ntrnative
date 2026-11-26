#ifndef POKEHEARTGOLD_WORK_H
#define POKEHEARTGOLD_WORK_H

// The Wi-Fi Plaza/club launch task env of sub_02078834 (allocated by sub_02078B2C) and the overlay 44
// launch record it owns at +0.

#include "overlay_44_0222a758_args.h"

typedef struct Work {
    UnkStruct_ov44_0222A758_args *unk0;
    int state;
    u16 *unk8;
    u8 unkC;
    u8 unkD;
    u8 pad_E[0x2];
    void *unk10; // UnkStruct_ov44_args (sub_02078B9C), UnkAppArgs3C / UnkAppArgs40 (sub_02078C18/C74/CC8), sub_020968B0
    int unk14;
} Work;

// sizeof(Work) == 0x18

#endif // POKEHEARTGOLD_WORK_H
