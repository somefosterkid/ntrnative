#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_ARENA_WORK_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_ARENA_WORK_H

// Ov96ArenaWork: the 0x7f4 heap object of a Pokeathlon course that is played on one screen: 4 players x 3 Pokemon
// ("mon") move around a bordered arena, collide and get ranked (ov96_021F010C allocates it in the
// PokeathlonCourse_AllocPtr4FromHeap slot). Offsets are the DS ones.

#include "bg_window.h"
#include "constants/heap.h"
#include "overlay_96_sprite_res_man.h"
#include "sprite.h"
#include "sys_task_api.h"

struct NNSG2dScreenData;
struct Ov96ArenaCpu;
struct PokeathlonCourseData;

// one Pokemon (0x90): Ov96ArenaWork.mon[player][slot]; the strides 0x1b0 / 0x90 of the m2c code are these arrays
struct Ov96SpriteMgr;

typedef struct Ov96ArenaMon {
    Ov96SpriteSetElem *spr; // 0x00
    Sprite *sprFront;       // 0x04 draw priority 2
    Sprite *sprBack;        // 0x08 draw priority 4
    VecFx32 unk_0C;
    s32 state;              // 0x18: 0 = not placed, 1 = on the field, 2 = placed by ov96_021F2A00
    VecFx32 unk_1C;
    VecFx32 pos;            // 0x28
    u8 unk_34[0xC];
    s8 unk_40;
    u8 unk_41;
    u16 unk_42;
    u8 unk_44;
    u8 unk_45;
    u8 unk_46[0x43];
    s8 unk_89;
    u16 unk_8A;
    u16 unk_8C;
    u16 unk_8E;
} Ov96ArenaMon; // size = 0x90

// the screen effect block at Ov96ArenaWork.fx (0x34): ov96_021F2B68 starts it, ov96_021F2D98 stops it
typedef struct Ov96ArenaFx {
    u8 unk_00; // ov96_021F2BB4 state
    u8 unk_01; // ov96_021F2C04 state
    u8 unk_02; // ov96_021F2CD0 state
    u8 unk_03;
    SysTask *unk_04;
    SysTask *unk_08;
    SysTask *unk_0C;
    SysTask *unk_10;
    SysTask *unk_14;
    u16 unk_18;
    u16 unk_1A;
    u16 unk_1C;
    u16 unk_1E;
    void *unk_20;
    s16 unk_24;
    u16 unk_26;
    s32 unk_28;
    s16 unk_2C;
    u16 unk_2E;
    struct PokeathlonCourseData *unk_30;
} Ov96ArenaFx; // size = 0x34

// one pair of the collision table _0221DCA0[a][b] (0x10 on the DS): ov96_021F21EC fills it, ov96_021F22FC reads it
typedef struct Ov96ArenaLink {
    s32 linked;
    s32 unk4;
    Ov96ArenaMon *other;
    s32 unkC;
} Ov96ArenaLink;

typedef struct Ov96ArenaWork {
    BgConfig *bgConfig;               // 0x000
    Window window;                    // 0x004
    enum HeapID heapId;               // 0x014
    struct Ov96SpriteMgr *unk_18;     // 0x018 ov96_021EB180 result
    Sprite *unk_1C;                   // 0x01c
    Ov96ArenaMon mon[4][3];           // 0x020
    u32 unk_6E0[4];
    u32 unk_6F0;
    u32 unk_6F4;
    u8 unk_6F8[0x28];
    u8 unk_720;
    u8 unk_721[3];
    u8 unk_724;
    u8 unk_725;
    u8 unk_726;
    u8 unk_727;
    u8 unk_728;
    u8 unk_729;
    u8 unk_72A;
    u8 unk_72B;
    void *unk_72C;                    // 0x72c ov96_021F30A4 result
    u16 unk_730;
    u16 unk_732;
    void *unk_734;                    // 0x734 raw screen data (GfGfxLoader_GetScrnData)
    struct NNSG2dScreenData *unk_738; // 0x738
    u8 unk_73C[0xC];
    u8 unk_748;
    u8 unk_749;
    u8 unk_74A;
    u8 unk_74B;
    u8 unk_74C;
    u8 unk_74D;
    u8 unk_74E;
    u8 unk_74F;
    u8 unk_750[12];
    u8 unk_75C[12];
    UnkStruct_ov96_021E9A78 *resSrc;  // 0x768
    Ov96SpriteSet *spriteSet;         // 0x76c
    struct Ov96ArenaCpu *cpu;         // 0x770 ov96_021F3390
    void *unk_774;                    // 0x774 ov96_021F3BF0 result
    u16 unk_778;
    u16 unk_77A;
    u8 unk_77C[0x2C];
    u32 unk_7A8;
    Ov96ArenaFx fx;                   // 0x7ac
    u8 unk_7E0[12];
    u32 unk_7EC;
    void *unk_7F0;                    // 0x7f0 Heap_Alloc(0x28)
} Ov96ArenaWork;

// DS layout (sizeof(Ov96ArenaWork) == 0x7f4)

extern Ov96ArenaLink _0221DCA0[12][12];

// Hit test of a touch point against the target set `table` (0xF = none); the DS callers pass the
// table id as u8 and the point as u16, so out-of-range values wrap there.
s32 ov96_021F3180(u8 table, u16 x, u16 y, u8 *outIdx);

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_ARENA_WORK_H
