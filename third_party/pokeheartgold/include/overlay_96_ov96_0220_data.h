#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_0220_DATA_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_0220_DATA_H

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"

struct NARC;
struct Ov96SpriteSet;
struct UnkStruct_ov96_021E9A78;

// Ov96_0220Data: the 0x270-byte work of the ov96_02209448 app (PokeathlonCourse_AllocPtr4FromHeap, reached through
// PokeathlonCourse_GetHeapAllocPtr4). Offsets below are DS offsets.

// One per player (4): the per-lane sprites, 0x70 bytes at 0x54 + 0x70 * i (built by asm ov96_02209F8C).
typedef struct Ov96_0220DataLane {
    u8 unk_0; // 0x00 last value passed to ov96_0220A254
    u8 pad_1[3];
    ManagedSprite *unk_4; // 0x04
    ManagedSprite *unk_8; // 0x08
    ManagedSprite *unk_C[20]; // 0x0C
    ManagedSprite *unk_5C[5]; // 0x5C
} Ov96_0220DataLane; // DS 0x70

// 0x264: touch drag state (ov96_0220A298).
typedef struct Ov96_0220DataDrag {
    u32 count : 8;
    u32 y1 : 8;
    u32 y0 : 8;
    u32 active : 1;
} Ov96_0220DataDrag;

typedef struct Ov96_0220Data {
    enum HeapID heapId; // 0x000
    BgConfig *bgConfig; // 0x004
    SpriteSystem *spriteSystem; // 0x008
    SpriteManager *spriteManager; // 0x00C
    struct Ov96SpriteSet *spriteSet; // 0x010
    struct UnkStruct_ov96_021E9A78 *resSrc; // 0x014
    String *names[3]; // 0x018
    ManagedSprite *unk_24[2]; // 0x024
    Sprite *unk_2C[3]; // 0x02C
    Sprite *unk_38[2]; // 0x038
    void *unk_40; // 0x040 ov96_0220A744 work
    void *unk_44; // 0x044 ov96_0220B7F4 work
    void *unk_48; // 0x048 ov96_0220C93C work
    void *unk_4C; // 0x04C ov96_0220B374 work
    struct NARC *narc; // 0x050
    Ov96_0220DataLane lanes[4]; // 0x054
    u16 unk_214; // 0x214
    u8 pad_216[2];
    Window windows[4]; // 0x218
    MsgData *msgData; // 0x258
    MessageFormat *msgFormat; // 0x25C
    s32 unk_260; // 0x260
    Ov96_0220DataDrag drag; // 0x264
    SysTask *unk_268; // 0x268
    s8 unk_26C; // 0x26C
    u8 unk_26D; // 0x26D
    u8 unk_26E; // 0x26E
    u8 pad_26F;
} Ov96_0220Data; // DS 0x270

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_0220_DATA_H
