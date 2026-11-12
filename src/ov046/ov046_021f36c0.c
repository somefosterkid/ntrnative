// Black's code at ov046 0x021f36c0-0x021f4100. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_exp_obj.h"
#include "field/field_map.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "field/field_nogrid_mapper.h"
#include "gfl/sound.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_mtx.h"


GameEventReturnCode func_ov046_021f3bfc(GameEvent *event, u32 *state, void *data);
void func_ov046_021f3fa8(FieldExpObjSystem *param_1, Field *param_2, BOOL param_3);
GameEventReturnCode func_ov046_021f3c8c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov046_021f3d40(GameEvent *event, u32 *state, void *data);
void func_ov046_021f4064(Field *param_1, u32 param_2, BOOL param_3);
FieldActor *func_ov046_021f4020(Field *field, u32 uid);

typedef struct Ov046GimmickAnim {
    u8 unk_00[3];
    u8 unk_03;
    s32 unk_04;
    fx32 unk_08;
    u32 unk_0C[2];
    s32 unk_14;
} Ov046GimmickAnim;

typedef struct Ov046Table {
    u32 unk_00;
    u32 unk_04[2];
} Ov046Table;

typedef struct Ov046GimmickWork {
    u32 unk_00;
    u32 unk_04;
    Ov046Table *unk_08;
    Ov046GimmickAnim anim;
    u32 unk_24;
} Ov046GimmickWork;

const G3DSceneAnimationSetup data_ov046_021f4100[2] = { { 0x11, 0 }, { 0x12, 0 } };

const u16 data_ov046_021f4108[3] = { 0x8000, 0x0, 0x0 };

const G3DSceneAnimationSetup data_ov046_021f410e[2] = { { 0xf, 0 }, { 0x10, 0 } };

const G3DSceneAnimationSetup data_ov046_021f4116[2] = { { 0xd, 0 }, { 0xe, 0 } };

const G3DSceneAnimationSetup data_ov046_021f4150[8] = {
#include "asset/ov046_021f36c0_data_ov046_021f4150.inc"
};

const VecFx32 data_ov046_021f4170[3] = {
    { 0xc8000, 0x0, 0x1d0000 },
    { 0x2a8000, 0x0, 0x170000 },
    { 0x1b8000, 0x60000, 0x1c0000 },
};

const VecFx32 data_ov046_021f4194[3] = {
#include "asset/ov046_021f36c0_data_ov046_021f4194.inc"
};

const VecFx32 data_ov046_021f41b8 = { 0x200000, 0x0, 0x200000 };

const VecFx32 data_ov046_021f4204[6] = {
#include "asset/ov046_021f36c0_data_ov046_021f4204.inc"
};

const G3DSceneResourceSetup data_ov046_021f424c[19] = {
#include "asset/ov046_021f36c0_data_ov046_021f424c.inc"
};

const G3DSceneActorSetup data_ov046_021f4330[16] = {
    { 0, 0, 0, 0, data_ov046_021f4150, 8 },
    { 1, 0, 1, 0, data_ov046_021f4116, 2 },
    { 2, 0, 2, 0, data_ov046_021f410e, 2 },
    { 4, 0, 4, 0, data_ov046_021f4100, 1 },
    { 4, 0, 4, 0, data_ov046_021f4100, 1 },
    { 0, 0, 0, 0, data_ov046_021f4150, 8 },
    { 1, 0, 1, 0, data_ov046_021f4116, 2 },
    { 2, 0, 2, 0, data_ov046_021f410e, 2 },
    { 4, 0, 4, 0, data_ov046_021f4100, 1 },
    { 4, 0, 4, 0, data_ov046_021f4100, 1 },
    { 0, 0, 0, 0, data_ov046_021f4150, 8 },
    { 1, 0, 1, 0, data_ov046_021f4116, 2 },
    { 2, 0, 2, 0, data_ov046_021f410e, 2 },
    { 4, 0, 4, 0, data_ov046_021f4100, 1 },
    { 4, 0, 4, 0, data_ov046_021f4100, 1 },
    { 3, 0, 3, 0, &data_ov046_021f4100[1], 1 },
};

const G3DSceneSetup data_ov046_021f4120 = { data_ov046_021f424c, 0x13, data_ov046_021f4330, 0x10 };


const s32 data_ov046_021f4130[8] = { 8, 16, 8, 0, -8, -16, -24, -32 };

