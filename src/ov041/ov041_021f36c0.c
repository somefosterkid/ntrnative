// Black's code at ov041 0x021f36c0-0x021f4080. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "struct_decls.h"
#include "system/game_system.h"
#include "system/game_data.h"
#include "field/field_map.h"
#include "system/game_event.h"
#include "field/field_camera.h"
#include "field/field_actor.h"
#include "field/field_script.h"
#include "gfl/sound.h"
#include "gfl/g3d.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_mtx.h"

u32 func_ov041_021f3f44(u32 param_1);
u32 func_ov041_021f3f70(u32 param_1, s32 param_2);
static u8 data_ov041_021f4660[32] = { 0, 0, 1, 3, 1, 2, 3, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
GameEventReturnCode func_ov041_021f3884(GameEvent *event, u32 *state, void *data);

typedef struct {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u8 unk_c;
    u8 unk_d;
    u8 unk_e;
    u8 unk_f;
} Ov041CameraWork;

void func_ov041_021f3fec(Ov041CameraWork *camera);

static const G3DSceneResourceSetup data_ov041_021f412c[9] = {
#include "asset/ov041_021f36c0_data_ov041_021f412c.inc"
};

static const G3DSceneAnimationSetup data_ov041_021f40d0[8] = {
#include "asset/ov041_021f36c0_data_ov041_021f40d0.inc"
};

static const G3DSceneActorSetup data_ov041_021f4280[10] = {
#include "asset/ov041_021f36c0_data_ov041_021f4280.inc"
};

static const G3DSceneSetup data_ov041_021f40a4 = { data_ov041_021f412c, 9, data_ov041_021f4280, 10 };

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
} Ov041Entry;

const Ov041Entry data_ov041_021f40f0[10] = {
#include "asset/ov041_021f36c0_data_ov041_021f40f0.inc"
};

typedef struct {
    u8 actor;
    u8 dir;
    u8 playing;
    u8 returned;
    u8 buffer[0x708];
    u8 unk_70c[8];
    G3DCurve *curve;
    u16 frame;
    u16 frameCount;
    u16 unk_71c;
    u16 unk_71e;
    Ov041CameraWork camera;
} Ov041Work;

static const u16 data_ov041_021f4084[4] = { 1, 1, 15, 15 };

static const u16 data_ov041_021f4094[4] = { 69, 69, 84, 84 };

static const u16 data_ov041_021f408c[4] = { 15, 15, 30, 30 };

static const u16 data_ov041_021f409c[4] = { 30, 30, 45, 45 };

static const u16 data_ov041_021f40b4[14] = {
#include "asset/ov041_021f36c0_data_ov041_021f40b4.inc"
};

typedef struct {
    u16 pitch;
    u16 yaw;
    u8 frames;
    u8 start;
    u8 length;
    u8 unk_7;
} Ov041CameraKey;

static const Ov041CameraKey data_ov041_021f4198[14] = {
#include "asset/ov041_021f36c0_data_ov041_021f4198.inc"
};

typedef struct {
    u32 curve;
    u32 x;
    u32 y;
    u32 z;
    u32 camera;
} Ov041Route;

static const Ov041Route data_ov041_021f4320[10][4] = {
#include "asset/ov041_021f36c0_data_ov041_021f4320.inc"
};

u16 func_ov041_021f3f18(u32 param_1);
u32 func_ov041_021f3fc0(u32 param_1, s32 param_2);
void func_ov041_021f3fd8(Ov041Work *work);
void func_ov041_021f4000(Ov041CameraWork *param_1, FieldCamera *param_2);

u32 func_ov041_021f3ee4(u32 param_1, u32 param_2);


