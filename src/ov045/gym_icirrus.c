// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov098/gym_icirrus.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/sound.h"
#include "field/field.h"
#include "field/field_controller.h"
#include "field/field_exp_obj.h"
#include "field/field_map.h"
#include "field/fieldmap_ctrl_hybrid.h"
#include "field/gym_icirrus.h"
#include "gfl/g3d.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

#define GIMMICK_WORK_ID 1
#define GIMMICK_STATE_ID 9
// The archive of the gym's models
#define GIMMICK_ARCID 0x98

#define SWITCH_COUNT 3
// The second model of each switch comes this many models after its first
#define SECOND_MODEL_OFFSET 3
#define PLACE_COUNT 8
// The controller type of maps with both grid and rail movement
#define CONTROLLER_TYPE_HYBRID 2

// What the save keeps of the gym: whether each switch is flipped
typedef struct {
    BOOL flipped[SWITCH_COUNT];
} IcirrusGymState;

// The switch that the running event animates
typedef struct {
    u32 switchIndex;
} IcirrusGymWork;

// A place where the player can change movement, and the state that its switch must be in
typedef struct {
    u16 switchIndex;
    u16 flipped;
} PlacePrecondition;

static BOOL func_ov045_021f3908(IcirrusGymState *state, int place);
static void func_ov045_021f3938(FieldExpObjSystem *expObj, BOOL flipped, int switchIndex, int second);
static BOOL func_ov045_021f39a8(IcirrusGymState *state, int switchIndex);
GameEventReturnCode func_ov045_021f39b8(GameEvent *event, u32 *state, void *data);
static GameEventReturnCode func_ov045_021f3a68(GameEvent *event, u32 *state, void *data);
void func_ov021_021be4a4(FieldExpObjSystem *expObj, u16 scene, u16 actor, BOOL value);

// The animation of a model
static inline FieldExpObjAnm *GetAnimation(FieldExpObjSystem *expObj, u8 model, u8 animation) {
    return FieldExpObj_GetAnmInfo(expObj, 0, model, animation);
}

// pokebw2's sResources
static const G3DSceneResourceSetup data_ov045_021f3c28[9] = {
#include "asset/gym_icirrus_data_ov045_021f3c28.inc"
};

// pokebw2's sModelAAnimations
static const G3DSceneAnimationSetup data_ov045_021f3b38[] = { { 5, 0 }, { 6, 0 } };
// pokebw2's sModelBAnimations
static const G3DSceneAnimationSetup data_ov045_021f3b40[] = { { 3, 0 }, { 4, 0 } };
// pokebw2's sMovingAnimations
static const G3DSceneAnimationSetup data_ov045_021f3b48[] = { { 7, 0 }, { 8, 0 } };

// pokebw2's sActors
static const G3DSceneActorSetup data_ov045_021f3bc8[6] = {
#include "asset/gym_icirrus_data_ov045_021f3bc8.inc"
};

// pokebw2's sSceneSetup
static const G3DSceneSetup data_ov045_021f3b50 = { data_ov045_021f3c28, NELEMS(data_ov045_021f3c28), data_ov045_021f3bc8, NELEMS(data_ov045_021f3bc8) };

// pokebw2's sFirstPositions
static const VecFx32 data_ov045_021f3b80[SWITCH_COUNT] = {
    { FX32_CONST(296), FX32_CONST(0), FX32_CONST(872) },
    { FX32_CONST(888), FX32_CONST(48), FX32_CONST(696) },
    { FX32_CONST(280), FX32_CONST(96), FX32_CONST(536) },
};

// pokebw2's sSecondPositions
static const VecFx32 data_ov045_021f3ba4[SWITCH_COUNT] = {
    { FX32_CONST(376), FX32_CONST(0), FX32_CONST(856) },
    { FX32_CONST(952), FX32_CONST(48), FX32_CONST(664) },
    { FX32_CONST(248), FX32_CONST(96), FX32_CONST(472) },
};

