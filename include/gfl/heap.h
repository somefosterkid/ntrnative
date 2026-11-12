// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/heap.h, the lines in use so far.
#ifndef BWPORT_GFL_HEAP_H
#define BWPORT_GFL_HEAP_H

// Names, layouts and constants from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nnsys/fnd.h"

typedef u16 HeapID;

enum {
    HEAPID_SYSTEM = 0x0,
    HEAPID_USER = 0x1,
    HEAPID_GAMESYSTEM = 0x3,
    HEAPID_GAMEEVENT = 0x4,
    HEAPID_BATTLE = 0x12,
    HEAPID_BATTLE_RECORDER = 0x5a,
    HEAPID_BATTLE_RECORDER_SYS = 0x59,
    HEAPID_NET = 0x8,
    HEAPID_DEVICE_ALLOC = 0x9,
    HEAPID_BAG = 0xe,
    HEAPID_DWC_UTILITY = 0xf,
    HEAPID_FIELDMAP = 0x15,
    HEAPID_MYSTERY = 0x1d,
    HEAPID_IRC_BATTLE_MENU = 0x1f,
    HEAPID_PMS_INPUT_SYS = 0x29,
    HEAPID_MB_PARENT = 0x5d,
    HEAPID_PMS_INPUT = 0x2a,
    HEAPID_MUSICAL_EVENT = 0x2c,
    HEAPID_MUSICAL_DRESSUP = 0x2d,
    HEAPID_IRC_COMPAT = 0x3c,
    HEAPID_P_STATUS = 0x42,
    HEAPID_WORLDTRADE = 0x48,
    HEAPID_BOX2 = 0x4b,
    HEAPID_BOX2_APP = 0x4c,
    HEAPID_GAME_CLEAR_SAVE = 0x54,
    HEAPID_MONOLITH = 0x61,
    HEAPID_VS_MULTI_LIST = 0x71,
    HEAPID_UN_SELECT = 0x7a,
    HEAPID_ZUKAN_OPEN = 0x7e,
    HEAPID_THE_END = 0x88,
    HEAPID_SUBWAY_MAP = 0x88,
    HEAPID_POKEMON_TRADE = 0x91,
    HEAPID_GAMESYNC = 0x67,
};

// Allocates from the end of the heap instead of the start
#define HEAPID_TAIL_BIT 0x8000
#define HEAPID_TAIL(heapId) ((HeapID)(((heapId) & (HEAPID_TAIL_BIT - 1)) | HEAPID_TAIL_BIT))

// A heap that GFL_MemInit creates from the main arena
typedef struct {
    u32 size;
    u32 unk4;
} HeapDef;

// Creates the root heaps, after reserving reserveSize bytes of the arena, with room for maxHeapIds heap IDs
void GFL_MemInit(const HeapDef *defs, u32 rootCount, u32 maxHeapIds, u32 reserveSize);
void GFL_HeapCreateChild(HeapID parentHeapId, HeapID heapId, u32 size);
// Creates a heap in memory the caller owns
void GFL_HeapCreateRoot(void *memory, u32 size, HeapID heapId);
void GFL_HeapDelete(HeapID heapId);
void *GFL_HeapAllocate(HeapID heapId, u32 size, BOOL clear, const char *file, u16 line);
void GFL_HeapFree(void *ptr);
void GFL_HeapCreateAllocator(NNSFndAllocator *allocator, HeapID heapId, int alignment);
// Shrinks or grows a block in place
void GFL_HeapResize(void *ptr, u32 size);
u32 GFL_HeapGetFreeSize(HeapID heapId);
BOOL GFL_HeapStatusValidate(HeapID heapId);
void GFL_HeapDumpOnFailure(HeapID heapId);
void GFL_HeapDTCMInit(u32 size);
void *GFL_HeapDTCMAllocate(u32 size);
void _freeBlkFromDTCM(void *ptr);

#endif // BWPORT_GFL_HEAP_H
