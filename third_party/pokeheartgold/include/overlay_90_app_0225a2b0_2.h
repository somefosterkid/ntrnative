#ifndef POKEHEARTGOLD_OVERLAY_90_APP_0225A2B0_2_H
#define POKEHEARTGOLD_OVERLAY_90_APP_0225A2B0_2_H

// Ov90App_0225A2B0_2: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"

typedef struct Ov90App_0225A2B0_2 {
    Window unk_0;
    u8 unk_10[0x4];
    struct String *unk_14;
    u8 unk_18[0x20];
    Window unk_38;
    u8 unk_48[0x3D8];
} Ov90App_0225A2B0_2;

// sizeof(Ov90App_0225A2B0_2) == 0x420

#endif // POKEHEARTGOLD_OVERLAY_90_APP_0225A2B0_2_H
