// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_palace.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_PALACE_H
#define BWPORT_FIELD_FIELD_PALACE_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

void FieldPalaceSys_InitPostFX(FieldPalaceSys *sys, u16 zoneId, HeapID heapId);
BOOL FieldPalaceSys_CheckEventFlag(GameData *gameData, u16 zoneId);

struct FieldPalaceSys {
    GameSystem *gsys;
    Field *field;
    void *luminanceTable;
};

FieldPalaceSys *FieldPalaceSys_Create(HeapID heapId, GameSystem *gsys, Field *field, u16 zoneId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void FieldPalaceSys_LoadLuminanceTable(FieldPalaceSys *sys, u32 fileId, u16 zoneId, u32 season, HeapID heapId);
void FieldPalaceSys_Free(FieldPalaceSys *sys);
void *FieldPalaceSys_GetLuminanceTable(FieldPalaceSys *sys);

#endif // BWPORT_FIELD_FIELD_PALACE_H
