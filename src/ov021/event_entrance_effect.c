// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/event_entrance_effect.c, ported by tools/port_file.py.
// The events that play an entrance, such as a door, when the player comes out of it or goes into it during a warp:
// the door's animations and sounds, the entrance camera, and the transitions. The name is descriptive; the ROM has no
// string for this file
#include "types.h"
#include "field/event_action_call.h"
#include "field/event_entrance_effect.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_camera.h"
#include "field/field_event.h"
#include "field/field_g3d_mapper.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_prop.h"
#include "field/field_rail.h"
#include "field/field_sound.h"
#include "field/zone.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "constants/directions.h"
#include "field/city_state.h"

GameEvent *func_ov036_021b74a0(GameSystem *gsys, WarpSequence *warp);

// The steps of an entrance event, which it plays in the order of its steps array
enum {
    ENTRANCE_STEP_START,
    ENTRANCE_STEP_TRANSITION,
    ENTRANCE_STEP_2,
    ENTRANCE_STEP_OPEN_DOOR,
    ENTRANCE_STEP_WAIT_OPEN,
    ENTRANCE_STEP_WALK,
    ENTRANCE_STEP_CAMERA,
    ENTRANCE_STEP_CLOSE_DOOR,
    ENTRANCE_STEP_END,
    ENTRANCE_STEP_WAIT_DELAY,
    ENTRANCE_STEP_WAIT_TIMER,
};

typedef struct {
    WarpSequence *warp;
    FieldSound *fieldSound;
    EntranceCamera *camera;
    EntranceCameraParam cameraParam;
    u16 timer;
    u8 dir;
    VecFx32 pos;
    FieldPropHandle *door;
    u8 steps[10];
    u8 stepIndex;
    // Whether the departure's steps return GAMEEVENT_CONTINUE_DIRECT, in a city
    BOOL continueDirect;
} EntranceEffectWork;

static void func_ov021_021a7b10(Field *field);
static void func_ov021_021a7b88(EntranceEffectWork *work);
GameEventReturnCode func_ov021_021a7ce8(GameEvent *event, u32 *state, void *data);
static void func_ov021_021a7eb4(EntranceEffectWork *work);
static GameEventReturnCode func_ov021_021a7f38(GameEvent *event, u32 *state, void *data);
static u8 Field_GetPlayerDirection(Field *field);
static void func_ov021_021a810c(Field *field, VecFx32 *pos);
static void func_ov021_021a811c(Field *field, VecFx32 *pos);
static void func_ov021_021a8140(Field *field, VecFx32 *pos);
static void func_ov021_021a8198(Field *field, VecFx32 *pos);
u8 func_ov021_021a81dc(EntranceEffectWork *work);
static u8 func_ov021_021a81ec(EntranceEffectWork *work);
static void func_ov021_021a8200(EntranceEffectWork *work);
static void func_ov021_021a8214(EntranceEffectWork *work);
static void func_ov021_021a8234(EntranceEffectWork *work);
static BOOL func_ov021_021a8244(EntranceEffectWork *work);
static BOOL func_ov021_021a8268(EntranceEffectWork *work);
static void func_ov021_021a8278(EntranceEffectWork *work);
void func_ov021_021a82a8(EntranceEffectWork *work);
void func_ov021_021a82d8(GameEvent *event, EntranceEffectWork *work);
static void func_ov021_021a8324(GameEvent *event, EntranceEffectWork *work);
static BOOL func_ov021_021a834c(EntranceEffectWork *work);

void func_ov021_021a7b08(Field *field, VecFx32 *pos) {
    func_ov021_021a811c(field, pos);
}

static void func_ov021_021a7b10(Field *field) {
    func_ov010_0216d4b4(FieldPlayer_GetActor(Field_GetPlayer(field)));
}

