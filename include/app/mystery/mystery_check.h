// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mystery/mystery_check.h.
#ifndef BWPORT_APP_MYSTERY_MYSTERY_CHECK_H
#define BWPORT_APP_MYSTERY_MYSTERY_CHECK_H

// Mystery Gift's check of a received gift (ov121, mystery_check.c, a descriptive name). pokebw2's names; swan has
// none for this overlay

#include "types.h"
#include "gfl/heap.h"
#include "save/mystery_gift.h"
#include "struct_decls.h"

// The number of things wrong with the gift: unterminated texts, an ID or kind out of range, or a Pokémon that can't be
// made
u32 func_ov121_021def6c(const MysteryGiftRecvData *recv, GameData *gameData, HeapID heapId);

#endif // BWPORT_APP_MYSTERY_MYSTERY_CHECK_H
