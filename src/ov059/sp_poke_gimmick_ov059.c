// Black's code at ov059 0x021f36c0-0x021f3e50. The name is the file name its asserts pass (tools/name_files.py).
// (sp_poke_gimmick_ov059.c: another module has a file of the name)

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_exp_obj.h"
#include "field/player_state.h"
#include "field/sp_poke_event.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "gfl/sound.h"
#include "field/sp_poke_gimmick.h"

GameEventReturnCode func_ov059_021f3c6c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov059_021f3cb8(GameEvent *event, u32 *state, void *data);

// Black's field expand object scene data of ov059, in ROM order: the animation lists, the scene setups, the actor lists
// and the resource lists (the scenes' ROM bytes, as the code loads them)
static const G3DSceneAnimationSetup data_ov059_021f3e50[3] = { { 5, 0 }, { 6, 0 }, { 7, 0 } };
static const G3DSceneAnimationSetup data_ov059_021f3e68[3] = { { 2, 0 }, { 3, 0 }, { 4, 0 } };
static const G3DSceneResourceSetup data_ov059_021f3fd4[12];
static const G3DSceneActorSetup data_ov059_021f3ee4[3];
static const G3DSceneSetup data_ov059_021f3e74 = { data_ov059_021f3fd4, 12, data_ov059_021f3ee4, 3 };
static const G3DSceneResourceSetup data_ov059_021f3f14[8];
static const G3DSceneActorSetup data_ov059_021f3ea4[2];
static const G3DSceneSetup data_ov059_021f3e84 = { data_ov059_021f3f14, 8, data_ov059_021f3ea4, 2 };
static const G3DSceneResourceSetup data_ov059_021f3f74[8];
static const G3DSceneActorSetup data_ov059_021f3ec4[2];
static const G3DSceneSetup data_ov059_021f3e94 = { data_ov059_021f3f74, 8, data_ov059_021f3ec4, 2 };
static const G3DSceneActorSetup data_ov059_021f3ea4[2] = {
    { 0, 0, 0, 0, data_ov059_021f3e68, 3 },
    { 1, 0, 1, 0, data_ov059_021f3e50, 3 },
};
static const G3DSceneActorSetup data_ov059_021f3ec4[2] = {
    { 0, 0, 0, 0, data_ov059_021f3e68, 3 },
    { 1, 0, 1, 0, data_ov059_021f3e50, 3 },
};
static const G3DSceneResourceSetup data_ov059_021f3f14[8] = {
    { 0xcc, 4, 0, 0 }, { 0xcc, 0, 0, 0 }, { 0xcc, 5, 0, 0 }, { 0xcc, 7, 0, 0 },
    { 0xcc, 6, 0, 0 }, { 0xcc, 1, 0, 0 }, { 0xcc, 3, 0, 0 }, { 0xcc, 2, 0, 0 },
};
static const G3DSceneResourceSetup data_ov059_021f3f74[8] = {
    { 0xcc, 4, 0, 0 }, { 0xcc, 0, 0, 0 }, { 0xcc, 5, 0, 0 }, { 0xcc, 7, 0, 0 },
    { 0xcc, 6, 0, 0 }, { 0xcc, 1, 0, 0 }, { 0xcc, 3, 0, 0 }, { 0xcc, 2, 0, 0 },
};
static const G3DSceneResourceSetup data_ov059_021f3fd4[12] = {
    { 0xcc, 4, 0, 0 }, { 0xcc, 0, 0, 0 }, { 0xcc, 5, 0, 0 }, { 0xcc, 7, 0, 0 },
    { 0xcc, 6, 0, 0 }, { 0xcc, 1, 0, 0 }, { 0xcc, 3, 0, 0 }, { 0xcc, 2, 0, 0 },
    { 0xda, 0, 0, 0 }, { 0xda, 1, 0, 0 }, { 0xda, 3, 0, 0 }, { 0xda, 2, 0, 0 },
};
static const G3DSceneAnimationSetup data_ov059_021f3e5c[3] = { { 9, 0 }, { 10, 0 }, { 11, 0 } };

