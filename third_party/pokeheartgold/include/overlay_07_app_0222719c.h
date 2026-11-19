#ifndef POKEHEARTGOLD_OVERLAY_07_APP_0222719C_H
#define POKEHEARTGOLD_OVERLAY_07_APP_0222719C_H

// Ov07App_0222719C: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov07App_0222719CRec_0 {
    u8 unk_0[0x18];
    s8 unk_18;
    s8 unk_19;
    u8 unk_1A[0x2];
    struct ManagedSprite *unk_1C;
    u8 unk_20[0xC];
} Ov07App_0222719CRec_0; // 0x2C

typedef struct Ov07App_0222719C {
    Ov07App_0222719CRec_0 unk_0[3];
    u8 unk_84[0xB0];
} Ov07App_0222719C;

// sizeof(Ov07App_0222719C) == 0x134

#endif // POKEHEARTGOLD_OVERLAY_07_APP_0222719C_H
