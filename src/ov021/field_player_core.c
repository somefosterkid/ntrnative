// Black's code at ov021 0x021a36bc-0x021a41b0. The name is pokebw2's file between field_player.c and
// field_player_grid.c (65 functions there, 71 here).

#include "types.h"
#include "field/field_actor.h"
#include "field/field_player_core.h"
#include "field/field_player_grid.h"
#include "field/field_player_nogrid.h"
#include "field/player_state.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "field/beacon_view.h"
#include "system/game_system.h"
#include "field/field.h"
#include "system/game_data.h"
#include "field/field_sound.h"
#include "field/field_effect.h"
#include "gfl/blact.h"
#include "field/fldmmdl_blact.h"
#include "field/field_rail_loader.h"

u16 func_ov021_021a3c6c(FieldPlayerCore *core, s32 idx); // gap draft, see its own function
void func_ov021_021d05d8(FieldActor *actor, FieldEffects *fieldEffects); // gap draft, owner file not yet known
// fldmmdl_blact.c defines it with a u16 code; the original passes this file's u32 code unnarrowed
void func_ov021_0219d208(FieldActor *actor, u32 objCode);
void func_ov021_021a3b3c(FieldPlayerCore *param_1, u32 param_2);
void FieldPlayerCore_ChangeObjCode(FieldPlayerCore *core, u32 heapId);
u32 FieldPlayer_GetExStateByObjCode(u32 objCode);
u32 GetMoveDirHByKey(u32 param_1);
u32 GetMoveDirVByKey(u32 param_1);
u32 FieldPlayerCore_IsBaseFormeObjCode(FieldPlayerCore *core);
u32 func_ov021_021a3d6c(FieldPlayerCore *core, s32 heapId);
u16 func_ov021_021a3bb4(FieldPlayerCore *core, u16 heldKeys);
void func_ov021_021a3c24(FieldPlayerCore *core, u16 heldKeys);

void FieldPlayerCore_ReqSyncBGM(FieldPlayerCore *core);

u32 func_ov021_02197c9c(FieldActor *);

// pokebw2's sDefaultNpc
static const ZoneNPC data_ov021_021d89a0 = { 0xff, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0xffff, FALSE, { { 0, 0, 0 } } };

typedef u32 (*ExStateChangeFunc)(FieldPlayerCore *core);

u32 func_ov021_021a3da0(FieldPlayerCore *core);
u32 func_ov021_021a3e14(FieldPlayerCore *core);
u32 func_ov021_021a3e48(FieldPlayerCore *core);
u32 func_ov021_021a3e54(FieldPlayerCore *core);
u32 func_ov021_021a3ee0(FieldPlayerCore *core);
u32 func_ov021_021a3f10(FieldPlayerCore *core);
u32 func_ov021_021a3f48(FieldPlayerCore *core);
u32 func_ov021_021a3f64(FieldPlayerCore *core);
u32 func_ov021_021a3f70(FieldPlayerCore *core);
u32 func_ov021_021a3f8c(FieldPlayerCore *core);
u32 func_ov021_021a3f98(FieldPlayerCore *core);
u32 func_ov021_021a3fb4(FieldPlayerCore *core);
u32 func_ov021_021a3fc0(FieldPlayerCore *core);
u32 func_ov021_021a3fdc(FieldPlayerCore *core);
u32 func_ov021_021a3fe8(FieldPlayerCore *core);
u32 func_ov021_021a408c(FieldPlayerCore *core);
u32 func_ov021_021a40a8(FieldPlayerCore *core);

static const ExStateChangeFunc data_ov021_021d895c[1] = { func_ov021_021a3fe8 };
static const ExStateChangeFunc data_ov021_021d8964[1] = { func_ov021_021a3e54 };
static const ExStateChangeFunc data_ov021_021d8960[1] = { func_ov021_021a3da0 };

static const ExStateChangeFunc data_ov021_021d8988[2] = { func_ov021_021a3fc0, func_ov021_021a3fdc };

