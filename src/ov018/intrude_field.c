// Black's code at ov018 0x0217ae20-0x0217c1d0. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/intrude_comm.h"
#include "field/player_state.h"
#include "gfl/std.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/ui.h"
#include "system/game_comm.h"
#include "field/field_map.h"
#include "field/field_g3d_mapper.h"
#include "field/intrude_field.h"
#include "field/field_player.h"
#include "gfl/msg.h"
#include "field/ov018_0217c458.h"
#include "field/event_mapchange.h"
#include "system/game_data.h"
#include "gfl/net_system.h"
#include "field/field_msgbg.h"
#include "gfl/key.h"
#include "field/intrude_work.h"
#include "system/wordset.h"
#include "gfl/str.h"
#include "field/ov021_021cfa58.h"



extern const ZoneNPC data_ov018_021835d4;

typedef struct IntrudeFieldTableEntry {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
} IntrudeFieldTableEntry;

const IntrudeFieldTableEntry data_ov018_02182b54[1] = {
    { 0x1c, 0x1d, 0x1e, 0x9, 0x64 },
};

const VecFx32 data_ov018_02182b60 = { 0x001ff000, 0x00020000, 0x001e8000 };
const ZoneNPC data_ov018_02182b6c = {
    0x64, 0x1c, 0xf, 0, 0, 5, 1, 0, 0, 0, 0xffff, 0xffff, 0,
};

// The event of a player walking off the hub's edge alone (func_ov018_0217b044): the player turns back and is told why
typedef struct IntrudeFieldEdgeEvent {
    Field *field;
    FieldActor *actor;
    MsgData *msgData;
    MsgBGSysWin *window;
    u32 unk_10;
    u16 dir;
    u16 unk_16;
} IntrudeFieldEdgeEvent;

// The event of a player walking off the hub's edge into the next one (func_ov018_0217b1dc)
typedef struct IntrudeFieldWarpEvent {
    Field *field;
    FieldActor *actor;
    MsgData *msgData;
    MsgBGSysWin *window;
    WordSet *wordSet;
    u16 dir;
    u8 unk_16;
    Ov018WarpSpin spin;  // func_ov018_0217c758's work
} IntrudeFieldWarpEvent;

// The event of the hub's exit (func_ov018_0217b5dc)
typedef struct IntrudeFieldExitEvent {
    MsgData *msgData;
    MsgBGSysWin *window;
    MsgBGTalkWin *talkWin;
    ListMenuUI *listMenu;
    Ov018WarpSpin spin;  // func_ov018_0217c658's work
    u16 timer;
} IntrudeFieldExitEvent;

// The event of the hub closing (func_ov018_0217b810)
typedef struct IntrudeFieldCloseEvent {
    MsgData *msgData;
    MsgBGTalkWin *talkWin;
    Ov018WarpSpin spin;  // func_ov018_0217c658's work
} IntrudeFieldCloseEvent;

// What func_ov018_0217ba54 gives the invasion comm (func_020263d8's commNo 2), like game_beacon_set.c's GameBeaconSend
typedef struct IntrudeFieldCommParam {
    GameSystem *gsys;
    GameCommSys *comm;
    u8 unk_08[6];
    u8 unk_0E;
} IntrudeFieldCommParam;

