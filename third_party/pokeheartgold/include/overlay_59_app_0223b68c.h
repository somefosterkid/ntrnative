#ifndef POKEHEARTGOLD_OVERLAY_59_APP_0223B68C_H
#define POKEHEARTGOLD_OVERLAY_59_APP_0223B68C_H

// Ov59App_0223B68C: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"

typedef struct Ov59App_0223B68C {
    u8 unk_0[0x70];
    struct String *unk_70[2];
    u8 unk_78[0x13C];
    Window unk_1B4;
    u8 unk_1C4[0x40];
    Window unk_204;
    u8 unk_214[0x4C];
    struct Sprite *unk_260[3];
    u8 unk_26C[0x80];
} Ov59App_0223B68C;

// sizeof(Ov59App_0223B68C) == 0x2EC

#endif // POKEHEARTGOLD_OVERLAY_59_APP_0223B68C_H
