// Black's code at ov056 0x021f36c0-0x021f4258. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field.h"
#include "field/field_buildmodel.h"
#include "field/field_camera.h"
#include "field/field_g3d_mapper.h"
#include "field/field_prop.h"
#include "field/field_sound.h"
#include "field/pokemon_league_gimmicks.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nnsys/g3d.h"
#include "save/event_work.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "field/field_actor.h"
#include "app/psel_graphic.h"
#include "gfl/arc.h"
#include "save/player_info.h"
#include "system/ica_anime.h"

static const u32 data_ov056_021f42b0[4] = { 28, 29, 30, 31 };
static const VecFx32 data_ov056_021f4308[4] = {
#include "asset/field_fk_sound_anime_data_ov056_021f4308.inc"
};
static const u16 data_ov056_021f4264[4] = { 0, 3, 0, 0 };
BOOL func_ov021_0218c8b0(FieldPropHandle *handle, u32 animation);
void func_ov021_0218ed14(FieldCamera *camera, VecFx32 *up);
void func_ov021_0218ed20(FieldCamera *camera, VecFx32 *up);

// The camera that follows the event's curve, at 0xc in the event's work
typedef struct FkSoundAnimeCamera {
    G3DCurve *curve;
    G3DCamera *camera;
    FieldCamera *fieldCamera;
    u16 transformType;
    u16 useBoundary;
    VecFx32 targetOffset;
    VecFx32 up;
    fx32 frame;
    u32 defaultsIndex;
} FkSoundAnimeCamera;

// The player's model that func_ov056_021f3b10 loads, at 0x3c in the event's work (fields named by offset where
// nothing shows their use)
typedef struct FkSoundAnimeModel {
    void *mdlRes;
    void *anmRes[2];
    u32 loaded;
    G3DModel *model;
    void *anm[2];
    G3DActor *g3dActor;
    SRTMatrix transform;
    u32 unk5C;
    PlayerInfo *playerInfo;
    FieldPlayer *player;
    FieldActor *actor;
    VecFx32 gpos;
    u16 dir;
    fx32 frame;
} FkSoundAnimeModel;

// The prop the event animates, at 0xbc in the event's work
typedef struct FkSoundAnimeProp {
    FieldPropHandle *handle;
    FieldChunkPropHolder *holder;
    FieldPropSystem *system;
} FkSoundAnimeProp;

// The sound effects a curve plays (func_ov056_021f4040): each of its nine channels at 0x1000 plays its sound effect,
// at 0x2000 stops its sequence's player
typedef struct FkSoundAnimeSeCurve {
    G3DCurve *curve;
    fx32 frame;
    u16 se[9];
} FkSoundAnimeSeCurve;

// The event's work (func_ov056_021f36f0)
typedef struct FkSoundAnimeWork {
    Field *field;
    GameSystem *gsys;
    u32 room;
    FkSoundAnimeCamera camera;
    FkSoundAnimeModel model;
    FkSoundAnimeProp prop;
    FkSoundAnimeSeCurve *seCurve;
    FieldAsyncProc *drawProc;
    TCB *vblankTask;
} FkSoundAnimeWork;

// The work of the prop sound that func_ov056_021f3dcc sets up and func_ov056_021f3e54 runs
typedef struct FkSoundAnimePropSound {
    FieldPropSystem *system;
    FieldChunkPropHolder *holder;
    fx32 frame;
    u32 index;
} FkSoundAnimePropSound;

// The work of the prop ambience that func_ov056_021f3e94 sets up and func_ov056_021f3ecc runs
typedef struct FkSoundAnimeAmbience {
    FieldPropSystem *system;
    FieldChunkPropHolder *holder;
    u16 playing;
    s16 timer;
    FieldSound *sound;
} FkSoundAnimeAmbience;

// The work of the ambiences that play until event work 0x4001 is set (func_ov056_021f3f30, func_ov056_021f3fb8)
typedef struct FkSoundAnimeFlagAmbience {
    u16 *flag;
    u16 stopped;
    EventWork *eventWork;
    FieldSound *sound;
} FkSoundAnimeFlagAmbience;

