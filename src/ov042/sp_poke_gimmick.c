// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov129/sp_poke_gimmick.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The field gimmick of a special Pokémon's zone in overlay 42: one scene of expansion objects with 15 resources and 12
// actors, and the events that set its actors' animations. The gimmick's state is in work block 1, three bytes that the
// events below set and read.
#include "types.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "field/field_map.h"
#include "field/sp_poke_gimmick.h"
#include "gfl/g3d.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

#define WORK_ID 1
#define WORK_SIZE 4

// The center of a grid square, in fx32
#define GRID_CENTER(n) ((n) * FX32_CONST(16) + FX32_CONST(8))

static GameEventReturnCode func_ov042_021f38d4(GameEvent *event, u32 *state, void *data);
static GameEventReturnCode func_ov042_021f3980(GameEvent *event, u32 *state, void *data);

// The animations of the scene's actors, which the resources of the scene load
static const G3DSceneAnimationSetup data_ov042_021f3a34[1] = { { 13, 0 } };
static const G3DSceneAnimationSetup data_ov042_021f3a38[1] = { { 14, 0 } };
static const G3DSceneAnimationSetup data_ov042_021f3a3c[1] = { { 12, 0 } };
static const G3DSceneAnimationSetup data_ov042_021f3a48[2] = { { 10, 0 }, { 11, 0 } };
static const G3DSceneAnimationSetup data_ov042_021f3a50[2] = { { 8, 0 }, { 9, 0 } };
static const G3DSceneAnimationSetup data_ov042_021f3a40[2] = { { 6, 0 }, { 7, 0 } };

static const G3DSceneResourceSetup data_ov042_021f3b40[15] = {
#include "asset/sp_poke_gimmick_data_ov042_021f3b40.inc"
};

static const G3DSceneActorSetup data_ov042_021f3bf4[12] = {
    { 0, 0, 0, 0, data_ov042_021f3a40, 2 }, { 1, 0, 1, 0, data_ov042_021f3a50, 2 }, { 2, 0, 2, 0, data_ov042_021f3a48, 2 },
    { 0, 0, 0, 0, data_ov042_021f3a40, 2 }, { 1, 0, 1, 0, data_ov042_021f3a50, 2 }, { 2, 0, 2, 0, data_ov042_021f3a48, 2 },
    { 0, 0, 0, 0, data_ov042_021f3a40, 2 }, { 1, 0, 1, 0, data_ov042_021f3a50, 2 }, { 2, 0, 2, 0, data_ov042_021f3a48, 2 },
    { 3, 0, 3, 0, data_ov042_021f3a3c, 1 }, { 4, 0, 4, 0, data_ov042_021f3a34, 1 }, { 5, 0, 5, 0, data_ov042_021f3a38, 1 },
};

static const G3DSceneSetup data_ov042_021f3a58 = { data_ov042_021f3b40, 15, data_ov042_021f3bf4, 12 };

// pokebw2's SpPokeGimmickA_Init
void func_ov042_021f36c0(Field *field) {
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    GimmickState *gimmick = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    u8 *userData = GimmickState_GetUserData(gimmick, 4);
    u8 i;

    Field_AllocGimmickWorkBlock(field, WORK_ID, Field_GetHeapID(field), WORK_SIZE);
    LoadFieldExpandObjData(system, &data_ov042_021f3a58, 0);

    VecFx32 positions[9] = {
        { GRID_CENTER(12), 0, GRID_CENTER(16) }, { GRID_CENTER(15), 0, GRID_CENTER(36) },
        { GRID_CENTER(9), 0, GRID_CENTER(26) },  { GRID_CENTER(12), 0, GRID_CENTER(36) },
        { GRID_CENTER(15), 0, GRID_CENTER(26) }, { GRID_CENTER(9), 0, GRID_CENTER(36) },
        { GRID_CENTER(12), 0, GRID_CENTER(26) }, { GRID_CENTER(15), 0, GRID_CENTER(16) },
        { GRID_CENTER(9), 0, GRID_CENTER(16) },
    };
    for (i = 0; i < 9; i++) {
        u8 actor = i + 0;
        SRTMatrix *matrix = FieldExpObj_GetActorMatrixPtr(system, 0, actor);

        matrix->translation = positions[i];
        func_ov021_021be4a4(system, 0, actor, TRUE);
    }

    VecFx32 extra[9] = {
        { GRID_CENTER(12), 0, GRID_CENTER(34) },
        { GRID_CENTER(12), 0, GRID_CENTER(24) },
        { GRID_CENTER(12), 0, GRID_CENTER(14) },
    };
    for (i = 0; i < 3; i++) {
        u8 actor = i + 9;
        SRTMatrix *matrix = FieldExpObj_GetActorMatrixPtr(system, 0, actor);

        matrix->translation = extra[i];
        func_ov021_021be4a4(system, 0, actor, TRUE);
    }

    for (i = 0; i < 3; i++) {
        if (userData[i] != 0) {
            u8 actor = i + 9;
            FieldExpObjAnm *anm;
            fx32 frame;

            FieldExpObj_SetAnm(system, 0, actor, 0, TRUE);
            anm = FieldExpObj_GetAnmInfo(system, 0, actor, 0);
            FieldExpObjAnm_SetPaused(anm, FALSE);
            FieldExpObjAnm_SetLooped(anm, FALSE);
            frame = func_ov021_021be748(anm);
            FieldExpObj_SetAnmFrame(system, 0, actor, 0, frame);
        }
    }
}

