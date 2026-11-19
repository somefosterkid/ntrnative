#ifndef POKEHEARTGOLD_OVERLAY_90_APP_02259EA0_H
#define POKEHEARTGOLD_OVERLAY_90_APP_02259EA0_H

// Ov90App_02259EA0: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"

typedef struct Ov90App_02259EA0 {
    Window unk_0;
    struct Sprite *unk_10;
    struct Sprite *unk_14;
    u8 unk_18[0x3D0];
} Ov90App_02259EA0;

// sizeof(Ov90App_02259EA0) == 0x3E8

#endif // POKEHEARTGOLD_OVERLAY_90_APP_02259EA0_H