const s32 data_ov046_021f41c4[4][4] = {
#include "asset/ov046_021f36c0_data_ov046_021f41c4.inc"
};
const u32 data_ov046_021f4430[3][4][6] = {
#include "asset/ov046_021f36c0_data_ov046_021f4430.inc"
};

u32 func_ov046_021f40d8(VecFx32 *param_1);
GameEventReturnCode func_ov046_021f3efc(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov046_021f3de8(GameEvent *event, u32 *state, void *data);
void func_ov046_021f4078(Field *param_1, u32 param_2, u32 param_3);
void func_ov046_021f3838(FieldExpObjSystem *system, s32 actor, const VecFx32 *pos, u16 *rot, s32 count, BOOL flag);
void func_ov046_021f38d4(Ov046Table *table, FieldExpObjSystem *system, s32 tableIndex);



void func_ov046_021f36c0(Field *field) {
    FieldExpObjSystem *system;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    Ov046Table *table;
    Ov046GimmickWork *work;
    u16 heapId;
    FieldPlayer *player;
    VecFx32 pos;
    VecFx32 playerPos;
    BOOL flag;
    u16 rot;
    s32 i;
    s32 j;

    system = Field_GetExpObjSystem(field);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    table = GimmickState_GetUserData(gimmick, 10);
    heapId = Field_GetHeapID(field);
    work = Field_AllocGimmickWorkBlock(field, 1, heapId, sizeof(Ov046GimmickWork));
    LoadFieldExpandObjData(system, &data_ov046_021f4120, 0);
    for (i = 0; i < 3; i++) {
        rot = data_ov046_021f4108[i];
        func_ov046_021f3838(system, i * 5, &data_ov046_021f4194[i], &rot, 8, FALSE);
        func_ov046_021f3838(system, i * 5 + 1, &data_ov046_021f4170[i], &rot, 2, FALSE);
        func_ov046_021f3838(system, i * 5 + 2, &data_ov046_021f4170[i], &rot, 2, FALSE);
        func_ov046_021f3838(system, i * 5 + 3, &data_ov046_021f4204[i * 2], NULL, 1, TRUE);
        func_ov046_021f3838(system, i * 5 + 4, &data_ov046_021f4204[i * 2 + 1], NULL, 1, TRUE);
        func_ov046_021f38d4(table, system, i);
    }
    pos = data_ov046_021f41b8;
    func_ov046_021f3838(system, 0xf, &pos, NULL, 0, FALSE);
    FieldExpObj_SetAnm(system, 0, 0xf, 0, TRUE);
    player = Field_GetPlayer(field);
    FieldPlayer_GetWPos(player, &playerPos);
    flag = TRUE;
    if (func_ov046_021f40d8(&playerPos) == 0) {
        flag = FALSE;
    }
    func_ov046_021f3fa8(system, field, flag);
    work->unk_24 = flag;
    for (j = 0; j < 3; j++) {
        func_ov046_021f4078(field, j, table[j].unk_00);
    }
}

void func_ov046_021f3838(FieldExpObjSystem *system, s32 actor, const VecFx32 *pos, u16 *rot, s32 count, BOOL flag) {
    SRTMatrix *mtx;
    FieldExpObjAnm *anm;
    s32 i;

    mtx = FieldExpObj_GetActorMatrixPtr(system, 0, actor);
    mtx->translation = *pos;
    if (rot != NULL) {
        MTX_RotY33_(&mtx->rotation, FX_SinCosTable_[(*rot >> 4) * 2], FX_SinCosTable_[(*rot >> 4) * 2 + 1]);
    }
    func_ov021_021be4a4(system, 0, actor, flag);
    for (i = 0; i < count; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, actor, i);
        FieldExpObjAnm_SetLooped(anm, 0);
        FieldExpObjAnm_SetPaused(anm, 1);
        FieldExpObj_SetAnm(system, 0, actor, i, 0);
    }
}