GameEvent *func_ov021_021a7b20(WarpSequence *warp) {
    GameEvent *event = GameEvent_Create(warp->gsys, NULL, func_ov021_021a7ce8, sizeof(EntranceEffectWork));
    EntranceEffectWork *work = GameEvent_GetData(event);

    work->warp = warp;
    work->fieldSound = GameData_GetFieldSoundSystem(warp->gameData);
    work->door = NULL;
    work->timer = 0;
    work->dir = Field_GetPlayerDirection(warp->field);
    work->stepIndex = 0;
    work->camera = func_ov021_021bdc94(warp->field);
    work->cameraParam.transitionType = warp->transitionType;
    work->cameraParam.isArrival = TRUE;
    work->cameraParam.isDeparture = FALSE;
    func_ov021_021a810c(warp->field, &work->pos);
    func_ov021_021a8214(work);
    func_ov021_021a7b88(work);
    return event;
}

static void func_ov021_021a7b88(EntranceEffectWork *work) {
    WarpSequence *warp = work->warp;
    Field *field = warp->field;

    if (func_ov021_021a8268(work)) {
        switch (Field_GetPlayerDirection(field)) {
        case DIR_UP:
        case DIR_DOWN:
        default:
            work->steps[0] = ENTRANCE_STEP_START;
            work->steps[1] = ENTRANCE_STEP_TRANSITION;
            work->steps[2] = ENTRANCE_STEP_WALK;
            work->steps[3] = ENTRANCE_STEP_2;
            work->steps[4] = ENTRANCE_STEP_OPEN_DOOR;
            work->steps[5] = ENTRANCE_STEP_WAIT_OPEN;
            work->steps[6] = ENTRANCE_STEP_CAMERA;
            work->steps[7] = ENTRANCE_STEP_CLOSE_DOOR;
            work->steps[8] = ENTRANCE_STEP_END;
            break;
        case DIR_LEFT:
        case DIR_RIGHT:
            work->steps[0] = ENTRANCE_STEP_START;
            work->steps[1] = ENTRANCE_STEP_TRANSITION;
            work->steps[2] = ENTRANCE_STEP_2;
            work->steps[3] = ENTRANCE_STEP_OPEN_DOOR;
            work->steps[4] = ENTRANCE_STEP_WAIT_OPEN;
            work->steps[5] = ENTRANCE_STEP_WALK;
            work->steps[6] = ENTRANCE_STEP_CAMERA;
            work->steps[7] = ENTRANCE_STEP_CLOSE_DOOR;
            work->steps[8] = ENTRANCE_STEP_END;
            break;
        }
    } else if (func_ov010_02162370(warp->transitionType)) {
        work->steps[0] = ENTRANCE_STEP_START;
        work->steps[1] = ENTRANCE_STEP_TRANSITION;
        work->steps[2] = ENTRANCE_STEP_WAIT_OPEN;
        work->steps[3] = ENTRANCE_STEP_WALK;
        work->steps[4] = ENTRANCE_STEP_END;
    } else {
        switch (Field_GetPlayerDirection(field)) {
        case DIR_UP:
        case DIR_DOWN:
        default:
            work->steps[0] = ENTRANCE_STEP_START;
            work->steps[1] = ENTRANCE_STEP_TRANSITION;
            work->steps[2] = ENTRANCE_STEP_WALK;
            work->steps[3] = ENTRANCE_STEP_WAIT_OPEN;
            work->steps[4] = ENTRANCE_STEP_END;
            break;
        case DIR_LEFT:
        case DIR_RIGHT:
            work->steps[0] = ENTRANCE_STEP_START;
            work->steps[1] = ENTRANCE_STEP_TRANSITION;
            work->steps[2] = ENTRANCE_STEP_WAIT_OPEN;
            work->steps[3] = ENTRANCE_STEP_WALK;
            work->steps[4] = ENTRANCE_STEP_END;
            break;
        }
    }
}

