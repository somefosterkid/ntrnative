// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/field_effect.c, ported by tools/port_file.py.
// The field effects: the effects that a map has loaded, by ID, and the tasks that play them. The name is the ROM's
// own, from GFL_HeapAllocate's file argument. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/field_effect.h"
#include "field/field.h"
#include "field/field_map.h"
#include "gfl/arc.h"
#include "gfl/blact.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nnsys/g3d.h"
#include "system/area_data.h"
#include "system/palanm.h"
#include "system/season.h"

struct FieldEffectTask {
    BOOL active;
    u32 param1;
    void *param2;
    VecFx32 pos;
    u8 work[0xa0];
    FieldEffectTaskVTable vtable;
    FieldEffectTaskStore *store;
    TCB *tcb;
};

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
AreaData *Field_GetAreaData(Field *field);

// What an effect loads and frees
// Black 2's, not checked against Black
typedef struct {
    u32 id;
    void *(*create)(FieldEffects *effects, HeapID heapId);
    void (*free)(FieldEffects *effects, void *data);
} FieldEffectVTable;

// A loaded effect
typedef struct {
    u32 id;
    void *data;
} FieldEffectHandle;

// Black 2's; twinned code confirms the offsets of count, tasks and tcbMgr in Black
struct FieldEffectTaskStore {
    u32 count;
    u16 heapId;
    FieldEffectTask *tasks;
    TCBManager *tcbMgr;
};

// Black 2's; twinned code confirms the offsets of count, arc, handles, season, isExterior, tcbMgrWork, tcbMgr,
// taskStore and luminanceTable in Black
struct FieldEffects {
    int count;
    u16 heapId;
    Field *field;
    ArcTool *arc;
    FieldEffectHandle *handles;
    u16 taskCount;
    u8 season;
    u8 isExterior;
    void *tcbMgrWork;
    TCBManager *tcbMgr;
    FieldEffectTaskStore *taskStore;
    const u8 *luminanceTable;
};

static void FieldEffects_InitEffectBuffer(FieldEffects *effects);
static void FieldEffects_FreeEffectBuffer(FieldEffects *effects);
static FieldEffectHandle *FieldEffects_GetHandleByID(FieldEffects *effects, u32 id);
static const FieldEffectVTable *FieldEffects_GetVTable(u32 id);
static void func_ov021_021abfc0(FieldEffects *effects);
static void FieldEffects_TCBManagerUpdate(FieldEffects *effects);
static void func_ov021_021abfec(FieldEffects *effects);
static FieldEffectTaskStore *FieldEffectTCBStore_Create(HeapID heapId, TCBManager *tcbMgr, u32 count);
static void func_ov021_021ac03c(FieldEffectTaskStore *store);
static void func_ov021_021ac074(FieldEffectTaskStore *store);
static FieldEffectTask *FieldTerrainEffectTCB_Create(FieldEffectTaskStore *store, const FieldEffectTaskVTable *vtable,
                                                     const VecFx32 *pos, u32 param1, void *param2, u32 priority);
static void func_ov021_021ac134(TCB *tcb, void *data);
static void func_ov021_021ac140(FieldEffectTask *task);
static void func_ov021_021ac18c(FieldEffectTask *task);

// The count of data_ov021_021d8f10's effects, which fieldmap.c passes with it
const u32 data_ov021_021d8f00 = 20;

// pokebw2's FIELD_EFFECT_SCALE
static const VecFx32 data_ov021_021d8f04 = { FX32_CONST(8), FX32_CONST(8), FX32_CONST(8) };

const u32 data_ov021_021d8f10[20] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 17, 13, 12, 18, 19, 20, 21, 22, 23,
};

