// Black's code at ov060 0x021f36c0-0x021f3938. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "save/high_link.h"
#include "ov060_021f36c0.h"

// The entralink comm work: the 3-bit value at 0x86 is compared with the gimmick work's first byte
typedef struct EntralinkCommWork {
    u8 unk_0[0x86];
    u16 unk_86 : 3;
} EntralinkCommWork;

EntralinkCommWork *func_ov060_021f3938(GameCommSys *comm);
void func_ov060_021f37e4(FunfestLevelsFlag *work, Field *field, GameData *gameData, EntralinkCommWork *commWork);
u8 func_ov060_021f3880(GameData *gameData, FieldExpObjSystem *system, EntralinkCommWork *commWork);
extern const G3DSceneSetup data_ov060_021f3a34;
extern const G3DSceneResourceSetup data_ov060_021f3a44[];
extern const G3DSceneActorSetup data_ov060_021f3a80[];
u64 _s32_div_f(u32 param_1, u32 param_2);
void func_ov060_021f3850(FieldExpObjSystem *param_1);


void func_ov060_021f36c0(Field *field) {
    u16 heapId;
    FieldExpObjSystem *system;
    GameSystem *gsys;
    GameData *gameData;
    EntralinkCommWork *commWork;
    FunfestLevelsFlag *work;

    heapId = Field_GetHeapID(field);
    system = Field_GetExpObjSystem(field);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    GameData_GetGimmickState(gameData);
    commWork = func_ov060_021f3938(GSYS_GetGameCommSystem(gsys));
    work = Field_AllocGimmickWorkBlock(field, 0, heapId, 4);
    FieldExpObj_AddScene(system, &data_ov060_021f3a34, 0);
    func_ov060_021f37e4(work, field, gameData, commWork);
    func_ov060_021f3950(work, system);
}

void func_ov060_021f3724(Field *param_1) {
    GameSystem *gsys;
    GameData *gameData;
    FieldExpObjSystem *system;

    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    GameData_GetGimmickState(gameData);
    system = Field_GetExpObjSystem(param_1);
    FieldExpObj_FreeScene(system, 0);
    Field_DeleteGimmickWorkBlock(param_1, 0);
}

void func_ov060_021f374c(Field *field) {
    GameSystem *gsys;
    GameData *gameData;
    FieldExpObjSystem *system;
    FunfestLevelsFlag *work;
    EntralinkCommWork *commWork;

    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    GameData_GetGimmickState(gameData);
    system = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 0);
    commWork = func_ov060_021f3938(GSYS_GetGameCommSystem(gsys));
    if (commWork != NULL && commWork->unk_86 != work->netId) {
        func_ov060_021f3850(system);
        work->netId = func_ov060_021f3880(gameData, system, commWork);
    }
    FieldExpObj_StepAllAnimations(system);
    if (work->active != 0) {
        func_ov060_021f39f4(work, system);
    }
}


void func_ov060_021f37b8(Field *field, VecFx32 *pos) {
    FunfestLevelsFlag *work;
    FieldExpObjSystem *system;

    work = Field_GetGimmickWorkBlock(field, 0);
    system = Field_GetExpObjSystem(field);
    func_ov060_021f399c(work, system, pos);
}

u8 func_ov060_021f37d8(Field *field) {
    FunfestLevelsFlag *work = Field_GetGimmickWorkBlock(field, 0);

    return work->active;
}

void func_ov060_021f37e4(FunfestLevelsFlag *work, Field *field, GameData *gameData, EntralinkCommWork *commWork) {
    FieldExpObjSystem *system;
    s32 i;

    system = Field_GetExpObjSystem(field);
    i = 0;
    do {
        SRTMatrix *matrix = FieldExpObj_GetActorMatrixPtr(system, 0, (u16)i);
        matrix->translation.x = FX32_CONST(504);
        matrix->translation.y = FX32_CONST(32);
        matrix->translation.z = FX32_CONST(440);
        func_ov021_021be4a4(system, 0, (u16)i, TRUE);
        FieldExpObj_SetActorHidden(system, 0, (u16)i, TRUE);
        i++;
    } while (i < 0x15);
    work->netId = func_ov060_021f3880(gameData, system, commWork);
}


