#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F7934_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F7934_H

#include "constants/heap.h"

#include "bg_window.h"
#include "sprite_system.h"

// The 0xAC-byte work object of the ov96 app run by ov96_021F7934 (PokeathlonCourse_AllocPtr4FromHeap),
// freed by ov96_021F7C70 (hk233). Its +0x1C part is the 0x64-byte sprite slide-in sub-object.

// One slide-in slot (ov96_021F8128, 82E4, 8334, 8354): 4 in a ring plus one extra.
struct Ov96SpriteMgr;

typedef struct UnkStruct_ov96_021F8128 {
    s32 active;   // 0x0
    void *sprite; // 0x4 handle returned by ov96_021F8830 (entry of the 0x334 scene)
    s32 delay;    // 0x8 frames before the slot starts to move
    s32 played;   // 0xC the arrival sound already played
} UnkStruct_ov96_021F8128;

// sizeof(UnkStruct_ov96_021F8128) == 0x10

typedef struct UnkStruct_ov96_021F8094 {
    void *scene;                      // 0x00 0x334-byte sprite scene (ov96_021F86E8)
    UnkStruct_ov96_021F8128 slots[4]; // 0x04 ring of pending slides
    UnkStruct_ov96_021F8128 extra;    // 0x44
    s32 holdFrames;                   // 0x54 countdown of state 3
    u8 state;                         // 0x58
    u8 total;                         // 0x59 number of sprites to show
    u8 shown;                         // 0x5A number queued so far
    u8 head;                          // 0x5B next ring slot
    u8 taken;                         // 0x5C participants taken so far (ov96_021F8378)
    u8 unk_5D[3];                     // 0x5D
    s32 fanfarePlayed;                // 0x60
} UnkStruct_ov96_021F8094;

// sizeof(UnkStruct_ov96_021F8094) == 0x64

typedef struct UnkStruct_ov96_021F7934 {
    enum HeapID heapID;                // 0x00
    u8 unk_04[8];                      // 0x04
    BgConfig *bgConfig;                // 0x0C
    struct Ov96SpriteMgr *unk_10;      // 0x10 ov96_021EB180
    void *unk_14;                      // 0x14 ov96_021F8EB0
    struct Ov96TextBox *unk_18;        // 0x18 ov96_021EE740
    UnkStruct_ov96_021F8094 unk_1C;    // 0x1C
    void *unk_80;                      // 0x80 ov96_021EE5B4
    Window window;                     // 0x84
    SpriteSystem *spriteSystem;        // 0x94
    SpriteManager *spriteManager;      // 0x98
    ManagedSprite *sprites[2];         // 0x9C
    u32 unk_A4;                        // 0xA4 counter bumped by ov96_021F7E74
    u32 unk_A8;                        // 0xA8
} UnkStruct_ov96_021F7934;

// sizeof(UnkStruct_ov96_021F7934) == 0xAC

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_021_F7934_H
