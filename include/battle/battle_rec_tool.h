// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/battle_rec_tool.h: the lines in use
// so far.
#ifndef BWPORT_BATTLE_BATTLE_REC_TOOL_H
#define BWPORT_BATTLE_BATTLE_REC_TOOL_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "save/battle_rec.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BattleRecTool_StoreSetup
// Store a battle's setup in the loaded video, or in Pokéstar Studios' block when setup->unkDD_3 is 1 or 2, and load
// it from either, by mode
void func_ov188_021eee78(BtlSetup *setup);
// pokebw2's BattleRecTool_LoadPkm
// Makes a party Pokémon of a recorded one, which it encrypts
void func_ov188_021eebf0(BattleRecPkm *rec, PartyPkm *pkm);
// pokebw2's BattleRecTool_LoadSetup
void func_ov188_021eeea8(BtlSetup *setup, HeapID heapId);

#endif // BWPORT_BATTLE_BATTLE_REC_TOOL_H
