// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov133/gimmick_nuvema.c, ported by tools/port_file.py.
// The gimmick of Nuvema Town (zones 389, 397 and 317 load it), a model standing in Nuvema Town. The name is a guess: the
// overlay embeds no file name
#include "types.h"
#include "constants/sound.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "field/field_map.h"
#include "field/gimmick_nuvema.h"
#include "gfl/g3d.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/rtc.h"

typedef struct {
    BOOL playing;
    BOOL night;
} GimmickWork;

static GameEventReturnCode func_ov065_021f38d0(GameEvent *event, u32 *state, void *data);

// pokebw2's sAnimations
static const G3DSceneAnimationSetup data_ov065_021f38f0[] = { { 1, 0 }, { 2, 0 } };

// pokebw2's sDayResources
static const G3DSceneResourceSetup data_ov065_021f3928[] = { { 0xe4, 0, 0 }, { 0xe4, 1, 0 }, { 0xe4, 2, 0 } };

// pokebw2's sActors
static const G3DSceneActorSetup data_ov065_021f3908[] = {
    { 0, 0, 0, 0, data_ov065_021f38f0, NELEMS(data_ov065_021f38f0) },
};

// pokebw2's sDaySceneSetup
static const G3DSceneSetup data_ov065_021f38f8 = { data_ov065_021f3928, NELEMS(data_ov065_021f3928), data_ov065_021f3908, NELEMS(data_ov065_021f3908) };

// pokebw2's sNightResources
static const G3DSceneResourceSetup data_ov065_021f394c[] = { { 0xe4, 3, 0 }, { 0xe4, 4, 0 }, { 0xe4, 5, 0 } };

// pokebw2's sNightSceneSetup
static const G3DSceneSetup data_ov065_021f3918 = { data_ov065_021f394c, NELEMS(data_ov065_021f394c), data_ov065_021f3908, NELEMS(data_ov065_021f3908) };

void func_ov065_021f36c0(Field *field) {
    s32 i;
    FieldExpObjSystem *system;
    GameData *gameData;
    u32 *state;
    GimmickWork *work;
    FieldExpObjAnm *anm;
    u32 period;

    gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    state = GimmickState_GetUserData(GameData_GetGimmickState(gameData), 0x36);
    system = Field_GetExpObjSystem(field);
    Field_AllocGimmickWorkBlock(field, 1, Field_GetHeapID(field), sizeof(GimmickWork));
    work = Field_GetGimmickWorkBlock(field, 1);
    work->playing = FALSE;
    period = GetRealTimeDayPeriod(GameData_GetSeason(gameData));
    if (period == 3 || period == 4) {
        LoadFieldExpandObjData(system, &data_ov065_021f3918, 0);
        work->night = TRUE;
    } else {
        LoadFieldExpandObjData(system, &data_ov065_021f38f8, 0);
        work->night = FALSE;
    }
    FieldExpObj_GetActorMatrixPtr(system, 0, 0)->translation = (VecFx32){ FX32_CONST(784 * 16), 0, FX32_CONST(752 * 16) };
    func_ov021_021be4a4(system, 0, 0, TRUE);
    FieldExpObj_SetActorHidden(system, 0, 0, *state ? FALSE : TRUE);
    for (i = 0; i < 2; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 0, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(system, 0, 0, i, *state ? TRUE : FALSE);
    }
}

void func_ov065_021f37c4(Field *field) {
    FieldExpObj_FreeScene(Field_GetExpObjSystem(field), 0);
    Field_DeleteGimmickWorkBlock(field, 1);
}

void func_ov065_021f37dc(Field *field) {
    FieldExpObjSystem *system;
    GimmickWork *work;
    u32 *state;

    system = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    if (work->playing) {
        FieldExpObj_StepAllAnimations(system);
        if (!work->night && func_ov021_021be6e8(system, 0, 0, 0) == FX32_CONST(60)) {
            GFL_SndSEPlay(SEQ_SE_OPEN_03);
        }
        if (FieldExpObjAnm_IsPlaybackFinished(FieldExpObj_GetAnmInfo(system, 0, 0, 0))) {
            FieldExpObj_SetActorHidden(system, 0, 0, 1);
            work->playing = FALSE;
            state = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), 0x36);
            *state = FALSE;
        }
    }
}

void func_ov065_021f385c(Field *field) {
    s32 i;
    FieldExpObjSystem *system;
    GimmickWork *work;

    system = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    work->playing = TRUE;
    FieldExpObj_SetActorHidden(system, 0, 0, 0);
    for (i = 0; i < 2; i++) {
        FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, 0, i), FALSE);
    }
    if (work->night) {
        GFL_SndSEPlay(0x7f7);
    } else {
        GFL_SndSEPlay(0x7f8);
    }
}



GameEvent *func_ov065_021f38bc(GameSystem *gsys) {
    return GameEvent_Create(gsys, NULL, func_ov065_021f38d0, 0);
}

static GameEventReturnCode func_ov065_021f38d0(GameEvent *event, u32 *state, void *data) {
    GimmickWork *work = Field_GetGimmickWorkBlock(GSYS_GetField(GameEvent_GetGameSystem(event)), 1);

    if (work->playing) {
        return GAMEEVENT_CONTINUE;
    }
    return GAMEEVENT_DONE;
}