void func_ov041_021f36c0(Field *param_1) {
    u32 actor;
    u8 i;
    FieldExpObjSystem *system;
    u16 heapId;
    SRTMatrix *matrix;

    system = Field_GetExpObjSystem(param_1);
    heapId = Field_GetHeapID(param_1);
    Field_AllocGimmickWorkBlock(param_1, 1, heapId, 0x730);
    LoadFieldExpandObjData(system, &data_ov041_021f40a4, 0);
    actor = 0;
    {
    VecFx32 pos[10] = {
#include "asset/ov041_021f36c0_pos.inc"
    };
    for (; actor < 10; actor = (u8)(actor + 1)) {
        matrix = FieldExpObj_GetActorMatrixPtr(system, 0, actor);
        func_ov021_021be4a4(system, 0, actor, 1);
        matrix->translation = pos[actor];
        MTX_RotY33_(&matrix->rotation, FX_SinIdx(data_ov041_021f4660[actor] * 0x4000), FX_CosIdx(data_ov041_021f4660[actor] * 0x4000));
        for (i = 0; i < 4; i++) {
            FieldExpObjAnm_SetLooped(FieldExpObj_GetAnmInfo(system, 0, actor, i * 2), 0);
            FieldExpObjAnm_SetLooped(FieldExpObj_GetAnmInfo(system, 0, actor, i * 2 + 1), 0);
        }
    }
    }
}

void func_ov041_021f37b8(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    Field_DeleteGimmickWorkBlock(param_1, 1);
    FieldExpObj_FreeScene(system, 0);
}

void func_ov041_021f37d4(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    FieldExpObj_StepAllAnimations(system);
}

GameEvent *func_ov041_021f37e0(GameSystem *param_1) {
    Field *field;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    u8 *gimmickWorkBlock;
    FieldPlayer *player;
    VecFx32 VStack_24;
    u32 val;
    u32 val2;
    u16 faceDir;
    GameEvent *pGVar3;

    field = GSYS_GetField(param_1);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 5);
    gimmickWorkBlock = Field_GetGimmickWorkBlock(field, 1);
    player = Field_GetPlayer(field);
    FieldPlayer_GetWPos(player, &VStack_24);
    val = (s32)(VStack_24.x + ((u32)(VStack_24.x >> 0xf) >> 0x10)) >> 0x10;
    val2 = (s32)(VStack_24.z + ((u32)(VStack_24.z >> 0xf) >> 0x10)) >> 0x10;
    faceDir = FieldPlayer_GetFaceDir(player);
    switch (faceDir) {
    case 0:
        val2 = val2 - 2;
        break;
    case 1:
        val2 = val2 + 2;
        break;
    case 2:
        val = val - 2;
        break;
    case 3:
        val = val + 2;
    }
    val2 = func_ov041_021f3ee4(val, val2);
    if (val2 >= 10) {
        return NULL;
    }
    pGVar3 = GameEvent_Create(param_1, (GameEvent *)0x0, func_ov041_021f3884, 0);
    *gimmickWorkBlock = (s8)val2;
    gimmickWorkBlock[1] = (s8)faceDir;
    return pGVar3;
}

