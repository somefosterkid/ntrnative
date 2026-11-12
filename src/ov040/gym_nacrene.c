// Nacrene City's gym (gimmick 3, overlay 40): the bookshelf that opens. pokebw2's gym_nacrene.c (overlay 93) has the
// same work block 1 and gimmick state slot 3, but Black 2's gym has no scene: Black's adds one actor and its event.

#include "types.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "struct_decls.h"
#include "field/field_map.h"
#include "system/game_data.h"
#include "system/game_system.h"

static GameEventReturnCode func_ov040_021f381c(GameEvent *event, u32 *state, void *data);

// The tables are defined at the end, after the code: GymNacrene_Init loads the position's two words from the ROM (a
// table it has seen defined is folded into immediates), and built with -ipa function (config/compiler_flags.toml) the
// tables are laid out in the order they are defined.
static const fx32 data_ov040_021f38d4[2];
static const G3DSceneSetup data_ov040_021f38ec;
static const G3DSceneResourceSetup data_ov040_021f38fc[2];

void GymNacrene_Init(Field *field) {
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    u8 *data;
    FieldExpObjAnm *anm;
    VecFx32 pos;

    Field_AllocGimmickWorkBlock(field, 1, Field_GetHeapID(field), 4);
    Field_GetGimmickWorkBlock(field, 1);
    FieldExpObj_AddScene(system, &data_ov040_021f38ec, 0);
    data = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), 3);
    pos.x = data_ov040_021f38d4[0];
    pos.y = 0;
    pos.z = data_ov040_021f38d4[1];
    FieldExpObj_GetActorMatrixPtr(system, 0, 0)->translation = pos;
    anm = FieldExpObj_GetAnmInfo(system, 0, 0, 0);
    FieldExpObjAnm_SetPaused(anm, 1);
    FieldExpObjAnm_SetLooped(anm, 0);
    if (data[4] != 0) {
        FieldExpObj_SetAnm(system, 0, 0, 0, TRUE);
        FieldExpObj_SetAnmFrame(system, 0, 0, 0, func_ov021_021be748(anm));
    }
}

void GymNacrene_End(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    Field_DeleteGimmickWorkBlock(param_1, 1);
    FieldExpObj_FreeScene(system, 0);
}


void GymNacrene_Update(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    FieldExpObj_StepAllAnimations(system);
}

void func_ov040_021f37a4(Field *param_1) {
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    u32 *userData;

    Field_GetGimmickWorkBlock(param_1, 1);
    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    userData = GimmickState_GetUserData(gimmick, 3);
    *userData = 1;
}

u32 func_ov040_021f37c8(Field *param_1) {
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    u32 *userData;

    Field_GetGimmickWorkBlock(param_1, 1);
    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    userData = GimmickState_GetUserData(gimmick, 3);
    return *userData;
}


GameEvent *func_ov040_021f37ec(GameSystem *gsys, u32 a1) {
    u16 *work = Field_GetGimmickWorkBlock(GSYS_GetField(gsys), 1);
    GameEvent *event;

    if (a1 >= 1) {
        return NULL;
    }
    event = GameEvent_Create(gsys, NULL, func_ov040_021f381c, 0);
    *work = a1;
    return event;
}

static GameEventReturnCode func_ov040_021f381c(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    Field *field = GSYS_GetField(gsys);
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    u16 *work = Field_GetGimmickWorkBlock(field, 1);
    GameData *gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    GimmickState *gimmick = GameData_GetGimmickState(gameData);
    u8 *userData = GimmickState_GetUserData(gimmick, 3);
    FieldExpObjAnm *anm;

    switch (*state) {
    case 0: {
        u8 actor = *work;

        FieldExpObj_SetAnm(system, 0, actor, 0, 1);
        anm = FieldExpObj_GetAnmInfo(system, 0, actor, 0);
        FieldExpObjAnm_SetPaused(anm, 0);
        FieldExpObj_SetAnmFrame(system, 0, actor, 0, 0);
        (*state)++;
        break;
    }
    case 1: {
        u8 actor = *work;

        anm = FieldExpObj_GetAnmInfo(system, 0, actor, 0);
        if (FieldExpObjAnm_IsPlaybackFinished(anm) && FieldExpObjAnm_IsPlaybackFinished(anm)) {
            (userData + *work)[4] = 1;
            return 1;
        }
        break;
    }
    }
    return 0;
}

// The bookshelf's one animation, its position (x, z), its actor, the scene and its two resources
static const G3DSceneAnimationSetup data_ov040_021f38d0[1] = { { 1, 0 } };
static const fx32 data_ov040_021f38d4[2] = { FX32_CONST(208), FX32_CONST(152) };
static const G3DSceneActorSetup data_ov040_021f38dc[1] = { { 0, 0, 0, 0, data_ov040_021f38d0, 1 } };
static const G3DSceneSetup data_ov040_021f38ec = { data_ov040_021f38fc, 2, data_ov040_021f38dc, 1 };
static const G3DSceneResourceSetup data_ov040_021f38fc[2] = {
    { 110, 0, 0, G3D_SCENE_RES_ARCSYS },
    { 110, 1, 0, G3D_SCENE_RES_ARCSYS },
};
