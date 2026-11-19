#ifndef POKEHEARTGOLD_OVERLAY_112_SUB_SCREEN_H
#define POKEHEARTGOLD_OVERLAY_112_SUB_SCREEN_H

// Ov112SubScreen: overlay 112's sub-screen info panel (the Pokemon/item card on the bottom screen), Heap_Alloc'd by
// ov112_021F039C (sizeof, HEAP_ID 0x9A), set up by ov112_021F03BC, drawn by ov112_021F04DC, freed by ov112_021F051C.
// Closed object (only ov112 code touches it); fields sit at the DS offsets in the comments, the DS size is 0x258
// (checked below on the DS build), the PC size is bigger.

#include "bg_window.h"
#include "g2d_renderer.h"

struct GF_2DGfxResMan;
struct MessageFormat;
struct MsgData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct String;

// Ov112SubScreenSprites: the panel's sprite resources, embedded at Ov112SubScreen+0x74 (ov112_021F09B4 / 021F0B28 build
// it, ov112_021F05CC + 021F051C tear it down, ov112_021F0C8C / 021F0D04 load the Pokemon picture into charData/plttData).
typedef struct Ov112SubScreenSprites {
    enum HeapID heapID;                        // 0x000
    struct SpriteList *spriteList;             // 0x004
    G2dRenderer renderer;                      // 0x008
    struct GF_2DGfxResMan *resMan[6];          // 0x130: one per GfGfxResType
    struct SpriteResource *resObj[2][6];       // 0x148: [set][GfGfxResType], char/pltt/cell/anim used
    u8 unk_178[0x24];
    struct Sprite *slotSprites[6];             // 0x19C
    struct Sprite *monSprite;                  // 0x1B4
    void *charData;                            // 0x1B8: 0xC80 bytes, the Pokemon picture
    u16 plttData[16];                          // 0x1BC
    u32 charLocation;                          // 0x1DC
    u32 plttLocation;                          // 0x1E0
} Ov112SubScreenSprites;

typedef struct Ov112SubScreen {
    enum HeapID heapID;                        // 0x00
    struct BgConfig *bgConfig;                 // 0x04
    Window windows[6];                         // 0x08
    struct MsgData *msgData;                   // 0x68
    struct MessageFormat *msgFormat;           // 0x6C
    struct String *string;                     // 0x70
    Ov112SubScreenSprites sprites;             // 0x74
} Ov112SubScreen;

#if !defined(SDK_PORT) && !defined(NONMATCHING)
typedef char Ov112SubScreenSprites_size_is_0x1E4[(sizeof(Ov112SubScreenSprites) == 0x1E4) ? 1 : -1];
typedef char Ov112SubScreen_size_is_0x258[(sizeof(Ov112SubScreen) == 0x258) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_112_SUB_SCREEN_H
