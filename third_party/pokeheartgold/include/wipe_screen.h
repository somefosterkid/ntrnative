#ifndef POKEHEARTGOLD_WIPE_SCREEN_H
#define POKEHEARTGOLD_WIPE_SCREEN_H

// WipeScreen: merged by tools/hg/promote.py from 8 m2c local copies (1 variants, base
// src/unk_0201010C_02010148_02010230_02010230.c) and 9 accesses; fields at every accessed offset, pads elsewhere.

struct WipeHBlank;
struct WipeWnd;

typedef struct WipeScreen {
    u32 type;
    s32 division;
    s32 sync;
    s32 seq;
    s32 screen;
    void *work;
    struct WipeWnd *wnd;
    struct WipeHBlank *hblank;
    u32 heapId;
    u16 color;
    u8 pad_26[0x2];
    BOOL endCovered;
    BOOL useBrightness;
} WipeScreen;

// sizeof(WipeScreen) == 0x30

#endif // POKEHEARTGOLD_WIPE_SCREEN_H
