#ifndef POKEHEARTGOLD_OVERLAY_83_APP_02246114_H
#define POKEHEARTGOLD_OVERLAY_83_APP_02246114_H

// Ov83App_02246114: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

#include "bg_window.h"
#include "unk_02009D48.h"

typedef struct Ov83App_02246114 {
    u8 unk_0[0x20];

    struct MsgData *unk_20;

    struct MessageFormat *unk_24;

    struct String *unk_28;

    u8 unk_2C[0x24];
    Window unk_50[36];
    u8 unk_290[0x2C];
    s32 unk_2BC;
    u8 unk_2C0[0xC];
    G2dRenderer unk_2CC;
    u8 unk_3F4[0x158];
    u8 *unk_54C;
    u8 unk_550[0x4];
    u8 *unk_554;
    u8 *unk_558;
    u8 unk_55C[0x60];
    void *unk_5BC;
    u8 unk_5C0[0xA];
    u16 unk_5CA;
    u8 unk_5CC[0x4];
    u16 unk_5D0;
    u16 unk_5D2;
    u16 unk_5D4;
    u16 unk_5D6;
    u16 unk_5D8;
    u16 unk_5DA;
    u16 unk_5DC;
    u8 unk_5DE[0x36];
} Ov83App_02246114;

// sizeof(Ov83App_02246114) == 0x614

#endif // POKEHEARTGOLD_OVERLAY_83_APP_02246114_H
