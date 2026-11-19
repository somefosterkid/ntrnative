#ifndef POKEHEARTGOLD_OVERLAY_96_APP_021EDE64_H
#define POKEHEARTGOLD_OVERLAY_96_APP_021EDE64_H

// Ov96App_021EDE64: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov96App_021EDE64 {
    u8 unk_0[0x180];

    s32 unk_180;

    u8 unk_184[0x4C];

    u16 unk_1D0;

    s16 unk_1D2;
} Ov96App_021EDE64;

// sizeof(Ov96App_021EDE64) == 0x1D4

#endif // POKEHEARTGOLD_OVERLAY_96_APP_021EDE64_H