void func_ov056_021f36e4(TCB *tcb, void *data);
GameEvent *func_ov056_021f36f0(GameSystem *gsys, Field *field, u32 room);
GameEventReturnCode func_ov056_021f3734(GameEvent *event, u32 *seq, void *data);
void func_ov056_021f3890(FkSoundAnimeProp *prop, FieldPropSystem *system, s32 room);
void func_ov056_021f38d4(FkSoundAnimeProp *prop);
void func_ov056_021f3904(FkSoundAnimeProp *prop);
BOOL func_ov056_021f3918(FkSoundAnimeProp *prop);
void func_ov056_021f392c(FkSoundAnimeCamera *work, FieldCamera *fieldCamera, HeapID heapId, s32 index);
void func_ov056_021f3a14(FkSoundAnimeCamera *work);
BOOL func_ov056_021f3a78(FkSoundAnimeCamera *work);
void func_ov056_021f3ab0(FkSoundAnimeCamera *work);
void func_ov056_021f3b10(FkSoundAnimeModel *model, FieldPlayer *player, PlayerInfo *playerInfo, u16 heapId, long index);
void func_ov056_021f3bf0(FkSoundAnimeModel *model);
void func_ov056_021f3c4c(FkSoundAnimeModel *model);
BOOL func_ov056_021f3c84(FkSoundAnimeModel *model);
void func_ov056_021f3cc4(FkSoundAnimeModel *model, BOOL show);
void func_ov056_021f3d2c(FkSoundAnimeModel *model);
void func_ov056_021f3d54(FieldAsyncProc *proc, Field *field, void *work);
void func_ov056_021f3d60(FkSoundAnimeSeCurve **seCurve, s32 room, HeapID heapId);
void func_ov056_021f3d9c(FkSoundAnimeSeCurve **seCurve);
BOOL func_ov056_021f3db8(FkSoundAnimeSeCurve **seCurve);
FkSoundAnimeSeCurve *func_ov056_021f4040(const u16 *se, u32 arcId, u32 fileId, u32 preloadCount, HeapID heapId);
void func_ov056_021f408c(FkSoundAnimeSeCurve *seCurve);
BOOL func_ov056_021f40a0(FkSoundAnimeSeCurve *seCurve);
void func_ov056_021f4224(FkSoundAnimeSeCurve *seCurve, s32 channel);
void func_ov056_021f423c(FkSoundAnimeSeCurve *seCurve, s32 channel);

GameEvent *func_ov056_021f36c0(GameSystem *gsys, Field *field, u16 room) {
    return func_ov056_021f36f0(gsys, field, room);
}

u32 func_ov056_021f36c8(u16 room) {
    return data_ov056_021f42b0[room];
}

s32 func_ov056_021f36d4(void *anm) {
    return NNS_G3dAnmObjGetNumFrame(GFL_G3DAnmGetRenderObj(anm));
}

void func_ov056_021f36e4(TCB *tcb, void *data) {
    FkSoundAnimeWork *work = data;

    func_ov056_021f3d2c(&work->model);
}


GameEvent *func_ov056_021f36f0(GameSystem *gsys, Field *field, u32 room) {
    GameEvent *event;
    FkSoundAnimeWork *work;

    Field_GetHeapID(field);
    event = GameEvent_Create(gsys, NULL, func_ov056_021f3734, sizeof(FkSoundAnimeWork));
    work = GameEvent_GetData(event);
    work->field = field;
    work->gsys = gsys;
    work->room = room;
    work->vblankTask = GFL_VBlankTCBAdd(func_ov056_021f36e4, work, 0);
    return event;
}

// The draw process's work: the model it draws
static const FieldAsyncProcDef data_ov056_021f42c0 = {
    0, sizeof(FkSoundAnimeModel *), NULL, NULL, NULL, func_ov056_021f3d54,
};

