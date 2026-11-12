// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/sta_act_obj.h: the lines in use
// so far.
#ifndef BWPORT_APP_MUSICAL_STA_ACT_OBJ_H
#define BWPORT_APP_MUSICAL_STA_ACT_OBJ_H

#include "types.h"
#include "gfl/blact.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

typedef struct {
    BOOL active;
    // Whether its actor needs its position again
    BOOL update;
    VecFx32 pos;
    u32 material;
    u32 actor;
} StaActObj;

struct StaActObjSys {
    HeapID heapId;
    // How far the stage has scrolled, in pixels
    u16 scroll;
    BlActScene *blact;
    StaActObj objs[5];
};

// pokebw2's StaActObj_InitSystem
StaActObjSys *func_ov112_021f69e4(HeapID heapId, BlActScene *blact);
// pokebw2's StaActObj_TermSystem
void func_ov112_021f6a24(StaActObjSys *sys);
// pokebw2's StaActObj_UpdateSystem
void func_ov112_021f6a58(StaActObjSys *sys);
// pokebw2's StaActObj_SetScrollOffset
void func_ov112_021f6af0(StaActObjSys *sys, u16 scroll);
// pokebw2's StaActObj_AddObj
// The id is not used, and the actor is made with the slot's material, which nothing in this file sets
StaActObj *func_ov112_021f6b18(StaActObjSys *sys, u16 objId);
// pokebw2's StaActObj_DelObj
void func_ov112_021f6b80(StaActObjSys *sys, StaActObj *obj);
// pokebw2's StaActObj_SetPosition
void func_ov112_021f6b9c(StaActObjSys *sys, StaActObj *obj, VecFx32 *pos);
// pokebw2's StaActObj_SetShowFlg
void func_ov112_021f6bc0(StaActObjSys *sys, StaActObj *obj, BOOL show);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's StaActObj_GetPosition
void func_ov112_021f6bb0(StaActObjSys *sys, StaActObj *obj, VecFx32 *pos);

#endif // BWPORT_APP_MUSICAL_STA_ACT_OBJ_H