static const ExStateChangeFunc data_ov021_021d8978[2] = { func_ov021_021a3f48, func_ov021_021a3f64 };
static const ExStateChangeFunc data_ov021_021d8968[2] = { func_ov021_021a3f70, func_ov021_021a3f8c };

static const ExStateChangeFunc data_ov021_021d8980[2] = { func_ov021_021a3ee0, func_ov021_021a3f10 };
static const ExStateChangeFunc data_ov021_021d8990[2] = { func_ov021_021a3e14, func_ov021_021a3e48 };
static const ExStateChangeFunc data_ov021_021d8970[2] = { func_ov021_021a3f98, func_ov021_021a3fb4 };

static const ExStateChangeFunc data_ov021_021d8998[2] = { func_ov021_021a408c, func_ov021_021a40a8 };

const ExStateChangeFunc *const PLAYER_EXSTATE_CHANGE_VTABLES[10] = {
    data_ov021_021d8960, data_ov021_021d8990, data_ov021_021d8964, data_ov021_021d8980, data_ov021_021d8978,
    data_ov021_021d8968, data_ov021_021d8970, data_ov021_021d8988, data_ov021_021d895c, data_ov021_021d8998,
};

void func_ov021_021a36bc(FieldPlayer *player, RailPosition *pos) {
    func_ov021_021a5a0c(player->nogrid, pos);
}

void func_ov021_021a36c8(FieldPlayer *player, u32 dir, RailPosition *pos) {
    func_ov021_021a5a18(player->nogrid, dir, pos);
}


void FieldPlayer_GetRailWorldPos(FieldPlayer *player, VecFx32 *pos) {
    func_ov021_021a5a24(player->nogrid, pos);
}


FieldPlayerGrid *func_ov021_021a36e0(FieldPlayer *player) {
    return player->grid;
}

FieldPlayerCore *FieldPlayerCore_Create(PlayerState *state, Field *field, const VecFx32 *pos, u32 sex, HeapID heapId) {
    FieldPlayerCore *core;
    MMSys *mmSys;
    FieldActor *actor;
    u16 objCode;

    core = GFL_HeapAllocate(heapId, sizeof(FieldPlayerCore), TRUE, "field_player_core.c", 151);
    core->playerState = state;
    core->field = field;
    core->gsys = Field_GetGameSystem(field);
    core->keyDirH = 9;
    core->keyDirV = 9;
    mmSys = Field_GetActorSystem(field);
    core->actor = FindFieldActor(mmSys, 0xff);
    core->sex = sex;
    objCode = FieldPlayerState_GetObjCode(core->playerState);
    actor = core->actor;
    if (actor == NULL) {
        ZoneNPC npc = data_ov021_021d89a0;

        ZoneNPCGridPosition *grid = &npc.pos.grid;

        grid->x = (pos->x >> 4) / 0x1000;
        grid->z = (pos->z >> 4) / 0x1000;
        grid->y = pos->y;
        if (objCode == 0xffff) {
            npc.modelId = FieldPlayer_GetObjCodeByExState(sex, FieldPlayerCore_GetExState(core));
        } else {
            npc.modelId = objCode;
        }
        core->actor = CreateNewActorByEntityNoWKOBJCODE(mmSys, &npc, 0);
    } else {
        s32 y = (pos->y >> 4) / 0x1000;
        s32 z = (pos->z >> 4) / 0x1000;

        SetActorGPosX(actor, (pos->x >> 4) / 0x1000);
        SetActorGPosY(actor, y);
        SetActorGPosZ(actor, z);
        SetActorWPosValue(actor, pos);
        if (objCode == 0xffff) {
            FieldPlayerCore_InitModel(core);
        }
    }
    SetActorFlag32(core->actor, TRUE);
    return core;
}


void FieldPlayerCore_Free(FieldPlayerCore *core) {
    GFL_HeapFree(core);
}