// pokebw2's SpPokeGimmickA_End
void func_ov042_021f380c(Field *field) {
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);

    Field_DeleteGimmickWorkBlock(field, WORK_ID);
    FieldExpObj_FreeScene(system, 0);
}

// No twin in pokebw2's C
void func_ov042_021f3828(Field *field) {
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);

    FieldExpObj_StepAllAnimations(system);
}

// No twin in pokebw2's C
GameEvent *func_ov042_021f3834(GameSystem *gsys, u32 state, u8 flag) {
    Field *field = GSYS_GetField(gsys);
    SpPokeGimmickWork *work;
    GimmickState *gimmick;
    GameEvent *event;

    Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, WORK_ID);
    gimmick = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    GimmickState_GetUserData(gimmick, 4);
    if (state >= 9) {
        return NULL;
    }
    event = GameEvent_Create(gsys, NULL, func_ov042_021f38d4, 0);
    if (event != NULL) {
        work->unk_0 = state;
        work->unk_2 = flag;
    }
    return event;
}

// No twin in pokebw2's C
GameEvent *func_ov042_021f3888(GameSystem *gsys, u32 state) {
    Field *field = GSYS_GetField(gsys);
    SpPokeGimmickWork *work;
    GimmickState *gimmick;
    GameEvent *event;

    Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, WORK_ID);
    gimmick = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    GimmickState_GetUserData(gimmick, 4);
    if (state > 3) {
        return NULL;
    }
    event = GameEvent_Create(gsys, NULL, func_ov042_021f3980, 0);
    work->unk_1 = state;
    return event;
}

// No twin in pokebw2's C
static GameEventReturnCode func_ov042_021f38d4(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    Field *field = GSYS_GetField(gsys);
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    SpPokeGimmickWork *work = Field_GetGimmickWorkBlock(field, WORK_ID);
    GimmickState *gimmick = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    FieldExpObjAnm *anm;
    u16 actor;
    fx32 frame;

    GimmickState_GetUserData(gimmick, 4);
    switch (*state) {
    case 0:
        actor = work->unk_0;
        frame = 0;
        FieldExpObj_SetAnm(system, 0, actor, 0, TRUE);
        anm = FieldExpObj_GetAnmInfo(system, 0, actor, 0);
        if (work->unk_2 != 0) {
            FieldExpObjAnm_SetPaused(anm, FALSE);
            frame = func_ov021_021be748(anm);
        } else {
            FieldExpObjAnm_SetPaused(anm, TRUE);
        }
        FieldExpObj_SetAnmFrame(system, 0, actor, 0, frame);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

// No twin in pokebw2's C
static GameEventReturnCode func_ov042_021f3980(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    Field *field = GSYS_GetField(gsys);
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    SpPokeGimmickWork *work = Field_GetGimmickWorkBlock(field, WORK_ID);
    GimmickState *gimmick = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    u8 *userData = GimmickState_GetUserData(gimmick, 4);

    switch (*state) {
    case 0: {
        u8 actor = work->unk_1 + 9;
        FieldExpObjAnm *anm;

        FieldExpObj_SetAnm(system, 0, actor, 0, TRUE);
        anm = FieldExpObj_GetAnmInfo(system, 0, actor, 0);
        FieldExpObjAnm_SetPaused(anm, FALSE);
        FieldExpObj_SetAnmFrame(system, 0, actor, 0, 0);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        (*state)++;
        break;
    }
    case 1: {
        u8 actor = work->unk_1 + 9;
        FieldExpObjAnm *anm = FieldExpObj_GetAnmInfo(system, 0, actor, 0);

        if (FieldExpObjAnm_IsPlaybackFinished(anm)) {
            userData[work->unk_1] = 1;
            return GAMEEVENT_DONE;
        }
        break;
    }
    }
    return GAMEEVENT_CONTINUE;
}