GameEventReturnCode func_ov056_021f3734(GameEvent *event, u32 *seq, void *data) {
    FkSoundAnimeWork *work = data;
    HeapID heapId;
    PlayerInfo *playerInfo;
    FieldCamera *camera;
    FkSoundAnimeModel **drawWork;
    BOOL curveDone;
    BOOL modelDone;
    BOOL propDone;

    heapId = Field_GetHeapID(work->field);
    switch (*seq) {
    case 0:
        playerInfo = GetGameDataPlayerInfo(GSYS_GetGameData(work->gsys));
        func_ov056_021f3b10(&work->model, Field_GetPlayer(work->field), playerInfo, heapId, work->room);
        (*seq)++;
        break;
    case 1:
        FieldCamera_FinishDelay(Field_GetCameraSystem(work->field));
        (*seq)++;
        break;
    case 2:
        camera = Field_GetCameraSystem(work->field);
        if (FieldCamera_IsDelayActive(camera)) {
            break;
        }
        func_ov056_021f392c(&work->camera, camera, heapId, work->room);
        func_ov056_021f3d60(&work->seCurve, work->room, heapId);
        func_ov056_021f3890(&work->prop, FieldG3DMapper_GetBMSystem(Field_GetG3DMapper(work->field)), work->room);
        func_ov056_021f3bf0(&work->model);
        work->drawProc = FieldAsyncProcManager_AddProc(0xffffffff, Field_GetAsyncProcMgr(work->field), &data_ov056_021f42c0);
        drawWork = FieldAsyncProc_GetData(work->drawProc);
        *drawWork = &work->model;
        func_ov056_021f3cc4(&work->model, TRUE);
        func_ov056_021f3904(&work->prop);
        (*seq)++;
        break;
    case 3:
        curveDone = func_ov056_021f3a78(&work->camera);
        func_ov056_021f3c84(&work->model);
        propDone = func_ov056_021f3db8(&work->seCurve);
        modelDone = func_ov056_021f3918(&work->prop);
        if (curveDone && modelDone && propDone) {
            (*seq)++;
        }
        break;
    case 4:
        func_ov056_021f3cc4(&work->model, FALSE);
        FieldAsyncProc_End(work->drawProc);
        GFL_TCBRemove(work->vblankTask);
        func_ov056_021f3c4c(&work->model);
        func_ov056_021f3d9c(&work->seCurve);
        func_ov056_021f3a14(&work->camera);
        func_ov056_021f38d4(&work->prop);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


static const u32 data_ov056_021f4290[4] = { 0, 0, 1, 1 };
static const VecFx32 data_ov056_021f42d8[4] = {
#include "asset/field_fk_sound_anime_data_ov056_021f42d8.inc"
};

void func_ov056_021f3890(FkSoundAnimeProp *prop, FieldPropSystem *system, s32 room) {
    if (data_ov056_021f4290[room] != 0) {
        prop->handle = FieldPropSystem_CreateHandleAtPos(system, 0, (VecFx32 *)&data_ov056_021f42d8[room]);
        prop->holder = FieldPropSystem_FindPropAtPos(system, 0, (VecFx32 *)&data_ov056_021f42d8[room]);
        prop->system = system;
        return;
    }
    prop->handle = NULL;
    prop->holder = NULL;
}

void func_ov056_021f38d4(FkSoundAnimeProp *prop) {
    if (prop->handle != NULL) {
        FieldPropHandle_Free(prop->handle);
        FieldChunkPropHolder_CallAnmCmd(prop->system, prop->holder, 0, 1);
        FieldChunkPropHolder_CallAnmCmd(prop->system, prop->holder, 0, 3);
        prop->handle = NULL;
        prop->holder = NULL;
        prop->system = NULL;
    }
}

void func_ov056_021f3904(FkSoundAnimeProp *prop) {
    if (prop->handle != NULL) {
        FieldPropHandle_CallAnmCmd(prop->handle, 0, 0);
    }
}

BOOL func_ov056_021f3918(FkSoundAnimeProp *prop) {
    if (prop->handle != NULL) {
        return func_ov021_0218c8b0(prop->handle, 0);
    }
    return TRUE;
}

// The up vector is a local of an inline helper: it sits below the stack slot of func_ov056_021f392c's later block,
// and its initializer is an object of its own, after the file's shared .rodata
static inline G3DCamera *FkSoundAnime_CreateCamera(VecFx32 *pos, VecFx32 *target, HeapID heapId) {
    VecFx32 up = { 0, FX32_ONE, 0 };
    return GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)), 0x1555, 0, FX32_ONE,
                               FX32_CONST(1024), 0, pos, &up, target, heapId);
}