void FieldPlayerCore_UpdateActionStatusObserver(FieldPlayerCore *core) {
    u32 moveStatus;
    u32 prevStatus;
    FieldActor *actor;

    actor = core->actor;
    prevStatus = core->observeStatus;
    core->observeStatus = 0;
    moveStatus = core->moveStatus;
    if (IsAllActorAcmdFinished(actor) == FALSE) {
        switch (moveStatus) {
        case 0:
            break;
        case 1:
            if (prevStatus == 0 || prevStatus == 3) {
                core->observeStatus = 1;
            } else {
                core->observeStatus = 2;
            }
            break;
        case 2:
            core->observeStatus = 2;
            break;
        }
    } else if (func_ov010_0216d454(actor) == 1) {
        switch (moveStatus) {
        case 0:
            break;
        case 1:
            if (prevStatus != 0) {
                if (prevStatus == 3) {
                    core->observeStatus = 0;
                } else {
                    core->observeStatus = 3;
                }
            }
            break;
        case 2:
            if (prevStatus != 0) {
                if (prevStatus == 3) {
                    core->observeStatus = 0;
                } else {
                    core->observeStatus = 3;
                }
            }
            break;
        }
    }
}



void FieldPlayerCore_GetWPos(FieldPlayerCore *core, VecFx32 *pos) {
    CopyActorWPos(core->actor, pos);
}

void func_ov021_021a38a4(FieldPlayerCore *core, VecFx32 *pos) {
    FieldActor *actor;
    s32 x = (pos->x >> 4) / 0x1000;
    s32 y = (pos->y >> 4) / 0x1000;
    s32 z = (pos->z >> 4) / 0x1000;

    actor = core->actor;
    SetActorInitialGPosX(actor, GetGPosX(actor));
    SetActorInitialGPosY(actor, FldAct_GetGPosY(actor));
    SetActorInitialGPosZ(actor, GetGPosZ(actor));
    SetActorGPosX(actor, x);
    SetActorGPosY(actor, y);
    SetActorGPosZ(actor, z);
    SetActorWPosValue(actor, pos);
}


u16 FieldPlayerCore_GetFaceDir(FieldPlayerCore *core) {
    return GetActorFaceDir(core->actor);
}

void func_ov021_021a392c(FieldPlayerCore *core, u16 dir) {
    CheckSetActorFaceDir(core->actor, dir);
}

void FieldPlayerCore_ForceObjCode(FieldPlayerCore *core, u16 objCode) {
    FieldPlayerCore_ChangeObjCode(core, objCode);
    FieldPlayerState_SetObjCode(core->playerState, objCode);
}


void FieldPlayerCore_ClearObjCode(FieldPlayerCore *core) {
    FieldPlayerState_SetObjCode(core->playerState, 0xffff);
}

Field *FieldPlayerCore_GetField(FieldPlayerCore *core) {
    return core->field;
}

FieldActor *FieldPlayerCore_GetActor(FieldPlayerCore *core) {
    return core->actor;
}

void FieldPlayerCore_SetMoveStatus(FieldPlayerCore *core, u32 status) {
    core->moveStatus = status;
}

u32 func_ov021_021a3968(FieldPlayerCore *core) {
    return core->moveStatus;
}

void FieldPlayerCore_ForceMoveObserveStatus(FieldPlayerCore *core, u32 status) {
    core->observeStatus = status;
}

u32 func_ov021_021a3970(FieldPlayerCore *core) {
    return core->observeStatus;
}

u32 FieldPlayerCore_GetExState(FieldPlayerCore *core) {
    return FieldPlayerState_GetExState(core->playerState);
}

void FieldPlayerCore_SetExState(FieldPlayerCore *core, u32 state) {
    SetPlayerSpecialState(core->playerState, state);
}

u32 FieldPlayerCore_GetSex(FieldPlayerCore *core) {
    return core->sex;
}