// pokebw2's sPlacePreconditions
static const PlacePrecondition data_ov045_021f3b60[PLACE_COUNT] = {
#include "asset/gym_icirrus_data_ov045_021f3b60.inc"
};

void func_ov045_021f36c0(Field *field) {
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);
    IcirrusGymState *state = GimmickState_GetUserData(
        GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), GIMMICK_STATE_ID);
    int i;
    int j;
    int model;
    SRTMatrix *matrix;
    FieldExpObjAnm *anm;

    Field_AllocGimmickWorkBlock(field, GIMMICK_WORK_ID, Field_GetHeapID(field), sizeof(IcirrusGymWork));
    LoadFieldExpandObjData(expObj, &data_ov045_021f3b50, 0);
    for (i = 0; i < SWITCH_COUNT; i++) {
        matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, i);
        matrix->translation = data_ov045_021f3b80[i];
        func_ov021_021be4a4(expObj, 0, i, TRUE);
        for (j = 0; j < 2; j++) {
            anm = FieldExpObj_GetAnmInfo(expObj, 0, i, j);
            FieldExpObjAnm_SetLooped(anm, FALSE);
            FieldExpObjAnm_SetPaused(anm, TRUE);
        }
        model = i + SECOND_MODEL_OFFSET;
        matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, model);
        matrix->translation = data_ov045_021f3ba4[i];
        func_ov021_021be4a4(expObj, 0, model, TRUE);
        for (j = 0; j < 2; j++) {
            anm = FieldExpObj_GetAnmInfo(expObj, 0, model, j);
            FieldExpObjAnm_SetLooped(anm, FALSE);
            FieldExpObjAnm_SetPaused(anm, TRUE);
        }
        func_ov045_021f3938(expObj, state->flipped[i], i, TRUE);
        func_ov045_021f3938(expObj, state->flipped[i], i, FALSE);
    }
}

void func_ov045_021f37f8(Field *field) {
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);

    Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    Field_DeleteGimmickWorkBlock(field, GIMMICK_WORK_ID);
    FieldExpObj_FreeScene(expObj, 0);
}

void func_ov045_021f381c(Field *field) {
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);

    Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))),
                             GIMMICK_STATE_ID);
    FieldExpObj_StepAllAnimations(expObj);
}

GameEvent *func_ov045_021f384c(GameSystem *gsys, int switchIndex) {
    Field *field = GSYS_GetField(gsys);
    IcirrusGymWork *work;

    GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    work = Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    if (switchIndex >= SWITCH_COUNT) {
        return NULL;
    }
    work->switchIndex = switchIndex;
    return GameEvent_Create(gsys, NULL, func_ov045_021f39b8, 0);
}

GameEvent *func_ov045_021f3888(GameSystem *gsys, int switchIndex) {
    Field *field = GSYS_GetField(gsys);
    IcirrusGymWork *work;

    GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    work = Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    if (switchIndex >= SWITCH_COUNT) {
        return NULL;
    }
    work->switchIndex = switchIndex;
    return GameEvent_Create(gsys, NULL, func_ov045_021f3a68, 0);
}

void func_ov045_021f38c4(GameSystem *gsys, int place) {
    Field *field = GSYS_GetField(gsys);
    u32 controllerType = Field_GetControllerTypeID(field);
    IcirrusGymState *state = GimmickState_GetUserData(
        GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), GIMMICK_STATE_ID);

    if (place < PLACE_COUNT && controllerType == CONTROLLER_TYPE_HYBRID) {
        if (func_ov045_021f3908(state, place)) {
            func_ov021_021a6dc4(Field_GetController(field), field);
        }
    }
}

// Whether the switch of the place is in the state that the place calls for
static BOOL func_ov045_021f3908(IcirrusGymState *state, int place) {
    u16 flipped = state->flipped[data_ov045_021f3b60[place].switchIndex] != 0 ? TRUE : FALSE;

    if (flipped == data_ov045_021f3b60[place].flipped) {
        return TRUE;
    }
    return FALSE;
}

