#ifndef POKEHEARTGOLD_OVERLAY_96_APP_021F8DF4_H
#define POKEHEARTGOLD_OVERLAY_96_APP_021F8DF4_H

// Ov96App_021F8DF4: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov96App_021F8DF4 {
    u8 unk_0[0x14];

    u32 unk_14;

    u8 unk_18[0x18];
} Ov96App_021F8DF4;

// sizeof(Ov96App_021F8DF4) == 0x30

#endif // POKEHEARTGOLD_OVERLAY_96_APP_021F8DF4_H
