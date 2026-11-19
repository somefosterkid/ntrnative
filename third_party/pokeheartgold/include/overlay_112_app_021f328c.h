#ifndef POKEHEARTGOLD_OVERLAY_112_APP_021F328C_H
#define POKEHEARTGOLD_OVERLAY_112_APP_021F328C_H

// Ov112App_021F328C: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov112App_021F328C {
    u8 unk_0[0x1F2];

    u16 unk_1F2;
} Ov112App_021F328C;

// sizeof(Ov112App_021F328C) == 0x1F4

#endif // POKEHEARTGOLD_OVERLAY_112_APP_021F328C_H
