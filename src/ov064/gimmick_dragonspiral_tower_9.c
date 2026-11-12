// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov132/gimmick_dragonspiral_tower_9.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/ov064.h"
#include "constants/sound.h"
#include "constants/version.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "field/gimmick_dragonspiral_tower_9.h"
#include "gfl/fade.h"
#include "gfl/g3d.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/version.h"

#define GIMMICK_WORK_ID 1
// The archive of the scene's models, a/2/8/2
#define GIMMICK_ARCID 0x132

// The models of the scene
enum {
    MODEL_RELEASE,
    MODEL_THROWN,
};

typedef struct {
    // The fade of the flash
    u32 fadeMode;
    // Frames to wait, with the screen white, before it fades back
    s32 fadeWait;
    // The sounds of the release that were played
    BOOL played[4];
    // The throw of the second model
    VecFx32 from;
    VecFx32 to;
    fx32 height;
    u16 frames;
    u16 frame;
} DT9GimmickWork;

GameEventReturnCode func_ov064_021f384c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov064_021f3af8(GameEvent *event, u32 *state, void *data);
static GameEventReturnCode func_ov064_021f3bac(GameEvent *event, u32 *state, void *data);
static GameEventReturnCode func_ov064_021f3bd8(GameEvent *event, u32 *state, void *data);

// pokebw2's sThrownAnimations
static const G3DSceneAnimationSetup data_ov064_021f3c04[] = { { 6, 0 }, { 7, 0 }, { 8, 0 } };
// pokebw2's sReleaseAnimations
static const G3DSceneAnimationSetup data_ov064_021f3c10[] = { { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 } };

// The Light Stone's model is the fifth file in White 2, the Dark Stone's in Black 2
// pokebw2's sResourcesWhite2
static const G3DSceneResourceSetup data_ov064_021f3c60[9] = {
#include "asset/gimmick_dragonspiral_tower_9_data_ov064_021f3c60.inc"
};

// pokebw2's sResourcesBlack2
static const G3DSceneResourceSetup data_ov064_021f3ccc[9] = {
#include "asset/gimmick_dragonspiral_tower_9_data_ov064_021f3ccc.inc"
};

// pokebw2's sActors
static const G3DSceneActorSetup data_ov064_021f3c40[2] = {
    { 0, 0, 0, 0, data_ov064_021f3c10, NELEMS(data_ov064_021f3c10) },
    { 5, 0, 5, 0, data_ov064_021f3c04, NELEMS(data_ov064_021f3c04) },
};

// pokebw2's sSceneSetupBlack2
static const G3DSceneSetup data_ov064_021f3c20 = { data_ov064_021f3ccc, NELEMS(data_ov064_021f3ccc), data_ov064_021f3c40, NELEMS(data_ov064_021f3c40) };
// pokebw2's sSceneSetupWhite2
static const G3DSceneSetup data_ov064_021f3c30 = { data_ov064_021f3c60, NELEMS(data_ov064_021f3c60), data_ov064_021f3c40, NELEMS(data_ov064_021f3c40) };

// pokebw2's GimmickDT9_End

void func_ov064_021f36c0(Field *field) {
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);
    DT9GimmickWork *work = Field_AllocGimmickWorkBlock(field, GIMMICK_WORK_ID, Field_GetHeapID(field), sizeof(DT9GimmickWork));
    FieldExpObjAnm *anm;
    int i;

    work->fadeWait = 0;
    if (getGameVersion() == VERSION_WHITE) {
        LoadFieldExpandObjData(expObj, &data_ov064_021f3c20, 0);
    } else {
        LoadFieldExpandObjData(expObj, &data_ov064_021f3c30, 0);
    }
    work->fadeMode = FADE_ENGINE_A_WHITE;
    FieldExpObj_SetActorHidden(expObj, 0, MODEL_RELEASE, TRUE);
    FieldExpObj_SetActorHidden(expObj, 0, MODEL_THROWN, TRUE);
    for (i = 0; i < 4; i++) {
        anm = FieldExpObj_GetAnmInfo(expObj, 0, MODEL_RELEASE, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(expObj, 0, MODEL_RELEASE, i, FALSE);
    }
    for (i = 0; i < 3; i++) {
        anm = FieldExpObj_GetAnmInfo(expObj, 0, MODEL_THROWN, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(expObj, 0, MODEL_THROWN, i, FALSE);
    }
}

void func_ov064_021f3790(Field *field) {
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);

    Field_DeleteGimmickWorkBlock(field, GIMMICK_WORK_ID);
    FieldExpObj_FreeScene(expObj, 0);
}

