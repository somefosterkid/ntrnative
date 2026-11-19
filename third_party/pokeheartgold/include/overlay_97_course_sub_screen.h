#ifndef POKEHEARTGOLD_OVERLAY_97_COURSE_SUB_SCREEN_H
#define POKEHEARTGOLD_OVERLAY_97_COURSE_SUB_SCREEN_H

// Ov97CourseSubScreen: overlay 97's bottom-screen course card (course name, the selected Pokemon's name and its five
// stat rows), Heap_Alloc'd by ov97_0221EE84 (sizeof, HEAP_ID_POKEATHLON), set up by ov97_0221EEA4, drawn by ov97_0221EFD0,
// freed by ov97_0221F020. Closed object (only the ov97_0221E898 code and its container touch it); fields sit at the DS
// offsets in the comments, the DS size is 0x2FC (checked below on the DS build), the PC size is bigger.
// Sibling of Ov112SubScreen (include/overlay_112_sub_screen.h), same sprite plumbing.

#include "bg_window.h"
#include "g2d_renderer.h"

struct GF_2DGfxResMan;
struct MessageFormat;
struct MsgData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct String;

// Ov97StatRowSprites: one stat row of the course card: an up/down arrow and five rating marks.
typedef struct Ov97StatRowSprites {
    struct Sprite *arrow;                      // 0x00: anim seq 4 = up, 5 = down
    struct Sprite *marks[5];                   // 0x04: anim seq 0..3
} Ov97StatRowSprites;

// Ov97CourseSubScreenSprites: renderer, resource managers/objects and sprites, embedded at Ov97CourseSubScreen+0x78.
typedef struct Ov97CourseSubScreenSprites {
    enum HeapID heapID;                        // 0x000
    struct SpriteList *spriteList;             // 0x004
    G2dRenderer renderer;                      // 0x008
    struct GF_2DGfxResMan *resMan[6];          // 0x130: one per GfGfxResType
    struct SpriteResource *resObj[3][6];       // 0x148: [set][GfGfxResType], char/pltt/cell/anim used
    u8 unk_190[0x24];                          // 0x190
    struct Sprite *unk_1B4[5];                 // 0x1B4
    struct Sprite *unk_1C8[5];                 // 0x1C8
    Ov97StatRowSprites statRows[5];            // 0x1DC: one per stat
    struct Sprite *monSprite;                  // 0x254
    void *charData;                            // 0x258: 0xC80 bytes, the Pokemon picture
    u16 plttData[16];                          // 0x25C
    u32 charLocation;                          // 0x27C
    u32 plttLocation;                          // 0x280
} Ov97CourseSubScreenSprites;

typedef struct Ov97CourseSubScreen {
    enum HeapID heapID;                        // 0x00
    struct BgConfig *bgConfig;                 // 0x04
    Window windows[6];                         // 0x08
    u16 courseId;                              // 0x68
    u16 unk_6A;                                // 0x6A
    struct MsgData *msgData;                   // 0x6C
    struct MessageFormat *msgFormat;           // 0x70
    struct String *string;                     // 0x74
    Ov97CourseSubScreenSprites sprites;        // 0x78
} Ov97CourseSubScreen;

#if !defined(SDK_PORT) && !defined(NONMATCHING)
typedef char Ov97CourseSubScreenSprites_size_is_0x284[(sizeof(Ov97CourseSubScreenSprites) == 0x284) ? 1 : -1];
typedef char Ov97CourseSubScreen_size_is_0x2FC[(sizeof(Ov97CourseSubScreen) == 0x2FC) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_97_COURSE_SUB_SCREEN_H