static const G3DSceneActorSetup data_ov059_021f3ee4[3] = {
    { 0, 0, 0, 0, data_ov059_021f3e68, 3 },
    { 1, 0, 1, 0, data_ov059_021f3e50, 3 },
    { 8, 0, 8, 0, data_ov059_021f3e5c, 3 },
};
GameEventReturnCode func_ov059_021f3d9c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov059_021f3b90(GameEvent *event, u32 *state, void *data);


void func_ov059_021f36c0(Field *field) {
    int i;
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    FieldExpObjAnm *anm;

    Field_AllocGimmickWorkBlock(field, 1, Field_GetHeapID(field), sizeof(SpPokeGimmickBlock));
    LoadFieldExpandObjData(system, &data_ov059_021f3e94, 0);
    FieldExpObj_SetActorHidden(system, 0, 1, TRUE);
    FieldExpObj_SetActorHidden(system, 0, 0, TRUE);
    for (i = 0; i < 3; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 0, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(system, 0, 0, i, FALSE);
        anm = FieldExpObj_GetAnmInfo(system, 0, 1, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(system, 0, 1, i, FALSE);
    }
}

void func_ov059_021f376c(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    Field_DeleteGimmickWorkBlock(param_1, 1);
    FieldExpObj_FreeScene(system, 0);
}

void func_ov059_021f3788(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    FieldExpObj_StepAllAnimations(system);
}

void func_ov059_021f3794(Field *field) {
    int i;
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    FieldExpObjAnm *anm;

    Field_AllocGimmickWorkBlock(field, 1, Field_GetHeapID(field), sizeof(SpPokeGimmickBlock));
    LoadFieldExpandObjData(system, &data_ov059_021f3e84, 0);
    FieldExpObj_SetActorHidden(system, 0, 1, TRUE);
    FieldExpObj_SetActorHidden(system, 0, 0, TRUE);
    for (i = 0; i < 3; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 0, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(system, 0, 0, i, FALSE);
        anm = FieldExpObj_GetAnmInfo(system, 0, 1, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(system, 0, 1, i, FALSE);
    }
}


void func_ov059_021f3840(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    Field_DeleteGimmickWorkBlock(param_1, 1);
    FieldExpObj_FreeScene(system, 0);
}

void func_ov059_021f385c(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    FieldExpObj_StepAllAnimations(system);
}

void func_ov059_021f3868(Field *field) {
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    int heapId = (Field_GetHeapID(field) & 0x7fff) | 0x8000;
    SpPokeGimmickBlock *work;
    FieldExpObjAnm *anm;
    int i;

    Field_AllocGimmickWorkBlock(field, 1, heapId, sizeof(SpPokeGimmickBlock));
    work = Field_GetGimmickWorkBlock(field, 1);
    work->npc = GFL_HeapAllocate(heapId, sizeof(SpPokeGimmickNpc), TRUE, "sp_poke_gimmick.c", 0x1d1);
    FieldExpObj_AddScene(system, &data_ov059_021f3e74, 0);
    FieldExpObj_SetActorHidden(system, 0, 1, TRUE);
    FieldExpObj_SetActorHidden(system, 0, 0, TRUE);
    for (i = 0; i < 3; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 0, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(system, 0, 0, i, FALSE);
        anm = FieldExpObj_GetAnmInfo(system, 0, 1, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(system, 0, 1, i, FALSE);
    }
    for (i = 0; i < 3; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 2, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
        FieldExpObj_SetAnm(system, 0, 2, i, FALSE);
    }
}


void func_ov059_021f3980(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    GFL_HeapFree(((SpPokeGimmickBlock *)Field_GetGimmickWorkBlock(param_1, 1))->npc);
    Field_DeleteGimmickWorkBlock(param_1, 1);
    FieldExpObj_FreeScene(system, 0);
}

void func_ov059_021f39ac(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    FieldExpObj_StepAllAnimations(system);
}

GameEvent *func_ov059_021f39b8(GameSystem *gsys, u32 mode, const VecFx32 *start, const VecFx32 *end, fx32 height,
                               u32 frames) {
    Field *field = GSYS_GetField(gsys);
    SpPokeGimmickBlock *work = Field_GetGimmickWorkBlock(field, 1);
    int actor = 1;
    int i;
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    GameEvent *event;

    if (mode == 0) {
        actor = 0;
        GFL_SndSEPlay(0x566);
    }
    FieldExpObj_GetActorMatrixPtr(system, 0, actor)->translation = *start;
    FieldExpObj_SetActorHidden(system, 0, actor, FALSE);
    for (i = 0; i < 3; i++) {
        FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, actor, i), TRUE);
        FieldExpObj_SetAnm(system, 0, actor, i, FALSE);
        FieldExpObj_SetAnmFrame(system, 0, actor, i, 0);
    }
    work->start = *start;
    work->end = *end;
    work->frames = frames;
    work->frame = 0;
    work->height = height;
    event = GameEvent_Create(gsys, NULL, func_ov059_021f3b90, 0);
    work->mode = mode;
    return event;
}


void func_ov059_021f3aa0(GameSystem *gsys, u16 param_2, const VecFx32 *pos) {
    Field *field = GSYS_GetField(gsys);
    int actor = 1;
    int i;
    FieldExpObjSystem *system;

    Field_GetGimmickWorkBlock(field, 1);
    system = Field_GetExpObjSystem(field);
    if (param_2 == 0) {
        actor = 0;
        GFL_SndSEPlay(0x567);
    } else {
        GFL_SndSEPlay(0x793);
    }
    FieldExpObj_GetActorMatrixPtr(system, 0, actor)->translation = *pos;
    FieldExpObj_SetActorHidden(system, 0, actor, FALSE);
    for (i = 0; i < 3; i++) {
        FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, actor, i), FALSE);
        FieldExpObj_SetAnm(system, 0, actor, i, TRUE);
        FieldExpObj_SetAnmFrame(system, 0, actor, i, 0);
    }
}


