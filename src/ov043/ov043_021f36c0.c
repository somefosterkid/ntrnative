// Black's code at ov043 0x021f36c0-0x021f4304. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/field_camera.h"
#include "field/field_exp_obj.h"
#include "field/field_map.h"
#include "gfl/g3d.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "gfl/sound.h"
#include "field/fieldmap.h"
#include "field/field_fog.h"
#include "field/height_ex.h"
#include "field/field_g3d_mapper.h"
#include "field/ov021_021a0138.h"
#include "gfl/heapsys.h"
#include "field/field_actor.h"

GameEventReturnCode func_ov043_021f3ac0(GameEvent *event, u32 *state, void *data);

const G3DSceneAnimationSetup data_ov043_021f4304[] = { { 7, 0 } };

const s32 data_ov043_021f4328[] = { 1, 0, 1, 0, 0, 1 };

const G3DSceneAnimationSetup data_ov043_021f4308[] = { { 3, 0 }, { 4, 0 } };
const G3DSceneAnimationSetup data_ov043_021f4310[] = { { 5, 0 }, { 6, 0 } };


const VecFx32 data_ov043_021f4358[6] = {
#include "asset/ov043_021f36c0_data_ov043_021f4358.inc"
};

const G3DSceneResourceSetup data_ov043_021f43a0[] = {
#include "asset/ov043_021f36c0_data_ov043_021f43a0.inc"
};

const G3DSceneActorSetup data_ov043_021f4400[] = {
#include "asset/ov043_021f36c0_data_ov043_021f4400.inc"
};

const G3DSceneSetup data_ov043_021f4318 = { data_ov043_021f43a0, 8, data_ov043_021f4400, 8 };

const s32 data_ov043_021f4510[][5] = {
#include "asset/ov043_021f36c0_data_ov043_021f4510.inc"
};

const s32 data_ov043_021f4480[][6] = {
#include "asset/ov043_021f36c0_data_ov043_021f4480.inc"
};

typedef struct {
    s32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    // The camera's bind while the elevator moves without it
    const VecFx32 *bind;
    s32 unk_14;
    BOOL unk_18;
    u16 unk_1C;
    u16 unk_1E;
    FieldCameraShake unk_20;
    BOOL unk_3C;
    u32 unk_40;
    FieldActor *unk_44[5];
} Ov043GimmickWork;

GameEventReturnCode func_ov043_021f3d78(GameEvent *event, u32 *state, void *data);
u32 func_ov043_021f3ecc(Ov043GimmickWork *work);
void func_ov043_021f41b8(u8 param_1, int param_2, BOOL param_3, FieldCamera *camera, FieldCameraShake *shake);
BOOL func_ov043_021f4210(FieldCameraShake *shake);
void func_ov043_021f4174(FieldExpObjSystem *param_1);
void func_ov043_021f3f38(s32 *param_1, FieldExpObjSystem *param_2, s32 param_3);
u32 func_ov043_021f3eec(u8 param_1, s32 param_2, s32 param_3);
u32 func_ov043_021f3fbc(s32 *param_1, s32 param_2);
void func_ov043_021f3fe8(GameSystem *gsys);


