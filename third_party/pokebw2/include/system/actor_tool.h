#ifndef POKEBW2_SYSTEM_ACTOR_TOOL_H
#define POKEBW2_SYSTEM_ACTOR_TOOL_H

#include "types.h"
#include "struct_decls.h"

// Tools for actors (actor_tool.c): palettes loaded into the first free run of an engine's OBJ palette slots. The names
// are ours

// mainCount and subCount are the slots of the main and sub engines' OBJ palettes the work hands out, up to 16 each
ActorPalSlots *ActorTool_CreatePalSlots(HeapID heapId, u8 mainCount, u8 subCount);
void ActorTool_DeletePalSlots(ActorPalSlots *slots);
// Load count palettes of an NCLR into free slots, and copy them into the fade's buffer, returning the palette's
// resource index
u32 ActorTool_LoadPalettesFade(PaletteFade *fade, ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count,
                               u32 heapId);
// The same with the whole NCLR, which takes count slots
u32 ActorTool_LoadPaletteFade(PaletteFade *fade, ActorPalSlots *slots, ArcTool *arc, u32 fileId, BOOL sub, u32 count,
                              u32 heapId);
// Free a palette that the two above loaded, and its slots
void ActorTool_FreePalette(ActorPalSlots *slots, u32 palette, BOOL sub);

#endif // POKEBW2_SYSTEM_ACTOR_TOOL_H
