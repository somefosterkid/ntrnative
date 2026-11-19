#ifndef POKEHEARTGOLD_OVERLAY_48_APP_0225AC34_H
#define POKEHEARTGOLD_OVERLAY_48_APP_0225AC34_H

// Ov48App_0225AC34: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"

typedef struct Ov48App_0225AC34 {
    u8 unk_0[0x6C];
    struct SpriteResource *unk_6C[4];
    u8 unk_7C[0xD0];
    Window unk_14C;
    u8 unk_15C[0x4];
} Ov48App_0225AC34;

// sizeof(Ov48App_0225AC34) == 0x160

#endif // POKEHEARTGOLD_OVERLAY_48_APP_0225AC34_H