void func_ov056_021f392c(FkSoundAnimeCamera *work, FieldCamera *fieldCamera, HeapID heapId, s32 index) {
    VecFx32 target;
    VecFx32 pos;
    G3DCamera *camera;

    work->frame = 0;
    work->curve = GFL_G3DCurveLoadFileStream(heapId, 0x96, index + 0x18, 10);
    camera = FieldCamera_GetG3DCamera(fieldCamera);
    GFL_G3DCameraGetLookatTarget(camera, &target);
    GFL_G3DCameraGetLookatPos(camera, &pos);
    work->camera = FkSoundAnime_CreateCamera(&pos, &target, heapId);
    work->fieldCamera = fieldCamera;
    work->transformType = FieldCamera_GetTransformType(work->fieldCamera);
    FieldCamera_ChangeTransformType(work->fieldCamera, 2);
    work->useBoundary = FieldCamera_IsUseBoundaryEnable(work->fieldCamera);
    FieldCamera_SetUseBoundaryEnable(work->fieldCamera, FALSE);
    FieldCamera_ClearBind(work->fieldCamera);
    {
        VecFx32 offset = {0, 0, 0};
        FieldCamera_CoordsGetTargetOffset(work->fieldCamera, &work->targetOffset);
        FieldCamera_CoordsSetTargetOffset(work->fieldCamera, &offset);
    }
    func_ov021_0218ed20(work->fieldCamera, &work->up);
    func_ov056_021f3ab0(work);
    work->defaultsIndex = data_ov056_021f42b0[index];
}


void func_ov056_021f3a14(FkSoundAnimeCamera *work) {
    VecFx32 *target;

    FieldCamera_ResetBind(work->fieldCamera);
    target = FieldCamera_GetBind(work->fieldCamera);
    FieldCamera_CoordsSetTarget(work->fieldCamera, target);
    FieldCamera_CoordsSetTargetOffset(work->fieldCamera, &work->targetOffset);
    FieldCamera_ChangeTransformType(work->fieldCamera, work->transformType);
    FieldCamera_SetUseBoundaryEnable(work->fieldCamera, work->useBoundary);
    func_ov021_0218ed14(work->fieldCamera, &work->up);
    FieldCamera_SetDefaultsIndex(work->fieldCamera, work->defaultsIndex);
    FieldCamera_EnableDelay(work->fieldCamera);
    GFL_G3DCameraFree(work->camera);
    GFL_G3DCurveFree(work->curve);
    MI_CpuFill8(work, 0, sizeof(FkSoundAnimeCamera));
}


BOOL func_ov056_021f3a78(FkSoundAnimeCamera *work) {
    BOOL ret;
    s32 last;
    s32 next;

    last = GFL_G3DCurveGetFrameCount(work->curve) << 12;
    next = work->frame + 0x1000;
    if (last <= next) {
        ret = TRUE;
        work->frame = last - 0x1000;
    } else {
        ret = FALSE;
        work->frame = next;
    }
    GFL_G3DCurveFrameSet(work->curve, work->frame);
    func_ov056_021f3ab0(work);
    return ret;
}

static const VecFx32 data_ov056_021f426c = { 0x100000, 0, 0x100000 };

void func_ov056_021f3ab0(FkSoundAnimeCamera *work) {
    VecFx32 target;
    VecFx32 eye;
    VecFx32 up;

    GFL_G3DCurveApplyCamera(work->camera, work->curve);
    GFL_G3DCameraGetLookatPos(work->camera, &eye);
    GFL_G3DCameraGetLookatTarget(work->camera, &target);
    GFL_G3DCameraGetLookatUpVector(work->camera, &up);
    VEC_Add(&eye, &data_ov056_021f426c, &eye);
    VEC_Add(&target, &data_ov056_021f426c, &target);
    FieldCamera_CoordsSetEye(work->fieldCamera, &eye);
    FieldCamera_CoordsSetTarget(work->fieldCamera, &target);
    func_ov021_0218ed14(work->fieldCamera, &up);
}

