#ifndef POKEHEARTGOLD_OVERLAY_70_S70_H
#define POKEHEARTGOLD_OVERLAY_70_S70_H

// S70: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_70_02238F10_0223AFFC_0223AFFC.c) and 3 accesses; fields at every accessed offset, pads elsewhere.


typedef struct S70 {
    u8 pad0[44];
    s32 f2C;
    u8 pad30[248];
    s32 f128;
    s32 f12C;
    u8 pad_130[0xa8c];
    void *unk_BBC;
    u8 pad_BC0[0x30];
    u32 unk_BF0;
} S70;

// sizeof(S70) == 0xbf4

#endif // POKEHEARTGOLD_OVERLAY_70_S70_H
