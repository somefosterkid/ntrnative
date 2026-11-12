// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov106/gimmick_league_lift.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "field/gimmick_league_lift.h"
#include "gfl/g3d.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

typedef struct {
    u16 heapId;
} GimmickWork;

void func_ov050_021f383c(GimmickWork *work, Field *field);
void func_ov050_021f3890(GimmickWork *work, Field *field);
void func_ov050_021f3894(GimmickWork *work, Field *field);

// pokebw2's sAnimations01
static const G3DSceneAnimationSetup data_ov050_021f3898[] = { { 1, 0 } };
// pokebw2's sAnimations03
static const G3DSceneAnimationSetup data_ov050_021f389c[] = { { 3, 0 }, { 4, 0 } };

// pokebw2's sActors
static const G3DSceneActorSetup data_ov050_021f38cc[2] = {
#include "asset/gimmick_league_lift_data_ov050_021f38cc.inc"
};

// pokebw2's sResources
static const G3DSceneResourceSetup data_ov050_021f38ec[] = {
#include "asset/gimmick_league_lift_data_ov050_021f38ec.inc"
};

// pokebw2's sSceneSetup
static const G3DSceneSetup data_ov050_021f38a4 = { data_ov050_021f38ec, NELEMS(data_ov050_021f38ec), data_ov050_021f38cc, NELEMS(data_ov050_021f38cc) };

// Where each actor is
// pokebw2's sActorPositions
static const VecFx32 data_ov050_021f38b4[2] = {
    { FX32_CONST(480), FX32_CONST(2), FX32_CONST(1296) },
    { FX32_CONST(480), FX32_CONST(2), FX32_CONST(1296) },
};

void func_ov050_021f36c0(Field *field) {
    u16 heapId;
    GimmickWork *work;

    heapId = Field_GetHeapID(field);
    work = Field_AllocGimmickWorkBlock(field, 1, heapId, sizeof(GimmickWork));
    work->heapId = heapId;
    func_ov050_021f383c(work, field);
    func_ov050_021f3890(work, field);
}

void func_ov050_021f36ec(Field *field) {
    GimmickWork *work;

    work = Field_GetGimmickWorkBlock(field, 1);
    func_ov050_021f3894(work, field);
    Field_DeleteGimmickWorkBlock(field, 1);
}

void func_ov050_021f3708(Field *field) {
    FieldExpObj_StepAllAnimations(Field_GetExpObjSystem(field));
}

void func_ov050_021f3714(Field *field) {
    FieldExpObjSystem *system;
    FieldExpObjAnm *anm;

    system = Field_GetExpObjSystem(field);
    FieldExpObj_SetAnm(system, 0, 0, 0, TRUE);
    anm = FieldExpObj_GetAnmInfo(system, 0, 0, 0);
    FieldExpObjAnm_SetLooped(anm, FALSE);
}

void func_ov050_021f3744(Field *field) {
    FieldExpObj_SetAnm(Field_GetExpObjSystem(field), 0, 0, 0, FALSE);
}

BOOL func_ov050_021f3758(Field *field) {
    s32 i;
    FieldExpObjSystem *system;
    FieldExpObjAnm *anm;

    system = Field_GetExpObjSystem(field);
    for (i = 0; i < 1; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 0, i);
        if (!FieldExpObjAnm_IsPlaybackFinished(anm)) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's func_ov106_021eed48
void func_ov050_021f3788(Field *field) {
    s32 i;
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(field);
    FieldExpObj_SetActorHidden(system, 0, 1, TRUE);
    for (i = 0; i < 2; i++) {
        FieldExpObj_SetAnm(system, 0, 1, i, FALSE);
    }
}

// pokebw2's func_ov106_021eed78
void func_ov050_021f37b8(Field *field) {
    s32 i;
    s32 j;
    FieldExpObjSystem *system;
    FieldExpObjAnm *anm;

    system = Field_GetExpObjSystem(field);
    FieldExpObj_SetActorHidden(system, 0, 1, FALSE);
    for (i = 0; i < 2; i++) {
        FieldExpObj_SetAnm(system, 0, 1, i, TRUE);
    }
    for (j = 0; j < 2; j++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 1, j);
        FieldExpObjAnm_SetLooped(anm, FALSE);
    }
}

BOOL func_ov050_021f3808(Field *field) {
    s32 i;
    FieldExpObjSystem *system;
    FieldExpObjAnm *anm;

    system = Field_GetExpObjSystem(field);
    for (i = 0; i < 2; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 1, i);
        if (!FieldExpObjAnm_IsPlaybackFinished(anm)) {
            return FALSE;
        }
    }
    return TRUE;
}

void func_ov050_021f383c(GimmickWork *work, Field *field) {
    FieldExpObjSystem *system;
    SRTMatrix *matrix;
    s32 i;

    system = Field_GetExpObjSystem(field);
    LoadFieldExpandObjData(system, &data_ov050_021f38a4, 0);
    for (i = 0; i < 2; i++) {
        matrix = FieldExpObj_GetActorMatrixPtr(system, 0, i);
        matrix->translation.x = data_ov050_021f38b4[i].x;
        matrix->translation.y = data_ov050_021f38b4[i].y;
        matrix->translation.z = data_ov050_021f38b4[i].z;
    }
    func_ov050_021f3744(field);
    func_ov050_021f3788(field);
}

void func_ov050_021f3890(GimmickWork *work, Field *field) {
}

void func_ov050_021f3894(GimmickWork *work, Field *field) {
}