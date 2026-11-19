#ifndef POKEHEARTGOLD_OVERLAY_49_APP_0225AC24_H
#define POKEHEARTGOLD_OVERLAY_49_APP_0225AC24_H

// Ov49App_0225AC24: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"

typedef struct Ov49App_0225AC24 {
    Window unk_0;
    s32 unk_10;
    u32 unk_14;
    struct String *unk_18;
} Ov49App_0225AC24;

// sizeof(Ov49App_0225AC24) == 0x1C

#endif // POKEHEARTGOLD_OVERLAY_49_APP_0225AC24_H
