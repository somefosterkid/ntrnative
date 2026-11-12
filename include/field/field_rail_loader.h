// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_rail_loader.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_FIELD_RAIL_LOADER_H
#define BWPORT_FIELD_FIELD_RAIL_LOADER_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "field/field_rail.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
RailLoader *FieldRailLoader_Create(u32 heapId);
void FieldRailLoader_Free(RailLoader *loader);
void FieldRailLoader_Reset(RailLoader *loader);
void FieldRailLoader_LoadFile(RailLoader *loader, u32 fileId, u32 heapId);
// The points, lines, curves and cameras of the loaded file, which the rail system works on
RailDataHandle *FieldRailLoader_GetRailData(RailLoader *loader);
// Sets byte 2 of the actor's billboard work
void func_ov021_0219f5c8(FieldActor *actor, u32 flag);

// A rail file's header: where its parts start, from the file's start (each part ends where the next starts)
typedef struct RailFileHeader {
    u32 unk_00;
    u32 tileWidthOffset;
    u32 pointsOffset;
    u32 linesOffset;
    u32 camerasOffset;
    u32 curvesOffset;
    u32 endOffset;
} RailFileHeader;

// The loaded rail file and the handle into it (0x38 bytes: FieldRailLoader_Create's allocation)
struct RailLoader {
    RailFileHeader *data;
    u32 size;
    RailDataHandle handle;
};

#endif // BWPORT_FIELD_FIELD_RAIL_LOADER_H