void func_ov043_021f36c0(Field *field) {
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    s32 *userData;
    FieldFog *fog;
    ResortMapCreateWork *renderer;
    SRTMatrix *matrix;
    FieldExpObjAnm *anm;
    FieldPlayer *player;
    VecFx32 pos;
    fx32 frame;
    s32 i;
    s32 j;
    u8 up;
    u8 index;

    fog = Field_GetFog(field);
    userData = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), 7);
    Field_AllocGimmickWorkBlock(field, 1, Field_GetHeapID(field), sizeof(Ov043GimmickWork));
    renderer = func_ov021_0218cee0(Field_GetG3DMapper(field));

    for (i = 0; i < 8; i++) {
        func_ov021_021c0628(i, data_ov043_021f4510[i][0], data_ov043_021f4510[i][1], data_ov043_021f4510[i][2],
            data_ov043_021f4510[i][3], data_ov043_021f4510[i][4], 0x800000, renderer);
    }
    for (i = 0; i < 6; i++) {
        index = i + 8;
        if (userData[i] != 0) {
            up = 1;
        } else {
            up = 0;
        }
        func_ov021_021c0628(index, data_ov043_021f4480[i][0], data_ov043_021f4480[i][1], data_ov043_021f4480[i][2],
            data_ov043_021f4480[i][3], data_ov043_021f4480[i][4 + up], 0x800000, renderer);
    }

    LoadFieldExpandObjData(system, &data_ov043_021f4318, 0);
    for (i = 0; i < 6; i++) {
        matrix = FieldExpObj_GetActorMatrixPtr(system, 0, i);
        matrix->translation = data_ov043_021f4358[i];
        up = userData[i] != 0;
        matrix->translation.y = data_ov043_021f4480[i][4 + up];
        func_ov021_021be4a4(system, 0, i, 1);
        for (j = 0; j < 2; j++) {
            anm = FieldExpObj_GetAnmInfo(system, 0, i, j);
            FieldExpObjAnm_SetLooped(anm, 0);
            FieldExpObjAnm_SetPaused(anm, 1);
        }
        func_ov043_021f3f38(userData, system, i);
    }

    matrix = FieldExpObj_GetActorMatrixPtr(system, 0, 7);
    VecFx32 pos1 = { 0xf8000, 0x320000, 0x198000 };
    matrix->translation = pos1;
    func_ov021_021be4a4(system, 0, 7, 1);
    for (j = 0; j < 2; j++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 7, j);
        FieldExpObjAnm_SetLooped(anm, 0);
        FieldExpObjAnm_SetPaused(anm, 1);
    }
    func_ov043_021f4174(system);

    matrix = FieldExpObj_GetActorMatrixPtr(system, 0, 6);
    VecFx32 pos2 = { 0xf8000, 0xffd80000, 0x188000 };
    matrix->translation = pos2;
    func_ov021_021be4a4(system, 0, 6, 1);
    FieldExpObj_SetAnm(system, 0, 6, 0, 1);
    anm = FieldExpObj_GetAnmInfo(system, 0, 6, 0);
    FieldExpObjAnm_SetLooped(anm, 0);
    FieldExpObjAnm_SetPaused(anm, 1);
    if (userData[6] != 0) {
        frame = func_ov021_021be748(anm);
        FieldExpObj_SetAnmFrame(system, 0, 6, 0, frame);
    }

    player = Field_GetPlayer(field);
    FieldPlayer_GetWPos(player, &pos);
    if (pos.y <= -0x280000) {
        FieldFog_SetOffset(fog, 0x7fdf);
        FieldFog_SetDepthShift(fog, 9);
    }
    GFL_HeapGetFreeSizeCore(0x15);
}

void func_ov043_021f3938(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    Field_GetGimmickWorkBlock(param_1, 1);
    Field_DeleteGimmickWorkBlock(param_1, 1);
    FieldExpObj_FreeScene(system, 0);
}

void func_ov043_021f395c(Field *param_1) {
    FieldExpObjSystem *system;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;

    system = Field_GetExpObjSystem(param_1);
    Field_GetGimmickWorkBlock(param_1, 1);
    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 7);
    FieldExpObj_StepAllAnimations(system);
}

GameEvent *func_ov043_021f398c(GameSystem *gsys, s32 param_2) {
    Field *field = GSYS_GetField(gsys);
    GameSystem *fieldGsys = Field_GetGameSystem(field);
    GameData *gameData = GSYS_GetGameData(fieldGsys);
    GimmickState *gimmick = GameData_GetGimmickState(gameData);
    s32 *userData = GimmickState_GetUserData(gimmick, 7);
    Ov043GimmickWork *work = Field_GetGimmickWorkBlock(field, 1);
    s32 amp;

    if (param_2 >= 6) {
        return NULL;
    }
    work->unk_00 = param_2;
    if (userData[param_2] != 0) {
        work->unk_04 = data_ov043_021f4480[param_2][5];
        work->unk_08 = data_ov043_021f4480[param_2][4];
    } else {
        work->unk_04 = data_ov043_021f4480[param_2][4];
        work->unk_08 = data_ov043_021f4480[param_2][5];
    }
    if (param_2 == 5) {
        amp = 2 << 14;
        work->unk_3C = 1;
    } else {
        amp = 1 << 14;
        work->unk_3C = 0;
    }
    if (work->unk_08 - work->unk_04 < 0) {
        work->unk_0C = -amp;
    } else {
        work->unk_0C = amp;
    }
    work->unk_40 = 0;
    return GameEvent_Create(gsys, NULL, func_ov043_021f3ac0, 0);
}