// pokebw2's GimmickDT9_Update
void func_ov064_021f37ac(Field *field) {
    FieldExpObj_StepAllAnimations(Field_GetExpObjSystem(field));
}

GameEvent *func_ov064_021f37b8(GameSystem *gsys) {
    int i;
    FieldExpObjAnm *anm;
    VecFx32 pos;
    Field *field = GSYS_GetField(gsys);
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);

    FieldPlayer_GetWPos(Field_GetPlayer(field), &pos);
    FieldExpObj_GetActorMatrixPtr(expObj, 0, MODEL_RELEASE)->translation = pos;
    FieldExpObj_SetActorHidden(expObj, 0, MODEL_RELEASE, FALSE);
    for (i = 0; i < 4; i++) {
        anm = FieldExpObj_GetAnmInfo(expObj, 0, MODEL_RELEASE, i);
        FieldExpObjAnm_SetPaused(anm, FALSE);
        FieldExpObj_SetAnm(expObj, 0, MODEL_RELEASE, i, TRUE);
        FieldExpObj_SetAnmFrame(expObj, 0, MODEL_RELEASE, i, 0);
    }
    return GameEvent_Create(gsys, NULL, func_ov064_021f384c, 0);
}

GameEventReturnCode func_ov064_021f384c(GameEvent *event, u32 *state, void *data) {
    Field *field = GSYS_GetField(GameEvent_GetGameSystem(event));
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);
    DT9GimmickWork *work = Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    s32 frame = func_ov021_021be6e8(expObj, 0, 0, 0);
    fx32 lastFrame = func_ov021_021be748(FieldExpObj_GetAnmInfo(expObj, 0, 0, 0));

    if (frame >= 0 && work->played[0] == FALSE) {
        GFL_SndStop();
        GFL_SndSEPlay(SEQ_SE_FLD_165);
        work->played[0] = TRUE;
    } else if (frame >= FX32_CONST(131) && work->played[1] == FALSE) {
        GFL_SndStop();
        GFL_SndSEPlay(SEQ_SE_FLD_162);
        work->played[1] = TRUE;
    } else if (frame >= FX32_CONST(108) && work->played[2] == FALSE) {
        GFL_SndStop();
        GFL_SndSEPlay(SEQ_SE_FLD_163);
        work->played[2] = TRUE;
    } else if (frame >= FX32_CONST(250) && work->played[3] == FALSE) {
        GFL_SndStop();
        GFL_SndSEPlay(SEQ_SE_FLD_175);
        work->played[3] = TRUE;
    }

    switch (*state) {
    case 0:
        if (frame == FX32_CONST(101)) {
            GFL_FadeSet(work->fadeMode, 0, 16, 0);
            work->fadeWait = 15;
            (*state)++;
        }
        break;
    case 1:
        if (GFL_FadeIsRunning() == FALSE) {
            if (work->fadeWait <= 0) {
                GFL_FadeSet(work->fadeMode, 16, 0, 0);
                (*state)++;
            } else {
                work->fadeWait--;
            }
        }
        break;
    case 2:
        if (GFL_FadeIsRunning() == FALSE) {
            (*state)++;
        }
        break;
    }
    return (frame >= lastFrame) ? GAMEEVENT_DONE : GAMEEVENT_CONTINUE;
}


