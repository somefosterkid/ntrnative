// The PC build compiles src/ov021/ov021_021c6400.c (the field's 3D objects, pokebw2's field_g3dobj.c) through this file
// (tools/port_sources.py): Smooth motion's tags (libntr's simulator/sim_g3tag.h) on the field's 3D objects: the effects drawn as
// models (the dust cloud of src/ov021/fldeff_kemuri.c, the Surf Pokemon, splashes) and the field actors that are models
// (src/ov021/fldmmdl_g3dobj.c). Each object func_ov021_021c64e0 (all of them) or FieldG3DObjSystem_DrawObj (one, for its effect)
// draws is tagged right before it (kind 5 effect: owner its resource group, inst the object, state 0); both end with the empty
// tag; FieldG3DObjSystem_AddObj gives the new object in a slot a new id. Off (the default) each site is one flag test and the
// command stream is unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1) (Smooth motion on) turns them on. The DS build never
// sees this file.
#pragma push_macro("func_ov021_021c64e0")
#pragma push_macro("FieldG3DObjSystem_DrawObj")
#pragma push_macro("FieldG3DObjSystem_AddObj")
#define func_ov021_021c64e0 func_ov021_021c64e0_game
#define FieldG3DObjSystem_DrawObj FieldG3DObjSystem_DrawObj_game
#define FieldG3DObjSystem_AddObj FieldG3DObjSystem_AddObj_game
#include "types.h"   // the game file's headers, before the hooks below (their include guards keep them out of them)
#include "field/field_g3dobj.h"
#include "gfl/g3d.h"
#include "nitro/mi.h"
#include "struct_decls.h"
#include "system/palanm.h"
#include "nitro/fx/fx_mtx.h"
#include <simulator/sim_g3tag.h>

static void PortFldObj_Tag(const FieldG3DObjSystem *sys, const FieldG3DObj *obj);

// GFL_G3DSysDrawObj and _BBoxCull are called only in func_ov021_021c64e0 and FieldG3DObjSystem_DrawObj (their sys and obj)
// White's build renames both (-include w_rename.h): the real calls sit in helpers that expand the names before the hook macros replace
// the renames
#define PORT_REAL_DrawObj GFL_G3DSysDrawObj
#define PORT_REAL_BBoxCull GFL_G3DSysDrawObjBBoxCull
static inline void PortFldObj_DrawObj(const FieldG3DObjSystem *sys, const FieldG3DObj *obj, G3DActor *actor, const SRTMatrix *srt) {
    if (SIM_G3TagOn) PortFldObj_Tag(sys, obj);
    PORT_REAL_DrawObj(actor, srt);
}
static inline BOOL PortFldObj_BBoxCull(const FieldG3DObjSystem *sys, const FieldG3DObj *obj, G3DActor *actor, SRTMatrix *srt) {
    if (SIM_G3TagOn) PortFldObj_Tag(sys, obj);
    return PORT_REAL_BBoxCull(actor, srt);
}
#define GFL_G3DSysDrawObj(actor, srt) PortFldObj_DrawObj(sys, obj, actor, srt)
#define GFL_G3DSysDrawObjBBoxCull(actor, srt) PortFldObj_BBoxCull(sys, obj, actor, srt)
#include "../../../../src/ov021/ov021_021c6400.c"
#undef GFL_G3DSysDrawObj
#undef GFL_G3DSysDrawObjBBoxCull
#pragma pop_macro("func_ov021_021c64e0")
#pragma pop_macro("FieldG3DObjSystem_DrawObj")
#pragma pop_macro("FieldG3DObjSystem_AddObj")

static void PortFldObj_Tag(const FieldG3DObjSystem *sys, const FieldG3DObj *obj) {
    SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_EFFECT, &sys->resGroups[obj->resGroupIdx]), SIM_G3TagId(obj), 0);
}

void func_ov021_021c64e0(FieldG3DObjSystem *sys) {
    func_ov021_021c64e0_game(sys);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

void FieldG3DObjSystem_DrawObj(FieldG3DObjSystem *sys, u16 objIdx) {
    FieldG3DObjSystem_DrawObj_game(sys, objIdx);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

u16 FieldG3DObjSystem_AddObj(FieldG3DObjSystem *sys, u16 resGroupIdx, u16 modelIdxInRes, VecFx32 *pos) {
    // the game takes the first free slot and returns 0 also when none is free
    BOOL free0 = sys->objCount != 0 && sys->objs[0].state == 0 && sys->objs[0].actor == NULL;
    u16 i = FieldG3DObjSystem_AddObj_game(sys, resGroupIdx, modelIdxInRes, pos);

    if (i != 0 || free0) {
        SIM_G3TagDrop(&sys->objs[i]);
    }
    return i;
}
