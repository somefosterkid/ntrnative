#ifndef POKEHEARTGOLD_OAM_MANAGER_H
#define POKEHEARTGOLD_OAM_MANAGER_H

#include "heap.h"
#include "save_layout.h"

// OamManager: the main/sub OAM manager pair OamManager_Create allocates into _021D0EB0 (0x3C bytes on the DS).
typedef struct OamManager {
    NNSG2dOamManagerInstance mainScreenOam;
    NNSG2dOamManagerInstance subScreenOam;
    enum HeapID heapID;
} OamManager;

#ifdef __MWERKS__
LAYOUT_CHECK(OamManager_size, sizeof(OamManager) == 0x3C);
LAYOUT_CHECK_OFFSET(OamManager_subScreenOam, OamManager, subScreenOam, 0x1C);
LAYOUT_CHECK_OFFSET(OamManager_heapID, OamManager, heapID, 0x38);
#endif

extern OamManager *_021D0EB0;

#endif // POKEHEARTGOLD_OAM_MANAGER_H