static const FieldEffectVTable FIELD_EFFECT_VTABLES[FLDEFF_MAX + 1] = {
    { 0, FieldEffect_Shadow_Create, FieldEffect_Shadow_Free },
    { 1, playSmokeEffect, freeSmokeEffect },
    { 2, loadGrassEffects, freeGrassEffects },
    { 3, func_ov021_021aca38, func_ov021_021aca5c },
    { 4, func_ov021_021bb484, func_ov021_021bb4a8 },
    { 5, func_ov021_021bbcbc, func_ov021_021bbcf8 },
    { 6, func_ov021_021bbf78, func_ov021_021bbf9c },
    { 7, func_ov021_021c47b4, func_ov021_021c47e8 },
    { 8, func_ov021_021c4978, func_ov021_021c49ac },
    { 9, func_ov021_021ad3d4, func_ov021_021ad3fc },
    { 10, func_ov021_021adcb8, func_ov021_021adcdc },
    { 11, func_ov021_021c8e68, func_ov021_021c8e8c },
    { 12, func_ov021_021c94bc, func_ov021_021c94e0 },
    { 13, func_ov021_021adedc, func_ov021_021adf00 },
    { 14, func_ov021_021d0140, func_ov021_021d0160 },
    { 15, func_ov021_021d038c, func_ov021_021d03b0 },
    { 16, func_ov021_021d0610, func_ov021_021d0634 },
    { 17, func_ov021_021c4b28, func_ov021_021c4b50 },
    { 18, func_ov021_021ad758, func_ov021_021ad7a0 },
    { 19, func_ov021_021ad764, func_ov021_021ad7a0 },
    { 20, func_ov021_021ad770, func_ov021_021ad7a0 },
    { 21, func_ov021_021ad77c, func_ov021_021ad7a0 },
    { 22, func_ov021_021ad788, func_ov021_021ad7a0 },
    { 23, func_ov021_021ad794, func_ov021_021ad7a0 },
    { FLDEFF_NONE, NULL, NULL },
};

u16 ZoneData_GetAreaID(u16 zoneId);
AreaData *func_ov021_02188d58(Field *field);
FieldEffects *FieldEffects_Create(Field *field, int count, HeapID heapId) {
    VecFx32 scale;
    u8 polyId;
    BlActScene *scene;
    FieldEffects *effects = GFL_HeapAllocate(heapId, sizeof(FieldEffects), TRUE, "field_effect.c", 126);

    effects->count = count;
    effects->heapId = heapId;
    effects->field = field;
    effects->arc = GFL_ArcSysCreateFileHandle(75, heapId);
    ZoneData_GetAreaID(Field_GetPlayerStateZoneID(field));
    effects->season = GameSystem_GetSeason(Field_GetGameSystem(field));
    effects->isExterior = AreaData_IsExterior(func_ov021_02188d58(field));
    FieldEffects_InitEffectBuffer(effects);
    scene = BlActSys_GetScene(Field_GetEffectBlAct(effects->field));
    BlActScene_SetGeomOrigin(scene, 0);
    scale = data_ov021_021d8f04;
    BlActScene_SetScale(scene, &scale);
    polyId = 0;
    BlActScene_SetBasePolyID(scene, &polyId);
    return effects;
}

void FieldEffects_Free(FieldEffects *effects) {
    func_ov021_021abfc0(effects);
    FieldEffects_FreeEffectBuffer(effects);
    GFL_ArcToolFree(effects->arc);
    GFL_HeapFree(effects);
}

void FieldEffects_Update(FieldEffects *effects) {
    FieldEffects_TCBManagerUpdate(effects);
}

void FieldEffects_Draw(FieldEffects *effects) {
    func_ov021_021abfec(effects);
}

Field *FieldEffects_GetField(FieldEffects *effects) {
    return effects->field;
}

struct FieldG3DObjSystem *func_ov021_021abe18(FieldEffects *effects) {
    return func_ov021_02188cd8(effects->field);
}

ArcTool *FieldEffects_GetArc(FieldEffects *effects) {
    return effects->arc;
}

u32 FieldEffects_GetSeason(FieldEffects *effects) {
    return effects->season;
}

BOOL FieldEffects_GetAreaIsExterior(FieldEffects *effects) {
    return effects->isExterior;
}

void FieldEffects_Load(FieldEffects *effects, const u32 *ids, u32 count) {
    FieldEffectHandle *handle;
    const FieldEffectVTable *vtable;

    while (count != 0) {
        handle = FieldEffects_GetHandleByID(effects, FLDEFF_NONE);
        vtable = FieldEffects_GetVTable(*ids);
        handle->id = *ids;
        handle->data = vtable->create(effects, effects->heapId);
        ids++;
        count--;
    }
}

