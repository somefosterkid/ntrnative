#ifndef POKEHEARTGOLD_OVERLAY_32_NAME_PICKER_DATA_H
#define POKEHEARTGOLD_OVERLAY_32_NAME_PICKER_DATA_H

#include "bg_window.h"
#include "g2d_renderer.h"

// NamePickerData: merged by tools/hg/promote.py from 2 m2c local copies (1 variants, base
// src/overlay_32_0225DC0C_0225DC0C.c) and 111 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct FieldSystem;
struct GF_2DGfxResMan;
struct GridInputHandler;
struct MessageFormat;
struct MsgData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SysTask;
struct WiFiList;

typedef struct NamePickerBlink {
    u8 isBg : 1;
    u8 target : 7;
    u8 seqA : 4;
    u8 seqB : 4;
    u8 phase;
    u8 timer;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} NamePickerBlink;

typedef struct NamePickerData {
    struct BgConfig *bgConfig;
    int unk4;
    struct FieldSystem *fieldSystem;
    int *unkC;
    struct SysTask *task;
    struct WiFiList *names;
    struct GridInputHandler *gridInput;
    NamePickerBlink blink;
    Window windows[18];
    struct MsgData *msgData;
    struct MessageFormat *msgFormat;
    struct SpriteList *spriteList;
    G2dRenderer renderer;
    struct GF_2DGfxResMan *resMans[4];
    struct SpriteResource *resObjs[4];
    struct Sprite *sprites[4];
    u8 state;
    u8 nextState;
    s8 page;
    u8 bufferToggle;
    int selection;
    int *result;
} NamePickerData;

// sizeof(NamePickerData) == 0x2b4

#endif // POKEHEARTGOLD_OVERLAY_32_NAME_PICKER_DATA_H
