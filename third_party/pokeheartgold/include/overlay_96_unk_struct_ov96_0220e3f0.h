#ifndef POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_0220E3F0_H
#define POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_0220E3F0_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"

struct Ov96SpriteSet;
struct UnkStruct_ov96_021E9A78;
struct Ov96App_0221040C;

// UnkStruct_ov96_0220E3F0: the 0x6bc-byte event work allocated by ov96_0220E3F0
// (PokeathlonCourse_AllocPtr4FromHeap) and reached through PokeathlonCourse_GetHeapAllocPtr4.
// Offsets below are DS offsets.

// 0xc-byte per-unit cell (ov96_0220F378 / ov96_0220F280); Unit.cell points at one.
typedef struct UnkStruct_ov96_0220E3F0_Cell {
    u8 unk_0; // 0x0 (ov96_0220F3B4 returns unk_0 * 2)
    u8 unk_1; // 0x1
    u8 unk_2; // 0x2
    u8 unk_3; // 0x3
    s32 unk_4; // 0x4 speed
    s32 unk_8; // 0x8
} UnkStruct_ov96_0220E3F0_Cell; // DS 0xc

// Unit.unk_40 as bitfields (ov96_0220D744); the other users mask the raw word.
typedef struct UnkStruct_ov96_0220E3F0_UnitBits {
    u32 unk_0 : 20; // 0..7 timer, 8..15 (ov96_0220D630)
    u32 unk_20 : 4; // direction (ov96_0220D6CC)
    u32 unk_24 : 2;
    u32 unk_26 : 1; // knocked back
    u32 unk_27 : 1; // moving (ov96_0220D6B0)
    u32 unk_28 : 1;
    u32 unk_29 : 3;
} UnkStruct_ov96_0220E3F0_UnitBits;

// 0x48-byte unit (ov96_0220D630 / D6B0 / D9A4 / DAA0); 3 per entry.
typedef struct UnkStruct_ov96_0220E3F0_Unit {
    PokeathlonCourseData *course; // 0x0
    void *unk_4; // 0x4 map object (ov96_021EAF8C / ov96_021EB06C)
    UnkStruct_ov96_0220E3F0_Cell *cell; // 0x8
    s32 state; // 0xc
    VecFx32 unk_10; // 0x10 target
    VecFx32 unk_1C; // 0x1c position
    VecFx32 unk_28; // 0x28
    VecFx32 unk_34; // 0x34 velocity
    union {
        u32 raw;
        UnkStruct_ov96_0220E3F0_UnitBits bits;
    } unk_40; // 0x40
    u8 unk_44; // 0x44 timer
    u8 unk_45; // 0x45 entry index
    u8 unk_46; // 0x46 unit index
    u8 unk_47; // 0x47
} UnkStruct_ov96_0220E3F0_Unit; // DS 0x48

// Entry.unk_4 as bitfields (ov96_0220F8C8).
typedef struct UnkStruct_ov96_0220E3F0_EntryBits {
    u32 lo : 14; // 0..13 counter
    u32 b14 : 1;
    u32 b15 : 1;
    u32 v16 : 13;
    u32 b29 : 1;
    u32 sel : 2; // unit index
} UnkStruct_ov96_0220E3F0_EntryBits;

typedef struct UnkStruct_ov96_0220E3F0_Entry {
    s16 unk_0; // 0x0
    s16 unk_2; // 0x2
    union {
        u32 raw;
        UnkStruct_ov96_0220E3F0_EntryBits bits;
    } unk_4; // 0x4
    UnkStruct_ov96_0220E3F0_Unit units[3]; // 0x8
    u32 unk_E0; // 0xe0 bits
} UnkStruct_ov96_0220E3F0_Entry; // DS 0xe4

// 0xc-byte record driven by ov96_0220FE38 / ov96_0220FB98.
typedef struct UnkStruct_ov96_0220E3F0_Record {
    s16 unk_0; // 0x0
    s16 unk_2; // 0x2
    s32 unk_4; // 0x4 bits
    u8 unk_8; // 0x8
    u8 unk_9; // 0x9
    u8 unk_A; // 0xa
    u8 unk_B; // 0xb
} UnkStruct_ov96_0220E3F0_Record; // DS 0xc

// hit result filled by ov96_0220FBEC and read by ov96_0220FD28 (a stack local, not part of the work).
typedef struct UnkStruct_ov96_0220E3F0_Hit {
    s16 x; // 0x0
    s16 y; // 0x2
    u8 entry; // 0x4
    u8 unit; // 0x5
} UnkStruct_ov96_0220E3F0_Hit; // DS 0x6