void FieldEffects_FreeEffect(s32 *param_1, s32 param_2) {
    FieldEffects *effects = (FieldEffects *)param_1;
    u32 id = (u32)param_2;
    FieldEffectHandle *handle = FieldEffects_GetHandleByID(effects, id);

    if (handle != NULL) {
        FieldEffects_GetVTable(id)->free(effects, handle->data);
        handle->id = FLDEFF_NONE;
        handle->data = NULL;
    }
}


BOOL FieldEffects_IsLoaded(FieldEffects *effects, u32 id) {
    if (FieldEffects_GetHandleByID(effects, id) != NULL) {
        return TRUE;
    }
    return FALSE;
}

void *FieldEffects_GetHandleData(FieldEffects *effects, u32 id) {
    return FieldEffects_GetHandleByID(effects, id)->data;
}

static void FieldEffects_InitEffectBuffer(FieldEffects *effects)
{
    int i;
    FieldEffectHandle *handle;

    handle = GFL_HeapAllocate(effects->heapId, effects->count * sizeof(FieldEffectHandle), TRUE, "field_effect.c",
                              372);
    i = 0;
    effects->handles = handle;
    for (; i < effects->count; i++, handle++) {
        handle->id = FLDEFF_NONE;
    }
}

static void FieldEffects_FreeEffectBuffer(FieldEffects *effects) {
    int i;
    FieldEffectHandle *handle = effects->handles;

    for (i = 0; i < effects->count; i++, handle++) {
        if (handle->id != FLDEFF_NONE) {
            FieldEffects_GetVTable(handle->id)->free(effects, handle->data);
            handle->id = FLDEFF_NONE;
            handle->data = NULL;
        }
    }
    GFL_HeapFree(effects->handles);
    effects->handles = NULL;
}

static FieldEffectHandle *FieldEffects_GetHandleByID(FieldEffects *effects, u32 id) {
    u32 count = effects->count;
    FieldEffectHandle *handle = effects->handles;

    for (; count != 0; count--, handle++) {
        if (handle->id == id) {
            return handle;
        }
    }
    return NULL;
}

// The effect's functions, or the empty last entry for an unknown ID
static const FieldEffectVTable *FieldEffects_GetVTable(u32 id) {
    int i;
    const FieldEffectVTable *vtable = FIELD_EFFECT_VTABLES;

    for (i = 0; i < FLDEFF_MAX; i++, vtable++) {
        if (vtable->id == id) {
            return vtable;
        }
    }
    return vtable;
}

void FieldEffects_TCBManagerInit(FieldEffects *effects, u32 count) {
    effects->taskCount = count;
    effects->tcbMgrWork = GFL_HeapAllocate(effects->heapId, GFL_TCBMgrCalcAllocSize(count), FALSE,
                                           "field_effect.c", 475);
    effects->tcbMgr = GFL_TCBMgrCreate(count, effects->tcbMgrWork);
    effects->taskStore = FieldEffectTCBStore_Create(effects->heapId, effects->tcbMgr, count);
}

FieldEffectTask *FieldEffects_TCBCreate(FieldEffects *effects, const FieldEffectTaskVTable *vtable,
                                        const VecFx32 *pos, u32 param1, void *param2, u32 priority) {
    return FieldTerrainEffectTCB_Create(effects->taskStore, vtable, pos, param1, param2, priority);
}

static void func_ov021_021abfc0(FieldEffects *effects) {
    if (effects->taskStore != NULL) {
        func_ov021_021ac03c(effects->taskStore);
        func_02030da4(effects->tcbMgr);
        GFL_HeapFree(effects->tcbMgrWork);
    }
}

static void FieldEffects_TCBManagerUpdate(FieldEffects *effects) {
    if (effects->tcbMgr != NULL) {
        GFL_TCBMgrUpdate(effects->tcbMgr);
    }
}

static void func_ov021_021abfec(FieldEffects *effects) {
    if (effects->taskStore != NULL) {
        func_ov021_021ac074(effects->taskStore);
    }
}

