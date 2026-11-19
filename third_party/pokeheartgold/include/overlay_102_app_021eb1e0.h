#ifndef POKEHEARTGOLD_OVERLAY_102_APP_021EB1E0_H
#define POKEHEARTGOLD_OVERLAY_102_APP_021EB1E0_H

// Ov102App_021EB1E0: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"

typedef struct Ov102App_021EB1E0 {
    void *unk_0; // the view (Ov102App_021E937C)
    u8 unk_4[0x8];
    struct Sprite *unk_C;
    u8 unk_10[0x4];
    struct MsgData *unk_14;
    struct String *unk_18[3];
    Window unk_24[3];
    void *unk_54;
    void *unk_58;
    void *unk_5C;
    u8 unk_60[0x28];
    Window unk_88;
    u8 unk_98[0xC];
} Ov102App_021EB1E0;

// The DS layout (ILP32): the checks run only under mwcc.
#ifdef __MWERKS__
typedef char Ov102App_021EB1E0_size_check[(sizeof(Ov102App_021EB1E0) == 0xA4) ? 1 : -1];
typedef char Ov102App_021EB1E0_24_check[(offsetof(Ov102App_021EB1E0, unk_24) == 0x24) ? 1 : -1];
typedef char Ov102App_021EB1E0_54_check[(offsetof(Ov102App_021EB1E0, unk_54) == 0x54) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_102_APP_021EB1E0_H
