#ifndef POKEHEARTGOLD_OVERLAY_102_APP_021EBC34_H
#define POKEHEARTGOLD_OVERLAY_102_APP_021EBC34_H

// Ov102App_021EBC34: the Easy Chat view part at view+0x1EC (Heap_Alloc 0x98 in ov102_021EBC34);
// pads where no code reads a field yet.

#include "bg_window.h"

typedef struct Ov102App_021EBC34 {
    void *unk_0; // the view (Ov102App_021E937C)
    void *unk_4;
    void *unk_8;
    BgConfig *unk_C;
    Window unk_10;
    Window unk_20;
    u8 unk_30[0xC];
    struct String *unk_3C;
    u8 unk_40[0x58];
} Ov102App_021EBC34;

// The DS layout (ILP32): the checks run only under mwcc.
#ifdef __MWERKS__
typedef char Ov102App_021EBC34_size_check[(sizeof(Ov102App_021EBC34) == 0x98) ? 1 : -1];
typedef char Ov102App_021EBC34_10_check[(offsetof(Ov102App_021EBC34, unk_10) == 0x10) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_102_APP_021EBC34_H
