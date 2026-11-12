// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/actor_tool.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "system/actor_tool.h"
#include "system/palanm.h"

// Tools for actors: a work that hands out runs of the OBJ palette slots of each engine, and loads palettes into them,
// also into a palette fade's buffer. The names are ours

#define PAL_SLOT_FREE 0xff

struct ActorPalSlots {
    u32 unk0;
    // How many slots of each engine it hands out
    u8 count[2];
    // The first slot of the run each slot belongs to, or PAL_SLOT_FREE
    u8 slots[2][16];
};

static int func_02025360(ActorPalSlots *slots, int count, BOOL sub);
static void func_020253d4(ActorPalSlots *slots, int first, BOOL sub);
static u32 func_02025404(ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count, u32 heapId);
static u32 func_02025440(ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count, u32 heapId);
static u32 func_02025508(ActorPalSlots *slots, u32 palette, BOOL sub);

// pokebw2's ActorTool_CreatePalSlots
ActorPalSlots *func_02025318(HeapID heapId, u8 mainCount, u8 subCount) {
    ActorPalSlots *slots = GFL_HeapAllocate(heapId, sizeof(ActorPalSlots), TRUE, "actor_tool.c", 81);
    int i, j;

    slots->count[0] = mainCount;
    slots->count[1] = subCount;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 16; j++) {
            slots->slots[i][j] = PAL_SLOT_FREE;
        }
    }
    return slots;
}

// pokebw2's ActorTool_DeletePalSlots
void func_02025358(ActorPalSlots *slots) {
    GFL_HeapFree(slots);
}

// pokebw2's ActorPalSlots_Alloc
static int func_02025360(ActorPalSlots *slots, int count, BOOL sub) {
    int engine = sub ? 1 : 0;
    int i, j;

    for (i = 0; i < slots->count[engine]; i++) {
        if (slots->slots[engine][i] == PAL_SLOT_FREE) {
            for (j = i; j < i + count; j++) {
                if (slots->slots[engine][j] != PAL_SLOT_FREE) {
                    i = j;
                    break;
                }
            }
            if (j == i + count) {
                for (j = i; j < i + count; j++) {
                    slots->slots[engine][j] = i;
                }
                return i;
            }
        }
    }
    return 0;
}

// pokebw2's ActorPalSlots_Free
static void func_020253d4(ActorPalSlots *slots, int first, BOOL sub) {
    int engine = sub ? 1 : 0;
    u32 i;

    for (i = first; first == slots->slots[engine][i]; i++) {
        slots->slots[engine][i] = PAL_SLOT_FREE;
    }
}

// pokebw2's ActorTool_LoadPalettes
static u32 func_02025404(ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count, u32 heapId) {
    int first = func_02025360(slots, count, sub);

    return func_0204ac18(arc, fileId, sub, first * 32, 0, count, heapId);
}

// pokebw2's ActorTool_LoadPalette
static u32 func_02025440(ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count, u32 heapId) {
    int first = func_02025360(slots, count, sub);

    return func_0204acec(arc, fileId, sub, first * 32, heapId);
}

u32 func_02025470(PaletteFade *fade, ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count,
                               u32 heapId) {
    u16 vram;
    u32 palette;

    if (sub == FALSE) {
        vram = PALFADE_VRAM_MAIN_OBJ;
    } else {
        vram = PALFADE_VRAM_SUB_OBJ;
    }
    palette = func_02025404(slots, arc, fileId, sub, count, heapId);
    func_020216d4(fade, vram, func_02025508(slots, palette, sub) * 16, count * 32);
    return palette;
}

u32 func_020254bc(PaletteFade *fade, ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count,
                              u32 heapId) {
    u16 vram;
    u32 palette;

    if (sub == FALSE) {
        vram = PALFADE_VRAM_MAIN_OBJ;
    } else {
        vram = PALFADE_VRAM_SUB_OBJ;
    }
    palette = func_02025440(slots, arc, fileId, sub, count, heapId);
    func_020216d4(fade, vram, func_02025508(slots, palette, sub) * 16, count * 32);
    return palette;
}

// pokebw2's ActorTool_GetPalSlot
static u32 func_02025508(ActorPalSlots *slots, u32 palette, BOOL sub) {
    return func_0204aef4(palette, sub) / 32;
}

// pokebw2's ActorTool_FreePalette
void func_02025518(ActorPalSlots *slots, u32 palette, BOOL sub) {
    func_020253d4(slots, func_02025508(slots, palette, sub), sub);
    func_0204ada4(palette);
}