GameSystem *FieldPlayerCore_GetGameSystem(FieldPlayerCore *core) {
    return core->gsys;
}

PlayerState *FieldPlayerCore_GetPlayerState(FieldPlayerCore *core) {
    return core->playerState;
}

void FieldPlayerCore_InitModel(FieldPlayerCore *core) {
    func_ov021_021a3b3c(core, FieldPlayerCore_GetExState(core));
}

void func_ov021_021a39ac(FieldPlayerCore *param_1, s32 param_2) {
    u16 heapId;
    s32 sex;
    FieldActor *actor;

    sex = FieldPlayerCore_GetSex(param_1);
    actor = FieldPlayerCore_GetActor(param_1);
    heapId = FieldPlayer_GetObjCodeByForme(sex, param_2);
    if ((u32)heapId != (u32)FldAct_GetObjCode(actor)) {
        FieldPlayerCore_ChangeObjCode(param_1, (u32)heapId);
    }
    param_1->unk_2c = 0;
}

BOOL func_ov021_021a39e4(FieldPlayerCore *core) {
    if (FldAct_GetBlActIdx(FieldPlayerCore_GetActor(core), 0) != 0xffff) {
        if (core->unk_2c == 1) {
            return 1;
        }
        core->unk_2c++;
    }
    return 0;
}


u32 FieldPlayerCore_DeriveExState(FieldPlayerCore *param_1) {
    FieldActor *actor;

    actor = FieldPlayerCore_GetActor(param_1);
    return FieldPlayer_GetExStateByObjCode((u32)FldAct_GetObjCode(actor));
}

BOOL func_ov021_021a3a20(FieldPlayerCore *param_1) {
    FieldActor *actor;

    actor = FieldPlayerCore_GetActor(param_1);
    if (FieldPlayer_GetExStateByObjCode((u32)FldAct_GetObjCode(actor)) != 10) {
        return 1;
    }
    return 0;
}

BOOL func_ov021_021a3a3c(FieldPlayerCore *core) {
    MMSys *sys;
    BlActSys *blSys;
    u32 blActIdx;
    u16 cmd;

    sys = GetActorMModelSystem(core->actor);
    blSys = FieldBlAct_GetBlActSys(func_ov010_0216d520(sys));
    if (func_ov021_02197c9c(core->actor) == 1) {
        if (func_ov021_0219c648(core->actor) == 1) {
            return 1;
        }
    }
    blActIdx = FldAct_GetBlActIdx(core->actor, 0);
    if (blActIdx != 0xffff) {
        if (func_020508ec(blSys, (u16)blActIdx, &cmd) != 0) {
            if (cmd == 0x3fff) {
                return 1;
            }
        }
    }
    return 0;
}


BOOL FieldPlayerCore_IsAcmdFinished(FieldPlayerCore *param_1) {
    FieldActor *actor;
    BOOL BVar1;

    actor = FieldPlayerCore_GetActor(param_1);
    BVar1 = IsAllActorAcmdFinished(actor);
    if (BVar1 == 1) {
        return 1;
    }
    return 0;
}

u16 FieldPlayerCore_GetMoveDirByKey(FieldPlayerCore *core, u32 heldKeys) {
    return func_ov021_021a3bb4(core, heldKeys);
}

void FieldPlayerCore_CalcSeparateKeyDirs(FieldPlayerCore *core, u32 heldKeys) {
    func_ov021_021a3c24(core, heldKeys);
}



void func_ov021_021a3ad0(FieldPlayerCore *core, FieldEffectTask *task) {
    core->terrainEffect = task;
}

FieldEffectTask *FieldPlayerCore_GetTerrainEffectTCB(FieldPlayerCore *core) {
    return core->terrainEffect;
}

