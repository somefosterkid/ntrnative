// Black's code at ov018 0x0217c458-0x0217cdbc. No name is known: the module and address.

#include "types.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/intrude_comm.h"
#include "field/intrude_field.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "field/ov018_0217da54.h"
#include "ov060_021f36c0.h"
#include "gfl/key.h"
#include "field/event_sound.h"
#include "gfl/net.h"
#include "gfl/msg.h"
#include "field/field_player.h"
#include "save/event_work.h"
#include "field/field_script.h"
#include "field/ov018_0217c458.h"
#include "field/event_comm_common.h"

typedef struct {
    HeapID heapId;
    Field *field;
    Ov018MsgWork msg;
    IntrudeMissionEntry entry;
    Ov018WarpSpin spin;
    u8 unk_84;
} UnkStruct_ov018_0217c4d0;

// The work of func_ov018_0217ca84's event
typedef struct Ov018CommExitWork {
    MsgData *msgData;
    MsgBGTalkWin *talkWin;
    Ov018WarpSpin spin;
    // func_ov018_0217ca84's param_2: which message to show
    u32 unk_20;
    u32 unk_24;
} Ov018CommExitWork;

GameEventReturnCode func_ov018_0217cafc(GameEvent *param_1, u32 *param_2, void *data);
GameEvent *func_ov018_0217ca40(GameSystem *gsys);
static ZoneNPC data_ov018_02183af0 = { 0, 0, 0, 0, 0, 0, 1 };
static const u8 data_ov018_021836a0[4] = { 0x00, 0x02, 0x01, 0x03 };
GameEventReturnCode func_ov018_0217c4d0(GameEvent *event, u32 *state, void *data);
GameEvent *func_ov021_021c06cc(GameSystem *gsys, Field *field);



GameEvent *func_ov018_0217c458(GameSystem *gsys, Field *field, IntrudeComm *comm, u16 param_4) {
    IntrudeMissionEntry *entry;
    GameEvent *event;
    UnkStruct_ov018_0217c4d0 *work;

    entry = func_ov011_02175b34(&comm->unk_1AC);
    GSYS_GetGameData(gsys);
    event = GameEvent_Create(gsys, NULL, func_ov018_0217c4d0, sizeof(UnkStruct_ov018_0217c4d0));
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(UnkStruct_ov018_0217c4d0));
    work->heapId = param_4;
    work->field = field;
    work->entry = *entry;
    work->unk_84 = 1;
    func_ov018_0217f2b4(&work->msg, gsys);
    func_ov011_02174e14(comm, 5);
    return event;
}

