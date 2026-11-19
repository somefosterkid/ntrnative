#ifndef POKEHEARTGOLD_OVERLAY_07_BATTLE_BG_ANIM_H
#define POKEHEARTGOLD_OVERLAY_07_BATTLE_BG_ANIM_H

#include "global.h"

#include "constants/heap.h"

struct BgConfig;
struct CustomBgScrollContext;
struct PaletteData;

// One horizontal band of the wave (scanline scroll) background animation.
typedef struct BgWaveStrip {
    s16 startScanline;
    s16 endScanline;
    s16 scrollSpeed;
    s16 scrollAccum;
    int baseOffset;
} BgWaveStrip;

typedef struct WaveAnimContext {
    BgWaveStrip strips[16];
    struct CustomBgScrollContext *bgScrollCtx;
} WaveAnimContext;

// Palette cycling state of the "extra" (kind 3) background animation, heap alloc 0x190 (ov07_0221E9D4, task ov07_0221E914).
typedef struct BattleBgPaletteAnim {
    enum HeapID heapID;
    struct PaletteData *paletteData;
    u8 paletteIdx[0x80];
    u16 frameCount[0x80];
    u8 numPalettes;
    u8 narcMemberBase;
    u8 timer;
    u8 state;
    u8 curPalette;
    u8 unk_18D[3];
} BattleBgPaletteAnim;

// Heap object of one battle background animation (heap alloc 0x28, ov07_0221E6D4 / ov07_0221E788).
// BattleAnimSystem::bgAnim[4] holds the four of them.
typedef struct BattleBgAnim {
    struct BgConfig *bgConfig;
    s16 offsetX;
    s16 offsetY;
    s16 stepX;
    s16 stepY;
    int bg;
    int unusedBg;
    BOOL unused;
    BOOL cancel;
    int kind; // 1: move, 2: wave, 3: extra (5 matches every kind)
    WaveAnimContext *waveAnimCtx;
    BattleBgPaletteAnim *paletteAnim; // kind 3 only
} BattleBgAnim;

void ov07_0221DD14(BattleBgAnim **bgAnim, int kind);
void ov07_0221DD38(BattleBgAnim **bgAnim, BattleBgAnim *anim, int kind);
BattleBgAnim *ov07_0221DD8C(BattleBgAnim **bgAnim, int kind);

#endif // POKEHEARTGOLD_OVERLAY_07_BATTLE_BG_ANIM_H