GameEventReturnCode func_ov041_021f3884(GameEvent *param_1, u32 *param_2, void *data) {
    GameSystem *gsys;
    Field *field;
    FieldExpObjSystem *system;
    Ov041Work *work;
    FieldCamera *camera;
    GameData *gameData;
    GimmickState *gimmick;

    gsys = GameEvent_GetGameSystem(param_1);
    field = GSYS_GetField(gsys);
    system = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    camera = Field_GetCameraSystem(field);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 5);
    switch (*param_2) {
    case 0:
        FieldCamera_FinishDelay(camera);
        func_ov041_021f3fec(&work->camera);
        *param_2 = *param_2 + 1;
        break;
    case 1: {
        u16 actor;
        u16 camIdx;
        s32 sub;
        u16 anm;
        s32 start;
        s32 len;
        u8 id;
        u32 fileId;

        if (FieldCamera_IsDelayActive(camera)) {
            break;
        }
        actor = work->actor;
        sub = func_ov041_021f3f44(work->dir);
        fileId = data_ov041_021f4320[work->actor][sub].curve;
        if (fileId == 0xFFFFFFFF) {
            return GAMEEVENT_DONE;
        }
        work->curve = GFL_G3DCurveCreateToLoadBufferEntire(0x8015, 121, fileId, work->buffer, 0x708);
        work->frameCount = GFL_G3DCurveGetFrameCount(work->curve);
        camIdx = data_ov041_021f4320[actor][sub].camera;
        start = data_ov041_021f4198[camIdx].start;
        len = data_ov041_021f4198[camIdx].length;
        if (start + len >= work->frameCount) {
            start = work->frameCount - (len + 1);
            work->unk_71c = start;
            work->unk_71e = len;
        } else {
            work->unk_71c = start;
            work->unk_71e = len;
        }
        work->returned = 0;
        id = work->actor;
        anm = func_ov041_021f3f18(func_ov041_021f3fc0(work->dir, id));
        FieldExpObj_SetAnm(system, 0, id, anm, TRUE);
        FieldExpObj_SetAnm(system, 0, id, anm + 1, TRUE);
        FieldExpObj_SetAnmFrame(system, 0, id, anm, 0);
        FieldExpObj_SetAnmFrame(system, 0, id, anm + 1, 0);
        FieldCamera_EVCameraInit(camera);
        *param_2 = *param_2 + 1;
        break;
    }
    case 2: {
        FieldEvCameraAnimationSetup setup;
        u32 idx;
        u16 camIdx;
        u16 pauseAnm;
        u8 actor;
        u32 sub;
        fx32 frame;

        actor = work->actor;
        idx = func_ov041_021f3fc0(work->dir, actor);
        frame = func_ov021_021be6e8(system, 0, actor, func_ov041_021f3f18(idx));
        if (data_ov041_021f4084[idx] == frame / FX32_ONE) {
            GFL_SndSEPlay(0x698);
        }
        if (frame / FX32_ONE < data_ov041_021f408c[idx]) {
            break;
        }
        pauseAnm = func_ov041_021f3f18(idx);
        FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, actor, pauseAnm), TRUE);
        FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, actor, pauseAnm + 1), TRUE);
        EventScriptCall_Start(param_1, 2, NULL, 0, 0x8015);
        sub = func_ov041_021f3f44(work->dir);
        camIdx = data_ov041_021f4320[work->actor][sub].camera;
        setup.targetCoords.pitch = data_ov041_021f4198[camIdx].pitch;
        setup.targetCoords.yaw = data_ov041_021f4198[camIdx].yaw;
        setup.flags.animateExtraTranslation = FALSE;
        setup.flags.animatePitch = TRUE;
        setup.flags.animateYaw = TRUE;
        setup.flags.animateTargetDistance = FALSE;
        setup.flags.animateFOV = FALSE;
        FieldCameraAnm_SetAnimation(camera, &setup, data_ov041_021f4198[camIdx].frames);
        *param_2 = *param_2 + 1;
        break;
    }
    case 3: {
        FieldActor *playerActor;
        u8 actor;
        u16 anm;

        if (FieldCamera_IsAnimating(camera)) {
            break;
        }
        FieldCamera_ChangeTransformType(camera, 2);
        playerActor = FieldPlayer_GetActor(Field_GetPlayer(field));
        SetActorHidden(playerActor, TRUE);
        SetActorFlag(playerActor, 0x200);
        SetActorMovementFlag(playerActor, 0x8000);
        actor = work->actor;
        anm = func_ov041_021f3f18(func_ov041_021f3fc0(work->dir, actor));
        FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, actor, anm), FALSE);
        FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, actor, anm + 1), FALSE);
        GFL_SndSEPlay(0x699);
        *param_2 = *param_2 + 1;
        break;
    }
    case 4: {
        u32 idx;
        u8 actor;
        s32 frame;
        u16 anm;

        actor = work->actor;
        idx = func_ov041_021f3fc0(work->dir, actor);
        anm = func_ov041_021f3f18(idx);
        frame = func_ov021_021be6e8(system, 0, actor, anm) / 0x1000;
        if (frame < data_ov041_021f409c[idx]) {
            break;
        }
        GFL_SndSEPlay(0x6a0);
        work->playing = TRUE;
        work->frame = 0;
        *param_2 = *param_2 + 1;
        break;
    }
    case 5: {
        u16 actor;
        s32 frame;
        u32 idx;
        u32 sub;
        u16 anm;

        actor = work->actor;
        idx = func_ov041_021f3fc0(work->dir, actor);
        anm = func_ov041_021f3f18(idx);
        frame = func_ov021_021be6e8(system, 0, actor, anm) / 0x1000;
        if (frame == data_ov041_021f4094[idx]) {
            GFL_SndSEPlay(0x696);
            if (work->returned == 0) {
                func_ov041_021f3fd8(work);
                FieldCamera_ChangeTransformType(camera, 1);
            }
            SetActorHidden(FieldPlayer_GetActor(Field_GetPlayer(field)), FALSE);
        }
        if (work->playing == 0) {
            FieldPlayer *player = Field_GetPlayer(field);
            VecFx32 pos = { 0, 0, 0 };
            const Ov041Route *route;

            sub = func_ov041_021f3f44(work->dir);
            route = &data_ov041_021f4320[work->actor][sub];
            pos.x = (route->x << 16) + 0x8000;
            pos.y = route->y << 16;
            pos.z = (route->z << 16) + 0x8000;
            FieldPlayer_SetWPos(player, &pos);
            *param_2 = *param_2 + 1;
        }
        break;
    }
    case 6: {
        u16 anm;
        u16 actor;

        ClearActorFlag(FieldPlayer_GetActor(Field_GetPlayer(field)), 0x200);
        GFL_G3DCurveFree(work->curve);
        actor = work->actor;
        anm = func_ov041_021f3f18(func_ov041_021f3fc0(work->dir, actor));
        FieldExpObj_SetAnm(system, 0, actor, anm, FALSE);
        FieldExpObj_SetAnm(system, 0, actor, anm + 1, FALSE);
        FieldCamera_EnableDelay(camera);
        FieldCameraAnm_EVCameraEnd(camera);
        return GAMEEVENT_DONE;
    }
    }
    if (work->playing != 0) {
        VecFx32 trans = { 0, 0, 0 };
        u16 actor;

        actor = work->actor;
        if (func_02011688(work->curve, &trans, work->frame)) {
            trans.x += (data_ov041_021f40f0[actor].unk_0 << 16) + 0x8000;
            trans.y += data_ov041_021f40f0[actor].unk_2 << 16;
            trans.z += (data_ov041_021f40f0[actor].unk_4 << 16) + 0x8000;
            FieldPlayer_SetWPos(Field_GetPlayer(field), &trans);
        }
        if (work->camera.unk_4 != 0 && work->frame < work->unk_71c) {
            VecFx32 wpos;
            VecFx32 eye;
            FieldEvCameraAnimationFlags flags;

            FieldPlayer_GetWPos(Field_GetPlayer(field), &wpos);
            if (work->camera.unk_8 == 0) {
                FieldCamera_ClearBind(camera);
                FieldCamera_CoordsGetEye(camera, &eye);
                flags.animateTargetPos = TRUE;
                flags.animateFOV = FALSE;
                flags.animateExtraTranslation = FALSE;
                flags.animatePitch = FALSE;
                flags.animateYaw = FALSE;
                flags.animateTargetDistance = FALSE;
                func_ov021_0218f930(camera, &eye, &wpos, &flags, work->unk_71c - work->frame);
                work->camera.unk_8 = 1;
            } else {
                func_ov021_0218fe70(camera)->targetPos = wpos;
            }
        }
        if (work->frame == work->unk_71c) {
            FieldEvCameraAnimationFlags returnFlags;

            if (work->camera.unk_0 != 0) {
                work->camera.unk_0 = 0;
            }
            FieldCamera_ResetBind(camera);
            FieldCamera_ChangeTransformType(camera, 0);
            returnFlags.animateExtraTranslation = TRUE;
            returnFlags.animatePitch = TRUE;
            returnFlags.animateYaw = TRUE;
            returnFlags.animateTargetDistance = TRUE;
            returnFlags.animateFOV = TRUE;
            returnFlags.animateTargetPos = FALSE;
            FieldCameraAnm_SetReturnAnimation(camera, &returnFlags, work->unk_71e);
            work->returned = TRUE;
        }
        {
            FieldActor *playerActor;
            u16 camIdx;

            actor = work->actor;
            camIdx = data_ov041_021f4320[actor][func_ov041_021f3f44(work->dir)].camera;
            playerActor = FieldPlayer_GetActor(Field_GetPlayer(field));
            if (work->frame == work->frameCount - 1 - data_ov041_021f40b4[camIdx] && camIdx != 11) {
                ClearActorMovementFlag(playerActor, 0x8000);
                GFL_SndSEPlay(0x697);
            }
            if (camIdx == 11) {
                if (work->frame == 70) {
                    GFL_SndSEPlay(0x6a1);
                }
                if (work->frame + 8 >= work->frameCount) {
                    if (CheckActorMovementFlag(playerActor, 0x8000)) {
                        ClearActorMovementFlag(playerActor, 0x8000);
                    }
                }
            }
        }
        work->frame++;
        if (work->frame >= work->frameCount) {
            work->playing = FALSE;
        }
        func_ov041_021f4000(&work->camera, camera);
    }
    return GAMEEVENT_CONTINUE;
}


