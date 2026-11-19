#ifndef POKEHEARTGOLD_OVERLAY_29_NUMBER_ENTRY_DATA_H
#define POKEHEARTGOLD_OVERLAY_29_NUMBER_ENTRY_DATA_H

#include "bg_window.h"
#include "g2d_renderer.h"

struct BgConfig;
struct GF_2DGfxResMan;
struct GridInputHandler;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SysTask;

typedef struct NumberEntryArgs {
    u32 max;
    int result;
} NumberEntryArgs;

typedef struct NumberEntryBlink {
    u8 isBg : 1;
    u8 target : 7;
    u8 seqA;
    u8 seqB;
    u8 phase : 3;
    u8 timer : 5;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} NumberEntryBlink;

typedef struct NumberEntryData {
    struct BgConfig *bgConfig;
    s32 unk4;
    s32 unk8;
    NumberEntryArgs *unkC;
    NumberEntryArgs *args;
    struct SysTask *task;
    struct SpriteList *spriteList;
    G2dRenderer renderer;
    struct GF_2DGfxResMan *resMans[4];
    struct SpriteResource *padResObjs[4];
    struct SpriteResource *digitResObjs[4];
    struct Sprite *sprites[11];
    Window windows[3];
    struct GridInputHandler *gridInput;
    NumberEntryBlink blink;
    u32 digits[6];
    int cursor;
    u16 state;
    u16 nextState;
    u32 numDigits;
    u32 value;
} NumberEntryData;

// sizeof(NumberEntryData) == 0x204

#endif // POKEHEARTGOLD_OVERLAY_29_NUMBER_ENTRY_DATA_H
