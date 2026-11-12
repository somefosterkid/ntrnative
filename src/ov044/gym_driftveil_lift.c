// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov097/gym_driftveil_lift.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/sound.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_camera.h"
#include "field/field_exp_obj.h"
#include "field/gym_driftveil_lift.h"
#include "gfl/g3d.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "save/event_work.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

#define POS_TO_GRID(pos) ((s16)(((pos) >> 4) / FX32_ONE))

// The height of the lift's lower stop; the upper one is 0
#define LIFT_BOTTOM_Y FX32_CONST(-80)
// How far the lift moves a frame
#define LIFT_SPEED FX32_CONST(2)

// The work var that says which lift is in use: 1 for the second one
#define WORK_LIFT 0x4000

typedef struct {
    fx32 y;
    fx32 targetY;
    fx32 speed;
    void *cameraBind;
    BOOL down;
    u32 unk_14;
    u16 lift;
    FieldActor *rider;
} LiftWork;

BOOL func_ov044_021f390c(LiftWork *work);
GameEventReturnCode func_ov044_021f37f0(GameEvent *event, u32 *state, void *data);
void func_ov044_021f392c(FieldExpObjSystem *system);
void func_ov044_021f3970(GameSystem *gsys);

// pokebw2's sAnimations
static const G3DSceneAnimationSetup data_ov044_021f39d0[] = { { 1, 0 }, { 2, 0 } };

// pokebw2's sActors
static const G3DSceneActorSetup data_ov044_021f39d8[1] = {
    { 0, 0, 0, 0, data_ov044_021f39d0, NELEMS(data_ov044_021f39d0) },
};

// pokebw2's sResources
static const G3DSceneResourceSetup data_ov044_021f39f8[] = {
    { 0x8a, 0, 0 },
    { 0x8a, 3, 0 },
    { 0x8a, 4, 0 },
};

// pokebw2's sSceneSetup
static const G3DSceneSetup data_ov044_021f39e8 = { data_ov044_021f39f8, NELEMS(data_ov044_021f39f8), data_ov044_021f39d8, NELEMS(data_ov044_021f39d8) };


void func_ov044_021f36c0(Field *field) {
    VecFx32 lift0Position = { FX32_CONST(120), 0, FX32_CONST(72) };
    FieldExpObjSystem *system;
    FieldExpObjAnm *anm;
    int i;

    system = Field_GetExpObjSystem(field);
    Field_AllocGimmickWorkBlock(field, 1, Field_GetHeapID(field), 0x14);
    LoadFieldExpandObjData(system, &data_ov044_021f39e8, 0);
    FieldExpObj_GetActorMatrixPtr(system, 0, 0)->translation = lift0Position;
    func_ov021_021be4a4(system, 0, 0, TRUE);
    for (i = 0; i < 2; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 0, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
    }
    func_ov044_021f392c(system);
}

void func_ov044_021f3740(Field *field) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(field);
    Field_GetGimmickWorkBlock(field, 1);
    Field_DeleteGimmickWorkBlock(field, 1);
    FieldExpObj_FreeScene(system, 0);
}

void func_ov044_021f3764(Field *field) {
    FieldExpObj_StepAllAnimations(Field_GetExpObjSystem(field));
}

GameEvent *func_ov044_021f3770(GameSystem *gsys, BOOL down) {
    Field *field;
    LiftWork *work;
    FieldExpObjSystem *system;

    field = GSYS_GetField(gsys);
    work = Field_GetGimmickWorkBlock(field, 1);
    work->down = down;
    if (down) {
        work->y = 0;
        work->targetY = LIFT_BOTTOM_Y;
    } else {
        work->y = LIFT_BOTTOM_Y;
        work->targetY = 0;
        system = Field_GetExpObjSystem(field);
        FieldExpObj_SetAnm(system, 0, 0, 0, TRUE);
        FieldExpObj_SetAnm(system, 0, 0, 1, FALSE);
    }
    if (work->targetY - work->y < 0) {
        work->speed = -LIFT_SPEED;
    } else {
        work->speed = LIFT_SPEED;
    }
    return GameEvent_Create(gsys, NULL, func_ov044_021f37f0, 0);
}

// pokebw2's DriftveilLift_MoveEvent
GameEventReturnCode func_ov044_021f37f0(GameEvent *event, u32 *state, void *data) {
    Field *field;
    FieldExpObjSystem *system;
    LiftWork *work;
    FieldCamera *camera;
    FieldPlayer *player;
    VecFx32 playerPos;

    field = GSYS_GetField(GameEvent_GetGameSystem(event));
    system = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    camera = Field_GetCameraSystem(field);
    switch (*state) {
    case 0:
        if (work->down) {
            work->cameraBind = FieldCamera_GetBind(camera);
            FieldCamera_ClearBind(camera);
        }
        GFL_SndSEPlay(!work->down ? SEQ_SE_FLD_75 : SEQ_SE_FLD_59);
        (*state)++;
        break;
    case 1:
        work->y += work->speed;
        if (func_ov044_021f390c(work)) {
            work->y = work->targetY;
            if (!work->down) {
                GFL_SndSEPlay(SEQ_SE_FLD_60);
            }
            (*state)++;
        }
        FieldExpObj_GetActorMatrixPtr(system, 0, 0)->translation.y = work->y;
        player = Field_GetPlayer(field);
        FieldPlayer_GetWPos(player, &playerPos);
        playerPos.y = work->y;
        FieldPlayer_SetWPos(player, &playerPos);
        break;
    case 2:
        if (work->down) {
            return GAMEEVENT_DONE;
        }
        if (work->cameraBind != NULL) {
            FieldCamera_SetBind(camera, work->cameraBind);
        }
        FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, 0, 0), FALSE);
        (*state)++;
        break;
    case 3:
        if (FieldExpObjAnm_IsPlaybackFinished(FieldExpObj_GetAnmInfo(system, 0, 0, 0))) {
            func_ov044_021f392c(system);
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

BOOL func_ov044_021f390c(LiftWork *work) {
    fx32 targetY;
    if (work->speed < 0) {
        targetY = work->targetY;
        if (targetY >= work->y) {
            return TRUE;
        }
    } else {
        targetY = work->targetY;
        if (targetY <= work->y) {
            return TRUE;
        }
    }
    return FALSE;
}


// pokebw2's DriftveilLift_HasArrived
void func_ov044_021f392c(FieldExpObjSystem *system) {
    FieldExpObj_SetAnm(system, 0, 0, 1, TRUE);
    FieldExpObj_SetAnm(system, 0, 0, 0, FALSE);
    FieldExpObj_SetAnmFrame(system, 0, 0, 1, 0);
    FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, 0, 1), TRUE);
}

// pokebw2's DriftveilLift_ResetAnm
void func_ov044_021f3970(GameSystem *gsys) {
    Field *field;
    FieldExpObjSystem *system;
    LiftWork *work;
    FieldCamera *camera;
    FieldPlayer *player;
    VecFx32 playerPos;

    field = GSYS_GetField(gsys);
    system = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    camera = Field_GetCameraSystem(field);
    work->cameraBind = FieldCamera_GetBind(camera);
    FieldCamera_ClearBind(camera);
    FieldExpObj_GetActorMatrixPtr(system, 0, 0)->translation.y = LIFT_BOTTOM_Y;
    player = Field_GetPlayer(field);
    FieldPlayer_GetWPos(player, &playerPos);
    playerPos.y = LIFT_BOTTOM_Y;
    FieldPlayer_SetWPos(player, &playerPos);
}