GameEvent *func_ov043_021f3a28(GameSystem *gsys, s32 param_2) {
    Field *field = GSYS_GetField(gsys);
    GameSystem *fieldGsys = Field_GetGameSystem(field);
    GameData *gameData = GSYS_GetGameData(fieldGsys);
    GimmickState *gimmick = GameData_GetGimmickState(gameData);
    Ov043GimmickWork *work;
    FieldExpObjSystem *expObj;

    GimmickState_GetUserData(gimmick, 7);
    work = Field_GetGimmickWorkBlock(field, 1);
    work->unk_14 = param_2;
    if (param_2 != 0) {
        work->unk_04 = 0x320000;
        work->unk_08 = 0x370000;
    } else {
        work->unk_04 = 0x370000;
        work->unk_08 = 0x320000;
        expObj = Field_GetExpObjSystem(field);
        FieldExpObj_SetAnm(expObj, 0, 7, 1, 1);
        FieldExpObj_SetAnm(expObj, 0, 7, 0, 0);
    }
    if (work->unk_08 - work->unk_04 < 0) {
        work->unk_0C = -0x2000;
    } else {
        work->unk_0C = 0x2000;
    }
    return GameEvent_Create(gsys, NULL, func_ov043_021f3d78, 0);
}

GameEventReturnCode func_ov043_021f3ac0(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    Field *field = GSYS_GetField(gsys);
    FieldExpObjSystem *system = Field_GetExpObjSystem(field);
    Ov043GimmickWork *work = Field_GetGimmickWorkBlock(field, 1);
    FieldCamera *camera = Field_GetCameraSystem(field);
    GameData *gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    s32 *userData = GimmickState_GetUserData(GameData_GetGimmickState(gameData), 7);
    VecFx32 playerPos;
    VecFx32 actorPos;
    VecFx32 pos;
    u32 index;
    FieldActor *actor;
    MMSys *mmSys;
    SRTMatrix *matrix;
    FieldExpObjAnm *anm;
    ResortMapCreateWork *renderer;
    FieldFog *fog;
    FieldPlayer *player;
    s32 count;
    s32 i;
    u32 id;

    switch (*state) {
    case 0:
        count = 0;
        index = 0;
        mmSys = Field_GetActorSystem(field);
        for (i = 0; i < 5; i++) {
            work->unk_44[i] = NULL;
        }
        player = Field_GetPlayer(field);
        FieldPlayer_GetWPos(player, &playerPos);
        while (NextActor(mmSys, &actor, &index) == TRUE) {
            s16 x = GetGPosX(actor);
            s16 z = GetGPosZ(actor);
            if (func_ov043_021f3eec(work->unk_00, x, z) != 0) {
                CopyActorWPos(actor, &actorPos);
                if (playerPos.y - 0x10000 < actorPos.y && actorPos.y < playerPos.y + 0x10000 && count < 5) {
                    work->unk_44[count++] = actor;
                }
            }
        }
        func_ov043_021f41b8(work->unk_00, work->unk_0C, TRUE, camera, &work->unk_20);
        (*state)++;
        break;
    case 1:
        if (func_ov043_021f4210(&work->unk_20) == FALSE) {
            break;
        }
        (*state)++;
        break;
    case 2:
        FieldCamera_FinishDelay(camera);
        (*state)++;
        break;
    case 3:
        if (FieldCamera_IsDelayActive(camera) != FALSE) {
            break;
        }
        GFL_SndSEPlay(0x6ca);
        (*state)++;
    case 4:
        if (work->unk_18 == 0) {
            work->unk_04 += work->unk_0C;
        }
        if (func_ov043_021f3ecc(work) != 0) {
            work->unk_04 = work->unk_08;
            func_ov043_021f41b8(work->unk_00, work->unk_0C, FALSE, camera, &work->unk_20);
            GFL_SndSEPlay(0x6cb);
            (*state)++;
        }
        matrix = FieldExpObj_GetActorMatrixPtr(system, 0, work->unk_00);
        matrix->translation.y = work->unk_04;
        for (i = 0; i < 5; i++) {
            FieldActor *rider = work->unk_44[i];
            if (rider != NULL) {
                pos = *GetMModelWPosPtr(rider);
                pos.y = work->unk_04;
                SetActorGPosY(rider, (pos.y >> 4) / 4096);
                SetActorWPosValue(rider, &pos);
            }
        }
        break;
    case 5:
        if (func_ov043_021f4210(&work->unk_20) == FALSE) {
            break;
        }
        (*state)++;
        break;
    case 6:
        FieldCamera_EnableDelay(camera);
        id = work->unk_00;
        anm = FieldExpObj_GetAnmInfo(system, 0, id, func_ov043_021f3fbc(userData, id));
        FieldExpObjAnm_SetPaused(anm, FALSE);
        (*state)++;
        break;
    case 7:
        id = work->unk_00;
        anm = FieldExpObj_GetAnmInfo(system, 0, id, func_ov043_021f3fbc(userData, id));
        if (FieldExpObjAnm_IsPlaybackFinished(anm) == FALSE) {
            break;
        }
        {
        long flag = FALSE;
        if (userData[work->unk_00] != 0) {
            userData[work->unk_00] = 0;
        } else {
            userData[work->unk_00] = 1;
            flag = TRUE;
        }
        renderer = func_ov021_0218cee0(Field_GetG3DMapper(field));
        func_ov021_021c06c0(work->unk_00 + 8, data_ov043_021f4480[work->unk_00][4 + flag], renderer);
        }
        func_ov043_021f3f38(userData, system, work->unk_00);
        if (work->unk_00 != 5) {
            return 1;
        }
        (*state)++;
        break;
    case 8:
        fog = Field_GetFog(field);
        if (FieldFog_IsAnimationRunning(fog) == FALSE) {
            return 1;
        }
        break;
    }
    func_ov043_021f3fe8(gsys);
    return 0;
}


