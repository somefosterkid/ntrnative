#ifndef POKEHEARTGOLD_OVERLAY_96_APP_022078B0_H
#define POKEHEARTGOLD_OVERLAY_96_APP_022078B0_H

// Ov96App_022078B0: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov96App_022078B0 {
    u8 unk_0[0x400];
    u8 unk_400[0x29C];
} Ov96App_022078B0;

// sizeof(Ov96App_022078B0) == 0x69C

#endif // POKEHEARTGOLD_OVERLAY_96_APP_022078B0_H