void FieldPlayerCore_ChangeObjCode(FieldPlayerCore *core, u32 heapId) {
    FieldActor *actor;
    MMSys *sys;
    u16 objCode;

    actor = FieldPlayerCore_GetActor(core);
    sys = GetActorMModelSystem(actor);
    objCode = FldAct_GetObjCode(actor);
    if (heapId == 1 || heapId == 4 || objCode == 1 || objCode == 4) {
        func_ov021_0219d208(actor, heapId);
        FldActSys_ForceFullSync(sys);
        return;
    }
    FldAct_UpdateBlInfoForNewObjCode(actor, FieldPlayer_GetObjCodeByExState((s32)FieldPlayerCore_GetSex(core), 0));
    FldActSys_ForceFullSync(sys);
    func_ov021_0219d208(actor, heapId);
    FldActSys_ForceFullSync(sys);
}


void func_ov021_021a3b3c(FieldPlayerCore *param_1, u32 param_2) {
    u16 heapId;
    s32 sex;
    FieldActor *actor;

    sex = FieldPlayerCore_GetSex(param_1);
    actor = FieldPlayerCore_GetActor(param_1);
    heapId = FieldPlayer_GetObjCodeByExState(sex, param_2);
    if ((u32)heapId != (u32)FldAct_GetObjCode(actor)) {
        FieldPlayerCore_ChangeObjCode(param_1, (u32)heapId);
    }
    FieldPlayerCore_SetExState(param_1, param_2);
}

u32 FieldPlayerCore_IsBaseFormeObjCode(FieldPlayerCore *core) {
    u16 objCode;

    objCode = FldAct_GetObjCode(FieldPlayerCore_GetActor(core));
    if (objCode == 1 || objCode == 4) {
        return 1;
    }
    return 0;
}

BOOL func_ov021_021a3b94(FieldPlayerCore *core) {
    u16 blActIdx;
    BOOL ret;

    ret = FALSE;
    blActIdx = FldAct_GetBlActIdx(core->actor, 0);
    if (blActIdx != 0xffff) {
        ret = TRUE;
    }
    return ret;
}

u16 func_ov021_021a3bb4(FieldPlayerCore *core, u16 heldKeys) {
    u32 dirX;
    u32 dirY;
    u32 actorDir;
    u16 prevX;
    u16 prevY;

    dirX = GetMoveDirHByKey(heldKeys);
    dirY = GetMoveDirVByKey(heldKeys);
    if (dirX == 9) {
        return dirY;
    }
    if (dirY == 9) {
        return dirX;
    }
    actorDir = GetActorMotionDir(FieldPlayerCore_GetActor(core));
    prevX = core->keyDirH;
    prevY = core->keyDirV;
    if (actorDir != 9) {
        if (dirX == prevX && dirY == prevY) {
            if (dirY == actorDir) {
                return dirY;
            }
            if (dirX == actorDir) {
                return dirX;
            }
        }
        if (dirY != prevY) {
            return dirY;
        }
        return dirX;
    }
    return dirY;
}




void func_ov021_021a3c24(FieldPlayerCore *core, u16 heldKeys) {
    core->keyDirH = GetMoveDirHByKey(heldKeys);
    core->keyDirV = GetMoveDirVByKey(heldKeys);
}

u32 GetMoveDirHByKey(u32 param_1) {
    if ((param_1 & 0x20) != 0) {
        return 2;
    }
    if ((param_1 & 0x10) != 0) {
        return 3;
    }
    return 9;
}

u32 GetMoveDirVByKey(u32 param_1) {
    if ((param_1 & 0x40) != 0) {
        return 0;
    }
    if ((param_1 & 0x80) != 0) {
        return 1;
    }
    return 9;
}

u16 func_ov021_021a3c6c(FieldPlayerCore *param_1, s32 param_2) {
    if (param_1->seqDone == 0) {
        const ExStateChangeFunc *funcs = PLAYER_EXSTATE_CHANGE_VTABLES[param_2];
        do {
        } while (funcs[param_1->seqStep](param_1) == 1);
    }
    return param_1->seqDone;
}

