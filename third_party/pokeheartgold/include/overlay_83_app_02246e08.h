#ifndef POKEHEARTGOLD_OVERLAY_83_APP_02246E08_H
#define POKEHEARTGOLD_OVERLAY_83_APP_02246E08_H

// Ov83App_02246E08: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "unk_02009D48.h"

typedef struct Ov83App_02246E08Rec_13C {
    struct SpriteResource *unk_0;
    struct SpriteResource *unk_4;
    u8 unk_8[0x8];
} Ov83App_02246E08Rec_13C; // 0x10

typedef struct Ov83App_02246E08 {
    u8 unk_0[0x4];
    G2dRenderer unk_4;
    u8 unk_12C[0x10];
    Ov83App_02246E08Rec_13C unk_13C[14];
    u8 unk_21C[0x138];
} Ov83App_02246E08;

// sizeof(Ov83App_02246E08) == 0x354

#endif // POKEHEARTGOLD_OVERLAY_83_APP_02246E08_H