GameEvent *func_ov059_021f3b40(GameSystem *gsys, u16 a1) {
    Field *field;
    SpPokeGimmickBlock *work;
    GameEvent *pGVar2;

    field = GSYS_GetField(gsys);
    work = Field_GetGimmickWorkBlock(field, 1);
    pGVar2 = GameEvent_Create(gsys, NULL, func_ov059_021f3c6c, 0);
    work->mode = a1;
    return pGVar2;
}

GameEvent *func_ov059_021f3b68(GameSystem *gsys, u16 a1) {
    Field *field;
    SpPokeGimmickBlock *work;
    GameEvent *pGVar2;

    field = GSYS_GetField(gsys);
    work = Field_GetGimmickWorkBlock(field, 1);
    pGVar2 = GameEvent_Create(gsys, NULL, func_ov059_021f3cb8, 0);
    work->mode = a1;
    return pGVar2;
}

static inline fx32 SpPokeGimmick_Lerp(fx32 start, fx32 end, int frame, int frames) {
    fx32 diff = end - start;
    return start + diff * frame / frames;
}

GameEventReturnCode func_ov059_021f3b90(GameEvent *event, u32 *state, void *data) {
    Field *field = GSYS_GetField(GameEvent_GetGameSystem(event));
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    SpPokeGimmickBlock *work = Field_GetGimmickWorkBlock(field, 1);
    int actor = 1;
    VecFx32 pos;
    int arcFrame;

    if (work->mode == 0) {
        actor = 0;
    }
    work->frame++;
    pos.x = SpPokeGimmick_Lerp(work->start.x, work->end.x, work->frame, work->frames);
    pos.y = SpPokeGimmick_Lerp(work->start.y, work->end.y, work->frame, work->frames);
    pos.z = SpPokeGimmick_Lerp(work->start.z, work->end.z, work->frame, work->frames);
    arcFrame = work->frame;
    if (arcFrame >= (work->frames + 1) / 2) {
        arcFrame = work->frames - arcFrame;
    }
    pos.y += work->height * arcFrame / work->frames;
    FieldExpObj_GetActorMatrixPtr(system, 0, actor)->translation = pos;
    if (work->frame >= work->frames) {
        if (work->mode == 1) {
            FieldExpObj_SetActorHidden(system, 0, actor, TRUE);
        }
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode func_ov059_021f3c6c(GameEvent *event, u32 *state, void *data) {
    Field *field = GSYS_GetField(GameEvent_GetGameSystem(event));
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    SpPokeGimmickBlock *work = Field_GetGimmickWorkBlock(field, 1);
    fx32 frame;
    s8 actor = 1;

    if (work->mode == 0) {
        frame = FX32_CONST(5);
        actor = 0;
    } else {
        frame = FX32_CONST(34);
    }
    if (func_ov021_021be6e8(system, 0, actor, 0) >= frame) {
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode func_ov059_021f3cb8(GameEvent *event, u32 *state, void *data) {
    Field *field = GSYS_GetField(GameEvent_GetGameSystem(event));
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    SpPokeGimmickBlock *work = Field_GetGimmickWorkBlock(field, 1);
    s8 actor = 1;

    if (work->mode == 0) {
        actor = 0;
    }
    if (FieldExpObjAnm_IsPlaybackFinished(FieldExpObj_GetAnmInfo(system, 0, actor, 0))) {
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov059_021f3cfc(GameSystem *gsys, const VecFx32 *pos, u16 actorId, u16 uid) {
    Field *field = GSYS_GetField(gsys);
    SpPokeGimmickBlock *work = Field_GetGimmickWorkBlock(field, 1);
    int i;
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    SpPokeGimmickNpc *npc = work->npc;

    npc->actorId = actorId;
    npc->uid = uid;
    FieldExpObj_GetActorMatrixPtr(system, 0, 2)->translation = *pos;
    FieldExpObj_SetActorHidden(system, 0, 2, FALSE);
    for (i = 0; i < 3; i++) {
        FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, 2, i), FALSE);
        FieldExpObj_SetAnm(system, 0, 2, i, TRUE);
        FieldExpObj_SetAnmFrame(system, 0, 2, i, 0);
    }
    return GameEvent_Create(gsys, NULL, func_ov059_021f3d9c, 0);
}



GameEventReturnCode func_ov059_021f3d9c(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys;
    Field *field;
    FieldExpObjSystem *system;
    SpPokeGimmickBlock *work;
    u32 count;
    u16 id;
    u16 uid;
    MMSys *mmSys;
    FieldActor *pFVar4;
    GameData *gameData;
    EventData *eventData;
    PlayerState *playerState;
    EventWork *eventWork;
    ZoneNPC *npcs;
    FieldExpObjAnm *anm;
    BOOL BVar5;


    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    system = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    count = func_ov021_021be6e8(system, 0, 2, 0);
    if (count == 0xf000) {
        id = work->npc->actorId;
        uid = work->npc->uid;
        mmSys = Field_GetActorSystem(field);
        pFVar4 = FindFieldActor(mmSys, id);
        DeleteActor(pFVar4);
        gameData = GSYS_GetGameData(gsys);
        eventData = GameData_GetEventData(gameData);
        count = GetZoneNPCsCount(eventData);
        playerState = GameData_GetPlayerState(gameData);
        id = PlayerState_GetZoneID(playerState);
        if (count != 0) {
            eventWork = GameData_GetEventWork(gameData);
            npcs = GetZoneNPCs(eventData);
            pFVar4 = func_ov010_0216cee4(mmSys, npcs, (u32)id, count, eventWork, uid);
            if (pFVar4 != (FieldActor *)0x0) {
                DisableActorMovement(pFVar4);
            }
        }
    }
    anm = FieldExpObj_GetAnmInfo(system, 0, 2, 0);
    BVar5 = FieldExpObjAnm_IsPlaybackFinished(anm);
    return BVar5 != FALSE ? GAMEEVENT_DONE : GAMEEVENT_CONTINUE;
}