s32 func_ov021_021a3c94(u32 heapId) {
    s32 i;

    i = 10;
    if (heapId != 0) {
        for (i = 0; i < 10; i++) {
            if (heapId & 1) {
                break;
            }
            heapId = heapId >> 1;
        }
    }
    return i;
}


void FieldPlayerCore_SetSpecialSeq(FieldPlayerCore *core, u32 seq) {
    core->specialSeq = seq;
    core->seqStep = 0;
    core->seqDone = 0;
    core->unk_2c = 0;
    core->unk_2e = 0;
}

BOOL func_ov021_021a3cc0(FieldPlayerCore *core) {
    s32 idx;

    idx = func_ov021_021a3c94(core->specialSeq);
    if (idx >= 10) {
        return 1;
    }
    if (func_ov021_021a3c6c(core, idx) == 1) {
        return 1;
    }
    return 0;
}

void func_ov021_021a3ce8(FieldPlayerCore *core) {
    s32 idx;
    u16 status;

    idx = func_ov021_021a3c94(core->specialSeq);
    if (idx < 10) {
        do {
            status = func_ov021_021a3c6c(core, idx);
        } while (status == 0);
    }
}



u32 FieldPlayerCore_CheckSpecialObjCode(FieldPlayerCore *param_1, u32 param_2) {
    FieldActor *actor;
    s32 sex;

    actor = FieldPlayerCore_GetActor(param_1);
    if (param_2 == FldAct_GetObjCode(actor)) {
        param_1->seqDone = 1;
        return 1;
    }
    sex = FieldPlayerCore_IsBaseFormeObjCode(param_1);
    if (sex == 1) {
        FieldPlayerCore_ChangeObjCode(param_1, param_2);
        param_1->seqDone = 1;
        return 1;
    }
    sex = FieldPlayerCore_GetSex(param_1);
    FieldPlayerCore_ChangeObjCode(param_1, (u32)FieldPlayer_GetObjCodeByForme(sex, 0));
    return 0;
}

u32 func_ov021_021a3d50(FieldPlayerCore *core, s32 forme) {
    return FieldPlayerCore_CheckSpecialObjCode(core, (u32)FieldPlayer_GetObjCodeByForme(FieldPlayerCore_GetSex(core), forme));
}

u32 func_ov021_021a3d6c(FieldPlayerCore *core, s32 heapId) {
    if (func_ov021_021a3b94(core) == 1) {
        core->unk_2e = core->unk_2e + 1;
        FieldPlayerCore_ChangeObjCode(core, FieldPlayer_GetObjCodeByForme(FieldPlayerCore_GetSex(core), heapId));
        core->seqDone = 1;
        return 1;
    }
    return 0;
}

u32 func_ov021_021a3da0(FieldPlayerCore *core) {
    VecFx32 pos = {0, 0, 0};
    FieldActor *actor;
    s32 base;
    s32 dir;
    FieldEffectTask *task;

    base = FieldPlayerCore_GetSex(core);
    actor = FieldPlayerCore_GetActor(core);
    dir = FieldPlayer_GetObjCodeByForme(base, 0);
    if (dir != FldAct_GetObjCode(actor)) {
        FieldPlayerCore_ChangeObjCode(core, dir);
    }
    FieldPlayerCore_SetExState(core, 0);
    FieldPlayerCore_ReqSyncBGM(core);
    task = FieldPlayerCore_GetTerrainEffectTCB(core);
    if (task != NULL) {
        func_ov021_021ac154(task);
        func_ov021_021a3ad0(core, NULL);
    }
    func_ov010_0216d8e0(actor, &pos);
    core->seqDone = 1;
    return 0;
}


u32 func_ov021_021a3e14(FieldPlayerCore *core) {
    FieldPlayerCore_GetSex(core);
    FieldPlayerCore_GetActor(core);
    FieldPlayerCore_SetExState(core, 1);
    FieldPlayerCore_ReqSyncBGM(core);
    if (func_ov021_021a3d50(core, 1) != 1) {
        core->seqStep = core->seqStep + 1;
    }
    return 0;
}



