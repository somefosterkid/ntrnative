#ifndef POKEHEARTGOLD_OVERLAY_96_APP_02210C98_H
#define POKEHEARTGOLD_OVERLAY_96_APP_02210C98_H

// Ov96App_02210C98: hk310. The 0x820-byte work object of the ov96 touch-screen minigame run by ov96_02210C98
// (PokeathlonCourse_AllocPtr4FromHeap(data, 0x820), asm). 4 players x 3 units, 2 movers, 4 text tasks.

#include "constants/heap.h"

#include "bg_window.h"

struct Ov96CourseHud;
struct Ov96SpriteSet;
struct Ov96SpriteSetElem;
struct SysTask;
struct String;
struct UnkStruct_ov96_021E9A78;

// One unit of a player: owns a slot of the sprite set.
typedef struct Ov96App_02210C98_Unit {
    struct Ov96SpriteSetElem *unk_0;
    u8 unk_4[0x20];
    VecFx32 unk_24; // 0x24 target (the CPU brain writes x/y; ov96_02214DBC copies a mover's position)
    VecFx32 unk_30; // 0x30 position
    u8 unk_3C[0xC];
    s32 unk_48;
    u8 unk_4C[0x12];
    u16 unk_5E;
    u8 unk_60[0x10];
    u8 unk_70; // 0x70 stats ov96_02214ABC ranks the units by
    u8 unk_71;
    u8 unk_72;
    u8 unk_73;
    u8 unk_74;
    u8 unk_75;
    u8 unk_76[0x2];
    s32 unk_78;
} Ov96App_02210C98_Unit;

// A moving object.
typedef struct Ov96App_02210C98_Mover {
    u8 unk_0[0x8];
    VecFx32 unk_8;
    VecFx32 unk_14;
    VecFx32 unk_20;
    u8 unk_2C[0xC];
    u8 unk_38;
    u8 unk_39;
    u8 unk_3A;
    u8 unk_3B;
    u8 unk_3C;
    u8 unk_3D;
    u8 unk_3E;
    u8 unk_3F;
    u8 unk_40;
    u8 unk_41;
    u8 unk_42;
    u8 unk_43;
    u8 unk_44;
    u8 unk_45;
    u16 unk_46;
    s32 unk_48;
} Ov96App_02210C98_Mover;

// hk389: the CPU-player brain (ov96_02214A24: Heap_Alloc(heapId, 0xA0)), kept in Ov96App_02210C98.unk_74C.
// Every tick (ov96_02214B84) one CPU player's current slot counts its timer down; at 0 ov96_022155A0 runs the
// slot's strategy for its unit (slot 0: ov96_02214EB4, 1: ov96_02214C3C, 2: ov96_02215184) and stores the new delay.
typedef struct Ov96App_02210C98_CpuSlot {
    s32 timer;
    Ov96App_02210C98_Unit *unit;
} Ov96App_02210C98_CpuSlot;

typedef struct Ov96App_02210C98_CpuPlayer {
    Ov96App_02210C98_CpuSlot slots[3]; // 0x00 (ov96_02214ABC fills them from units[] by rank)
    Ov96App_02210C98_Unit *units[3];   // 0x18 (ov96_02214A6C)
} Ov96App_02210C98_CpuPlayer; // 0x24

typedef struct Ov96App_02210C98_Cpu {
    Ov96App_02210C98_CpuPlayer players[4]; // 0x00
    Ov96App_02210C98_Mover *movers;        // 0x90 Ov96App_02210C98.unk_62C (ov96_02214B74)
    BOOL running;                          // 0x94
    u8 numCpu;                             // 0x98
    u8 cur;                                // 0x99 CPU player handled this tick (from firstCpu)
    u8 tick;                               // 0x9A
    u8 interval;                           // 0x9B
    u8 slot;                               // 0x9C
    u8 firstCpu;                           // 0x9D
    u8 numHuman;                           // 0x9E 4 - numCpu
} Ov96App_02210C98_Cpu; // 0xA0

typedef struct Ov96App_02210C98_Touch {
    s32 unk_0;
    s32 unk_4;
    s32 unk_8;
} Ov96App_02210C98_Touch;

struct Ov96App_02210C98;

typedef struct Ov96App_02210C98_Task {
    u8 unk_0;
    s8 unk_1;
    u8 unk_2;
    s8 unk_3;
    struct SysTask *unk_4;
    struct Ov96App_02210C98 *unk_8;
    u8 unk_C[0x2];
    u8 unk_E;
    u8 unk_F;
} Ov96App_02210C98_Task;

typedef struct Ov96App_02210C98 {
    void *unk_0;
    struct BgConfig *unk_4;
    Window unk_8[5];
    enum HeapID unk_58;
    Ov96App_02210C98_Unit unk_5C[4][3];
    Ov96App_02210C98_Mover unk_62C[2];
    void *unk_6C4[4];
    void *unk_6D4[4];
    void *unk_6E4[4];
    s32 unk_6F4[4];
    Ov96App_02210C98_Touch unk_704[4];
    u8 unk_734[4];
    s32 unk_738;
    u8 unk_73C;
    u8 unk_73D;
    u8 unk_73E;
    u8 unk_73F;
    u8 unk_740[0x2];
    u8 unk_742;
    u8 unk_743;
    struct UnkStruct_ov96_021E9A78 *unk_744;
    struct Ov96SpriteSet *unk_748;
    Ov96App_02210C98_Cpu *unk_74C;
    struct Ov96CourseHud *unk_750;
    u8 unk_754[0x60];
    u8 unk_7B4[4];
    struct String *unk_7B8[3];
    struct String *unk_7C4[4];
    Ov96App_02210C98_Task unk_7D4[4];
    u8 unk_814[4];
    u8 unk_818[4];
    void *unk_81C;
} Ov96App_02210C98;

// sizeof(Ov96App_02210C98) == 0x820

#endif // POKEHEARTGOLD_OVERLAY_96_APP_02210C98_H