GameEventReturnCode func_ov043_021f3d78(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    Field *field = GSYS_GetField(gsys);
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);
    Ov043GimmickWork *work = Field_GetGimmickWorkBlock(field, 1);
    FieldCamera *camera = Field_GetCameraSystem(field);
    GameSystem *fieldGsys = Field_GetGameSystem(field);
    GameData *gameData = GSYS_GetGameData(fieldGsys);
    GimmickState *gimmick = GameData_GetGimmickState(gameData);
    SRTMatrix *mtx;
    FieldPlayer *player;
    FieldExpObjAnm *anm;
    VecFx32 pos;
    u16 se;

    GimmickState_GetUserData(gimmick, 7);
    switch (*state) {
    case 0:
        if (work->unk_14 != 0) {
            work->bind = FieldCamera_GetBind(camera);
            FieldCamera_ClearBind(camera);
        }
        if (work->unk_14 == 0) {
            se = 0x6da;
        } else {
            se = 0x6ca;
        }
        GFL_SndSEPlay(se);
        (*state)++;
        break;
    case 1:
        work->unk_04 += work->unk_0C;
        if (func_ov043_021f3ecc(work)) {
            work->unk_04 = work->unk_08;
            if (work->unk_14 == 0) {
                func_ov043_021f41b8(0xff, work->unk_0C, FALSE, camera, &work->unk_20);
                GFL_SndSEPlay(0x6cb);
            }
            (*state)++;
        }
        mtx = FieldExpObj_GetActorMatrixPtr(expObj, 0, 7);
        mtx->translation.y = work->unk_04;
        player = Field_GetPlayer(field);
        FieldPlayer_GetWPos(player, &pos);
        pos.y = work->unk_04;
        FieldPlayer_SetWPos(player, &pos);
        break;
    case 2:
        if (work->unk_14 != 0) {
            return 1;
        }
        if (func_ov043_021f4210(&work->unk_20)) {
            if (work->bind != NULL) {
                FieldCamera_SetBind(camera, work->bind);
            }
            anm = FieldExpObj_GetAnmInfo(expObj, 0, 7, 1);
            FieldExpObjAnm_SetPaused(anm, 0);
            (*state)++;
        }
        break;
    case 3:
        anm = FieldExpObj_GetAnmInfo(expObj, 0, 7, 1);
        if (FieldExpObjAnm_IsPlaybackFinished(anm)) {
            func_ov043_021f4174(expObj);
            return 1;
        }
        break;
    }
    return 0;
}




