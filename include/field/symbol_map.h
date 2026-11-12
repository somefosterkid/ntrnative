// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/symbol_map.h: the lines in use so far.
#ifndef BWPORT_FIELD_SYMBOL_MAP_H
#define BWPORT_FIELD_SYMBOL_MAP_H

#include "types.h"
#include "field/entree_forest.h"
#include "gfl/heap.h"
#include "struct_decls.h"

BOOL func_ov010_02166618(u32 area);
BOOL func_ov010_02166640(u32 area);

// Black 2's, not checked against Black
struct SymbolMapList {
    EntreeForestPokemon pokemon[20];
    u32 count : 6;
    u32 unk50_6 : 4;
    u32 unk50_10 : 4;
    u32 unk50_14 : 4;
    u32 unk50_18 : 6;
    u32 unk50_24 : 8;
};

BOOL func_ov010_0216628c(AreaNPCSave *npcData, u32 index);
// The zone of an area of the forest
u16 func_ov010_021665d0(GameSystem *gsys, u8 area);
// The area next to an area of the forest, in a direction
u32 func_ov010_021665f0(GameSystem *gsys, u8 area, u32 dir);
BOOL func_ov010_0216662c(u32 area);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
SymbolMapList *func_ov010_021664cc(HeapID heapId, GameSystem *gsys, u32 *count);

#endif // BWPORT_FIELD_SYMBOL_MAP_H