void func_ov046_021f38d4(Ov046Table *table, FieldExpObjSystem *system, s32 tableIndex) {
    Ov046Table *entry;
    FieldExpObjAnm *anmInfo;
    int actor;
    int anm;

    entry = &table[tableIndex];
    switch (entry->unk_00) {
    case 0:
        anm = 1;
        break;
    case 1:
        anm = 5;
        break;
    case 3:
        anm = 0;
        break;
    case 2:
        anm = 2;
        break;
    default:
        anm = 1;
        break;
    }
    actor = tableIndex * 5;
    anmInfo = FieldExpObj_GetAnmInfo(system, 0, actor, anm);
    FieldExpObjAnm_SetPaused(anmInfo, 0);
    FieldExpObj_SetAnm(system, 0, actor, anm, TRUE);
    FieldExpObj_SetAnmFrame(system, 0, actor, anm, func_ov021_021be748(anmInfo));

    actor = tableIndex * 5 + 1;
    anm = entry->unk_04[0] == 0 ? 0 : 1;
    anmInfo = FieldExpObj_GetAnmInfo(system, 0, actor, anm);
    FieldExpObjAnm_SetPaused(anmInfo, 0);
    FieldExpObj_SetAnm(system, 0, actor, anm, TRUE);
    FieldExpObj_SetAnmFrame(system, 0, actor, anm, func_ov021_021be748(anmInfo));

    actor = tableIndex * 5 + 2;
    anm = entry->unk_04[1] == 0 ? 0 : 1;
    anmInfo = FieldExpObj_GetAnmInfo(system, 0, actor, anm);
    FieldExpObjAnm_SetPaused(anmInfo, 0);
    FieldExpObj_SetAnm(system, 0, actor, anm, TRUE);
    FieldExpObj_SetAnmFrame(system, 0, actor, anm, func_ov021_021be748(anmInfo));
}


void func_ov046_021f3a04(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    Field_GetGimmickWorkBlock(param_1, 1);
    Field_DeleteGimmickWorkBlock(param_1, 1);
    FieldExpObj_FreeScene(system, 0);
}




void func_ov046_021f3a28(Field *field) {
    FieldExpObjSystem *system;
    Ov046GimmickWork *work;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    FieldPlayer *player;
    VecFx32 pos;
    u32 flag;

    system = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 10);
    FieldExpObj_StepAllAnimations(system);
    player = Field_GetPlayer(field);
    FieldPlayer_GetWPos(player, &pos);
    flag = 1;
    if (func_ov046_021f40d8(&pos) == 0) {
        flag = 0;
    }
    if (work->unk_24 != flag) {
        func_ov046_021f3fa8(system, field, flag);
        work->unk_24 = flag;
    }
}


void func_ov046_021f3a88(GameSystem *param_1, s32 param_2, s32 param_3) {
    Field *field;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    Ov046Table *table;
    Ov046GimmickWork *work;

    field = GSYS_GetField(param_1);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    table = GimmickState_GetUserData(gimmick, 10);
    work = Field_GetGimmickWorkBlock(field, 1);
    work->unk_00 = param_2;
    work->unk_04 = param_3;
    work->unk_08 = &table[param_2];
    GameEvent_Create(param_1, (GameEvent *)0x0, (void *)func_ov046_021f3bfc, 0);
}

GameEvent *func_ov046_021f3ad0(GameSystem *param_1, s32 param_2, s32 param_3) {
    Field *field;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    Ov046Table *table;
    Ov046GimmickWork *work;

    field = GSYS_GetField(param_1);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    table = GimmickState_GetUserData(gimmick, 10);
    work = Field_GetGimmickWorkBlock(field, 1);
    work->unk_00 = param_2;
    work->unk_04 = param_3;
    work->unk_08 = &table[param_2];
    return GameEvent_Create(param_1, (GameEvent *)0x0, (void *)func_ov046_021f3c8c, 0);
}

GameEvent *func_ov046_021f3b18(GameSystem *param_1, s32 param_2) {
    Field *field;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    Ov046Table *table;
    Ov046GimmickWork *work;

    field = GSYS_GetField(param_1);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    table = GimmickState_GetUserData(gimmick, 10);
    work = Field_GetGimmickWorkBlock(field, 1);
    work->unk_00 = param_2;
    work->unk_08 = &table[param_2];
    return GameEvent_Create(param_1, (GameEvent *)0x0, (void *)func_ov046_021f3d40, 0);
}

GameEvent *func_ov046_021f3b5c(GameSystem *param_1, s32 param_2) {
    Field *field;
    Ov046GimmickWork *work;
    Ov046GimmickAnim *anim;
    VecFx32 pos;
    GameEvent *event;
    FieldPlayer *player;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;

    field = GSYS_GetField(param_1);
    gsys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 10);
    work = Field_GetGimmickWorkBlock(field, 1);
    event = GameEvent_Create(param_1, NULL, func_ov046_021f3efc, 0);
    anim = &work->anim;
    anim->unk_03 = 0;
    anim->unk_04 = 1 << 13;
    player = Field_GetPlayer(field);
    FieldPlayer_GetWPos(player, &pos);
    anim->unk_08 = pos.y;
    if (param_2 == 2) {
        anim->unk_04 *= -1;
    }
    return event;
}


