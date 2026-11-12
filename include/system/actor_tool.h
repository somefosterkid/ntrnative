// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/actor_tool.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_ACTOR_TOOL_H
#define BWPORT_SYSTEM_ACTOR_TOOL_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's ActorTool_CreatePalSlots
// mainCount and subCount are the slots of the main and sub engines' OBJ palettes the work hands out, up to 16 each
ActorPalSlots *func_02025318(HeapID heapId, u8 mainCount, u8 subCount);
// pokebw2's ActorTool_LoadPalettesFade
// Load count palettes of an NCLR into free slots, and copy them into the fade's buffer, returning the palette's
// resource index
u32 func_02025470(PaletteFade *fade, ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count,
                               u32 heapId);
// pokebw2's ActorTool_FreePalette
// Free a palette that the two above loaded, and its slots
void func_02025518(ActorPalSlots *slots, u32 palette, BOOL sub);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ActorTool_DeletePalSlots
void func_02025358(ActorPalSlots *slots);
// pokebw2's ActorTool_LoadPaletteFade
// The same with the whole NCLR, which takes count slots
u32 func_020254bc(PaletteFade *fade, ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count,
                              u32 heapId);

#endif // BWPORT_SYSTEM_ACTOR_TOOL_H