void func_ov056_021f3b10(FkSoundAnimeModel *model, FieldPlayer *player, PlayerInfo *playerInfo, u16 heapId, long index) {
    ArcTool *handle;

    handle = GFL_ArcSysCreateFileHandle(0x96, (heapId & 0x7fff) | 0x8000);
    model->frame = 0;
    model->unk5C = 0;
    model->gpos = data_ov056_021f4308[index];
    model->playerInfo = playerInfo;
    model->dir = data_ov056_021f4264[index];
    model->player = player;
    model->actor = FieldPlayer_GetActor(player);
    model->transform.translation = data_ov056_021f426c;
    model->transform.scale.x = FX32_ONE;
    model->transform.scale.y = FX32_ONE;
    model->transform.scale.z = FX32_ONE;
    MTX_Identity33_(&model->transform.rotation);
    if (getTrainerGender(playerInfo) == 0) {
        model->mdlRes = GFL_G3DSysReadArcToolResource(handle, index * 6 + 3);
        model->anmRes[1] = GFL_G3DSysReadArcToolResource(handle, index * 6 + 4);
    } else {
        model->mdlRes = GFL_G3DSysReadArcToolResource(handle, index * 6 + 1);
        model->anmRes[1] = GFL_G3DSysReadArcToolResource(handle, index * 6 + 2);
    }
    model->anmRes[0] = GFL_G3DSysReadArcToolResource(handle, index * 6 + 5);
    GFL_G3DResSetupTexData(model->mdlRes);
    model->loaded = TRUE;
    GFL_ArcToolFree(handle);
}


void func_ov056_021f3bf0(FkSoundAnimeModel *model) {
    s32 i;
    fx32 frame;

    model->model = GFL_G3DMdlCreate(model->mdlRes, 0, model->mdlRes);
    model->anm[0] = GFL_G3DAnmCreate(model->model, model->anmRes[0], 0);
    model->anm[1] = GFL_G3DAnmCreate(model->model, model->anmRes[1], 0);
    model->g3dActor = GFL_G3DActorCreate(model->model, model->anm, 2);
    frame = 0;
    for (i = 0; i < 2; i++) {
        GFL_G3DActorSetAnmFrame(model->g3dActor, i, &frame);
        GFL_G3DActorBindAnm(model->g3dActor, i);
    }
}




void func_ov056_021f3c4c(FkSoundAnimeModel *model) {
    GFL_G3DActorFree(model->g3dActor);
    GFL_G3DAnmFree(model->anm[0]);
    GFL_G3DAnmFree(model->anm[1]);
    GFL_G3DMdlFree(model->model);
    GFL_G3DResFreeTexData(model->mdlRes);
    GFL_G3DResFree(model->mdlRes);
    GFL_G3DResFree(model->anmRes[0]);
    GFL_G3DResFree(model->anmRes[1]);
}

BOOL func_ov056_021f3c84(FkSoundAnimeModel *model) {
    BOOL ret;
    s32 end;
    s32 next;

    end = func_ov056_021f36d4(model->anm[0]);
    next = model->frame + 0x1000;
    if (end <= next) {
        ret = TRUE;
        model->frame = end - 0x1000;
    } else {
        ret = FALSE;
        model->frame = next;
    }
    GFL_G3DActorSetAnmFrame(model->g3dActor, 0, &model->frame);
    GFL_G3DActorSetAnmFrame(model->g3dActor, 1, &model->frame);
    return ret;
}

void func_ov056_021f3cc4(FkSoundAnimeModel *model, BOOL show) {
    VecFx32 pos;

    if (show) {
        model->unk5C = 1;
        SetActorHidden(model->actor, 1);
        return;
    }
    model->unk5C = 0;
    SetActorHidden(model->actor, 0);
    SetActorGPos(model->actor, model->gpos.x, model->gpos.y, model->gpos.z, model->dir);
    CheckRecalcActorY(model->actor);
    CopyActorWPos(model->actor, &pos);
    FieldPlayer_SetWPos(model->player, &pos);
    FldAct_SetReflectingFlag(model->actor, 0);
}



