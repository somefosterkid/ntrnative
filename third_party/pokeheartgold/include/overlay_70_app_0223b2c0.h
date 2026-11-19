#ifndef POKEHEARTGOLD_OVERLAY_70_APP_0223B2C0_H
#define POKEHEARTGOLD_OVERLAY_70_APP_0223B2C0_H

// Ov70App_0223B2C0: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov70App_0223B2C0 {
    u8 unk_0[0x400];
    u8 unk_400[0x400];
    u8 unk_800[0x3A0];
    struct MsgData *unk_BA0;
    u8 unk_BA4[0x5C];
    u8 unk_C00[0x310];
} Ov70App_0223B2C0;

// sizeof(Ov70App_0223B2C0) == 0xF10

#endif // POKEHEARTGOLD_OVERLAY_70_APP_0223B2C0_H
