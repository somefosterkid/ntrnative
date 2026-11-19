#ifndef POKEHEARTGOLD_OVERLAY_80_APP_0222F210_H
#define POKEHEARTGOLD_OVERLAY_80_APP_0222F210_H

// Ov80App_0222F210: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov80App_0222F210 {
    u8 unk_0[0x34];

    struct SpriteSystem *unk_34;

    struct SpriteManager *unk_38;
} Ov80App_0222F210;

// sizeof(Ov80App_0222F210) == 0x3C

#endif // POKEHEARTGOLD_OVERLAY_80_APP_0222F210_H