u32 func_ov041_021f3ee4(u32 param_1, u32 param_2) {
    u8 i;

    for (i = 0; i < 10; i++) {
        if (param_1 == data_ov041_021f40f0[i].unk_0 && param_2 == data_ov041_021f40f0[i].unk_4) {
            return i;
        }
    }
    return i;
}


u16 func_ov041_021f3f18(u32 param_1) {
    switch(param_1) {
    case 0:
        return 0;
    case 1:
        return 2;
    case 2:
        return 4;
    case 3:
        return 6;
    default:
        return 0;
    }
}

u32 func_ov041_021f3f44(u32 param_1) {
    switch (param_1) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 3:
        return 2;
    case 2:
        return 3;
    default:
        return 0;
    }
}

u32 func_ov041_021f3f70(u32 param_1, s32 param_2) {
    u8 table[4] = { 0, 2, 1, 3 };
    u8 i;

    for (i = 0; i < 4; i++) {
        if (table[i] == param_1) {
            return table[(u8)((i + param_2) % 4)];
        }
    }
    return param_1;
}



u32 func_ov041_021f3fc0(u32 param_1, s32 param_2) {
    return func_ov041_021f3f70(func_ov041_021f3f44(param_1), (u32)(u8)data_ov041_021f4660[param_2]);
}

