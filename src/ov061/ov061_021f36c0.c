// Black's code at ov061 0x021f36c0-0x021f396c. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "gfl/g3d.h"
#include "struct_decls.h"
#include "system/game_system.h"
#include "system/game_data.h"
#include "field/zone.h"
#include "field/field_player.h"
#include "field/event_data.h"

u32 func_ov021_021a3080(FieldPlayer *player);
u32 func_ov021_021a308c(FieldPlayer *player);
void func_ov010_02162a64(ZoneWarp *warp, VecFx32 *pos);
BOOL func_ov010_02162478(EventData *eventData, const VecFx32 *pos);

// The gimmick work block: fields named by offset, the assembly's names unknown
typedef struct {
    u32 unk_0;
    SRTMatrix *matrix;
    u32 zoneId;
} Ov061GimmickWork;

void func_ov061_021f388c(Ov061GimmickWork *work, Field *param_2);
u32 func_ov061_021f3868(Ov061GimmickWork *work, FieldPlayer *player);

static const G3DSceneAnimationSetup sAnimations0[] = { { 1, 0 } };
static const G3DSceneAnimationSetup sAnimations1[] = { { 3, 0 } };

static const G3DSceneActorSetup sActors[9] = {
    { 0, 0, 0, 0, sAnimations0, NELEMS(sAnimations0) },
    { 2, 0, 2, 0, sAnimations1, NELEMS(sAnimations1) },
    { 2, 0, 2, 0, sAnimations1, NELEMS(sAnimations1) },
    { 2, 0, 2, 0, sAnimations1, NELEMS(sAnimations1) },
    { 2, 0, 2, 0, sAnimations1, NELEMS(sAnimations1) },
    { 2, 0, 2, 0, sAnimations1, NELEMS(sAnimations1) },
    { 2, 0, 2, 0, sAnimations1, NELEMS(sAnimations1) },
    { 2, 0, 2, 0, sAnimations1, NELEMS(sAnimations1) },
    { 2, 0, 2, 0, sAnimations1, NELEMS(sAnimations1) },
};

static const G3DSceneResourceSetup sResources[] = {
    { 0xd2, 1, 0, 0 },
    { 0xd2, 2, 0, 0 },
    { 0xd2, 7, 0, 0 },
    { 0xd2, 8, 0, 0 },
};

static const G3DSceneSetup sSceneSetup = { sResources, NELEMS(sResources), sActors, NELEMS(sActors) };

void func_ov061_021f36c0(Field *param_1) {
    u16 heapId;
    FieldExpObjSystem *system;
    Ov061GimmickWork *work;
    FieldExpObjAnm *anm;
    s32 actor;

    heapId = Field_GetHeapID(param_1);
    system = Field_GetExpObjSystem(param_1);
    actor = 0;
    work = Field_AllocGimmickWorkBlock(param_1, 0, heapId, sizeof(Ov061GimmickWork));
    LoadFieldExpandObjData(system, &sSceneSetup, 0);
    do {
        FieldExpObj_SetActorHidden(system, 0, (u16)actor, 1);
        actor = actor + 1;
    } while (actor < 9);
    work->matrix = FieldExpObj_GetActorMatrixPtr(system, 0, 0);
    anm = FieldExpObj_GetAnmInfo(system, 0, 0, 0);
    FieldExpObj_SetAnm(system, 0, 0, 0, 1);
    FieldExpObjAnm_SetPaused(anm, 1);
    FieldExpObjAnm_SetLooped(anm, 0);
    func_ov061_021f388c(work, param_1);
    work->zoneId = Field_GetPlayerStateZoneID(param_1);
}

void func_ov061_021f3754(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    FieldExpObj_FreeScene(system, 0);
    Field_DeleteGimmickWorkBlock(param_1, 0);
}

void func_ov061_021f376c(Field *param_1) {
    Ov061GimmickWork *work;
    FieldExpObjSystem *system;
    FieldPlayer *player;
    GameSystem *gsys;
    GameData *gameData;
    EventData *eventData;
    FieldExpObjAnm *anm;
    VecFx32 pos;
    u16 zoneId;
    u16 faceDir;

    system = Field_GetExpObjSystem(param_1);
    player = Field_GetPlayer(param_1);
    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    eventData = GameData_GetEventData(gameData);
    work = Field_GetGimmickWorkBlock(param_1, 0);
    zoneId = Field_GetPlayerStateZoneID(param_1);
    if (work->zoneId != zoneId) {
        func_ov061_021f388c(work, param_1);
        work->zoneId = zoneId;
    }
    anm = FieldExpObj_GetAnmInfo(system, 0, 0, 0);
    FieldExpObj_StepAllAnimations(system);
    if (work->unk_0 != 0 && FieldExpObjAnm_IsPlaybackFinished(anm)) {
        FieldExpObj_SetActorHidden(system, 0, 0, 1);
        work->unk_0 = 0;
    }
    if (func_ov061_021f3868(work, player)) {
        faceDir = FieldPlayer_GetFaceDir(player);
        FieldPlayer_GetWPosInDir(player, faceDir, &pos);
        pos.x -= 0x8000;
        pos.z -= 0x8000;
        if (func_ov010_02162478(eventData, &pos)) {
            work->unk_0 = 1;
            FieldExpObj_SetActorHidden(system, 0, 0, 0);
            anm = FieldExpObj_GetAnmInfo(system, 0, 0, 0);
            FieldExpObj_SetAnmFrame(system, 0, 0, 0, 0);
            FieldExpObjAnm_SetPaused(anm, 0);
            work->matrix->translation = pos;
        }
    }
}


u32 func_ov061_021f3868(Ov061GimmickWork *work, FieldPlayer *player) {
    u32 result;

    result = func_ov021_021a3080(player);
    if ((func_ov021_021a308c(player) == 3) && (result == 0)) {
        return 1;
    }
    return 0;
}

void func_ov061_021f388c(Ov061GimmickWork *work, Field *param_2) {
    GameSystem *gsys;
    GameData *gameData;
    EventData *eventData;
    FieldExpObjSystem *system;
    ZoneWarp *warp;
    SRTMatrix *matrix;
    VecFx32 pos;
    s32 i;
    int count;

    gsys = Field_GetGameSystem(param_2);
    gameData = GSYS_GetGameData(gsys);
    eventData = GameData_GetEventData(gameData);
    count = GetWarpAtPosition(eventData);
    Field_GetHeapID(param_2);
    system = Field_GetExpObjSystem(param_2);
    i = 0;
    do {
        FieldExpObj_SetActorHidden(system, 0, (u16)(i + 1), 1);
        i++;
    } while (i < 8);
    if (count > 0) {
        i = 0;
        do {
            warp = GetZoneWarpByID(eventData, (u16)i);
            if (warp != NULL && GetWarpTransitionType(warp) == 6) {
                func_ov010_02162a64(warp, &pos);
                if (Field_GetPlayerStateZoneID(param_2) == 0x12e) {
                    pos.y -= 0x2000;
                }
                FieldExpObj_SetActorHidden(system, 0, (u16)(i + 1), 0);
                matrix = FieldExpObj_GetActorMatrixPtr(system, 0, (u16)(i + 1));
                matrix->translation = pos;
                FieldExpObj_GetAnmInfo(system, 0, (u16)(i + 1), 0);
                FieldExpObj_SetAnm(system, 0, (u16)(i + 1), 0, 1);
            }
            i++;
        } while (i < count);
    }
}

