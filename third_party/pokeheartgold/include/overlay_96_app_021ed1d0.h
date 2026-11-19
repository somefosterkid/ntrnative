#ifndef POKEHEARTGOLD_OVERLAY_96_APP_021ED1D0_H
#define POKEHEARTGOLD_OVERLAY_96_APP_021ED1D0_H

#include "constants/heap.h"

#include "bg_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_96_sprite_res_man.h"

// Ov96RankEntry: one of the four players' result rows, sorted by ov96_021ECC7C / ov96_021EE290.
typedef struct Ov96RankEntry {
    u32 val : 16;
    u32 rank : 8;
    u32 idx : 4;
    u32 extra : 4;
} Ov96RankEntry;

// Ov96PlayerSlot: one of the four players' result slots (ov96_021ECBB8 allocates 4 of them, 0x70 bytes); the first
// four sprites of the group are the ones ov96_021EC5C0 creates for the player.
typedef struct Ov96PlayerSlot {
    ManagedSprite *sprites[4];
    u32 target;    // result value, set by ov96_021ECDD4
    u32 shown;     // value counted up on screen by ov96_021ECD24
    BOOL counting; // set by ov96_021ECDD4, cleared when shown reaches target
} Ov96PlayerSlot; // size = 0x1c

// Ov96CryTask: the 0x10-byte data of the SysTask ov96_021EC218 (Heap_AllocAtEnd by ov96_021EC298), which plays out one
// Pokemon's reveal animation and cry.
typedef struct Ov96CryTask {
    u16 state;
    s32 timer;
    Ov96SpriteSetElem *elem;
    s32 *running; // points at Ov96App_021ED1D0::cryRunning, cleared when the task ends
} Ov96CryTask; // size = 0x10

// Ov96App_021ED1D0: the 0xbc-byte work object of the ov96 app run by ov96_021ED1D0 (PokeathlonCourse_AllocPtr4FromHeap),
// freed by ov96_021ED48C. It shows each player's three Pokemon one after the other.
typedef struct Ov96App_021ED1D0 {
    enum HeapID heapId;
    BgConfig *bgConfig;
    SysTask *cryTask;               // task running ov96_021EC218, created by ov96_021EC298
    struct Ov96TextBox *textBox;    // ov96_021EE740 object (message window + MessageFormat)
    UnkStruct_ov96_021E9A78 *resSrc;
    Ov96SpriteSet *spriteSet;       // 12 Pokemon sprites (4 players x 3)
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[26];     // [0..4] per-player icon, [5..20] 4 x 4 (ov96_021EC5C0), [21..23] ov96_021EC68C, [24..25] ov96_021EC70C
    Ov96CryTask *cryTaskData;
    Ov96PlayerSlot *playerSlots;    // array of 4, ov96_021ECBB8
    void *unk_90;                   // ov96_021ED054 object
    s32 timer;
    s32 cryRunning;
    Ov96RankEntry rank[4];
    u8 revealCount[4];              // Pokemon already shown per player
    u8 busy;
    u8 courseId;
    u8 playerIdx;
    u8 monIdx;
    u8 counter;
    u8 state;
    u8 unk_B6;
    u8 step;
    s32 unk_B8;
} Ov96App_021ED1D0;

// sizeof(Ov96App_021ED1D0) == 0xbc

#endif // POKEHEARTGOLD_OVERLAY_96_APP_021ED1D0_H
