#ifndef POKEHEARTGOLD_OVERLAY_96_APP_021E9394_H
#define POKEHEARTGOLD_OVERLAY_96_APP_021E9394_H

// Ov96App_021E9394: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov96App_021E9394 {
    u8 unk_0[0xC];
    s32 unk_C[13];
    u8 unk_40[0x104];
    s32 unk_144[12];
} Ov96App_021E9394;

// sizeof(Ov96App_021E9394) == 0x174

#endif // POKEHEARTGOLD_OVERLAY_96_APP_021E9394_H
