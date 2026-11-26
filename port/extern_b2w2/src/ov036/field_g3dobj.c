// Black 2's and White 2's field 3D objects (port/extern/src/ov021/ov021_021c6400.c's twin): pokebw2's src/ov036/field_g3dobj.c
// compiled through this file (tools/port_sources.py --b2w2), with Smooth motion's tags (libntr's simulator/sim_g3tag.h) on the
// effects drawn as models (the dust cloud of src/ov036/fldeff_dust.c, the Surf Pokemon, splashes) and the field actors that are
// models. Each object FieldG3DObjSystem_Draw (all of them) or FieldG3DObjSystem_DrawObj (one, for its effect) draws is tagged
// right before it (kind 5 effect: owner its resource group, inst the object, state 0); both end with the empty tag;
// FieldG3DObjSystem_AddObj gives the new object in a slot a new id. Off (the default) each site is one flag test and the command
// stream is unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1) (Smooth motion on) turns them on. The DS build never sees
// this file.
#pragma push_macro("FieldG3DObjSystem_Draw")
#pragma push_macro("FieldG3DObjSystem_DrawObj")
#pragma push_macro("FieldG3DObjSystem_AddObj")
#define FieldG3DObjSystem_Draw FieldG3DObjSystem_Draw_game
#define FieldG3DObjSystem_DrawObj FieldG3DObjSystem_DrawObj_game
#define FieldG3DObjSystem_AddObj FieldG3DObjSystem_AddObj_game
#include "types.h"   // the game file's headers, before the hooks below (their include guards keep them out of them)
#include "field/field.h"
#include "field/field_g3dobj.h"
#include "gfl/arc.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/fx.h"
#include "system/aeabi.h"
#include <simulator/sim_g3tag.h>

static void PortFldObj_Tag(const FieldG3DObjSystem *system, u16 resGroup, const void *obj);

// GFL_G3DSysDrawObj and _BBoxCull are called only in FieldG3DObjSystem_Draw and _DrawObj (their system and obj)
#define GFL_G3DSysDrawObj(actor, srt)                                                                                  \
    ((SIM_G3TagOn ? PortFldObj_Tag(system, obj->resGroup, obj) : (void)0), GFL_G3DSysDrawObj(actor, srt))
#define GFL_G3DSysDrawObjBBoxCull(actor, srt)                                                                          \
    ((SIM_G3TagOn ? PortFldObj_Tag(system, obj->resGroup, obj) : (void)0), GFL_G3DSysDrawObjBBoxCull(actor, srt))
#include "../../../../third_party/pokebw2/src/ov036/field_g3dobj.c"
#undef GFL_G3DSysDrawObj
#undef GFL_G3DSysDrawObjBBoxCull
#pragma pop_macro("FieldG3DObjSystem_Draw")
#pragma pop_macro("FieldG3DObjSystem_DrawObj")
#pragma pop_macro("FieldG3DObjSystem_AddObj")

static void PortFldObj_Tag(const FieldG3DObjSystem *system, u16 resGroup, const void *obj) {
    SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_EFFECT, &system->resGroups[resGroup]), SIM_G3TagId(obj), 0);
}

void FieldG3DObjSystem_Draw(FieldG3DObjSystem *system) {
    FieldG3DObjSystem_Draw_game(system);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

void FieldG3DObjSystem_DrawObj(FieldG3DObjSystem *system, u16 objIndex) {
    FieldG3DObjSystem_DrawObj_game(system, objIndex);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

u16 FieldG3DObjSystem_AddObj(FieldG3DObjSystem *system, u16 resGroup, u16 modelIndex, const VecFx32 *position) {
    // the game takes the first free slot and returns 0 also when none is free
    BOOL free0 = system->objCount != 0 && system->objs[0].state == G3DOBJ_STATE_NONE && system->objs[0].actor == NULL;
    u16 i = FieldG3DObjSystem_AddObj_game(system, resGroup, modelIndex, position);

    if (i != 0 || free0) {
        SIM_G3TagDrop(&system->objs[i]);
    }
    return i;
}