GameEventReturnCode func_ov018_0217b084(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov018_0217b214(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov018_0217b600(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov018_0217b82c(GameEvent *event, u32 *state, void *data);
void func_ov018_0217ba54(GameSystem *param_1);
GameEvent *func_ov018_02182124(GameSystem *param_1, void *param_2, IntrudeMissionEntry *param_3);

extern const u16 data_ov010_02170128[];
extern const u32 data_ov010_021700a4;
GameEvent *func_ov018_0217b5dc(GameSystem *gsys, Field *field, IntrudeComm *comm);
void func_ov018_0217ba48(void *arg, BOOL exitReqAgain);
void func_ov018_0217b968(Field *field, GameSystem *gsys, GameCommSys *comm, FieldPlayer *player);
void func_ov018_0217bbd8(Field *field, GameSystem *gsys, IntrudeComm *comm);
void func_ov018_0217bab4(Field *field, GameSystem *gsys, FieldPlayer *player, IntrudeComm *intrude);



void func_ov018_0217ae20(Field *param_1, GameSystem *gsys, FieldPlayer *player) {
    GameCommSys *commSys;
    IntrudeComm *intrude;
    GameData *gameData;
    PlayerState *playerState;
    u16 zoneId;
    int netId;
    u32 result;
    u16 result2;

    commSys = GSYS_GetGameCommSystem(gsys);
    intrude = func_02026474(commSys);
    gameData = GSYS_GetGameData(gsys);
    if (GameData_CheckEventsPaused(gameData) == TRUE) {
        return;
    }
    func_ov018_0217b968(param_1, gsys, commSys, player);
    if (GameCommSys_BootCheck(commSys) != 2) {
        return;
    }
    if (intrude == NULL) {
        return;
    }
    playerState = GameData_GetPlayerState(gameData);
    zoneId = PlayerState_GetZoneID(playerState);
    if (intrude->unk_910 == 0 && IsZoneEntralinkHub(zoneId) == TRUE) {
        intrude->unk_910 = 1;
    }
    func_ov018_0217bbd8(param_1, gsys, intrude);
    if (intrude->unk_8FD != 7 || func_0200fb24() != FALSE) {
        return;
    }
    if (GameData_IsForceSeasonSync(gameData) == FALSE) {
        if (intrude->unk_904_2 == 0) {
            if (func_ov018_0217c11c(intrude->unk_070.zoneId) == TRUE) {
                intrude->unk_904_2 = 1;
            } else {
                intrude->unk_904_2 = 2;
            }
        }
        if (intrude->unk_904_2 == 2) {
            return;
        }
    }
    result = func_02012b58(gameData);
    func_ov011_02175004(intrude);
    func_ov010_02167590(gsys, intrude->commPlayer);
    for (netId = 0; netId < 3; netId++) {
        if (netId == result || (intrude->unk_8FB & (1 << netId)) == 0) {
            continue;
        }
        if (func_ov010_021677e4(intrude->commPlayer, netId) == FALSE) {
            func_02012950(gameData, netId);
            result2 = func_ov011_02175598(&intrude->unk_08C[netId], func_02012950(gameData, netId));
            if (intrude->unk_08C[netId].unk_18 != 0 || GameData_IsForceSeasonSync(gameData) != FALSE) {
                func_ov010_021675d0(intrude->commPlayer, netId, result2, &intrude->unk_08C[netId].status);
            }
        } else {
            func_ov010_02167848(intrude->commPlayer, netId, &intrude->unk_08C[netId].status);
            if (intrude->unk_08C[netId].status.unk0 == 0 && (intrude->unk_08C[netId].unk_16_9 == 2 || intrude->unk_08C[netId].unk_16_9 == 3)) {
                func_ov010_021679c4(intrude->commPlayer, netId);
            }
        }
    }
    func_ov018_0217bab4(param_1, gsys, player, intrude);
}

u32 func_ov018_0217afc4(IntrudeComm *param_1, FieldPlayer *param_2, u32 *param_3) {
    u32 uStack_18;
    u32 uStack_1c = 0;
    s16 pos[3];
    u32 exState;

    if (param_1 == NULL || param_1->commPlayer == NULL) {
        return 0;
    }
    exState = FieldPlayer_GetExState(param_2);
    if (exState != 0 && exState != 1) {
        return 0;
    }
    FieldPlayer_GetGPosInFront(param_2, &pos[2], &pos[1], &pos[0]);
    while (TRUE) {
        exState = func_ov010_021679a4(param_1->commPlayer, pos[2], pos[1], pos[0], &uStack_18, &uStack_1c);
        if (exState == 1) {
            if (param_1->unk_08C[uStack_18].status.unk0 == 0) {
                *param_3 = uStack_18;
                return 1;
            }
        } else {
            return 0;
        }
    }
}

GameEvent *func_ov018_0217b044(GameSystem *gsys, Field *field, FieldPlayer *player, s32 param_4, u16 dir, u32 param_6) {
    GameEvent *event;
    IntrudeFieldEdgeEvent *work;

    event = GameEvent_Create(gsys, NULL, func_ov018_0217b084, sizeof(IntrudeFieldEdgeEvent));
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(IntrudeFieldEdgeEvent));
    work->field = field;
    work->actor = FieldPlayer_GetActor(player);
    work->unk_16 = param_4;
    work->dir = dir;
    work->unk_10 = param_6;
    return event;
}

const VecFx32 data_ov018_02182b90 = { 0x000f8000, 0x00000000, 0x00178000 };

GameEventReturnCode func_ov018_0217b084(GameEvent *event, u32 *state, void *data) {
    IntrudeFieldEdgeEvent *work = data;
    void *msgBGSys;
    u16 acmd;
    switch (*state) {
    case 0:
        if (IsAllActorAcmdFinished(work->actor) != TRUE) {
            break;
        }
        acmd = (work->dir == 3) ? 3 : 2;
        if (work->dir != GetActorFaceDir(work->actor)) {
            FldAct_SetAcmd(work->actor, acmd);
            (*state)++;
        } else {
            *state = 2;
        }
        break;
    case 1:
        if (func_ov010_0216d454(work->actor) != TRUE) {
            break;
        }
        func_ov010_0216d480(work->actor);
        (*state)++;
        break;
    case 2:
        DisableAllActorsMovement(Field_GetActorSystem(work->field));
        msgBGSys = Field_GetMsgBGSys(work->field);
        work->msgData = func_ov021_021904b0(msgBGSys, 0x2d);
        work->window = func_ov021_021907a8(msgBGSys, work->msgData);
        func_ov021_02190740(work->window, 0, 0, work->unk_10 == 0 ? 0x52 : 0x4f);
        (*state)++;
        break;
    case 3:
        if (func_ov021_0219077c(work->window) != TRUE) {
            break;
        }
        (*state)++;
        break;
    case 4:
        if (work->unk_10 == 0) {
            if ((GCTX_HIDGetPressedKeys() & 3) == 0) {
                break;
            }
            (*state)++;
        } else {
            if ((GCTX_HIDGetPressedKeys() & 0xf3) == 0) {
                break;
            }
            *state = 8;
        }
        break;
    case 5:
        func_ov021_02190788(work->window);
        func_ov021_02190740(work->window, 0, 0, 0x53);
        (*state)++;
        break;
    case 6:
        if (func_ov021_0219077c(work->window) != TRUE) {
            break;
        }
        (*state)++;
        break;
    case 7:
        if ((GCTX_HIDGetPressedKeys() & 0xf3) == 0) {
            break;
        }
        (*state)++;
        break;
    case 8:
        func_ov021_02190728(work->window);
        GFL_MsgDataFree(work->msgData);
        (*state)++;
        break;
    case 9:
        if (IsAllActorAcmdFinished(work->actor) != TRUE) {
            break;
        }
        acmd = (work->dir == 3) ? 10 : 11;
        FldAct_SetAcmd(work->actor, acmd);
        (*state)++;
        break;
    case 10:
        if (func_ov010_0216d454(work->actor) != TRUE) {
            break;
        }
        func_ov010_0216d480(work->actor);
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *func_ov018_0217b1dc(GameSystem *gsys, Field *field, FieldPlayer *player, u16 dir) {
    GameEvent *event;
    IntrudeFieldWarpEvent *work;

    event = GameEvent_Create(gsys, NULL, func_ov018_0217b214, sizeof(IntrudeFieldWarpEvent));
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(IntrudeFieldWarpEvent));
    work->field = field;
    work->actor = FieldPlayer_GetActor(player);
    work->dir = dir;
    return event;
}

GameEventReturnCode func_ov018_0217b214(GameEvent *event, u32 *state, void *data) {
    IntrudeFieldWarpEvent *work = data;
    GameSystem *gsys;
    GameCommSys *commSys;
    GameData *gameData;
    IntrudeComm *intrude;
    void *msgBGSys;
    StrBuf *dest;
    StrBuf *src;
    PlayerInfo *info;
    PlaceName *placeName;
    u16 uStack_1e;
    u8 uStack_1d;
    u8 uStack_1c;
    gsys = GameEvent_GetGameSystem(event);
    commSys = GSYS_GetGameCommSystem(gsys);
    gameData = GSYS_GetGameData(gsys);
    intrude = func_ov010_02158344(commSys);
    switch (*state) {
    case 0:
        DisableAllActorsMovement(Field_GetActorSystem(work->field));
        (*state)++;
        break;
    case 1:
        if (IsAllActorAcmdFinished(work->actor) == FALSE) {
            break;
        }
        if (intrude == NULL) {
            *state = 6;
            break;
        }
        if (func_ov011_02175be4(&intrude->unk_1AC) == TRUE) {
            work->unk_16 = 1;
            func_ov018_0217c758(&work->spin, gsys, work->field, intrude, 0, 0, 0, work->dir, 0);
        } else if (func_ov010_021584a4(commSys, gameData, work->dir) == TRUE) {
            func_ov018_0217c758(&work->spin, gsys, work->field, intrude, 0, 0, 0, work->dir, 1);
        } else {
            info = GetGameDataPlayerInfo(gameData);
            func_ov011_02175548(info, &uStack_1e, &uStack_1d, &uStack_1c);
            func_ov018_0217c758(&work->spin, gsys, work->field, intrude, uStack_1e, uStack_1d, uStack_1c, work->dir, 1);
        }
        (*state)++;
        break;
    case 2:
        if (func_ov018_0217c820(&work->spin, intrude) != 1) {
            break;
        }
        (*state)++;
        break;
    case 3:
        if (intrude == NULL) {
            *state = 6;
            break;
        }
        msgBGSys = Field_GetMsgBGSys(work->field);
        work->msgData = func_ov021_021904b0(msgBGSys, 0x2d);
        work->window = func_ov021_021907a8(msgBGSys, work->msgData);
        work->wordSet = GFL_WordSetSystemCreateDefault(4);
        if (intrude->unk_070.unk_16_0 == func_02012b58(gameData)) {
            if (work->unk_16 == 1) {
                func_ov021_02190740(work->window, 0, 0, 0x4f);
            } else {
                func_ov021_02190740(work->window, 0, 0, 0x4e);
            }
        } else {
            dest = GFL_StrBufCreate(0x100, 0x15);
            info = func_02012950(gameData, intrude->unk_070.unk_16_0);
            copyVarForText(work->wordSet, 0, info);
            if (work->unk_16 == 1) {
                src = GFL_MsgDataLoadStrbufNew(work->msgData, 0x50);
            } else {
                src = GFL_MsgDataLoadStrbufNew(work->msgData, 0x51);
            }
            GFL_WordSetFormatStrbuf(work->wordSet, dest, src);
            func_ov021_02190758(work->window, 0, 0, dest);
            GFL_StrBufFree(src);
            GFL_StrBufFree(dest);
        }
        (*state)++;
        break;
    case 4:
        if (func_ov021_0219077c(work->window) != TRUE) {
            break;
        }
        placeName = Field_GetPlaceName(work->field);
        BeginForcePlaceNameDisp(placeName, Field_GetPlayerStateZoneID(work->field));
        (*state)++;
        break;
    case 5:
        if ((GCTX_HIDGetPressedKeys() & 0xf3) == 0) {
            break;
        }
        (*state)++;
        break;
    case 6:
        if (work->window != NULL) {
            func_ov021_02190728(work->window);
            GFL_MsgDataFree(work->msgData);
            GFL_WordSetSystemFree(work->wordSet);
        }
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *func_ov018_0217b410(Field *field, GameSystem *gsys, FieldPlayer *player) {
    u16 zoneId;
    VecFx32 pos;
    IntrudeComm *comm;
    s32 i;
    fx32 startX;
    fx32 x;
    fx32 z;

    zoneId = PlayerState_GetZoneID(GSYS_GetPlayerState(gsys));
    comm = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    if (!IsZoneEntralinkHub(zoneId)) {
        return NULL;
    }
    FieldPlayer_GetWPos(player, &pos);
    for (i = 0; i < 3; i++) {
        x = (0x7e << 14) + (i << 22);
        z = 0xb2 << 14;
        if (pos.x >= x && pos.x <= x && pos.z >= z && pos.z <= z) {
            return func_ov018_0217b5dc(gsys, field, comm);
        }
    }
    startX = 0x7e << 14;
    for (i = 0; i < 3; i++) {
        if (pos.x >= startX && pos.x < startX + (2 << 16) && pos.z == (0x56 << 14)) {
            VecFx32 warp = data_ov018_02182b90;
            warp.x += pos.x - startX;
            return EventEntreeForestWarp_Create(gsys, 1, &warp, 0, 4);
        }
        startX += 1 << 22;
    }
    return NULL;
}

GameEvent *func_ov018_0217b4d8(GameSystem *gsys, Field *field, FieldPlayer *param_3, VecFx32 *param_4, s32 param_5, u16 param_6) {
    IntrudeComm *comm;
    u16 kind;
    s32 cellX;
    s32 t;
    s32 x;
    s32 n;

    comm = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    kind = 9;
    x = param_4->x;
    n = 1;
    if (comm != NULL) {
        n = comm->unk_902;
    }
    if (x <= 0) {
        x += n << 22;
    } else if (x >= n << 22) {
        x -= n << 22;
    }
    cellX = (x >> 12) / 16 % 64;
    t = x % 0x400000;
    if (param_6 == 2) {
        if (param_5 != 3 || (param_5 == 3 && t <= 0x8000)) {
            if (cellX == 0) {
                kind = 2;
            }
        }
    } else if (param_6 == 3) {
        if (param_5 != 2 || (param_5 == 2 && t >= 0x3f8000)) {
            if (cellX == 0x3f) {
                kind = 3;
            }
        }
    }
    if (kind != 9) {
        if (comm == NULL || func_0203d1b4() <= 1 || comm->unk_902 < 2) {
            return func_ov018_0217b044(gsys, field, param_3, param_5, kind, 0);
        }
        if (func_ov011_02175be4(&comm->unk_1AC) == 1) {
            return func_ov018_0217b044(gsys, field, param_3, param_5, kind, 1);
        }
        return func_ov018_0217b1dc(gsys, field, param_3, kind);
    }
    return NULL;
}



GameEvent *func_ov018_0217b5dc(GameSystem *gsys, Field *field, IntrudeComm *comm) {
    GameEvent *event;
    IntrudeFieldExitEvent *work;

    event = GameEvent_Create(gsys, NULL, func_ov018_0217b600, sizeof(IntrudeFieldExitEvent));
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(IntrudeFieldExitEvent));
    return event;
}

GameEventReturnCode func_ov018_0217b600(GameEvent *event, u32 *state, void *data) {
    IntrudeFieldExitEvent *work = data;
    GameSystem *gsys;
    Field *field;
    GameCommSys *commSys;
    GameData *gameData;
    FieldPlayer *player;
    FieldActor *actor;
    void *msgBGSys;
    MsgData *msgData;
    MsgBGTalkWin *talkWin;
    MsgBGSysWin *window;
    ListMenuUI *listMenu;
    PlayerState *playerState;
    GameEvent *next;
    u32 result;
    s32 i;
    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    commSys = GSYS_GetGameCommSystem(gsys);
    switch (*state) {
    case 0:
        DisableAllActorsMovement(Field_GetActorSystem(field));
        msgBGSys = Field_GetMsgBGSys(field);
        msgData = func_ov021_021904b0(msgBGSys, 0x2d);
        work->msgData = msgData;
        talkWin = func_ov021_02190f64(msgBGSys, msgData);
        work->talkWin = talkWin;
        func_ov021_02190e74(talkWin, 0, 0, 0x54);
        (*state)++;
        break;
    case 1:
        if (func_ov021_02190ef0(work->talkWin) != TRUE) {
            break;
        }
        msgBGSys = Field_GetMsgBGSys(field);
        listMenu = func_ov021_02190be4(msgBGSys, 0);
        work->listMenu = listMenu;
        (*state)++;
        break;
    case 2:
        result = func_ov021_02190c78(work->listMenu);
        if (result == 2) {
            break;
        }
        func_ov021_021909bc(work->listMenu);
        func_ov021_02190e40(work->talkWin);
        work->listMenu = NULL;
        work->talkWin = NULL;
        if (result == 0) {
            *state = 3;
        } else {
            *state = 11;
        }
        break;
    case 3:
        if (func_0200fb24() == FALSE && GameCommSys_BootCheck(commSys) == 2) {
            func_02026400(commSys);
            msgBGSys = Field_GetMsgBGSys(field);
            window = func_ov021_021907a8(msgBGSys, work->msgData);
            work->window = window;
            func_ov021_02190740(window, 0, 0, 0x14);
        }
        (*state)++;
        break;
    case 4:
        if (work->window != NULL) {
            if (func_ov021_0219077c(work->window) != TRUE) {
                break;
            }
        }
        (*state)++;
        break;
    case 5:
        work->timer += 1;
        if (work->window != NULL && work->timer <= 0x3c) {
            break;
        }
        if (GameCommSys_BootCheck(commSys) == 2) {
            break;
        }
        gameData = GSYS_GetGameData(gsys);
        {
            VecFx32 pos = { 0, 0, 0 };
            for (i = 0; i < 3; i++) {
                playerState = func_02012798(gameData, i);
                PlayerState_SetWPos(playerState, &pos);
            }
        }
        (*state)++;
        break;
    case 6:
        if (work->window != NULL) {
            func_ov021_02190728(work->window);
        }
        GFL_MsgDataFree(work->msgData);
        (*state)++;
        break;
    case 7:
        field = GSYS_GetField(gsys);
        if (Field_CheckMapLoadFinished(field) != TRUE) {
            break;
        }
        player = Field_GetPlayer(field);
        if (func_ov021_021a3238(player) == 0) {
            func_ov018_0217c658(&work->spin, gsys, field, 0, 0, 0);
            (*state)++;
        } else {
            *state = 9;
        }
        break;
    case 8:
        if (func_ov018_0217c688(&work->spin, 0) != TRUE) {
            break;
        }
        (*state)++;
        break;
    case 9:
        next = func_ov020_02185214(gsys);
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    case 10:
        *state = 14;
        break;
    case 11:
        GFL_MsgDataFree(work->msgData);
        *state = 12;
        break;
    case 12:
        player = Field_GetPlayer(field);
        actor = FieldPlayer_GetActor(player);
        if (IsAllActorAcmdFinished(actor) != TRUE) {
            break;
        }
        FldAct_SetAcmd(actor, 8);
        (*state)++;
        break;
    case 13:
        player = Field_GetPlayer(field);
        actor = FieldPlayer_GetActor(player);
        if (func_ov010_0216d480(actor) != TRUE) {
            break;
        }
        *state = 14;
        break;
    case 14:
        EnableAllActorsMovement(Field_GetActorSystem(field));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *func_ov018_0217b810(GameSystem *gsys, Field *field) {
    GameEvent *event;

    event = GameEvent_Create(gsys, NULL, func_ov018_0217b82c, sizeof(IntrudeFieldCloseEvent));
    GameEvent_GetData(event);
    return event;
}

GameEventReturnCode func_ov018_0217b82c(GameEvent *event, u32 *state, void *data) {
    IntrudeFieldCloseEvent *work = data;
    GameSystem *gsys;
    Field *field;
    FieldPlayer *player;
    void *msgBGSys;
    MsgData *msgData;
    MsgBGTalkWin *talkWin;
    GameEvent *next;
    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    GSYS_GetGameCommSystem(gsys);
    switch (*state) {
    case 0:
        DisableAllActorsMovement(Field_GetActorSystem(field));
        msgBGSys = Field_GetMsgBGSys(field);
        msgData = func_ov021_021904b0(msgBGSys, 0x2d);
        work->msgData = msgData;
        talkWin = func_ov021_02190f64(msgBGSys, msgData);
        work->talkWin = talkWin;
        func_ov021_02190e74(talkWin, 0, 0, 0x60);
        (*state)++;
        break;
    case 1:
        if (func_ov021_02190ef0(work->talkWin) != 1) {
            break;
        }
        func_ov021_02190e40(work->talkWin);
        GFL_MsgDataFree(work->msgData);
        (*state)++;
        break;
    case 2:
        field = GSYS_GetField(gsys);
        if (Field_CheckMapLoadFinished(field) != 1) {
            break;
        }
        player = Field_GetPlayer(field);
        if (func_ov021_021a3238(player) == 0) {
            func_ov018_0217c658(&work->spin, gsys, field, 0, 0, 0);
            (*state)++;
        } else {
            *state = 4;
        }
        break;
    case 3:
        if (func_ov018_0217c688(&work->spin, 0) != 1) {
            break;
        }
        (*state)++;
        break;
    case 4:
        next = func_ov020_02185214(gsys);
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    case 5:
        if (field == NULL) {
            break;
        }
        if (Field_CheckMapLoadFinished(field) != 1) {
            break;
        }
        EnableAllActorsMovement(Field_GetActorSystem(field));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


u32 func_ov018_0217b930(s32 *param_1) {
    if (*param_1 >= 0x348000 || *param_1 <= 0xb8000) {
        return 1;
    }
    return 0;
}

u32 func_ov018_0217b94c(s32 *param_1) {
    if (*param_1 > 0x308000 || *param_1 < 0xe8000) {
        return 1;
    }
    return 0;
}

void func_ov018_0217b968(Field *field, GameSystem *gsys, GameCommSys *comm, FieldPlayer *player) {
    VecFx32 pos;
    u16 zoneId;

    if (func_0202648c(comm) != 0) {
        return;
    }
    if (func_02026460(comm) == 1) {
        return;
    }
    if (func_0200fb24() != 0) {
        return;
    }
    if (func_02035390() == 1 && GetIsShouldUICallback() == 1) {
        return;
    }
    if (func_02012204(gsys) == 1) {
        return;
    }
    if (GSYS_GetEventRunningFlag(gsys) == 1) {
        return;
    }
    zoneId = PlayerState_GetZoneID(GameData_GetPlayerState(GSYS_GetGameData(gsys)));
    if (IsZoneEntralinkHub(zoneId) == 0) {
        return;
    }
    FieldPlayer_GetWPos(player, &pos);
    switch (GameCommSys_BootCheck(comm)) {
    case 0:
        if (func_ov018_0217b930((s32 *)&pos) == 1) {
            func_ov018_0217ba54(gsys);
        }
        return;
    case 2:
        if (func_ov018_0217b930((s32 *)&pos) == 0 && func_020392b8() <= 1 && ((IntrudeComm *)func_02026474(comm))->unk_8FD != 4) {
            func_02026400(comm);
        }
        return;
    case 1:
        if (func_ov018_0217b930((s32 *)&pos) == 1) {
            func_0202640c(comm, func_ov018_0217ba48, gsys);
        }
        return;
    default:
        return;
    }
}




void func_ov018_0217ba48(void *arg, BOOL exitReqAgain) {
    if (exitReqAgain == FALSE) {
        func_ov018_0217ba54(arg);
    }
}

void func_ov018_0217ba54(GameSystem *param_1) {
    GameCommSys *comm;
    IntrudeFieldCommParam *param;
    comm = GSYS_GetGameCommSystem(param_1);
    if (((func_0200fb24() == 0) && (func_02012204(param_1) != 1)) && ((func_02035390() != 1 || (GetIsShouldUICallback() != 1)))) {
        param = GFL_HeapAllocate(0x8005, sizeof(IntrudeFieldCommParam), 1, "intrude_field.c", 0x528);
        param->unk_0E = 1;
        param->gsys = param_1;
        param->comm = comm;
        func_020263d8(comm, 2, param);
    }
}

void func_ov018_0217bab4(Field *field, GameSystem *gsys, FieldPlayer *player, IntrudeComm *intrude) {
    PlayerState *playerState;
    GameCommSys *commSys;
    VecFx32 wpos;
    VecFx32 newPos;
    s32 x;
    s32 w;
    BOOL moved;
    u16 zoneId;
    s32 cell;
    int prevNetId;
    playerState = GSYS_GetPlayerState(gsys);
    zoneId = PlayerState_GetZoneID(playerState);
    PlayerState_CalcDirection(playerState);
    commSys = GSYS_GetGameCommSystem(gsys);
    moved = FALSE;
    GSYS_GetGameData(gsys);
    if (GameCommSys_BootCheck(commSys) != 2) {
        return;
    }
    if (intrude == NULL) {
        return;
    }
    if (IsZoneEntralinkHub(zoneId) == FALSE) {
        return;
    }
    if (intrude->unk_902 < 2) {
        return;
    }
    if (func_ov011_02175e90(intrude, &intrude->unk_1AC) == TRUE) {
        return;
    }
    FieldPlayer_GetWPos(player, &wpos);
    newPos = wpos;
    prevNetId = intrude->unk_070.unk_16_0;
    x = wpos.x;
    w = (intrude->unk_901 + 1) << 22;
    if (x < 0) {
        newPos.x = w + x;
        moved = TRUE;
    } else if (x > w) {
        newPos.x = x - w;
        moved = TRUE;
    }
    if (moved == TRUE) {
        x = newPos.x;
        if (GSYS_GetEventRunningFlag(gsys) == 1) {
            newPos = wpos;
            moved = FALSE;
        }
    }
    cell = x / 0x400000;
    if (moved == TRUE) {
        FieldPlayer_SetWPos(player, &newPos);
        func_ov011_02175274(intrude, newPos.x);
    }
    cell += func_02039338();
    if (cell > intrude->unk_901) {
        cell -= intrude->unk_901 + 1;
    }
    if (prevNetId != cell) {
        func_ov011_02175710(intrude, cell);
        intrude->unk_900 = 1;
        func_ov010_02161fec(&intrude->unk_35C, cell);
    }
}


void func_ov018_0217bbcc(Field *field, GameSystem *gsys, IntrudeComm *comm) {
    comm->unk_901 = 0;
}

void func_ov018_0217bbd8(Field *field, GameSystem *gsys, IntrudeComm *comm) {
    VecFx32 pos;
    FieldG3DMapper *mapper;
    MapMatrix *matrix;
    s32 limit;

    if (comm == NULL) {
        return;
    }
    if (field == NULL) {
        return;
    }
    if (Field_GetG3DMapper(field) == NULL) {
        return;
    }
    if (IsZoneEntralinkHub(Field_GetPlayerStateZoneID(field)) == FALSE) {
        return;
    }
    limit = comm->unk_902 - 1;
    if (limit <= 0) {
        limit = 1;
    }
    if (comm->unk_8F8_5 == 1) {
        limit = 1;
    }
    if (comm->unk_901 < limit) {
        FieldPlayer *player = Field_GetPlayer(field);
        if (player == NULL) {
            return;
        }
        FieldPlayer_GetWPos(player, &pos);
        pos.x = pos.x % 0x400000;
        if (func_ov018_0217b94c(&pos.x) == 1) {
            return;
        }
        matrix = InitMapMatrix(0x8015);
        MapMatrix_Load(matrix, 0x76, 0x117, 0x8015);
        do {
            mapper = Field_GetG3DMapper(field);
            func_ov021_0218cd20(field, mapper, matrix);
            comm->unk_901++;
        } while (comm->unk_901 < limit);
        FreeMapMatrix(matrix);
    }
}


void func_ov018_0217bc94(Field *field, s32 count) {
    MapMatrix *matrix;
    FieldG3DMapper *mapper;

    if (count >= 3) {
        count = 2;
    }
    matrix = InitMapMatrix(0x15);
    MapMatrix_Load(matrix, 0x76, 0x76 + 0xa1, 0x8015);
    for (; count > 0; count--) {
        mapper = Field_GetG3DMapper(field);
        func_ov021_0218cd20(field, mapper, matrix);
    }
    FreeMapMatrix(matrix);
}

void func_ov018_0217bcdc(IntrudeComm *comm, GameSystem *gsys, u16 param_3, u8 param_4, u8 param_5) {
    GameData *gameData = GSYS_GetGameData(gsys);
    PlayerInfo *info;
    VecFx32 pos = { 0, 0, 0 };
    u32 gender;
    u32 objCode;
    FieldPlayer *player;
    u16 id;
    if (GSYS_CheckField(gsys)) {
        info = GetGameDataPlayerInfo(GSYS_GetGameData(gsys));
        gender = getTrainerGender(info);
        if (comm != NULL) {
            id = param_3;
            if (id == 0xffff && comm->unk_070.unk_16_0 == func_02012b58(gameData)) {
                id = 0;
            }
            if (id == 0) {
                param_4 = (gender == 0) ? 3 : 4;
                param_5 = gender;
            }
            if (comm->unk_070.unk_18 == id) {
                return;
            }
            comm->unk_070.unk_18 = id;
            comm->unk_070.unk_1A_0 = param_4;
            comm->unk_070.unk_1A_4 = param_5;
            comm->unk_900 = 1;
            objCode = func_ov011_02175598(&comm->unk_070, info);
        } else {
            id = 0;
            objCode = 1;
            if (gender != 0) {
                objCode = 4;
            }
        }
        player = Field_GetPlayer(GSYS_GetField(gsys));
        FieldPlayer_ForceObjCode(player, objCode);
        SetActorWPosOffset(FieldPlayer_GetActor(player), &pos);
        if (id == 0) {
            FieldPlayer_ClearObjCode(player);
        }
    }
}



GameEvent *func_ov018_0217bde0(GameSystem *param_1, IntrudeComm *param_2, FieldPlayer *param_3) {
    u16 zoneID;
    BOOL BVar2;
    IntrudeMissionEntry *pIVar3;
    PlayerState *playerState;
    FieldActor *actor;
    u32 val;
    GameEvent *pGVar5;
    GridPos GStack_1c;

    BVar2 = func_ov011_02175b8c(&param_2->unk_1AC);
    if (((((BVar2 == 1) && (BVar2 = func_ov011_02175bbc(&param_2->unk_1AC), BVar2 == 0)) &&
              (pIVar3 = func_ov011_02175b34(&param_2->unk_1AC), pIVar3 != (IntrudeMissionEntry *)0x0)) &&
            (((pIVar3->mission).kind == 4 &&
              ((param_2->unk_070).unk_16_0 == (u16)(pIVar3->player).unk_10)))) &&
          (BVar2 = func_ov011_02175cf4(param_2, &param_2->unk_1AC), BVar2 == 0)) {
        playerState = GSYS_GetPlayerState(param_1);
        zoneID = PlayerState_GetZoneID(playerState);
        actor = FieldPlayer_GetActor(param_3);
        FldAct_GetGPos(actor, &GStack_1c);
        val = (u32)(pIVar3->mission).unk_14;
        if (zoneID == data_ov021_021db810[val] && GStack_1c.x == data_ov021_021db834[val].unk00
            && GStack_1c.y == data_ov021_021db834[val].unk02 && GStack_1c.z == data_ov021_021db834[val].unk04) {
            pGVar5 = func_ov018_02182124(param_1, param_2, pIVar3);
            return pGVar5;
        }
    }
    return (GameEvent *)0x0;
}

const IntrudeFieldUnkEntry data_ov018_02182b9c[0xda] = {
    { 329, 304, 289, 0, 0, 0, 0, 0x7672 },
    { 330, 427, 289, 0, 0, 0, 0, 0x7672 },
    { 331, 305, 290, 0, 0, 0, 0, 0x6a39 },
    { 332, 427, 290, 0, 0, 0, 0, 0x6a39 },
    { 336, 427, 290, 0, 0, 0, 0, 0x6a39 },
    { 333, 427, 290, 0, 0, 0, 0, 0x6a39 },
    { 334, 427, 290, 0, 0, 0, 0, 0x6a39 },
    { 335, 427, 290, 0, 0, 0, 0, 0x6a39 },
    { 337, 306, 291, 0, 0, 0, 0, 0x4535 },
    { 343, 427, 291, 0, 0, 0, 0, 0x4535 },
    { 344, 427, 291, 0, 0, 0, 0, 0x4535 },
    { 338, 427, 291, 0, 0, 0, 0, 0x4535 },
    { 339, 427, 291, 0, 0, 0, 0, 0x4535 },
    { 340, 427, 291, 0, 0, 0, 0, 0x4535 },
    { 341, 427, 291, 0, 0, 0, 0, 0x4535 },
    { 342, 427, 291, 0, 0, 0, 0, 0x4535 },
    { 345, 307, 292, 0, 0, 0, 0, 0x3055 },
    { 347, 427, 292, 0, 0, 0, 0, 0x3055 },
    { 346, 427, 292, 0, 0, 0, 0, 0x3055 },
    { 348, 308, 293, 0, 0, 0, 0, 0x3071 },
    { 349, 427, 293, 0, 0, 0, 0, 0x3071 },
    { 350, 427, 293, 0, 0, 0, 0, 0x3071 },
    { 351, 427, 293, 0, 0, 0, 0, 0x3071 },
    { 352, 427, 293, 0, 0, 0, 0, 0x3071 },
    { 353, 427, 293, 0, 0, 0, 0, 0x3071 },
    { 354, 427, 293, 0, 0, 0, 0, 0x3071 },
    { 365, 309, 293, 0, 0, 0, 0, 0x308b },
    { 366, 427, 293, 0, 0, 0, 0, 0x308b },
    { 367, 427, 293, 0, 0, 0, 0, 0x308b },
    { 368, 310, 315, 0, 0, 0, 0, 0x30a6 },
    { 369, 427, 315, 0, 0, 0, 0, 0x30a6 },
    { 370, 311, 315, 0, 0, 0, 0, 0x45ca },
    { 371, 427, 315, 0, 0, 0, 0, 0x45ca },
    { 372, 427, 315, 0, 0, 0, 0, 0x45ca },
    { 373, 427, 315, 0, 0, 0, 0, 0x45ca },
    { 374, 312, 316, 0, 0, 0, 0, 0x6dca },
    { 375, 427, 316, 0, 0, 0, 0, 0x6dca },
    { 376, 427, 316, 0, 0, 0, 0, 0x6dca },
    { 377, 427, 316, 0, 0, 0, 0, 0x6dca },
    { 378, 313, 294, 0, 0, 0, 0, 0x76a6 },
    { 379, 427, 294, 0, 0, 0, 0, 0x76a6 },
    { 380, 427, 294, 0, 0, 0, 0, 0x76a6 },
    { 382, 427, 294, 0, 0, 0, 0, 0x76a6 },
    { 381, 427, 294, 0, 0, 0, 0, 0x76a6 },
    { 383, 314, 289, 0, 0, 0, 0, 0x768b },
    { 384, 427, 289, 0, 0, 0, 0, 0x768b },
    { 385, 427, 289, 0, 0, 0, 0, 0x768b },
    { 386, 427, 289, 0, 0, 0, 0, 0x768b },
    { 253, 301, 290, 0, 0, 0, 0, 0x7664 },
    { 254, 302, 293, 0, 0, 0, 0, 0x3064 },
    { 255, 427, 315, 0, 0, 0, 0, 0x309d },
    { 256, 427, 315, 0, 0, 0, 0, 0x309d },
    { 257, 427, 315, 0, 0, 0, 0, 0x309d },
    { 258, 427, 315, 0, 0, 0, 0, 0x309d },
    { 259, 427, 315, 0, 0, 0, 0, 0x309d },
    { 260, 427, 315, 0, 0, 0, 0, 0x309d },
    { 261, 427, 315, 0, 0, 0, 0, 0x309d },
    { 262, 427, 315, 0, 0, 0, 0, 0x309d },
    { 263, 303, 294, 0, 0, 0, 0, 0x769d },
    { 62, 289, 289, 0, 0, 0, 0, 0x7680 },
    { 63, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 64, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 65, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 66, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 67, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 68, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 69, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 70, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 71, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 72, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 73, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 74, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 75, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 76, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 77, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 78, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 79, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 80, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 81, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 82, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 83, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 84, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 85, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 86, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 87, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 88, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 89, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 90, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 91, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 92, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 93, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 94, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 95, 427, 289, 0, 0, 0, 0, 0x7680 },
    { 96, 290, 290, 0, 0, 0, 0, 0x7649 },
    { 97, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 98, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 99, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 100, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 101, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 102, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 103, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 104, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 105, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 106, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 107, 291, 291, 0, 0, 0, 0, 0x5329 },
    { 108, 427, 291, 0, 0, 0, 0, 0x5329 },
    { 109, 427, 291, 0, 0, 0, 0, 0x5329 },
    { 110, 427, 291, 0, 0, 0, 0, 0x5329 },
    { 111, 427, 291, 0, 0, 0, 0, 0x5329 },
    { 112, 427, 291, 0, 0, 0, 0, 0x5329 },
    { 113, 292, 292, 0, 0, 0, 0, 0x3049 },
    { 114, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 115, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 116, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 117, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 118, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 119, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 120, 293, 293, 0, 0, 0, 0, 0x3080 },
    { 121, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 122, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 123, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 124, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 125, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 126, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 127, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 128, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 129, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 130, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 131, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 132, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 133, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 134, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 135, 427, 293, 0, 0, 0, 0, 0x3080 },
    { 406, 315, 315, 0, 0, 0, 0, 0x30ba },
    { 407, 427, 315, 0, 0, 0, 0, 0x30ba },
    { 408, 427, 315, 0, 0, 0, 0, 0x30ba },
    { 409, 427, 315, 0, 0, 0, 0, 0x30ba },
    { 410, 427, 315, 0, 0, 0, 0, 0x30ba },
    { 411, 427, 315, 0, 0, 0, 0, 0x30ba },
    { 412, 316, 316, 0, 0, 0, 0, 0x53db },
    { 413, 427, 316, 0, 0, 0, 0, 0x53db },
    { 414, 427, 316, 0, 0, 0, 0, 0x53db },
    { 415, 427, 316, 0, 0, 0, 0, 0x53db },
    { 416, 427, 316, 0, 0, 0, 0, 0x53db },
    { 417, 427, 316, 0, 0, 0, 0, 0x53db },
    { 0, 294, 294, 0, 0, 0, 0, 0x76ba },
    { 1, 427, 294, 0, 0, 0, 0, 0x76ba },
    { 2, 427, 294, 0, 0, 0, 0, 0x76ba },
    { 3, 427, 294, 0, 0, 0, 0, 0x76ba },
    { 4, 427, 294, 0, 0, 0, 0, 0x76ba },
    { 5, 427, 294, 0, 0, 0, 0, 0x76ba },
    { 424, 295, 295, 0, 0, 0, 0, 0x76ba },
    { 425, 427, 295, 0, 0, 0, 0, 0x76ba },
    { 426, 427, 295, 0, 0, 0, 0, 0x76ba },
    { 191, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 192, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 193, 427, 290, 0, 0, 0, 0, 0x7649 },
    { 194, 297, 290, 0, 0, 0, 0, 0x6736 },
    { 195, 427, 290, 0, 0, 0, 0, 0x6736 },
    { 196, 427, 290, 0, 0, 0, 0, 0x6736 },
    { 197, 427, 290, 0, 0, 0, 0, 0x6736 },
    { 198, 298, 291, 0, 0, 0, 0, 0x4039 },
    { 199, 427, 291, 0, 0, 0, 0, 0x4039 },
    { 200, 427, 291, 0, 0, 0, 0, 0x4039 },
    { 201, 427, 291, 0, 0, 0, 0, 0x4039 },
    { 202, 427, 291, 0, 0, 0, 0, 0x4039 },
    { 203, 427, 291, 0, 0, 0, 0, 0x4039 },
    { 204, 427, 291, 0, 0, 0, 0, 0x4039 },
    { 205, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 206, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 207, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 208, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 209, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 210, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 211, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 212, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 213, 427, 292, 0, 0, 0, 0, 0x3049 },
    { 230, 427, 315, 0, 0, 0, 0, 0x45ca },
    { 231, 427, 315, 0, 0, 0, 0, 0x45ca },
    { 232, 427, 315, 0, 0, 0, 0, 0x45ca },
    { 233, 427, 315, 0, 0, 0, 0, 0x45ca },
    { 234, 427, 315, 0, 0, 0, 0, 0x45ca },
    { 240, 427, 316, 0, 0, 0, 0, 0x53db },
    { 289, 289, 289, 0, 406, 1, 470, 0x7680 },
    { 290, 290, 290, 0, 187, 0, 415, 0x7649 },
    { 291, 291, 291, 0, 108, 0, 307, 0x5329 },
    { 292, 292, 292, 0, 184, 3, 198, 0x3049 },
    { 293, 293, 293, 0, 425, 0, 175, 0x3080 },
    { 294, 294, 294, 0, 43, 0, 39, 0x76ba },
    { 295, 295, 295, 0, 43, 0, 39, 0x76ba },
    { 301, 301, 290, 0, 187, 0, 415, 0x7664 },
    { 302, 302, 293, 0, 425, 0, 175, 0x3064 },
    { 303, 303, 294, 0, 43, 0, 39, 0x769d },
    { 304, 304, 289, 0, 406, 1, 470, 0x7672 },
    { 305, 305, 290, 0, 187, 0, 415, 0x6a39 },
    { 306, 306, 291, 0, 108, 0, 307, 0x4535 },
    { 307, 307, 292, 0, 184, 3, 198, 0x3055 },
    { 308, 308, 293, 0, 425, 0, 175, 0x3071 },
    { 309, 309, 293, 0, 425, 0, 175, 0x308b },
    { 310, 310, 315, 0, 660, 0, 187, 0x30a6 },
    { 311, 311, 315, 0, 660, 0, 187, 0x45ca },
    { 312, 312, 316, 0, 760, -4, 302, 0x6dca },
    { 313, 313, 294, 0, 43, 0, 39, 0x76a6 },
    { 314, 314, 289, 0, 405, 1, 470, 0x768b },
    { 315, 315, 315, 0, 660, 0, 187, 0x30ba },
    { 316, 316, 316, 0, 760, -4, 302, 0x53db },
    { 297, 297, 290, 0, 187, 0, 415, 0x6736 },
    { 298, 298, 291, 0, 108, 0, 307, 0x4039 },
    { 279, 279, 279, 0, 31, 2, 30, 0x5380 },
    { 280, 280, 279, 0, 31, 2, 30, 0x5380 },
    { 281, 281, 279, 0, 31, 2, 30, 0x5380 },
    { 282, 282, 279, 0, 31, 2, 30, 0x5380 },
    { 283, 283, 279, 0, 31, 2, 30, 0x5380 },
    { 284, 284, 279, 0, 31, 2, 30, 0x5380 },
    { 285, 285, 279, 0, 31, 2, 30, 0x5380 },
    { 286, 286, 279, 0, 31, 2, 30, 0x5380 },
    { 287, 287, 279, 0, 31, 2, 30, 0x5380 },
    { 288, 288, 279, 0, 31, 2, 30, 0x5380 },
};

IntrudeFieldUnkEntry *func_ov018_0217bea8(u32 param_1) {
    u32 i;

    for (i = 0; i < 0xda; i++) {
        IntrudeFieldUnkEntry *entry = &data_ov018_02182b9c[i];
        if (param_1 == entry->unk_00_0) {
            return entry;
        }
    }
    return NULL;
}



BOOL func_ov018_0217bed4(u16 zoneId, VecFx32 *param_2) {
    IntrudeFieldUnkEntry *entry;
    fx32 x;
    fx32 y;
    fx32 z;

    if (IsZoneEntralinkHub(zoneId) != TRUE) {
        entry = func_ov018_0217bea8(zoneId);
        if (entry == NULL) {
            return FALSE;
        }
        z = (s32)entry->unk_08 << 16;
        y = (s32)entry->unk_06 << 16;
        x = (s32)entry->unk_04 << 16;
        param_2->x = x;
        param_2->y = y;
        param_2->z = z;
    } else {
        *param_2 = data_ov018_02182b60;
    }
    return TRUE;
}



void func_ov018_0217bf1c(IntrudeHighLinkData *data) {
    MI_CpuFill8(data, 0, sizeof(IntrudeHighLinkData));
}

u8 func_ov018_0217bf28(IntrudeHighLinkData *data) {
    return data->unk_0E;
}

void func_ov018_0217bf2c(Field *field, u16 gridX, u16 gridZ, s32 gridY, u16 modelId, u16 scrId, u16 uid) {
    MMSys *mmSys;
    ZoneNPC npc;
    ZoneNPCGridPosition *grid;
    u16 zoneId;

    mmSys = Field_GetActorSystem(field);
    zoneId = Field_GetPlayerStateZoneID(field);
    npc = data_ov018_02182b6c;
    npc.uid = uid;
    npc.modelId = modelId;
    npc.scrId = scrId;
    grid = &npc.pos.grid;
    grid->x = gridX;
    grid->z = gridZ;
    grid->y = gridY;
    CreateNewActorByEntityNoWKOBJCODE(mmSys, &npc, zoneId);
}

void func_ov018_0217bf84(Field *field) {
    s32 j;
    s32 i;

    for (i = 1; i < 3; i++) {
        for (j = 0; j < NELEMS(data_ov018_02182b54); j++) {
            func_ov018_0217bf2c(field, data_ov018_02182b54[j].unk_02 + i * 64, data_ov018_02182b54[j].unk_04, 0x2001f,
                data_ov018_02182b54[j].unk_00, data_ov018_02182b54[j].unk_06, data_ov018_02182b54[j].unk_08 + i - 1);
        }
    }
}

BOOL func_ov018_0217bfd8(GameSystem *gsys, u16 zoneId) {
    GameData *sp0;
    GameCommSys *commSys;
    GameData *gameData;
    IntrudeComm *intrudeComm;

    gameData = GSYS_GetGameData(gsys);
    sp0 = gameData;
    commSys = GSYS_GetGameCommSystem(gsys);
    intrudeComm = func_ov010_02158344(commSys);
    if (GameData_IsForceSeasonSync(gameData) == 0) {
        return 0U;
    }
    if (intrudeComm != NULL) {
        return 0U;
    }
    if (func_02012204(gsys) == 1) {
        return 1U;
    }
    if (IsZoneEntralinkHub(zoneId) == 1) {
        if (func_0202648c(commSys) != 0) {
            return 1U;
        }
        return 0U;
    }
    if (GetZoneIsEntreeForest(zoneId) == 1) {
        if ((GameCommSys_BootCheck(commSys) == 2) || ((GameCommSys_BootCheck(commSys) == 0) && (func_0202647c(commSys) == 2) && (func_0202648c(commSys) != 0))) {
            return 1U;
        }
        return 0U;
    }
    return 1U;
}



BOOL func_ov018_0217c070(IntrudeComm *work) {
    if (work->unk_93F_0 == TRUE || func_ov011_02176258(work) == TRUE) {
        return TRUE;
    }
    return FALSE;
}



void func_ov018_0217c094(IntrudeComm *comm) {
    if (comm != NULL) {
        comm->unk_8F8_5 = 1;
    }
}

static inline PlayerInfo *IntrudeField_GetPlayerInfo(GameCommSys *commSys, GameData *gameData) {
    IntrudeComm *intcomm;

    if (!GameData_IsForceSeasonSync(gameData)) {
        return GetGameDataPlayerInfo(gameData);
    }
    if (func_02012b80(gameData) == func_02012b58(gameData)) {
        return GetGameDataPlayerInfo(gameData);
    }
    intcomm = NULL;
    if (GameCommSys_BootCheck(commSys) == 2 && func_02026474(commSys) != NULL) {
        intcomm = func_02026474(commSys);
    }
    if (intcomm == NULL) {
        return GetGameDataPlayerInfo(gameData);
    }
    if (IsZoneEntralinkHub(intcomm->unk_070.zoneId) == TRUE) {
        return GetGameDataPlayerInfo(gameData);
    }
    return func_02012950(gameData, func_02012b80(gameData));
}

u32 func_ov018_0217c0a8(GameCommSys *param_1) {
    GameData *gameData = getBasePlayerBlk(param_1);

    return getTrainerGender(IntrudeField_GetPlayerInfo(param_1, gameData));
}


BOOL func_ov018_0217c11c(u16 zoneId) {
    u32 i;
    i = 0;
    while (i < data_ov010_021700a4) {
        if (zoneId == data_ov010_02170128[i]) {
            return 1;
        }
        i = i + 1;
    }
    return 0;
}

void func_ov018_0217c14c(Field *field, u16 gridX, u16 gridZ, s32 gridY) {
    MMSys *mmSys;
    ZoneNPC npc;
    ZoneNPCGridPosition *grid;
    u16 zoneId;

    mmSys = Field_GetActorSystem(field);
    zoneId = Field_GetPlayerStateZoneID(field);
    npc = data_ov018_021835d4;
    grid = &npc.pos.grid;
    grid->x = gridX;
    grid->z = gridZ;
    grid->y = gridY;
    CreateNewActorByEntityNoWKOBJCODE(mmSys, &npc, zoneId);
}

void func_ov018_0217c194(Field *field) {
    u16 zoneId;
    s32 i;

    zoneId = Field_GetPlayerStateZoneID(field);
    if (IsZoneEntralinkHub(zoneId) != FALSE) {
        for (i = 0; i < 3; i++) {
            func_ov018_0217c14c(field, (u16)((((i << 22) + 0x1f8000) >> 12) / 16), 0x1d, 0x20);
        }
    }
}

const ZoneNPC data_ov018_021835d4 = {
    0x63, 0, 0xf, 0, 0, 4, 1, 0, 0, 0, 0xffff, 0xffff, 0,
};