u32 func_ov046_021f3bc8(Ov046Table *param_1) {
    if (param_1->unk_04[0] == 0) {
        if (param_1->unk_04[1] == 0) {
            return 0;
        }
        return 3;
    }
    if (param_1->unk_04[1] == 0) {
        return 2;
    }
    return 1;
}


s32 func_ov046_021f3be8(Ov046Table *param_1, s32 param_2) {
    return data_ov046_021f41c4[param_1->unk_00][param_2] - 5;
}

GameEventReturnCode func_ov046_021f3bfc(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys;
    Field *field;
    Ov046GimmickWork *work;
    GameEvent *next;

    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), 10);
    switch (*state) {
    case 0:
        work->anim.unk_00[0] = (work->unk_04 == 0 ? 3 : 4) + work->unk_00 * 5;
        work->anim.unk_00[1] = 1;
        work->anim.unk_0C[0] = 0;
        work->anim.unk_00[2] = 1;
        work->anim.unk_0C[1] = 0x6d3;
        work->anim.unk_14 = -1;
        next = GameEvent_Create(gsys, NULL, func_ov046_021f3de8, 0);
        GameEvent_ChainNext(event, next);
        *state += 1;
        break;
    case 1:
        return 1;
    }
    return 0;
}

GameEventReturnCode func_ov046_021f3c8c(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys;
    Field *field;
    Ov046GimmickWork *work;
    GameEvent *next;
    u32 flag;
    u32 index;

    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), 10);
    switch (*state) {
    case 0:
        index = work->unk_04;
        flag = 1;
        if (work->unk_08->unk_04[index] != 0) {
            flag = 0;
        }
        work->anim.unk_00[0] = (index == 0 ? 1 : 2) + work->unk_00 * 5;
        work->anim.unk_00[1] = 1;
        work->anim.unk_0C[0] = flag;
        work->anim.unk_00[2] = 2;
        work->anim.unk_0C[1] = 0x6fc;
        work->anim.unk_14 = 0x6fd;
        next = GameEvent_Create(gsys, NULL, func_ov046_021f3de8, 0);
        GameEvent_ChainNext(event, next);
        work->unk_08->unk_04[work->unk_04] = flag;
        *state += 1;
        break;
    case 1:
        return 1;
    }
    return 0;
}

GameEventReturnCode func_ov046_021f3d40(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys;
    Field *field;
    Ov046GimmickWork *work;
    GameSystem *gameSys;
    GameData *gameData;
    GimmickState *gimmick;
    GameEvent *next;
    u32 val;
    s32 flag;

    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    gameSys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gameSys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 10);
    switch (*state) {
    case 0:
        val = func_ov046_021f3bc8(work->unk_08);
        flag = func_ov046_021f3be8(work->unk_08, val);
        work->anim.unk_00[0] = work->unk_00 * 5;
        work->anim.unk_00[1] = 1;
        work->anim.unk_0C[0] = flag;
        work->anim.unk_00[2] = 8;
        work->anim.unk_0C[1] = 0x6d4;
        work->anim.unk_14 = 0x6d9;
        next = GameEvent_Create(gsys, NULL, func_ov046_021f3de8, 0);
        GameEvent_ChainNext(event, next);
        work->unk_08->unk_00 = val;
        *state += 1;
        break;
    case 1:
        func_ov046_021f4078(field, work->unk_00, work->unk_08->unk_00);
        return 1;
    }
    return 0;
}

GameEventReturnCode func_ov046_021f3de8(GameEvent *event, u32 *state, void *data) {
    Field *field;
    FieldExpObjSystem *system;
    Ov046GimmickAnim *anim;
    u8 actor;
    u8 anm;
    s32 i;

    field = GSYS_GetField(GameEvent_GetGameSystem(event));
    system = Field_GetExpObjSystem(field);
    anim = &((Ov046GimmickWork *)Field_GetGimmickWorkBlock(field, 1))->anim;
    GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), 10);
    switch (*state) {
    case 0:
        actor = anim->unk_00[0];
        for (i = 0; i < anim->unk_00[2]; i++) {
            FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, actor, i), 1);
            FieldExpObj_SetAnm(system, 0, actor, (u8)i, FALSE);
        }
        for (i = 0; i < anim->unk_00[1]; i++) {
            anm = anim->unk_0C[i];
            FieldExpObjAnm_SetPaused(FieldExpObj_GetAnmInfo(system, 0, actor, anm), 0);
            FieldExpObj_SetAnm(system, 0, actor, anm, TRUE);
            FieldExpObj_SetAnmFrame(system, 0, actor, anm, 0);
            GFL_SndSEPlay(anim->unk_0C[1]);
        }
        *state += 1;
        break;
    case 1:
        if (FieldExpObjAnm_IsPlaybackFinished(FieldExpObj_GetAnmInfo(system, 0, anim->unk_00[0], anim->unk_0C[0]))) {
            GFL_SndStop();
            if (anim->unk_14 != -1) {
                GFL_SndSEPlay(anim->unk_14);
            }
            return 1;
        }
        break;
    }
    return 0;
}


