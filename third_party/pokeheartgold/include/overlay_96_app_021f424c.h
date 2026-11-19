#ifndef POKEHEARTGOLD_OVERLAY_96_APP_021F424C_H
#define POKEHEARTGOLD_OVERLAY_96_APP_021F424C_H

// Ov96App_021F424C: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"

typedef struct Ov96App_021F424C {
    u8 unk_0[0x4];

    struct PokeathlonCourseData *unk_4;

    u8 unk_8[0x4];
    Window unk_C;
    Window unk_1C;
    u8 unk_2C[0x20];
    Window unk_4C;
    u8 unk_5C[0x8];
    s32 unk_64;
    u8 unk_68[0xC8];
    s32 unk_130;
    u8 unk_134[0x40];
} Ov96App_021F424C;

// sizeof(Ov96App_021F424C) == 0x174

#endif // POKEHEARTGOLD_OVERLAY_96_APP_021F424C_H
