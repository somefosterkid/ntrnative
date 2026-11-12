#ifndef POKEBW2_PML_HM_CHECK_H
#define POKEBW2_PML_HM_CHECK_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Whether a Pokémon's moves are HMs, which it can't forget. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// Whether a move is an HM. The game data and heap go unused
BOOL isPkmMoveHmMove(GameData *gameData, u16 move, HeapID heapId);
// Whether the Pokémon knows no HM, despite swan's name. The second parameter goes unused
BOOL doesPkmHaveTmMove(BoxPkm *pkm, u32 unused);

#endif // POKEBW2_PML_HM_CHECK_H