GameEventReturnCode func_ov046_021f3efc(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys;
    Field *field;
    Ov046GimmickWork *work;
    Ov046GimmickAnim *anim;
    GameSystem *gameSys;
    GameData *gameData;
    GimmickState *gimmick;
    FieldPlayer *player;
    VecFx32 pos;

    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, 1);
    anim = &work->anim;
    gameSys = Field_GetGameSystem(field);
    gameData = GSYS_GetGameData(gameSys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 10);
    switch (*state) {
    case 0:
        GFL_SndSEPlay(0x55e);
        *state += 1;
        break;
    case 1:
        player = Field_GetPlayer(field);
        FieldPlayer_GetWPos(player, &pos);
        pos.x = pos.x + anim->unk_04;
        pos.y = anim->unk_08 + (data_ov046_021f4130[anim->unk_03] << 12);
        FieldPlayer_SetWPos(player, &pos);
        anim->unk_03 += 1;
        if (anim->unk_03 >= 8) {
            GFL_SndSEPlay(0x6d3);
            return 1;
        }
        break;
    }
    return 0;
}






void func_ov046_021f3fa8(FieldExpObjSystem *param_1, Field *param_2, BOOL param_3) {
    FieldExpObj_SetActorHidden(param_1, 0, 0xf, param_3);
    FieldExpObj_SetActorHidden(param_1, 0, 10, param_3);
    FieldExpObj_SetActorHidden(param_1, 0, 0xb, param_3);
    FieldExpObj_SetActorHidden(param_1, 0, 0xc, param_3);
    FieldExpObj_SetActorHidden(param_1, 0, 0xd, param_3);
    FieldExpObj_SetActorHidden(param_1, 0, 0xe, param_3);
    func_ov046_021f4064(param_2, 4, param_3);
    func_ov046_021f4064(param_2, 5, param_3);
    func_ov046_021f4064(param_2, 9, param_3);
    func_ov046_021f4064(param_2, 6, param_3);
}

FieldActor *func_ov046_021f4020(Field *field, u32 uid) {
    MMSys *mmSys;
    u32 index;
    FieldActor *actor;

    index = 0;
    mmSys = Field_GetActorSystem(field);
    if (NextActor(mmSys, &actor, &index) == TRUE) {
        do {
            if (uid == GetActorUID(actor)) {
                return actor;
            }
        } while (NextActor(mmSys, &actor, &index) == TRUE);
    }
    return NULL;
}


void func_ov046_021f4064(Field *param_1, u32 param_2, BOOL param_3) {
    FieldActor *actor;

    actor = func_ov046_021f4020(param_1, param_2);
    if (actor != (FieldActor *)0x0) {
        SetActorHidden(actor, param_3);
    }
}

void func_ov046_021f4078(Field *param_1, u32 param_2, u32 param_3) {
    NoGridMapper *mapper;
    u32 ids[6];
    BOOL enables[6];
    int i;

    mapper = Field_GetNoGridMapper(param_1);
    for (i = 0; i < 6; i++) {
        ids[i] = data_ov046_021f4430[param_2][param_3][i];
        enables[i] = FALSE;
    }
    if (param_3 != 1) {
        enables[0] = TRUE;
        enables[1] = TRUE;
    }
    for (i = 0; i < 6; i++) {
        func_ov021_021bb3dc(mapper, ids[i], enables[i]);
    }
}


u32 func_ov046_021f40d8(VecFx32 *param_1) {
    if (param_1->y > 0x40000) {
        return 0;
    }
    if (param_1->x >= 0x100000) {
        if (param_1->z >= 0x1e0000) {
            return 1;
        }
    }
    return 0;
}

