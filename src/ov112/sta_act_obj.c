// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/sta_act_obj.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/sta_act_obj.h"
#include "gfl/blact.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

// Overlay 209's sta_act_obj.c: the stage's objects, billboard actors placed in pixels and scrolled with the stage

static void func_ov112_021f6a80(StaActObjSys *sys, StaActObj *obj);

// pokebw2's STA_ACT_OBJ_SHOW
static const BOOL data_ov112_021f8328 = TRUE;

// pokebw2's StaActObj_InitSystem
StaActObjSys *func_ov112_021f69e4(HeapID heapId, BlActScene *blact) {
    u8 i;
    StaActObjSys *sys = GFL_HeapAllocate(heapId, sizeof(StaActObjSys), FALSE, "sta_act_obj.c", 79);

    sys->heapId = heapId;
    sys->blact = blact;
    sys->scroll = 0;
    for (i = 0; i < 5; i++) {
        sys->objs[i].active = FALSE;
        sys->objs[i].update = FALSE;
    }
    return sys;
}

// pokebw2's StaActObj_TermSystem
void func_ov112_021f6a24(StaActObjSys *sys) {
    u8 i;

    for (i = 0; i < 5; i++) {
        if (sys->objs[i].active == TRUE) {
            func_ov112_021f6b80(sys, &sys->objs[i]);
        }
    }
    GFL_HeapFree(sys);
}

// pokebw2's StaActObj_UpdateSystem
void func_ov112_021f6a58(StaActObjSys *sys) {
    int i;

    for (i = 0; i < 5; i++) {
        if (sys->objs[i].active == TRUE) {
            func_ov112_021f6a80(sys, &sys->objs[i]);
        }
    }
}

// pokebw2's StaActObj_UpdateObj
static void func_ov112_021f6a80(StaActObjSys *sys, StaActObj *obj) {
    if (obj->update == TRUE) {
        VecFx32 pos;
        fx32 scroll = FX32_CONST(sys->scroll);

        pos.x = (obj->pos.x - scroll) / 16;
        pos.y = (FX32_CONST(192) - obj->pos.y) / 16;
        pos.z = obj->pos.z;
        BlActScene_SetActorPos(sys->blact, obj->actor, &pos);
        obj->update = FALSE;
    }
}

// pokebw2's StaActObj_SetScrollOffset
void func_ov112_021f6af0(StaActObjSys *sys, u16 scroll) {
    u8 i;

    sys->scroll = scroll;
    for (i = 0; i < 5; i++) {
        if (sys->objs[i].active == TRUE) {
            sys->objs[i].update = TRUE;
        }
    }
}

// pokebw2's StaActObj_AddObj
StaActObj *func_ov112_021f6b18(StaActObjSys *sys, u16 objId) {
    u8 i;
    StaActObj *obj;

    for (i = 0; i < 5; i++) {
        if (sys->objs[i].active == FALSE) {
            break;
        }
    }
    obj = &sys->objs[i];
    sys->objs[i].active = TRUE;
    obj->update = TRUE;
    obj->pos.x = 0;
    obj->pos.y = 0;
    obj->pos.z = 0;
    obj->actor = BlActScene_AddNewActor(sys->blact, obj->material, FX32_ONE, FX32_ONE, &obj->pos, 31, 0, 0);
    BlActScene_SetActorHidden(sys->blact, obj->actor, &data_ov112_021f8328);
    return obj;
}

// pokebw2's StaActObj_DelObj
void func_ov112_021f6b80(StaActObjSys *sys, StaActObj *obj) {
    BlActScene_ClearActorMaterial(sys->blact, obj->actor);
    BlActScene_FreeMaterial(sys->blact, obj->material);
    obj->active = FALSE;
}

// pokebw2's StaActObj_SetPosition
void func_ov112_021f6b9c(StaActObjSys *sys, StaActObj *obj, VecFx32 *pos) {
    obj->pos.x = pos->x;
    obj->pos.y = pos->y;
    obj->pos.z = pos->z;
    obj->update = TRUE;
}

// pokebw2's StaActObj_GetPosition
void func_ov112_021f6bb0(StaActObjSys *sys, StaActObj *obj, VecFx32 *pos) {
    pos->x = obj->pos.x;
    pos->y = obj->pos.y;
    pos->z = obj->pos.z;
}

// pokebw2's StaActObj_SetShowFlg
void func_ov112_021f6bc0(StaActObjSys *sys, StaActObj *obj, BOOL show) {
    BlActScene_SetActorHidden(sys->blact, obj->actor, &show);
}