void func_ov060_021f3850(FieldExpObjSystem *param_1) {
    s32 actor;

    actor = 0;
    do {
        FieldExpObj_SetActorHidden(param_1, 0, (u16)actor, 1);
        actor = actor + 1;
    } while (actor < 0x14);
}

s32 func_ov060_021f3870(u32 param_1) {
    s32 result;

    result = (s32)_s32_div_f(param_1, 3);
    if (9 < result) {
        result = 9;
    }
    return result;
}

u8 func_ov060_021f3880(GameData *gameData, FieldExpObjSystem *system, EntralinkCommWork *commWork) {
    u32 netId;
    s32 offset;
    PassPowerLevel *levels;
    s32 whiteSteps;
    s32 blackSteps;
    SRTMatrix *matrix;
    fx32 x;

    netId = func_02012b80(gameData);
    offset = netId - func_02012b58(gameData);
    if (offset < 0) {
        offset += GameData_GetLastSubscreen(gameData);
    }
    if (netId == func_02012b58(gameData)) {
        levels = func_020127b8(gameData);
    } else {
        levels = func_020127ac(gameData, netId);
    }
    whiteSteps = func_ov060_021f3870(func_0200bdf8(levels));
    blackSteps = func_ov060_021f3870(func_0200bdfc(levels));
    matrix = FieldExpObj_GetActorMatrixPtr(system, 0, whiteSteps + 10);
    x = offset * FX32_CONST(1024) + FX32_CONST(504);
    matrix->translation.x = x;
    matrix->translation.y = FX32_CONST(32);
    matrix->translation.z = FX32_CONST(440);
    matrix = FieldExpObj_GetActorMatrixPtr(system, 0, blackSteps);
    matrix->translation.x = x;
    matrix->translation.y = FX32_CONST(32);
    matrix->translation.z = FX32_CONST(440);
    FieldExpObj_SetActorHidden(system, 0, whiteSteps + 10, FALSE);
    FieldExpObj_SetActorHidden(system, 0, blackSteps, FALSE);
    return netId;
}


const G3DSceneAnimationSetup data_ov060_021f3a28[3] = {
    { 2, 0 },
    { 3, 0 },
    { 4, 0 },
};

const G3DSceneSetup data_ov060_021f3a34 = {
    data_ov060_021f3a44,
    5,
    data_ov060_021f3a80,
    21,
};

const G3DSceneResourceSetup data_ov060_021f3a44[5] = {
    { 0xcf, 0, 0, 0 },
    { 0xd2, 5, 0, 0 },
    { 0xd2, 3, 0, 0 },
    { 0xd2, 6, 0, 0 },
    { 0xd2, 4, 0, 0 },
};

const G3DSceneActorSetup data_ov060_021f3a80[21] = {
    { 0, 0, 0, 0, NULL, 0 },
    { 0, 1, 0, 0, NULL, 0 },
    { 0, 2, 0, 0, NULL, 0 },
    { 0, 3, 0, 0, NULL, 0 },
    { 0, 4, 0, 0, NULL, 0 },
    { 0, 5, 0, 0, NULL, 0 },
    { 0, 6, 0, 0, NULL, 0 },
    { 0, 7, 0, 0, NULL, 0 },
    { 0, 8, 0, 0, NULL, 0 },
    { 0, 9, 0, 0, NULL, 0 },
    { 0, 10, 0, 0, NULL, 0 },
    { 0, 11, 0, 0, NULL, 0 },
    { 0, 12, 0, 0, NULL, 0 },
    { 0, 13, 0, 0, NULL, 0 },
    { 0, 14, 0, 0, NULL, 0 },
    { 0, 15, 0, 0, NULL, 0 },
    { 0, 16, 0, 0, NULL, 0 },
    { 0, 17, 0, 0, NULL, 0 },
    { 0, 18, 0, 0, NULL, 0 },
    { 0, 19, 0, 0, NULL, 0 },
    { 1, 0, 1, 0, data_ov060_021f3a28, 3 },
};