// Sets the switch's first or second model to the end of the animation of its state, paused
static void func_ov045_021f3938(FieldExpObjSystem *expObj, BOOL flipped, int switchIndex, int second) {
    int playing;
    int other;
    int model = switchIndex;

    if (flipped != FALSE) {
        playing = 1;
        other = 0;
    } else {
        playing = 0;
        other = 1;
    }
    if (second != TRUE) {
        model += SECOND_MODEL_OFFSET;
    }
    FieldExpObj_SetAnm(expObj, 0, model, playing, TRUE);
    FieldExpObj_SetAnm(expObj, 0, model, other, FALSE);
    FieldExpObj_SetAnmFrame(expObj, 0, model, playing, 0);
    FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(expObj, 0, model, playing), TRUE);
}

static BOOL func_ov045_021f39a8(IcirrusGymState *state, int switchIndex) {
    if (state->flipped[switchIndex] != 0) {
        return TRUE;
    }
    return FALSE;
}

GameEventReturnCode func_ov045_021f39b8(GameEvent *event, u32 *state, void *data) {
    Field *field = GSYS_GetField(GameEvent_GetGameSystem(event));
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);
    IcirrusGymWork *work = Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    IcirrusGymState *gymState = GimmickState_GetUserData(
        GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), GIMMICK_STATE_ID);
    u32 switchIndex;
    BOOL flipped = TRUE;

    switch (*state) {
    case 0:
        switchIndex = work->switchIndex;
        FieldExpObjAnm_SetPaused(
            GetAnimation(expObj, switchIndex + SECOND_MODEL_OFFSET, func_ov045_021f39a8(gymState, switchIndex)),
            FALSE);
        (*state)++;
        break;
    case 1:
        switchIndex = work->switchIndex;
        if (FieldExpObjAnm_IsPlaybackFinished(
                GetAnimation(expObj, switchIndex + SECOND_MODEL_OFFSET,
                                   func_ov045_021f39a8(gymState, switchIndex)))) {
            if (gymState->flipped[work->switchIndex] != 0) {
                flipped = FALSE;
            }
            func_ov045_021f3938(expObj, flipped, work->switchIndex, FALSE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static GameEventReturnCode func_ov045_021f3a68(GameEvent *event, u32 *state, void *data) {
    Field *field = GSYS_GetField(GameEvent_GetGameSystem(event));
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);
    IcirrusGymWork *work = Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    IcirrusGymState *gymState = GimmickState_GetUserData(
        GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), GIMMICK_STATE_ID);
    u32 switchIndex;
    BOOL flipped = TRUE;
    BOOL newFlipped;

    switch (*state) {
    case 0:
        switchIndex = work->switchIndex;
        FieldExpObjAnm_SetPaused(GetAnimation(expObj, switchIndex, func_ov045_021f39a8(gymState, switchIndex)), FALSE);
        GFL_SndSEPlay(SEQ_SE_FLD_65);
        (*state)++;
        break;
    case 1:
        switchIndex = work->switchIndex;
        if (FieldExpObjAnm_IsPlaybackFinished(
                GetAnimation(expObj, switchIndex, func_ov045_021f39a8(gymState, switchIndex)))) {
            GFL_SndStop();
            GFL_SndSEPlay(SEQ_SE_FLD_95);
            if (gymState->flipped[work->switchIndex] != 0) {
                flipped = FALSE;
            }
            func_ov045_021f3938(expObj, flipped, work->switchIndex, TRUE);
            newFlipped = TRUE;
            if (gymState->flipped[work->switchIndex] != 0) {
                newFlipped = FALSE;
            }
            gymState->flipped[work->switchIndex] = newFlipped;
            return TRUE;
        }
        break;
    }
    return FALSE;
}
