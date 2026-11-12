// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_nogrid_mapper.h, the lines in
// use so far.
#ifndef BWPORT_FIELD_FIELD_NOGRID_MAPPER_H
#define BWPORT_FIELD_FIELD_NOGRID_MAPPER_H

#include "types.h"
#include "struct_decls.h"
#include "field/field_scene_area.h"

// field_nogrid_mapper.c: the mapper of the maps without a grid, which ties together the rail system that the player
// moves on, the loader of its data, the rails' tile attributes and the camera areas. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// Turns the camera areas on or off; the rail system's camera only follows the player while they are on
void FieldNoGridMapper_SetCameraAreaEnabled(NoGridMapper *mapper, BOOL enabled);
void FieldNoGridMapper_ForceUpdateCamera(NoGridMapper *mapper);
u32 FieldNoGridMapper_GetTileAtPos(NoGridMapper *mapper, const RailPosition *pos);
void FieldNoGridMapper_SetPlayerPos(NoGridMapper *mapper, const RailPosition *pos);
FieldRailSystem *FieldNoGridMapper_GetRailSystem(NoGridMapper *mapper);

NoGridMapper *FieldNoGridMapper_Create(u32 heapId, FieldCamera *camera, FieldSceneArea *sceneArea,
                                       FieldSceneAreaLoader *sceneAreaLoader);
void FieldNoGridMapper_Free(NoGridMapper *mapper);
void FieldNoGridMapper_UpdateCamera(NoGridMapper *mapper);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Makes the camera follow the unit, or not
void FieldNoGridMapper_SetCameraParent(NoGridMapper *mapper, RailUnit *unit);
void FieldNoGridMapper_ClearCameraParent(NoGridMapper *mapper);
// Loads the rails and camera areas of the file in ARCID_RAIL_DATA
void FieldNoGridMapper_LoadByHeader(NoGridMapper *mapper, u32 railId, u32 heapId);
RailUnit *FieldNoGridMapper_AcquireUnit(NoGridMapper *mapper);
void FieldNoGridMapper_ReleaseUnit(NoGridMapper *mapper, RailUnit *unit);
// Sets pos to line componentId of another zone's rails, front steps along it and side steps from its edge
void FieldNoGridMapper_CreatePosExternal(NoGridMapper *mapper, u32 zoneId, u16 componentId, u16 front, u16 side,
                                         RailPosition *pos, HeapID heapId);
// pokebw2's FieldNoGridMapper_SetLineEnabled
// Turns a rail line on or off; the rail system treats a line that is off as missing
void func_ov021_021bb3dc(NoGridMapper *mapper, u32 lineId, BOOL enabled);

#endif // BWPORT_FIELD_FIELD_NOGRID_MAPPER_H
