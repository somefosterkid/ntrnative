#ifndef POKEHEARTGOLD_OVERLAY_90_APP_0225A2B0_H
#define POKEHEARTGOLD_OVERLAY_90_APP_0225A2B0_H

// Ov90App_0225A2B0: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"

typedef struct Ov90App_0225A2B0 {
    Window unk_0;
    s32 unk_10;
    struct String *unk_14;
    u8 unk_18[0x3F8];
    u8 unk_410[0x10];
} Ov90App_0225A2B0;

// sizeof(Ov90App_0225A2B0) == 0x420

#endif // POKEHEARTGOLD_OVERLAY_90_APP_0225A2B0_H
