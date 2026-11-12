// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov130/gimmick_funfest_levels.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/sound.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "field/player_state.h"
#include "gfl/g3d.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "save/high_link.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "ov060_021f36c0.h"

#define GIMMICK_WORK_ID 0

// The archives of the scene's models
#define GIMMICK_ARCID_BODY 0xcc
#define GIMMICK_ARCID_PARTS 0xcf

// Models 0 to 4 show a player's Black level, 5 to 9 their White level, one lit each
#define MODEL_BLACK_LEVEL 0
#define MODEL_WHITE_LEVEL 5
#define MODEL_LEVEL_COUNT 10
// The models of the effects
enum {
    MODEL_EFFECT_0 = 10,
    MODEL_EFFECT_1,
    MODEL_START_EFFECT,
    MODEL_LEAVE_EFFECT,
    MODEL_COUNT,
};

// A level counts for one lit model every 6 levels, up to 4
#define LEVELS_PER_STEP 6
#define MAX_STEPS 4

// The x of the tile in front of the effect, and the z of the tiles that start and end it
#define TRIGGER_X FX32_CONST(504)
#define START_Z_MIN FX32_CONST(552)
#define START_Z_MAX FX32_CONST(568)
#define END_Z FX32_CONST(584)

// Black 2's, not checked against Black
typedef struct {
    FieldExpObjSystem *expObj;
    Field *field;
    u8 netId;
    BOOL effectOn;
} FunfestLevelsWork;

// What func_02026474 returns while the Entralink is running
// Black 2's, not checked against Black
typedef struct {
    u8 unk00[0x8a];
    u16 netId : 3;
    u16 unk8A_3 : 13;
} EntralinkCommWork;

static void GimmickFunfestLevels_InitModels(FunfestLevelsWork *work);
static void GimmickFunfestLevels_Free(FunfestLevelsWork *work);
static void GimmickFunfestLevels_StepAnimations(FunfestLevelsWork *work);
static void GimmickFunfestLevels_PlayAnimation(FieldExpObjSystem *expObj, u16 model, u16 anm, BOOL looped);
static void GimmickFunfestLevels_InitState(FunfestLevelsWork *work, Field *field, GameData *gameData,
                                           EntralinkCommWork *commWork);
static void GimmickFunfestLevels_HideLevels(FieldExpObjSystem *expObj);
static int GetLevelSteps(u16 level);
static u8 GimmickFunfestLevels_ShowLevels(GameData *gameData, FieldExpObjSystem *expObj, EntralinkCommWork *commWork);
EntralinkCommWork *func_ov060_021f3938(GameCommSys *comm);
static void GimmickFunfestLevels_ShowEffect0(FunfestLevelsWork *work);
static void GimmickFunfestLevels_PlayEffect1(FunfestLevelsWork *work);
static void GimmickFunfestLevels_ShowStartEffect(FunfestLevelsWork *work);
static void GimmickFunfestLevels_ShowLeaveEffect(FunfestLevelsWork *work);
static BOOL IsAtEffectStart(FunfestLevelsWork *work);
static BOOL IsAtEffectEnd(FunfestLevelsWork *work);

static const G3DSceneAnimationSetup sLeaveEffectAnimations[] = { { 11, 0 } };
static const G3DSceneAnimationSetup sEffect1Animations[] = { { 5, 0 } };
static const G3DSceneAnimationSetup sEffect0Animations[] = { { 2, 0 }, { 3, 0 } };
static const G3DSceneAnimationSetup sStartEffectAnimations[] = { { 7, 0 }, { 8, 0 }, { 9, 0 } };

static const int sAnimationCounts[MODEL_COUNT - MODEL_EFFECT_0] = { 2, 1, 3, 1 };

static const G3DSceneResourceSetup sFunfestLevelsResources[12] = {
    { GIMMICK_ARCID_BODY, 0, 0 },  { GIMMICK_ARCID_PARTS, 1, 0 },  { GIMMICK_ARCID_PARTS, 0, 0 },
    { GIMMICK_ARCID_PARTS, 2, 0 }, { GIMMICK_ARCID_PARTS, 3, 0 },  { GIMMICK_ARCID_PARTS, 4, 0 },
    { GIMMICK_ARCID_PARTS, 5, 0 }, { GIMMICK_ARCID_PARTS, 7, 0 },  { GIMMICK_ARCID_PARTS, 8, 0 },
    { GIMMICK_ARCID_PARTS, 6, 0 }, { GIMMICK_ARCID_PARTS, 10, 0 }, { GIMMICK_ARCID_PARTS, 9, 0 },
};

static const G3DSceneActorSetup sFunfestLevelsActors[MODEL_COUNT] = {
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
    { 1, 0, 1, 0, sEffect0Animations, NELEMS(sEffect0Animations) },
    { 4, 0, 4, 0, sEffect1Animations, NELEMS(sEffect1Animations) },
    { 6, 0, 6, 0, sStartEffectAnimations, NELEMS(sStartEffectAnimations) },
    { 10, 0, 10, 0, sLeaveEffectAnimations, NELEMS(sLeaveEffectAnimations) },
};

static const G3DSceneSetup sFunfestLevelsSceneSetup = { sFunfestLevelsResources, NELEMS(sFunfestLevelsResources),
                                                        sFunfestLevelsActors, NELEMS(sFunfestLevelsActors) };

// pokebw2's GetCommWork
EntralinkCommWork *func_ov060_021f3938(GameCommSys *comm) {
    if (GameCommSys_BootCheck(comm) == GAME_COMM_NO_INTRUDE) {
        return func_02026474(comm);
    }
    return NULL;
}

void func_ov060_021f3950(FunfestLevelsFlag *work, FieldExpObjSystem *param_2) {
    FieldExpObjAnm *anm;
    s32 anmIdx;

    anmIdx = 0;
    FieldExpObj_SetActorHidden(param_2, 0, 0x14, 1);
    do {
        FieldExpObj_SetAnm(param_2, 0, 0x14, (u16)anmIdx, 1);
        anm = FieldExpObj_GetAnmInfo(param_2, 0, 0x14, (u16)anmIdx);
        FieldExpObjAnm_SetLooped(anm, 0);
        FieldExpObjAnm_SetPaused(anm, 0);
        anmIdx = anmIdx + 1;
    } while (anmIdx < 3);
}

void func_ov060_021f399c(FunfestLevelsFlag *param_1, FieldExpObjSystem *param_2, VecFx32 *param_3) {
    SRTMatrix *matrix;
    s32 i;

    i = 0;
    FieldExpObj_SetActorHidden(param_2, 0, 0x14, 0);
    do {
        FieldExpObj_SetAnmFrame(param_2, 0, 0x14, (u16)i, 0);
        i++;
    } while (i < 3);
    matrix = FieldExpObj_GetActorMatrixPtr(param_2, 0, 0x14);
    matrix->translation = *param_3;
    param_1->active = 1;
}


void func_ov060_021f39f4(FunfestLevelsFlag *work, FieldExpObjSystem *param_2) {
    FieldExpObjAnm *anm;
    BOOL BVar1;

    if (work->active != 0) {
        anm = FieldExpObj_GetAnmInfo(param_2, 0, 0x14, 0);
        BVar1 = FieldExpObjAnm_IsPlaybackFinished(anm);
        if (BVar1 != 0) {
            FieldExpObj_SetActorHidden(param_2, 0, 0x14, 1);
            work->active = 0;
        }
    }
}