GameEventReturnCode func_ov021_021a7ce8(GameEvent *event, u32 *state, void *data) {
    EntranceEffectWork *work = data;
    WarpSequence *warp = work->warp;
    GameSystem *gsys = warp->gsys;
    GameData *gameData = warp->gameData;
    Field *field = warp->field;
    FieldSound *fieldSound = work->fieldSound;
    FieldCamera *camera = Field_GetCameraSystem(field);
    FieldTaskManager *taskManager = Field_GetTaskManager(field);

    Field_GetPlayerStateZoneID(field);
    switch (*state) {
    case 0:
        Field_SetPlayerHidden(field, TRUE);
        if (!FieldCamera_SupportsDelay(camera)) {
            *state = func_ov021_021a81ec(work);
            func_ov021_021a8200(work);
            break;
        }
        FieldCamera_FinishDelay(camera);
        *state = 9;
        break;
    case 9:
        if (!FieldCamera_IsDelayActive(camera)) {
            *state = func_ov021_021a81ec(work);
            func_ov021_021a8200(work);
        }
        break;
    case 1:
        func_ov021_021bdcc8(work->camera, &work->cameraParam);
        FieldSnd_FadeInImmediate(fieldSound, gameData);
        func_ov021_021a82d8(event, work);
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        break;
    case 2:
        func_ov021_021a8278(work);
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        break;
    case 3:
        if (func_ov021_021a8244(work) == TRUE) {
            *state = func_ov021_021a81ec(work);
            func_ov021_021a8200(work);
        }
        break;
    case 4:
        Field_SetPlayerHidden(field, FALSE);
        GameEvent_ChainNext(event, CallMoveOneTileFrontEvent(gsys, field));
        if (func_ov021_021bdd40(work->camera) && func_ov021_021bdd48(work->camera)) {
            *state = 10;
            break;
        }
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        break;
    case 6:
        func_ov021_021a82a8(work);
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        break;
    case 10:
        if (work->timer++ > 15) {
            *state = func_ov021_021a81ec(work);
            func_ov021_021a8200(work);
        }
        break;
    case 5:
        func_ov021_021bdd20(work->camera);
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        break;
    case 7:
        if (func_ov021_021a8244(work) == TRUE) {
            *state = func_ov021_021a81ec(work);
            func_ov021_021a8200(work);
        }
        break;
    }
    if (func_ov021_021a81dc(work) == 8 && FieldTaskManager_IsIdle(taskManager)) {
        func_ov021_021bdd34(work->camera);
        FieldCamera_LoadDefaults(camera);
        func_ov021_021bdcb8(work->camera);
        func_ov021_021a8234(work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *func_ov021_021a7e3c(WarpSequence *warp) {
    GameEvent *event = GameEvent_Create(warp->gsys, NULL, func_ov021_021a7f38, sizeof(EntranceEffectWork));
    EntranceEffectWork *work = GameEvent_GetData(event);

    work->warp = warp;
    work->fieldSound = GameData_GetFieldSoundSystem(warp->gameData);
    work->door = NULL;
    work->timer = 0;
    work->dir = Field_GetPlayerDirection(warp->field);
    work->stepIndex = 0;
    work->camera = func_ov021_021bdc94(warp->field);
    work->cameraParam.transitionType = warp->unk10;
    work->cameraParam.isArrival = FALSE;
    work->cameraParam.isDeparture = TRUE;
    func_ov021_021a811c(warp->field, &work->pos);
    func_ov021_021a8214(work);
    func_ov021_021a7eb4(work);
    if (func_ov010_02162370(warp->unk10)) {
        work->continueDirect = TRUE;
    } else {
        work->continueDirect = FALSE;
    }
    return event;
}

static void func_ov021_021a7eb4(EntranceEffectWork *work) {
    if (func_ov021_021a8268(work)) {
        work->steps[0] = 0;
        work->steps[1] = 3;
        work->steps[2] = 1;
        work->steps[3] = 2;
        work->steps[4] = 5;
        work->steps[5] = 6;
        work->steps[6] = 7;
        work->steps[7] = 8;
    } else {
        work->steps[0] = 0;
        work->steps[1] = 1;
        work->steps[2] = 2;
        work->steps[3] = 5;
        work->steps[4] = 6;
        work->steps[5] = 7;
        work->steps[6] = 8;
    }
}

// The player goes into the entrance
static GameEventReturnCode func_ov021_021a7f38(GameEvent *event, u32 *state, void *data) {
    EntranceEffectWork *work = data;
    WarpSequence *warp = work->warp;
    GameSystem *gsys = warp->gsys;
    GameData *gameData = warp->gameData;
    Field *field = warp->field;
    FieldSound *fieldSound = work->fieldSound;
    FieldTaskManager *taskManager = Field_GetTaskManager(field);

    switch (*state) {
    case 0:
        func_ov021_021bdcc8(work->camera, &work->cameraParam);
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        if (work->continueDirect) {
            return GAMEEVENT_CONTINUE_DIRECT;
        }
        break;
    case 1:
        func_ov021_021bdd20(work->camera);
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        if (work->continueDirect) {
            return GAMEEVENT_CONTINUE_DIRECT;
        }
        break;
    case 3:
        func_ov021_021a8278(work);
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        if (work->continueDirect) {
            return GAMEEVENT_CONTINUE_DIRECT;
        }
        break;
    case 2:
        if (!FieldTaskManager_IsIdle(taskManager)) {
            break;
        }
        if (func_ov021_021bdd40(work->camera) && func_ov021_021bdd48(work->camera)) {
            *state = 9;
        } else {
            *state = func_ov021_021a81ec(work);
            func_ov021_021a8200(work);
        }
        if (work->continueDirect) {
            return GAMEEVENT_CONTINUE_DIRECT;
        }
        break;
    case 4:
        if (func_ov021_021a8244(work) == TRUE) {
            *state = func_ov021_021a81ec(work);
            func_ov021_021a8200(work);
            if (work->continueDirect) {
                return GAMEEVENT_CONTINUE_DIRECT;
            }
        }
        break;
    case 9:
        if (work->timer++ > 15) {
            *state = func_ov021_021a81ec(work);
            func_ov021_021a8200(work);
            if (work->continueDirect) {
                return GAMEEVENT_CONTINUE_DIRECT;
            }
        }
        break;
    case 5:
        func_ov021_021a7b10(field);
        GameEvent_ChainNext(event, CallMoveOneTileFrontEvent(gsys, field));
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        if (work->continueDirect) {
            return GAMEEVENT_CONTINUE_DIRECT;
        }
        break;
    case 6:
        FieldSnd_SetZoneBGM(fieldSound, gameData, warp->spawn.zoneId, warp->endSeason);
        warp->unk48 = TRUE;
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        break;
    case 7:
        if (func_ov021_021a834c(work)) {
            GFL_SndSEPlay(0x559);
        }
        func_ov021_021a8324(event, work);
        *state = func_ov021_021a81ec(work);
        func_ov021_021a8200(work);
        break;
    case 8:
        func_ov021_021bdd34(work->camera);
        func_ov021_021bdcb8(work->camera);
        func_ov021_021a8234(work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

static u8 Field_GetPlayerDirection(Field *field) {
    return FieldPlayer_GetFaceDir(Field_GetPlayer(field));
}

static void func_ov021_021a810c(Field *field, VecFx32 *pos) {
    FieldPlayer_GetWPos(Field_GetPlayer(field), pos);
}

static void func_ov021_021a811c(Field *field, VecFx32 *pos) {
    if (Field_GetResolvedControllerTypeID(field) == 1) {
        func_ov021_021a8198(field, pos);
    } else {
        func_ov021_021a8140(field, pos);
    }
}

// On the grid: a tile in front of the player, at the map's height there
static void func_ov021_021a8140(Field *field, VecFx32 *pos) {
    FieldPlayer *player = Field_GetPlayer(field);
    u32 dir = FieldPlayer_GetFaceDir(player);
    VecFx32 playerPos;
    VecFx32 dirVec;
    VecFx32 front;
    fx32 height;

    FieldPlayer_GetWPos(player, &playerPos);
    func_ov021_021a3484(player, dir, &dirVec);
    VEC_MultAdd(FX32_CONST(16), &dirVec, &playerPos, &front);
    GetHeightFromMap(FieldPlayer_GetActor(player), &front, &height);
    front.y = height;
    *pos = front;
}

// On rails: a step in front of the player on its rail
static void func_ov021_021a8198(Field *field, VecFx32 *pos) {
    FieldPlayer *player = Field_GetPlayer(field);
    u32 dir = FieldPlayer_GetFaceDir(player);
    FieldRailSystem *rail = FieldNoGridMapper_GetRailSystem(Field_GetNoGridMapper(field));
    VecFx32 front;
    RailPosition railPos;

    func_ov021_021a36c8(player, dir, &railPos);
    func_ov021_021b7d38(rail, &railPos, &front);
    *pos = front;
}

u8 func_ov021_021a81dc(EntranceEffectWork *work) {
    return work->steps[work->stepIndex];
}

static u8 func_ov021_021a81ec(EntranceEffectWork *work) {
    return work->steps[(u8)(work->stepIndex + 1)];
}

static void func_ov021_021a8200(EntranceEffectWork *work) {
    work->stepIndex++;
    work->timer = 0;
}

static void func_ov021_021a8214(EntranceEffectWork *work) {
    work->door = FieldPropSystem_CreateHandleAtPos(FieldG3DMapper_GetBMSystem(Field_GetG3DMapper(work->warp->field)),
                                                   1, &work->pos);
}

static void func_ov021_021a8234(EntranceEffectWork *work) {
    if (work->door != NULL) {
        FieldPropHandle_Free(work->door);
    }
}

// Whether the door's animation is done, or there is no door
static BOOL func_ov021_021a8244(EntranceEffectWork *work) {
    if (!func_ov021_021a8268(work)) {
        return TRUE;
    }
    if (FieldPropHandle_IsAnmFinished(work->door)) {
        return TRUE;
    }
    return FALSE;
}

// Whether there is a door
static BOOL func_ov021_021a8268(EntranceEffectWork *work) {
    if (work->door != NULL) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov021_021a8278(EntranceEffectWork *work) {
    u16 soundId;

    if (func_ov021_021a8268(work)) {
        FieldPropHandle_CallAnmCmd(work->door, 0, 0);
        if (FieldPropHandle_GetAnimSoundID(work->door, &soundId)) {
            GFL_SndSEPlay(soundId);
        }
    }
}

void func_ov021_021a82a8(EntranceEffectWork *work) {
    u16 soundId;

    if (func_ov021_021a8268(work)) {
        FieldPropHandle_CallAnmCmd(work->door, 1, 0);
        if (FieldPropHandle_GetAnimSoundID(work->door, &soundId)) {
            GFL_SndSEPlay(soundId);
        }
    }
}

void func_ov021_021a82d8(GameEvent *event, EntranceEffectWork *work) {
    WarpSequence *warp = work->warp;
    GameEvent *transition;

    if (warp->seasonChanged) {
        transition = CallFieldMapEntranceInTransition(warp->gsys, warp->field, 3, 0, 0, warp->startSeason,
                                                      warp->endSeason);
    } else {
        transition = CallFieldMapEntranceInTransition(warp->gsys, warp->field, warp->inTransition, 0, 1, 0, 0);
    }
    GameEvent_ChainNext(event, transition);
}

static void func_ov021_021a8324(GameEvent *event, EntranceEffectWork *work) {
    WarpSequence *warp = work->warp;
    GameEvent *transition;

    if (warp->seasonChanged) {
        transition = CallFieldMapEntranceOutTransitionDefault(warp->gsys, warp->field, 0, 0);
    } else {
        transition = CallFieldMapEntranceOutTransitionDefault(warp->gsys, warp->field, warp->outTransition, 0);
    }
    GameEvent_ChainNext(event, transition);
}

// Whether to play the sound of going in without a door
static BOOL func_ov021_021a834c(EntranceEffectWork *work) {
    WarpSequence *warp = work->warp;

    if (func_ov021_021a8268(work)) {
        return FALSE;
    }
    if (warp->outTransition != 2) {
        return TRUE;
    }
    return FALSE;
}