void func_ov041_021f3fd8(Ov041Work *work) {
    func_ov041_021f3fec(&work->camera);
    work->camera.unk_0 = 1;
}

void func_ov041_021f3fec(Ov041CameraWork *camera) {
    camera->unk_0 = 0;
    camera->unk_4 = 0;
    camera->unk_8 = 0;
    camera->unk_d = 0;
    camera->unk_c = 0;
    camera->unk_e = 0;
    camera->unk_f = 0;
}

void func_ov041_021f4000(Ov041CameraWork *param_1, FieldCamera *param_2) {
    VecFx32 eye;
    s32 step;

    if (param_1->unk_0 != 0) {
        param_1->unk_f++;
        if (param_1->unk_c == 0) {
            if (param_1->unk_d % 2 == 0) {
                step = 0x8000;
            } else {
                step = -0x8000;
                param_1->unk_e++;
            }
            param_1->unk_d = (param_1->unk_d + 1) % 2;
            FieldCamera_CoordsGetEye(param_2, &eye);
            eye.y += step;
            FieldCamera_CoordsSetEye(param_2, &eye);
            param_1->unk_c = 1;
        } else {
            param_1->unk_c--;
        }
        if (param_1->unk_e >= 2) {
            param_1->unk_0 = 0;
            param_1->unk_4 = 1;
        }
    }
}

