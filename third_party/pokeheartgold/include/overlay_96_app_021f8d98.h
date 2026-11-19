#ifndef POKEHEARTGOLD_OVERLAY_96_APP_021F8D98_H
#define POKEHEARTGOLD_OVERLAY_96_APP_021F8D98_H

// Ov96App_021F8D98: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov96App_021F8D98 {
    u8 unk_0[0x10];

    struct UnkStruct_02013910 *unk_10;

    u32 unk_14;

    u8 unk_18[0x18];
} Ov96App_021F8D98;

// sizeof(Ov96App_021F8D98) == 0x30

#endif // POKEHEARTGOLD_OVERLAY_96_APP_021F8D98_H
