#ifndef POKEHEARTGOLD_OVERLAY_43_APP_0222B944_H
#define POKEHEARTGOLD_OVERLAY_43_APP_0222B944_H

// Ov43App_0222B944: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"

typedef struct Ov43App_0222B944 {
    u8 unk_0[0x10];
    Window unk_10;
    u8 unk_20[0x98];
    Window unk_B8;
    Window unk_C8;
    u8 unk_D8[0xC];
    struct ListMenuItem *unk_E4;
    u8 unk_E8[0x24];
    Window unk_10C;
    Window unk_11C;
    Window unk_12C;
    u8 unk_13C[0x108];
} Ov43App_0222B944;

// sizeof(Ov43App_0222B944) == 0x244

#endif // POKEHEARTGOLD_OVERLAY_43_APP_0222B944_H
