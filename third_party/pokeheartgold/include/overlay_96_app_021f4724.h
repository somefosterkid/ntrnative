#ifndef POKEHEARTGOLD_OVERLAY_96_APP_021F4724_H
#define POKEHEARTGOLD_OVERLAY_96_APP_021F4724_H

// Ov96App_021F4724: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov96App_021F4724 {
    u8 unk_0[0x90];
    s32 unk_90[2];
    u8 unk_98[0x54];
} Ov96App_021F4724;

// sizeof(Ov96App_021F4724) == 0xEC

#endif // POKEHEARTGOLD_OVERLAY_96_APP_021F4724_H