static FieldEffectTaskStore *FieldEffectTCBStore_Create(HeapID heapId, TCBManager *tcbMgr, u32 count) {
    FieldEffectTaskStore *store = GFL_HeapAllocate(heapId, sizeof(FieldEffectTaskStore), TRUE, "field_effect.c", 570);

    store->count = count;
    store->heapId = heapId;
    store->tasks = GFL_HeapAllocate(heapId, count * sizeof(FieldEffectTask), TRUE, "field_effect.c", 574);
    store->tcbMgr = tcbMgr;
    return store;
}

static void func_ov021_021ac03c(FieldEffectTaskStore *store) {
    u32 i;

    for (i = 0; i < store->count; i++) {
        if (store->tasks[i].active) {
            func_ov021_021ac154(&store->tasks[i]);
        }
    }
    GFL_HeapFree(store->tasks);
    GFL_HeapFree(store);
}

static void func_ov021_021ac074(FieldEffectTaskStore *store) {
    u32 i;

    for (i = 0; i < store->count; i++) {
        if (store->tasks[i].active) {
            func_ov021_021ac18c(&store->tasks[i]);
        }
    }
}

static FieldEffectTask *FieldTerrainEffectTCB_Create(FieldEffectTaskStore *store, const FieldEffectTaskVTable *vtable,
                                                     const VecFx32 *pos, u32 param1, void *param2, u32 priority) {
    u32 i;
    FieldEffectTask *task = store->tasks;

    i = 0;
    do {
        if (!task->active) {
            MI_CpuFill8(task, 0, sizeof(FieldEffectTask));
            task->active = TRUE;
            task->vtable = *vtable;
            task->param1 = param1;
            task->param2 = param2;
            if (pos != NULL) {
                task->pos = *pos;
            }
            task->store = store;
            task->tcb = GFL_TCBMgrAddTask(store->tcbMgr, func_ov021_021ac134, task, priority);
            break;
        }
        i++;
        task++;
    } while (i < store->count);
    if (i < store->count) {
        func_ov021_021ac140(task);
        return task;
    }
    return NULL;
}

static void func_ov021_021ac134(TCB *tcb, void *data) {
    func_ov021_021ac178(data);
}

static void func_ov021_021ac140(FieldEffectTask *task) {
    if (task != NULL) {
        task->vtable.init(task, task->work);
    }
}

void func_ov021_021ac154(FieldEffectTask *task) {
    if (task != NULL) {
        task->vtable.delete(task, task->work);
        GFL_TCBRemove(task->tcb);
        task->active = FALSE;
    }
}

void func_ov021_021ac178(FieldEffectTask *task) {
    if (task != NULL) {
        task->vtable.update(task, task->work);
    }
}

static void func_ov021_021ac18c(FieldEffectTask *task) {
    if (task != NULL) {
        task->vtable.draw(task, task->work);
    }
}

u32 func_ov021_021ac1a0(FieldEffectTask *task) {
    if (task != NULL) {
        return task->param1;
    }
    return 0;
}

void *func_ov021_021ac1ac(FieldEffectTask *task) {
    if (task != NULL) {
        return task->param2;
    }
    return NULL;
}

void func_ov021_021ac1b8(FieldEffectTask *task, VecFx32 *pos) {
    if (task != NULL) {
        *pos = task->pos;
    }
}

void func_ov021_021ac1cc(FieldEffectTask *task, const VecFx32 *pos) {
    if (task != NULL) {
        task->pos = *pos;
    }
}

void *func_ov021_021ac1e0(FieldEffectTask *task) {
    if (task != NULL) {
        return task->work;
    }
    return NULL;
}

void FieldEffects_SetLuminanceTable(FieldEffects *effects, const u8 *table) {
    effects->luminanceTable = table;
}

// Recolor the texture's palettes through the luminance table, if the map has one
void FieldEffects_ApplyLuminanceTable(FieldEffects *effects, void *resource) {
    NNSG3dResTex *tex;

    if (effects->luminanceTable != NULL) {
        tex = NNS_G3dGetTex(GFL_G3DResGetResData(resource));
        ColorFilter_ApplyLUT((u16 *)((u8 *)tex + tex->plttInfo.ofsPlttData),
                             (tex->plttInfo.sizePltt << 3) / sizeof(u16), effects->luminanceTable);
    }
}