// Whether the lift or elevator has reached its target height
u32 func_ov043_021f3ecc(Ov043GimmickWork *work) {
    if (work->unk_0C < 0) {
        if (work->unk_08 >= work->unk_04) {
            return 1;
        }
    } else if (work->unk_08 <= work->unk_04) {
        return 1;
    }
    return 0;
}

u32 func_ov043_021f3eec(u8 param_1, s32 param_2, s32 param_3) {
    s32 x = data_ov043_021f4480[param_1][0];
    s32 y = data_ov043_021f4480[param_1][1];
    s32 w = data_ov043_021f4480[param_1][2];
    s32 h = data_ov043_021f4480[param_1][3];
    if (x <= param_2 && param_2 <= x + w - 1 && y <= param_3 && param_3 <= y + h - 1) {
        return 1;
    }
    return 0;
}

void func_ov043_021f3f38(s32 *param_1, FieldExpObjSystem *param_2, s32 param_3) {
    int playing;
    s8 other;

    if (param_1[param_3] != 0) {
        if (data_ov043_021f4328[param_3] != 0) {
            playing = 0;
            other = 1;
        } else {
            playing = 1;
            other = 0;
        }
    } else {
        if (data_ov043_021f4328[param_3] != 0) {
            playing = 1;
            other = 0;
        } else {
            playing = 0;
            other = 1;
        }
    }
    FieldExpObj_SetAnm(param_2, 0, (u16)param_3, playing, 1);
    FieldExpObj_SetAnm(param_2, 0, (u16)param_3, other, 0);
    FieldExpObj_SetAnmFrame(param_2, 0, (u16)param_3, playing, 0);
    FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(param_2, 0, (u16)param_3, playing), 1);
}