u32 func_ov021_021a3e48(FieldPlayerCore *param_1) {
    func_ov021_021a3d6c(param_1, 1);
    return 0;
}

u32 func_ov021_021a3e54(FieldPlayerCore *core) {
    FieldActor *actor;
    FieldEffects *fieldEffects;
    VecFx32 pos;
    s32 base;
    s32 dir;
    u16 faceDir;

    base = FieldPlayerCore_GetSex(core);
    actor = FieldPlayerCore_GetActor(core);
    dir = FieldPlayer_GetObjCodeByForme(base, 2);
    if (dir != FldAct_GetObjCode(actor)) {
        FieldPlayerCore_ChangeObjCode(core, dir);
    }
    if (FieldPlayerCore_GetExState(core) != 2) {
        FieldPlayerCore_SetExState(core, 2);
    }
    FieldPlayerCore_ReqSyncBGM(core);
    if (FieldPlayerCore_GetTerrainEffectTCB(core) == NULL) {
        fieldEffects = Field_GetFieldEffects(core->field);
        faceDir = GetActorFaceDir(actor);
        CopyActorWPos(actor, &pos);
        func_ov021_021a3ad0(core, func_ov021_021acb38(fieldEffects, faceDir, &pos, actor, 1));
    }
    core->seqDone = 1;
    return 0;
}


u32 func_ov021_021a3ee0(FieldPlayerCore *param_1) {
    u32 exState;
    s32 sex;

    exState = FieldPlayerCore_GetExState(param_1);
    sex = FieldPlayerCore_GetSex(param_1);
    sex = FieldPlayerCore_CheckSpecialObjCode(param_1, (u32)FieldPlayer_GetObjCodeByExState(sex, exState));
    if (sex != 1) {
        param_1->seqStep = param_1->seqStep + 1;
    }
    return 0;
}

u32 func_ov021_021a3f10(FieldPlayerCore *core) {
    u32 exState;

    if (func_ov021_021a3b94(core) == 1) {
        core->unk_2e = core->unk_2e + 1;
        exState = FieldPlayerCore_GetExState(core);
        FieldPlayerCore_ChangeObjCode(core, FieldPlayer_GetObjCodeByExState((s32)FieldPlayerCore_GetSex(core), exState));
        core->seqDone = 1;
    }
    return 0;
}


u32 func_ov021_021a3f48(FieldPlayerCore *core) {
    if (func_ov021_021a3d50(core, 4) != 1) {
        core->seqStep = core->seqStep + 1;
    }
    return 0;
}


u32 func_ov021_021a3f64(FieldPlayerCore *param_1) {
    func_ov021_021a3d6c(param_1, 4);
    return 0;
}

u32 func_ov021_021a3f70(FieldPlayerCore *core) {
    if (func_ov021_021a3d50(core, 5) != 1) {
        core->seqStep = core->seqStep + 1;
    }
    return 0;
}


u32 func_ov021_021a3f8c(FieldPlayerCore *param_1) {
    func_ov021_021a3d6c(param_1, 5);
    return 0;
}

u32 func_ov021_021a3f98(FieldPlayerCore *core) {
    if (func_ov021_021a3d50(core, 6) != 1) {
        core->seqStep = core->seqStep + 1;
    }
    return 0;
}


u32 func_ov021_021a3fb4(FieldPlayerCore *param_1) {
    func_ov021_021a3d6c(param_1, 6);
    return 0;
}

u32 func_ov021_021a3fc0(FieldPlayerCore *core) {
    if (func_ov021_021a3d50(core, 9) != 1) {
        core->seqStep = core->seqStep + 1;
    }
    return 0;
}


u32 func_ov021_021a3fdc(FieldPlayerCore *param_1) {
    func_ov021_021a3d6c(param_1, 9);
    return 0;
}