GameEvent *func_ov064_021f3970(GameSystem *gsys, const VecFx32 *from, const VecFx32 *to, fx32 height, u32 frames) {
    int i;
    FieldExpObjAnm *anm;
    Field *field = GSYS_GetField(gsys);
    DT9GimmickWork *work = Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);

    GFL_SndSEPlay(SEQ_SE_NAGERU);
    FieldExpObj_GetActorMatrixPtr(expObj, 0, MODEL_THROWN)->translation = *from;
    FieldExpObj_SetActorHidden(expObj, 0, MODEL_THROWN, FALSE);
    for (i = 0; i < 3; i++) {
        anm = FieldExpObj_GetAnmInfo(expObj, 0, MODEL_THROWN, i);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(expObj, 0, MODEL_THROWN, i, FALSE);
        FieldExpObj_SetAnmFrame(expObj, 0, MODEL_THROWN, i, 0);
    }
    work->from = *from;
    work->to = *to;
    work->frames = frames;
    work->frame = 0;
    work->height = height;
    return GameEvent_Create(gsys, NULL, func_ov064_021f3af8, 0);
}



void func_ov064_021f3a44(GameSystem *gsys, const VecFx32 *pos) {
    int i;
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(GSYS_GetField(gsys));

    GFL_SndSEPlay(SEQ_SE_BOWA2);
    FieldExpObj_GetActorMatrixPtr(expObj, 0, MODEL_THROWN)->translation = *pos;
    FieldExpObj_SetActorHidden(expObj, 0, MODEL_THROWN, FALSE);
    for (i = 0; i < 3; i++) {
        FieldExpObjAnm *anm = FieldExpObj_GetAnmInfo(expObj, 0, MODEL_THROWN, i);
        FieldExpObjAnm_SetPaused(anm, FALSE);
        FieldExpObj_SetAnm(expObj, 0, MODEL_THROWN, i, TRUE);
        FieldExpObj_SetAnmFrame(expObj, 0, MODEL_THROWN, i, 0);
    }
}


// pokebw2's GimmickDT9_CreateWaitFifthFrameEvent
GameEvent *func_ov064_021f3ac0(GameSystem *gsys) {
    GSYS_GetField(gsys);
    return GameEvent_Create(gsys, NULL, func_ov064_021f3bac, 0);
}

// pokebw2's GimmickDT9_CreateWaitThrownDoneEvent
GameEvent *func_ov064_021f3adc(GameSystem *gsys) {
    GSYS_GetField(gsys);
    return GameEvent_Create(gsys, NULL, func_ov064_021f3bd8, 0);
}

// Moves the thrown model along its arc, a frame at a time
// pokebw2's GimmickDT9_ThrowEvent
GameEventReturnCode func_ov064_021f3af8(GameEvent *event, u32 *state, void *data) {
    Field *field = GSYS_GetField(GameEvent_GetGameSystem(event));
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);
    DT9GimmickWork *work = Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    VecFx32 pos;
    fx32 start;
    fx32 delta;
    int frame;

    work->frame++;
    start = work->from.x;
    delta = work->to.x - start;
    pos.x = start + delta * work->frame / work->frames;
    start = work->from.y;
    delta = work->to.y - start;
    pos.y = start + delta * work->frame / work->frames;
    start = work->from.z;
    delta = work->to.z - start;
    pos.z = start + delta * work->frame / work->frames;
    frame = work->frame;
    if (frame >= (work->frames + 1) / 2) {
        frame = work->frames - frame;
    }
    pos.y += work->height * frame / work->frames;
    FieldExpObj_GetActorMatrixPtr(expObj, 0, MODEL_THROWN)->translation = pos;
    if (work->frame >= work->frames) {
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

static GameEventReturnCode func_ov064_021f3bac(GameEvent *event, u32 *state, void *data) {
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(GSYS_GetField(GameEvent_GetGameSystem(event)));
    s32 frame = func_ov021_021be6e8(expObj, 0, 1, 0);

    if (frame >= FX32_CONST(5)) {
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


// pokebw2's GimmickDT9_ThrownDoneEvent
static GameEventReturnCode func_ov064_021f3bd8(GameEvent *event, u32 *state, void *data) {
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(GSYS_GetField(GameEvent_GetGameSystem(event)));

    if (FieldExpObjAnm_IsPlaybackFinished(FieldExpObj_GetAnmInfo(expObj, 0, MODEL_THROWN, 0))) {
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
