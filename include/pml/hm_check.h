// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/hm_check.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_PML_HM_CHECK_H
#define BWPORT_PML_HM_CHECK_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Whether the Pokémon knows no HM, despite swan's name. The second parameter goes unused
BOOL doesPkmHaveTmMove(BoxPkm *pkm, u32 unused);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Whether a move is an HM. The game data and heap go unused
BOOL isPkmMoveHmMove(GameData *gameData, u16 move, HeapID heapId);

#endif // BWPORT_PML_HM_CHECK_H