void func_ov056_021f3d2c(FkSoundAnimeModel *model) {
    if (model->loaded) {
        GFL_G3DResUploadTexDataCore(model->mdlRes);
        model->loaded = FALSE;
    }
}

void func_ov056_021f3d44(FkSoundAnimeModel *model) {
    GFL_G3DSysDrawObj(model->g3dActor, &model->transform);
}

void func_ov056_021f3d54(FieldAsyncProc *proc, Field *field, void *work) {
    func_ov056_021f3d44(*(FkSoundAnimeModel **)work);
}


static const u32 data_ov056_021f42a0[4] = { 1, 1, 1, 1 };
// The ambience sounds of each room, 0xffff past the last
static const u16 data_ov056_021f43a4[4][9] = {
#include "asset/field_fk_sound_anime_data_ov056_021f43a4.inc"
};

void func_ov056_021f3d60(FkSoundAnimeSeCurve **seCurve, s32 room, HeapID heapId) {
    if (data_ov056_021f42a0[room] != 0) {
        *seCurve = func_ov056_021f4040(data_ov056_021f43a4[room], 0x96, room + 0x1c, 10, heapId);
        return;
    }
    MI_CpuFill8(seCurve, 0, sizeof(*seCurve));
}

void func_ov056_021f3d9c(FkSoundAnimeSeCurve **seCurve) {
    if (*seCurve != NULL) {
        func_ov056_021f408c(*seCurve);
    }
    MI_CpuFill8(seCurve, 0, sizeof(*seCurve));
}

BOOL func_ov056_021f3db8(FkSoundAnimeSeCurve **seCurve) {
    BOOL result;

    result = TRUE;
    if (*seCurve != NULL) {
        result = func_ov056_021f40a0(*seCurve);
    }
    return result;
}

static const VecFx32 data_ov056_021f4278 = { 0x100000, 0, 0xf0000 };
static const u32 data_ov056_021f4258[1] = { 0x6000 };

void func_ov056_021f3dcc(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimePropSound *work = data;
    FieldG3DMapper *mapper;
    FieldPropSystem *system;
    FieldChunkPropHolder *holder;
    GameSystem *gsys;
    GameData *gameData;
    FieldSound *fieldSound;
    u32 result;
    u32 i;

    mapper = Field_GetG3DMapper(field);
    system = FieldG3DMapper_GetBMSystem(mapper);
    holder = FieldPropSystem_FindPropAtPos(system, 0, &data_ov056_021f4278);
    work->holder = holder;
    work->system = system;
    result = func_ov021_0218c6f4(system, holder, 1);
    work->frame = 0x6000;
    work->index = 0;
    for (i = 0; i < 1; i++) {
        if ((s32)result < (s32)data_ov056_021f4258[i]) {
            work->frame = data_ov056_021f4258[i];
            work->index = i;
            break;
        }
    }
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    fieldSound = GameData_GetFieldSoundSystem(gameData);
    FieldSnd_PlayAmbience(fieldSound, 0x746);
}


void func_ov056_021f3e38(FieldAsyncProc *proc, Field *field, void *data) {
    GameSystem *gsys;
    GameData *gameData;
    FieldSound *fieldSound;

    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    fieldSound = GameData_GetFieldSoundSystem(gameData);
    FieldSnd_StopAmbience(fieldSound, 0x746);
}

void func_ov056_021f3e54(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimePropSound *work = data;
    GameSystem *gsys;
    GameData *gameData;
    FieldSound *fieldSound;

    if (work->frame == func_ov021_0218c6f4(work->system, work->holder, 1)) {
        gsys = Field_GetGameSystem(field);
        gameData = GSYS_GetGameData(gsys);
        fieldSound = GameData_GetFieldSoundSystem(gameData);
        work->index = 0;
        work->frame = data_ov056_021f4258[work->index];
        FieldSnd_PlayAmbience(fieldSound, 0x692);
    }
}

const FieldAsyncProcDef data_ov056_021f4344 = {
    0x80, sizeof(FkSoundAnimePropSound), func_ov056_021f3dcc, func_ov056_021f3e38, func_ov056_021f3e54, NULL,
};

static const VecFx32 data_ov056_021f4284 = { 0x76000, 0, 0xec000 };