// 0x4ec sub-object (ov96_0220FF64 / ov96_0220FF88 / ov96_02210030).
typedef struct UnkStruct_ov96_0220E3F0_Score {
    UnkStruct_ov96_0220E3F0_Entry *entries; // 0x0
    UnkStruct_ov96_0220E3F0_Record records[2]; // 0x4
    u32 unk_1C; // 0x1c bits
} UnkStruct_ov96_0220E3F0_Score; // DS 0x20

// HUD sprite group (ov96_0220DB3C / ov96_0220DBE8 / ov96_0220DC7C).
typedef struct UnkStruct_ov96_0220E3F0_HudGroup {
    ManagedSprite *unk_0[4]; // 0x0 ([j] and [j + 2] pair up)
    ManagedSprite *unk_10; // 0x10
    ManagedSprite *unk_14; // 0x14
    u32 unk_18; // 0x18 bits
} UnkStruct_ov96_0220E3F0_HudGroup; // DS 0x1c

// scale animation task data (ov96_0220D468 / ov96_0220D52C).
typedef struct UnkStruct_ov96_0220E3F0_ScaleAnim {
    void *unk_0; // 0x0 (ov96_021EB10C)
    SysTask *task; // 0x4
    f32 scale; // 0x8
    u16 counter; // 0xc
    u16 phase; // 0xe
} UnkStruct_ov96_0220E3F0_ScaleAnim; // DS 0x10

typedef struct UnkStruct_ov96_0220E3F0_HudSlot {
    ManagedSprite *sprite; // 0x0
    UnkStruct_ov96_0220E3F0_ScaleAnim anim; // 0x4
    s32 unk_14; // 0x14
    u32 unk_18; // 0x18 bits
} UnkStruct_ov96_0220E3F0_HudSlot; // DS 0x1c

// 0x510 sub-object (ov96_0221007C / ov96_0221013C / ov96_0220DC7C / ov96_0220DEAC).
typedef struct UnkStruct_ov96_0220E3F0_Hud {
    struct Ov96App_0221040C *unk_0; // 0x0 (ov96_02210240 result)
    UnkStruct_ov96_0220E3F0_HudGroup groups[2]; // 0x4
    UnkStruct_ov96_0220E3F0_HudSlot slots[12]; // 0x3c
    u16 unk_18C[4]; // 0x18c
    u8 unk_194[4]; // 0x194
} UnkStruct_ov96_0220E3F0_Hud; // DS 0x198

// 0x6a8 sub-object (ov96_0220D408 / ov96_0220D420 / ov96_0220D428).
typedef struct UnkStruct_ov96_0220E3F0_6A8 {
    SpriteSystem *unk_0; // 0x0
    BgConfig *unk_4; // 0x4
    SysTask *task; // 0x8
    u8 unk_C; // 0xc
    u8 unk_D; // 0xd
    u32 unk_10; // 0x10
} UnkStruct_ov96_0220E3F0_6A8; // DS 0x14

typedef struct UnkStruct_ov96_0220E3F0 {
    enum HeapID heapId; // 0x0
    PokeathlonCourseData *course; // 0x4
    BgConfig *bgConfig; // 0x8
    SpriteSystem *spriteSystem; // 0xc
    SpriteManager *spriteManager; // 0x10
    MessageFormat *msgFormat; // 0x14
    MsgData *msgData; // 0x18
    struct UnkStruct_ov96_021E9A78 *unk_1C; // 0x1c
    struct Ov96SpriteSet *unk_20; // 0x20
    Window window; // 0x24
    UnkStruct_ov96_0220E3F0_Cell cells[12]; // 0x34 [entry * 3 + unit]
    struct Ov96App_0221040C *unk_C4; // 0xc4 (ov96_02210240 result)
    void *unk_C8; // 0xc8 (ov96_022107F0 result)
    u8 hitFlags[12][12]; // 0xcc
    UnkStruct_ov96_0220E3F0_Entry entries[4]; // 0x15c
    UnkStruct_ov96_0220E3F0_Score score; // 0x4ec
    s32 timer; // 0x50c
    UnkStruct_ov96_0220E3F0_Hud hud; // 0x510
    UnkStruct_ov96_0220E3F0_6A8 unk_6A8; // 0x6a8
} UnkStruct_ov96_0220E3F0; // DS 0x6bc

#endif // POKEHEARTGOLD_OVERLAY_96_UNK_STRUCT_OV96_0220E3F0_H