GameEventReturnCode func_ov018_0217c4d0(GameEvent *event, u32 *state, void *data) {
    UnkStruct_ov018_0217c4d0 *work = data;
    GameSystem *gsys;
    IntrudeComm *comm;
    GameEvent *next;
    u16 kind;
    u16 msgId;
    u8 idx;

    gsys = GameEvent_GetGameSystem(event);
    comm = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    if (comm == NULL && *state == 0) {
        *state = 8;
    }
    switch (*state) {
    case 0:
        if (work->unk_84 == 0) {
            *state = 4;
            break;
        }
        func_ov018_0217f528(&work->msg, gsys, 1, 1, 0x1e, 0x10);
        func_ov011_021762d8(&work->entry.mission, &work->entry.player, work->msg.wordSet, work->heapId);
        kind = work->entry.mission.kind;
        msgId = work->entry.mission.msgId;
        func_ov018_0217f574(&work->msg, kind, 8, 0);
        func_ov018_0217f574(&work->msg, msgId, 8, 0x10);
        (*state)++;
        break;
    case 1:
        next = EventMEPlay_Create(gsys, 0x52b);
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    case 2:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            next = EventPushBGMFinish_Create(gsys, 0, 6);
            GameEvent_ChainNext(event, next);
            (*state)++;
        }
        break;
    case 3:
        func_ov018_0217f58c(&work->msg);
        (*state)++;
        break;
    case 4:
        idx = func_02012b58(GSYS_GetGameData(gsys));
        func_ov018_0217c658(&work->spin, gsys, work->field, work->entry.mission.unk_0A[idx], work->entry.mission.unk_10[idx], work->entry.mission.unk_08_0 & (1 << idx));
        (*state)++;
        break;
    case 5:
        if (func_ov018_0217c688(&work->spin, comm) == TRUE) {
            (*state)++;
        }
        break;
    case 6:
        if (comm != NULL) {
            if (func_ov011_02173fc0(comm, &comm->unk_1AC) == TRUE) {
                (*state)++;
            }
        } else {
            (*state)++;
        }
        break;
    case 7:
        if (comm != NULL) {
            if (func_ov011_02175a24(&comm->unk_1AC) == TRUE) {
                (*state)++;
            }
        } else {
            (*state)++;
        }
        break;
    case 8:
        func_ov018_0217f318(&work->msg);
        if (comm != NULL) {
            func_ov011_02174e14(comm, 0);
        }
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


void func_ov018_0217c658(Ov018WarpSpin *spin, GameSystem *gsys, Field *field, u16 param_4, u8 param_5, u8 param_6) {
    MI_CpuFill8(spin, 0, sizeof(Ov018WarpSpin));
    spin->gsys = gsys;
    spin->field = field;
    spin->unk_0c = param_4;
    spin->unk_0e = param_5;
    spin->unk_0f = param_6;
    spin->unk_0a = 0xf;
    spin->unk_08 = spin->unk_0a;
}

u32 func_ov018_0217c688(Ov018WarpSpin *spin, IntrudeComm *comm) {
    FieldPlayer *player;
    FieldActor *actor;

    switch (spin->state) {
    case 0:
        spin->unk_08 = spin->unk_0a = 0xf;
        GFL_SndSEPlay(0x738);
        spin->state = spin->state + 1;
        break;
    case 1:
        if (Field_CheckMapLoadFinished(spin->field) == TRUE) {
            player = Field_GetPlayer(spin->field);
            actor = FieldPlayer_GetActor(player);
            if (IsAllActorAcmdFinished(actor) == TRUE) {
                if (spin->unk_0a >= 0xf && spin->unk_12 != 0 && data_ov018_021836a0[spin->unk_11] == 1) {
                    spin->state = spin->state + 1;
                } else {
                    spin->unk_11 = (spin->unk_11 + 1) & 3;
                    spin->unk_08 = spin->unk_0a;
                    if (spin->unk_12 == 0) {
                        spin->unk_0a--;
                        if (spin->unk_0a <= 1) {
                            GSYS_GetGameData(spin->gsys);
                            func_ov018_0217bcdc(comm, spin->gsys, spin->unk_0c, spin->unk_0e, spin->unk_0f);
                            spin->unk_12++;
                        }
                    } else if (spin->unk_0a < 0xf) {
                        spin->unk_0a++;
                    }
                    FldAct_SetAcmd(actor, data_ov018_021836a0[spin->unk_11]);
                }
            }
        }
        break;
    default:
        return 1;
    }
    return 0;
}


void func_ov018_0217c758(Ov018WarpSpin *spin, GameSystem *gsys, Field *field, IntrudeComm *param_4, u16 param_5, u8 param_6, u8 param_7, u16 param_8, s32 param_9) {
    FieldPlayer *player;
    FieldActor *actor;
    MMSys *mmSys;
    u16 zoneId;
    FieldActor *npcActor;
    s16 x;
    s16 z;
    fx32 y;

    MI_CpuFill8(spin, 0, sizeof(Ov018WarpSpin));
    spin->gsys = gsys;
    spin->field = field;
    spin->unk_0c = param_5;
    spin->unk_0e = param_6;
    spin->unk_0f = param_7;
    spin->unk_0a = 0xf;
    spin->unk_08 = spin->unk_0a;
    spin->direction = param_8;
    if (param_9 != 0) {
        mmSys = GameData_GetMMSys(GSYS_GetGameData(gsys));
        player = Field_GetPlayer(field);
        actor = FieldPlayer_GetActor(player);
        zoneId = Field_GetPlayerStateZoneID(field);
        data_ov018_02183af0.modelId = FldAct_GetObjCode(actor);
        data_ov018_02183af0.direction = param_8;
        x = GetGPosX(actor);
        z = GetGPosZ(actor);
        y = GetActorPosY(actor);
        func_ov010_0216e618(&data_ov018_02183af0, x, z, y);
        npcActor = CreateNewActorByEntityNoWKOBJCODE(mmSys, &data_ov018_02183af0, zoneId);
        spin->unk_14 = npcActor;
        func_ov018_0217bcdc(param_4, spin->gsys, spin->unk_0c, spin->unk_0e, spin->unk_0f);
        SetActorHidden(actor, TRUE);
        SetActorHidden(spin->unk_14, FALSE);
    }
}

u32 func_ov018_0217c820(Ov018WarpSpin *spin, IntrudeComm *param_2) {
    FieldPlayer *player;
    FieldActor *actor;
    VecFx32 pos;
    u16 acmd;

    player = Field_GetPlayer(spin->field);
    actor = FieldPlayer_GetActor(player);
    switch (spin->state) {
    case 0:
        acmd = spin->direction == 3 ? 7 : 6;
        FldAct_SetAcmd(actor, acmd);
        if (spin->unk_14 != NULL) {
            FldAct_SetAcmd(spin->unk_14, acmd);
        }
        FieldPlayer_GetWPos(player, &pos);
        if (spin->direction == 3) {
            pos.x = pos.x + 0x8000;
        } else {
            pos.x = pos.x - 0x8000;
        }
        func_ov060_021f37b8(spin->field, &pos);
        GFL_SndSEPlay(0x774);
        spin->state++;
        break;
    case 1:
        spin->unk_0a--;
        if (spin->unk_0a <= 0) {
            if (spin->unk_14 != NULL) {
                SetActorHidden(actor, FALSE);
                SetActorHidden(spin->unk_14, TRUE);
            }
            spin->state++;
        }
        break;
    case 2:
        if (func_ov060_021f37d8(spin->field) == 0 && func_ov010_0216d454(actor) == TRUE) {
            func_ov010_0216d480(actor);
            if (spin->unk_14 != NULL) {
                func_ov010_0216d480(spin->unk_14);
            }
            spin->state++;
        }
        break;
    case 3:
        if (spin->unk_14 != NULL) {
            DeleteActor(spin->unk_14);
            spin->unk_14 = NULL;
        }
        return 1;
    }
    return 0;
}

u32 func_ov018_0217c914(IntrudeComm *work, IntrudeCommEvent *param_2, u8 *state) {
    switch (*state) {
    case 0:
        func_ov018_0217f3c4(&param_2->common.msg, 0);
        (*state)++;
        break;
    case 1:
        if (func_ov011_02173a2c(work, work->unk_154.netId, &param_2->common.entry, param_2->common.unk_41) == TRUE) {
            (*state)++;
        }
        break;
    case 2:
        switch (func_ov011_02175514(work)) {
        case 1:
            *state = 6;
            break;
        case 2:
        case 3:
            *state = 3;
            break;
        default:
            if (func_ov018_0217f40c(&param_2->common.msg) == TRUE && (GCTX_HIDGetPressedKeys() & 2)
                && func_ov011_02173b9c(work->unk_154.netId, work->unk_154.unk_04) == TRUE) {
                func_ov011_021753b8(work);
                *state = 9;
            }
            break;
        }
        break;
    case 3:
        if (func_ov018_0217f40c(&param_2->common.msg) == TRUE) {
            func_ov018_0217f3c4(&param_2->common.msg, 2);
            (*state)++;
        }
        break;
    case 4:
        if (func_ov018_0217f40c(&param_2->common.msg) == TRUE) {
            (*state)++;
        }
        break;
    case 5:
        if (func_ov018_0217f420() == TRUE) {
            *state = 8;
        }
        break;
    case 6:
        if (func_ov011_02173ae0(work, work->unk_154.netId, 1, work->unk_154.unk_04) == TRUE) {
            *state = 7;
        }
        break;
    case 7:
        if (func_ov018_0217f40c(&param_2->common.msg) == TRUE) {
            return 1;
        }
        break;
    case 8:
        return 2;
    case 9:
        return 3;
    }
    return 0;
}




GameEvent *func_ov018_0217ca1c(GameSystem *param_1) {
    VecFx32 VStack_20;

    func_ov018_0217bed4(0x117, &VStack_20);
    return EventMapChangeEnding_Create(param_1, 0x117, &VStack_20);
}

GameEvent *func_ov018_0217ca40(GameSystem *param_1) {
    GameCommSys *commSys;
    GameData *gameData;
    IntrudeComm *work;
    u32 netId;
    commSys = GSYS_GetGameCommSystem(param_1);
    gameData = GSYS_GetGameData(param_1);
    work = func_ov010_02158344(commSys);
    if (work != (IntrudeComm *)0x0) {
        netId = func_02012b58(gameData);
        func_ov011_02175710(work, netId);
    } else {
        netId = func_02012b58(gameData);
        func_02012b8c(gameData, netId);
    }
    return func_ov018_0217ca1c(param_1);
}

GameEvent *func_ov018_0217ca84(GameSystem *param_1, u32 param_2) {
    u8 result;
    GameCommSys *comm;
    GameEvent *event;
    Ov018CommExitWork *work;
    IntrudeComm *intrude;
    BOOL BVar5;

    comm = GSYS_GetGameCommSystem(param_1);
    event = GameEvent_Create(param_1, NULL, func_ov018_0217cafc, sizeof(Ov018CommExitWork));
    work = GameEvent_GetData(event);
    work->unk_20 = param_2;
    result = GameCommSys_BootCheck(comm);
    if ((result == 2) && (intrude = func_02026474(comm), intrude != NULL)) {
        func_ov018_0217c094(func_02026474(comm));
        intrude->unk_8F8_7 = 1;
    }
    BVar5 = func_02012204(param_1);
    if ((BVar5 == 1) && (result = func_0202647c(comm), result == 1)) {
        work->unk_24 = 1;
    }
    GFL_SndSEPlay(0x737);
    return event;
}

GameEventReturnCode func_ov018_0217cafc(GameEvent *param_1, u32 *param_2, void *data) {
    Ov018CommExitWork *work = data;
    GameSystem *gsys;
    GameData *gameData;
    GameCommSys *comm;
    GameEvent *next;
    Field *field;
    void *msgBGSys;
    MsgData *msgData;
    u32 messageId;
    BOOL flag;
    u32 scriptId;
    u32 commMode;

    gsys = GameEvent_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    comm = GSYS_GetGameCommSystem(gsys);
    switch (*param_2) {
    case 0:
        next = func_ov018_0217ca40(gsys);
        GameEvent_ChainNext(param_1, next);
        (*param_2)++;
        break;
    case 1:
        if (func_0200fb24() != FALSE || func_02026460(comm) == FALSE) {
            if (func_0200fb24() == FALSE && (GameCommSys_BootCheck(comm) == 2 || GameCommSys_BootCheck(comm) == 1)) {
                func_02026400(comm);
            }
            (*param_2)++;
        }
        break;
    case 2:
        if (func_0200fb24() != FALSE || GameCommSys_BootCheck(comm) == 0) {
            if (work->unk_24 == 1) {
                *param_2 = 7;
            } else {
                (*param_2)++;
            }
        }
        break;
    case 3:
        if (GSYS_CheckField(gsys) == TRUE) {
            flag = FALSE;
            field = GSYS_GetField(gsys);
            if (Field_CheckMapLoadFinished(field) == TRUE) {
                msgBGSys = Field_GetMsgBGSys(field);
                msgData = func_ov021_021904b0(msgBGSys, 0x2d);
                work->msgData = msgData;
                work->talkWin = func_ov021_02190f64(msgBGSys, msgData);
                if (work->unk_20 != 0) {
                    messageId = work->unk_20 == 1 || work->unk_20 == 2 ? 0x5c : 0x59;
                    flag = TRUE;
                } else {
                    switch (func_0202648c(comm)) {
                    case 1:
                        messageId = 0x5a;
                        break;
                    case 2:
                        messageId = 0x56;
                        break;
                    case 3:
                        messageId = 0x57;
                        flag = TRUE;
                        break;
                    case 4:
                        messageId = 0x58;
                        flag = TRUE;
                        break;
                    case 0:
                    case 5:
                    default:
                        messageId = 0x59;
                        break;
                    }
                }
                func_ov021_02190e74(work->talkWin, 0, 0, messageId);
                if (flag == TRUE) {
                    next = EventMEPlay_Create(gsys, 0x52d);
                    GameEvent_ChainNext(param_1, next);
                    *param_2 = 4;
                } else {
                    *param_2 = 5;
                }
            }
        }
        break;
    case 4:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            next = EventPushBGMFinish_Create(gsys, 0, 6);
            GameEvent_ChainNext(param_1, next);
            *param_2 = 5;
        }
        break;
    case 5:
        if (func_ov021_02190ef0(work->talkWin) == TRUE) {
            (*param_2)++;
        }
        break;
    case 6:
        if (GCTX_HIDGetPressedKeys() & 0xf3) {
            func_ov021_02190e40(work->talkWin);
            GFL_MsgDataFree(work->msgData);
            (*param_2)++;
        }
        break;
    case 7:
        field = GSYS_GetField(gsys);
        if (Field_CheckMapLoadFinished(field) == TRUE) {
            if (func_ov021_021a3238(Field_GetPlayer(field)) == FALSE) {
                func_ov018_0217c658(&work->spin, gsys, field, 0, 0, 0);
                (*param_2)++;
            } else {
                *param_2 = 10;
            }
        }
        break;
    case 8:
        if (func_ov018_0217c688(&work->spin, NULL) == TRUE) {
            if (work->unk_24 == 1) {
                *param_2 = 10;
            } else {
                (*param_2)++;
            }
        }
        break;
    case 9:
        if (EventWork_FlagGet(GameData_GetEventWork(gameData), 0x980) == FALSE) {
            commMode = func_0202648c(comm);
            scriptId = 0;
            switch (commMode) {
            case 2:
                scriptId = 0xc;
                break;
            case 3:
            case 4:
                scriptId = 0xd;
                break;
            case 0:
            case 1:
            case 5:
                break;
            }
            EventScriptCall_Start(param_1, scriptId, NULL, 0, 0x8015);
        }
        *param_2 = 10;
        break;
    case 10:
        if (func_02012b58(gameData) != 0 || func_02012204(gsys) == TRUE) {
            if (func_0201212c(gsys) == TRUE) {
                field = GSYS_GetField(gsys);
                next = func_ov021_021c06cc(gsys, field);
                GameEvent_ChainNext(param_1, next);
            }
        }
        (*param_2)++;
        break;
    case 11:
        func_02026484(comm);
        func_020264f4(comm);
        if (func_02012b58(gameData) != 0 || func_02012204(gsys) == TRUE) {
            next = func_ov020_02185214(gsys);
            GameEvent_ChainNext(param_1, next);
        }
        (*param_2)++;
        break;
    case 12:
        return 1;
    }
    return 0;
}