void func_ov056_021f3e94(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimeAmbience *work = data;
    FieldG3DMapper *mapper;
    FieldPropSystem *system;
    GameSystem *gsys;
    GameData *gameData;

    mapper = Field_GetG3DMapper(field);
    system = FieldG3DMapper_GetBMSystem(mapper);
    work->holder = FieldPropSystem_FindPropAtPos(system, 0, &data_ov056_021f4284);
    work->system = system;
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    work->sound = GameData_GetFieldSoundSystem(gameData);
}

void func_ov056_021f3ec8(FieldAsyncProc *proc, Field *field, void *data) {
}

static const u32 data_ov056_021f425c[2] = { 0x5a000, 0xd5000 };

void func_ov056_021f3ecc(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimeAmbience *work = data;
    u32 animation;
    s32 i;

    animation = func_ov021_0218c6f4(work->system, work->holder, 0);
    for (i = 0; i < 2; i++) {
        if (animation == data_ov056_021f425c[i]) {
            FieldSnd_PlayAmbience(work->sound, 0x743);
            work->playing = 1;
            work->timer = 0x1f;
            break;
        }
    }
    if (work->playing != 0) {
        work->timer--;
        if (work->timer <= 0) {
            FieldSnd_PlayAmbience(work->sound, 0x76e);
            work->playing = 0;
            work->timer = 0;
        }
    }
}

const FieldAsyncProcDef data_ov056_021f435c = {
    0x80, sizeof(FkSoundAnimeAmbience), func_ov056_021f3e94, func_ov056_021f3ec8, func_ov056_021f3ecc, NULL,
};

void func_ov056_021f3f30(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimeFlagAmbience *work = data;
    GameSystem *gsys;
    GameData *gameData;
    u16 *wkPtr;

    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    work->eventWork = GameData_GetEventWork(gameData);
    work->sound = GameData_GetFieldSoundSystem(gameData);
    wkPtr = EventWork_GetWkPtr(work->eventWork, 0x4001);
    work->flag = wkPtr;
    if (*wkPtr == 0) {
        FieldSnd_PlayAmbience(work->sound, 0x756);
        work->stopped = FALSE;
        return;
    }
    work->stopped = TRUE;
}

void func_ov056_021f3f7c(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimeFlagAmbience *work = data;

    if (!work->stopped) {
        FieldSnd_StopAmbience(work->sound, 0x756);
    }
}

void func_ov056_021f3f94(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimeFlagAmbience *work = data;

    if (*work->flag != 0 && !work->stopped) {
        FieldSnd_StopAmbience(work->sound, 0x756);
        work->stopped = TRUE;
    }
}

const FieldAsyncProcDef data_ov056_021f4374 = {
    0x80, sizeof(FkSoundAnimeFlagAmbience), func_ov056_021f3f30, func_ov056_021f3f7c, func_ov056_021f3f94, NULL,
};

void func_ov056_021f3fb8(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimeFlagAmbience *work = data;
    GameSystem *gsys;
    GameData *gameData;
    u16 *wkPtr;

    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    work->eventWork = GameData_GetEventWork(gameData);
    work->sound = GameData_GetFieldSoundSystem(gameData);
    wkPtr = EventWork_GetWkPtr(work->eventWork, 0x4001);
    work->flag = wkPtr;
    if (*wkPtr == 0) {
        FieldSnd_PlayAmbience(work->sound, 0x7ef);
        work->stopped = FALSE;
        return;
    }
    work->stopped = TRUE;
}

void func_ov056_021f4004(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimeFlagAmbience *work = data;

    if (!work->stopped) {
        FieldSnd_StopAmbience(work->sound, 0x7ef);
    }
}

void func_ov056_021f401c(FieldAsyncProc *proc, Field *field, void *data) {
    FkSoundAnimeFlagAmbience *work = data;

    if (*work->flag != 0 && !work->stopped) {
        FieldSnd_StopAmbience(work->sound, 0x7ef);
        work->stopped = TRUE;
    }
}

const FieldAsyncProcDef data_ov056_021f438c = {
    0x80, sizeof(FkSoundAnimeFlagAmbience), func_ov056_021f3fb8, func_ov056_021f4004, func_ov056_021f401c, NULL,
};