u32 func_ov043_021f3fbc(s32 *param_1, s32 param_2) {
    u32 ret;

    if (param_1[param_2] != 0) {
        if (data_ov043_021f4328[param_2] != 0) {
            ret = 0;
        } else {
            ret = 1;
        }
    } else {
        if (data_ov043_021f4328[param_2] != 0) {
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return ret;
}


void func_ov043_021f3fe8(GameSystem *gsys) {
    Field *field = GSYS_GetField(gsys);
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);
    Ov043GimmickWork *work = Field_GetGimmickWorkBlock(field, 1);
    FieldCamera *camera = Field_GetCameraSystem(field);
    GameSystem *fieldGsys = Field_GetGameSystem(field);
    GameData *gameData = GSYS_GetGameData(fieldGsys);
    GimmickState *gimmick = GameData_GetGimmickState(gameData);
    s32 *user = GimmickState_GetUserData(gimmick, 7);
    FieldExpObjAnm *anm;
    FieldFog *fog;
    void *fogSrc;
    BOOL fogOn;
    u32 fogColor;
    u32 fogDepth;
    u32 duration;
    s32 player;

    if (work->unk_00 == 5) {
        if (user[6] == 0 && work->unk_04 <= -0x230000) {
            user[6] = 1;
            anm = FieldExpObj_GetAnmInfo(expObj, 0, 6, 0);
            FieldExpObjAnm_SetPaused(anm, 0);
            GFL_SndSEPlay(0x6cd);
            work->unk_04 = -0x230000;
            work->unk_18 = 1;
            work->unk_1C = 0;
            work->unk_1E = 0;
            work->unk_40 = 1;
        }
        if (work->unk_18 != 0) {
            switch (work->unk_1E) {
            case 0:
                func_ov043_021f41b8(work->unk_00, work->unk_0C, FALSE, camera, &work->unk_20);
                GFL_SndSEPlay(0x6cb);
                work->unk_1E++;
                break;
            case 1:
                if (func_ov043_021f4210(&work->unk_20)) {
                    work->unk_1C++;
                    if (work->unk_1C >= 100) {
                        work->unk_0C = -0x2000;
                        work->unk_18 = 0;
                        GFL_SndSEPlay(0x6ca);
                    }
                }
                break;
            }
        }
        if (work->unk_3C != 0) {
            fogOn = FALSE;
            if (work->unk_0C >= 0) {
                if (work->unk_04 >= -0x2a0000) {
                    fogSrc = func_ov021_02188c68(field);
                    fogOn = TRUE;
                    fogColor = func_ov021_021a03a4(fogSrc);
                    fogDepth = func_ov021_021a03b8(fogSrc);
                }
            } else if (work->unk_04 <= -0x310000) {
                fogOn = TRUE;
                fogColor = 0x7fdf;
                fogDepth = 9;
            }
            if (fogOn) {
                fog = Field_GetFog(field);
                duration = 0x20;
                if (work->unk_0C != -0x2000) {
                    duration = 0x10;
                }
                FieldFog_Animate(fog, fogColor, fogDepth, duration);
                work->unk_3C = 0;
            }
        }
        if (work->unk_40 != 0) {
            if (work->unk_40 == 1) {
                work->unk_40 = 2;
                return;
            }
            anm = FieldExpObj_GetAnmInfo(expObj, 0, 6, 0);
            if (FieldExpObjAnm_IsPlaybackFinished(anm)) {
                player = GFL_SndSeqGetPlayerIndex(0x6cd);
                GFL_SndPlayerStop(player);
                work->unk_40 = 0;
            }
        }
    }
}





void func_ov043_021f4174(FieldExpObjSystem *param_1) {
    FieldExpObjAnm *anm;

    FieldExpObj_SetAnm(param_1, 0, 7, 0, 1);
    FieldExpObj_SetAnm(param_1, 0, 7, 1, 0);
    FieldExpObj_SetAnmFrame(param_1, 0, 7, 0, 0);
    anm = FieldExpObj_GetAnmInfo(param_1, 0, 7, 0);
    FieldExpObjAnm_SetPaused(anm, 1);
}

void func_ov043_021f41b8(u8 param_1, int param_2, BOOL param_3, FieldCamera *camera, FieldCameraShake *shake) {
    u8 limit;

    shake->unk_00 = 0;
    shake->unk_01 = 0;
    shake->unk_08 = camera;
    shake->unk_0C = FieldCamera_GetBind(camera);
    FieldCamera_CoordsGetTargetOffset(camera, &shake->unk_10);
    if (param_3 != 0) {
        shake->unk_04 = 1 << 12;
        limit = 2;
    } else {
        if (param_1 == 5) {
            shake->unk_04 = 1 << 14;
        } else {
            shake->unk_04 = 2 << 12;
        }
        limit = 4;
    }
    shake->unk_02 = limit;
    if (param_2 < 0) {
        shake->unk_04 *= -1;
    }
}

BOOL func_ov043_021f4210(FieldCameraShake *shake) {
    VecFx32 offset;

    switch (shake->unk_00) {
    case 0:
        FieldCamera_ClearBind(shake->unk_08);
        shake->unk_00++;
    case 1:
        if (shake->unk_03 % 2 == 0) {
            FieldCamera_CoordsGetTargetOffset(shake->unk_08, &offset);
            offset.y += shake->unk_04;
            FieldCamera_CoordsSetTargetOffset(shake->unk_08, &offset);
            shake->unk_04 *= -1;
            shake->unk_01++;
            if (shake->unk_01 >= shake->unk_02 * 2) {
                if (shake->unk_0C != NULL) {
                    FieldCamera_SetBind(shake->unk_08, shake->unk_0C);
                }
                FieldCamera_CoordsSetTargetOffset(shake->unk_08, &shake->unk_10);
                return TRUE;
            }
        }
        shake->unk_03 = (shake->unk_03 + 1) % 2;
        break;
    }
    return FALSE;
}



void func_ov043_021f42a8(GameSystem *param_1) {
    Field *field;
    FieldExpObjSystem *system;
    Ov043GimmickWork *work;
    FieldCamera *camera;
    SRTMatrix *pSVar3;
    FieldPlayer *player;
    VecFx32 VStack_20;

    field = GSYS_GetField(param_1);
    system = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    camera = Field_GetCameraSystem(field);
    work->bind = FieldCamera_GetBind(camera);
    FieldCamera_ClearBind(camera);
    pSVar3 = FieldExpObj_GetActorMatrixPtr(system, 0, 7);
    (pSVar3->translation).y = 0x370000;
    player = Field_GetPlayer(field);
    FieldPlayer_GetWPos(player, &VStack_20);
    VStack_20.y = 0x370000;
    FieldPlayer_SetWPos(player, &VStack_20);
}