u32 func_ov021_021a3fe8(FieldPlayerCore *core) {
    FieldActor *actor;
    FieldEffects *fieldEffects;
    FieldEffects *effects;
    VecFx32 pos;
    s32 base;
    s32 dir;
    u16 faceDir;

    fieldEffects = Field_GetFieldEffects(core->field);
    base = FieldPlayerCore_GetSex(core);
    actor = FieldPlayerCore_GetActor(core);
    dir = FieldPlayer_GetObjCodeByForme(base, 3);
    if (dir != FldAct_GetObjCode(actor)) {
        FieldPlayerCore_ChangeObjCode(core, dir);
    }
    if (FieldPlayerCore_GetExState(core) != 3) {
        FieldPlayerCore_SetExState(core, 3);
    }
    if (FieldPlayerCore_GetTerrainEffectTCB(core) == NULL) {
        effects = Field_GetFieldEffects(core->field);
        faceDir = GetActorFaceDir(actor);
        CopyActorWPos(actor, &pos);
        func_ov021_021a3ad0(core, func_ov021_021acb38(effects, faceDir, &pos, actor, 1));
    }
    func_ov021_021d05d8(core->actor, fieldEffects);
    if (core->terrainEffect != NULL) {
        func_ov021_021acbc8(core->terrainEffect, FALSE);
    }
    core->seqDone = 1;
    return 0;
}


u32 func_ov021_021a408c(FieldPlayerCore *core) {
    if (func_ov021_021a3d50(core, 8) != 1) {
        core->seqStep = core->seqStep + 1;
    }
    return 0;
}

u32 func_ov021_021a40a8(FieldPlayerCore *core) {
    u32 found;
    FieldActor *actor;
    MMSys *sys;
    s16 pos[2];

    func_ov021_021a3d6c(core, 8);
    if (core->seqDone == 1) {
        if (FieldPlayerCore_GetExState(core) == 2) {
            found = 0;
            actor = FieldPlayerCore_GetActor(core);
            sys = GetActorMModelSystem(actor);
            pos[1] = GetGPosX(actor);
            pos[0] = GetGPosZ(actor);
            AdjusGridXZByDir(1, &pos[1], &pos[0], 1);
            if (GetFirstActorOnGPos(sys, pos[1], pos[0], 1) != NULL) {
                found = 1;
            }
            func_ov021_0219f5c8(actor, found);
        }
    }
    return 0;
}


void FieldPlayerCore_ReqSyncBGM(FieldPlayerCore *core) {
    GameData *gameData;
    FieldSound *fieldSound;
    u32 zoneId;

    gameData = GSYS_GetGameData(Field_GetGameSystem(core->field));
    fieldSound = GameData_GetFieldSoundSystem(gameData);
    zoneId = Field_GetPlayerStateZoneID(core->field);
    FieldSnd_SetBGMOnPlayerExStateChange(fieldSound, gameData, zoneId);
}

FieldPlayerGrid *func_ov021_021a4140(FieldPlayerCore *core, HeapID heapId) {
    FieldPlayerGrid *grid;

    grid = GFL_HeapAllocate(heapId, sizeof(FieldPlayerGrid), TRUE, "field_player_grid.c", 381);
    grid->core = core;
    grid->field = FieldPlayerCore_GetField(core);
    grid->stepSE = 0x546;
    FieldPlayerGrid_SetConstMoveEndSpotFlag(grid);
    switch (FieldPlayerCore_GetExState(grid->core)) {
    case 2:
        FieldPlayerCore_SetSpecialSeq(grid->core, 4);
        func_ov021_021a3ce8(grid->core);
        break;
    case 3:
        FieldPlayerCore_SetSpecialSeq(grid->core, 0x100);
        func_ov021_021a3ce8(grid->core);
        break;
    }
    return grid;
}



void func_ov021_021a41a8(FieldPlayerGrid *grid) {
    GFL_HeapFree(grid);
}