FkSoundAnimeSeCurve *func_ov056_021f4040(const u16 *se, u32 arcId, u32 fileId, u32 preloadCount, HeapID heapId) {
    FkSoundAnimeSeCurve *seCurve;

    seCurve = GFL_HeapAllocate(heapId, sizeof(FkSoundAnimeSeCurve), 1, "field_fk_sound_anime.c", 0x4d);
    seCurve->frame = 0;
    seCurve->curve = GFL_G3DCurveLoadFileStream(heapId, arcId, fileId, preloadCount);
    MI_CpuCopy8(se, seCurve->se, sizeof(seCurve->se));
    return seCurve;
}

void func_ov056_021f408c(FkSoundAnimeSeCurve *seCurve) {
    GFL_G3DCurveFree(seCurve->curve);
    GFL_HeapFree(seCurve);
}

BOOL func_ov056_021f40a0(FkSoundAnimeSeCurve *seCurve) {
    BOOL ret;
    s32 last;
    VecFx32 translation;
    VecFx32 vec;
    VecFx32 rotation;

    last = GFL_G3DCurveGetFrameCount(seCurve->curve) << 12;
    if (last <= seCurve->frame) {
        ret = TRUE;
        seCurve->frame = last - 0x1000;
    } else {
        ret = FALSE;
    }
    GFL_G3DCurveFrameSet(seCurve->curve, seCurve->frame);
    GFL_G3DCurveGetNowTranslation(seCurve->curve, &translation);
    func_02011660(seCurve->curve, &vec);
    GFL_G3DCurveGetNowRotation(seCurve->curve, &rotation);
    if (translation.x == 0x1000) {
        func_ov056_021f4224(seCurve, 0);
    } else if (translation.x == 0x2000) {
        func_ov056_021f423c(seCurve, 0);
    }
    if (translation.y == 0x1000) {
        func_ov056_021f4224(seCurve, 1);
    } else if (translation.y == 0x2000) {
        func_ov056_021f423c(seCurve, 1);
    }
    if (translation.z == 0x1000) {
        func_ov056_021f4224(seCurve, 2);
    } else if (translation.z == 0x2000) {
        func_ov056_021f423c(seCurve, 2);
    }
    if (vec.x == 0x1000) {
        func_ov056_021f4224(seCurve, 3);
    } else if (vec.x == 0x2000) {
        func_ov056_021f423c(seCurve, 3);
    }
    if (vec.y == 0x1000) {
        func_ov056_021f4224(seCurve, 4);
    } else if (vec.y == 0x2000) {
        func_ov056_021f423c(seCurve, 4);
    }
    if (vec.z == 0x1000) {
        func_ov056_021f4224(seCurve, 5);
    } else if (vec.z == 0x2000) {
        func_ov056_021f423c(seCurve, 5);
    }
    if (rotation.x == 0x1000) {
        func_ov056_021f4224(seCurve, 6);
    } else if (rotation.x == 0x2000) {
        func_ov056_021f423c(seCurve, 6);
    }
    if (rotation.y == 0x1000) {
        func_ov056_021f4224(seCurve, 7);
    } else if (rotation.y == 0x2000) {
        func_ov056_021f423c(seCurve, 7);
    }
    if (rotation.z == 0x1000) {
        func_ov056_021f4224(seCurve, 8);
    } else if (rotation.z == 0x2000) {
        func_ov056_021f423c(seCurve, 8);
    }
    seCurve->frame += 0x1000;
    return ret;
}


void func_ov056_021f4224(FkSoundAnimeSeCurve *seCurve, s32 channel) {
    u32 se;

    se = seCurve->se[channel];
    if (se != 0xffff) {
        GFL_SndSEPlay(se);
    }
}

void func_ov056_021f423c(FkSoundAnimeSeCurve *seCurve, s32 channel) {
    u32 seq;
    s32 player;

    seq = seCurve->se[channel];
    if (seq != 0xffff) {
        player = GFL_SndSeqGetPlayerIndex(seq);
        GFL_SndPlayerStop(player);
    }
}
