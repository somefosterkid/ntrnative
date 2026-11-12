// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov105/gimmick_league_statue.c, the same code in
// Black 2/White 2's overlay 105. Black's flags are two higher and its archive is 0x9d, not 0x9b.
#include "types.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "field/gimmick_league_statue.h"
#include "gfl/g3d.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "save/event_work.h"
#include "system/game_data.h"
#include "system/game_system.h"

typedef struct {
    u16 heapId;
} GimmickWork;

void func_ov049_021f3714(GimmickWork *work, Field *field);
void func_ov049_021f3864(GimmickWork *work, Field *field);
void func_ov049_021f3868(GimmickWork *work, Field *field);

static const G3DSceneAnimationSetup sAnimations01[] = { { 1, 0 } };
static const G3DSceneAnimationSetup sAnimations08[] = { { 8, 0 }, { 9, 0 } };
static const G3DSceneAnimationSetup sAnimations03[] = { { 3, 0 } };
static const G3DSceneAnimationSetup sAnimations05[] = { { 5, 0 }, { 6, 0 } };
static const G3DSceneAnimationSetup sAnimations0B[] = { { 11, 0 }, { 12, 0 } };
static const G3DSceneAnimationSetup sAnimations0E[] = { { 14, 0 }, { 15, 0 } };

static const G3DSceneActorSetup sActors[6] = {
    { 0, 0, 0, 0, sAnimations01, NELEMS(sAnimations01) },
    { 2, 0, 2, 0, sAnimations03, NELEMS(sAnimations03) },
    { 4, 0, 4, 0, sAnimations05, NELEMS(sAnimations05) },
    { 7, 0, 7, 0, sAnimations08, NELEMS(sAnimations08) },
    { 10, 0, 10, 0, sAnimations0B, NELEMS(sAnimations0B) },
    { 13, 0, 13, 0, sAnimations0E, NELEMS(sAnimations0E) },
};

static const G3DSceneResourceSetup sResources[] = {
#include "asset/gimmick_league_statue_sResources.inc"
};

static const G3DSceneSetup sSceneSetup = { sResources, NELEMS(sResources), sActors, NELEMS(sActors) };

// Where each actor is
static const VecFx32 sActorPositions[6] = {
    { FX32_CONST(512), 0, FX32_CONST(672) },
    { FX32_CONST(512), 0, FX32_CONST(672) },
    { FX32_CONST(544), 0, FX32_CONST(704) },
    { FX32_CONST(448), 0, FX32_CONST(624) },
    { FX32_CONST(448), 0, FX32_CONST(704) },
    { FX32_CONST(544), 0, FX32_CONST(624) },
};

void func_ov049_021f36c0(Field *field) {
    u16 heapId;
    GimmickWork *work;

    heapId = Field_GetHeapID(field);
    work = Field_AllocGimmickWorkBlock(field, 0, heapId, sizeof(GimmickWork));
    work->heapId = heapId;
    func_ov049_021f3714(work, field);
    func_ov049_021f3864(work, field);
}

void func_ov049_021f36ec(Field *field) {
    GimmickWork *work;

    work = Field_GetGimmickWorkBlock(field, 0);
    func_ov049_021f3868(work, field);
    Field_DeleteGimmickWorkBlock(field, 0);
}

void func_ov049_021f3708(Field *field) {
    FieldExpObj_StepAllAnimations(Field_GetExpObjSystem(field));
}

void func_ov049_021f3714(GimmickWork *work, Field *field) {
    EventWork *eventWork;
    FieldExpObjSystem *system;
    SRTMatrix *matrix;
    s32 flags[4];
    s32 i;

    eventWork = GameData_GetEventWork(GSYS_GetGameData(Field_GetGameSystem(field)));
    flags[0] = EventWork_FlagGet(eventWork, 0x96b);
    flags[1] = EventWork_FlagGet(eventWork, 0x96a);
    flags[2] = EventWork_FlagGet(eventWork, 0x969);
    flags[3] = EventWork_FlagGet(eventWork, 0x96c);
    system = Field_GetExpObjSystem(field);
    LoadFieldExpandObjData(system, &sSceneSetup, 0);
    for (i = 0; i < 6; i++) {
        matrix = FieldExpObj_GetActorMatrixPtr(system, 0, i);
        matrix->translation.x = sActorPositions[i].x;
        matrix->translation.y = sActorPositions[i].y;
        matrix->translation.z = sActorPositions[i].z;
    }
    if (flags[0] && flags[1] && flags[2] && flags[3]) {
        FieldExpObj_SetAnm(system, 0, 0, 0, TRUE);
        FieldExpObj_SetAnm(system, 0, 1, 0, TRUE);
    }
    if (flags[0]) {
        FieldExpObj_SetAnm(system, 0, 2, 1, TRUE);
    } else {
        FieldExpObj_SetAnm(system, 0, 2, 0, TRUE);
    }
    if (flags[1]) {
        FieldExpObj_SetAnm(system, 0, 3, 1, TRUE);
    } else {
        FieldExpObj_SetAnm(system, 0, 3, 0, TRUE);
    }
    if (flags[2]) {
        FieldExpObj_SetAnm(system, 0, 4, 1, TRUE);
    } else {
        FieldExpObj_SetAnm(system, 0, 4, 0, TRUE);
    }
    if (flags[3]) {
        FieldExpObj_SetAnm(system, 0, 5, 1, TRUE);
    } else {
        FieldExpObj_SetAnm(system, 0, 5, 0, TRUE);
    }
}

void func_ov049_021f3864(GimmickWork *work, Field *field) {
}

void func_ov049_021f3868(GimmickWork *work, Field *field) {
}
