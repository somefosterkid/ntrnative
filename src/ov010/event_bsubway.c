// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_bsubway.c, ported by tools/port_file.py.
// The Battle Subway's events: picking the Pokémon to enter, the battle, the trainers' and leaders' messages, and its
// records screen. The name is descriptive
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "app/bsubway_records.h"
#include "app/p_status.h"
#include "app/pokelist.h"
#include "battle/battle_proc.h"
#include "battle/btl_net.h"
#include "battle/btl_setup.h"
#include "battle/regulation.h"
#include "field/bsubway_scr.h"
#include "field/encounter.h"
#include "field/event_battle.h"
#include "field/event_sound.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_camera.h"
#include "field/field_event.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/poke_party.h"
#include "save/bsubway_save.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/pms.h"
#include "field/event_action_call.h"

GameEventReturnCode func_ov010_0216c9f4(GameEvent *event, u32 *state, void *work);

// The timing the partners synchronize at before the battle
#define BSUBWAY_COMM_TIMING_BATTLE 200

typedef struct {
    GameSystem *gsys;
    Field *field;
    PStatusParam summaryParam;
    PokeListParam partyParam;
    u16 *result;
    u16 *choice;
    u8 *picked;
    u32 unkE4;
} BSubwayPokeSelectData;

typedef struct {
    GameSystem *gsys;
    u32 unk04;
    VecFx32 pos;
    void *msgWin;
    StrBuf *strbuf;
    u16 actorId;
    u8 unk1E[0xba];
} BSubwayTrainerMsgData;

typedef struct {
    Field *field;
    GameSystem *gsys;
    BSubwayRecordsParam param;
} BSubwayRecordsData;

typedef struct {
    GameProcManager *procManager;
} BSubwayCommBattleWork;

typedef struct {
    GameData *gameData;
    BtlSetup *setup;
    void *unk08;
} BSubwayCommBattleParam;

typedef struct {
    GameSystem *gsys;
    BtlSetup *setup;
    void *unk08;
    BSubwayCommBattleParam param;
    u8 unk18[0x24];
} BSubwayCommBattleData;

typedef struct {
    GameSystem *gsys;
    Field *field;
    BtlSetup *setup;
    void *unk0C;
} BSubwayBattleData;

typedef struct {
    GameSystem *gsys;
    u32 unk04;
    VecFx32 pos;
    void *msgWin;
    StrBuf *strbuf;
} BSubwayLeaderMsgData;

// A leader of the Battle Subway, as the save keeps it: an entry of block 0x3a's list
typedef struct {
    u8 unk00[0x18];
    PMSData message;
    u8 unk20[2];
} BSubwayLeader;

BOOL func_ov010_0216c888(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov010_0216c8b0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov010_0216c8cc(GameProc *proc, u32 *state, void *param, void *work);

static const GameProcFunctions data_ov010_02172548 = {
    func_ov010_0216c888,
    func_ov010_0216c8cc,
    func_ov010_0216c8b0,
};

GameEvent *func_ov010_0216cad4(BSubwayScrWork *bsw, GameSystem *gsys, Field *field);

GameEventReturnCode func_ov010_0216c484(GameEvent *event, u32 *state, void *work) {
    BSubwayPokeSelectData *data = work;
    GameSystem *gsys = data->gsys;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, data->field, 0, 0));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, data->field));
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, EventPokeList_Create(gsys, data->field, &data->partyParam, &data->summaryParam));
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 4:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, data->field, 0, 0, 1, 0, 0));
        MI_CpuCopy8(data->partyParam.picked, data->picked, sizeof(data->partyParam.picked));
        *data->choice = data->partyParam.index;
        *data->result = data->partyParam.result;
        GFL_HeapFree(data->partyParam.regulation);
        (*state)++;
        break;
    case 5:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

static inline void InitSummaryParam(BSubwayPokeSelectData *data, PokeParty *party, GameData *gameData) {
    PStatusParam *param = &data->summaryParam;
    PokeDexSave *pokedex = GameData_GetPokedex(gameData);

    MI_CpuFill8(param, 0, sizeof(PStatusParam));
    param->party = party;
    param->dataType = PSTATUS_DATA_PARTY;
    param->partyCount = PokeParty_GetPkmCount(party);
    param->mode = PSTATUS_MODE_NORMAL;
    param->page = PSTATUS_PAGE_INFO;
    param->gameData = gameData;
    param->isNationalDex = PokeDex_IsNationalObtained(pokedex);
}

GameEvent *func_ov010_0216c538(BSubwayScrWork *bsw, GameSystem *gsys, u8 rental) {
    Field *field;
    GameEvent *event;
    GameData *gameData;
    BSubwayPokeSelectData *data;
    PokeParty *party;
    PokeListParam *partyParam;
    u32 regulationId;
    u32 mode;

    gameData = GSYS_GetGameData(gsys);
    field = GSYS_GetField(gsys);
    event = GameEvent_Create(gsys, NULL, func_ov010_0216c484, sizeof(BSubwayPokeSelectData));
    data = GameEvent_GetData(event);
    data->gsys = gsys;
    data->field = field;
    data->picked = bsw->memberChoices;
    data->choice = &bsw->unk82;
    data->result = &bsw->unk84;
    if (rental == FALSE) {
        party = GameData_GetParty(gameData);
    } else {
        party = bsw->allocatedBuffer;
    }
    regulationId = 20;
    mode = 0;
    partyParam = &data->partyParam;
    MI_CpuFill8(partyParam, 0, sizeof(PokeListParam));
    partyParam->party = party;
    switch (bsw->playMode) {
    case 1:
    case 6:
        regulationId = 21;
        mode = 1;
        break;
    case 2:
    case 3:
    case 7:
    case 8:
        regulationId = 22;
        mode = 2;
        break;
    }
    partyParam->regulation = func_0201a354(regulationId, HEAPID_GAMEEVENT);
    partyParam->mode = 0x16;
    partyParam->unk38 = mode;
    InitSummaryParam(data, party, gameData);
    return event;
}


GameEvent *func_ov010_0216c640(BSubwayScrWork *bsw, GameSystem *gsys, Field *field) {
    func_0200b400(0x4);
    bsw->btlSetup = func_ov021_021cbd08(bsw, gsys);
    if (bsw->playMode == 3 || bsw->playMode == 8) {
        return func_ov010_0216cad4(bsw, gsys, field);
    }
    return func_ov010_0216e8cc(gsys, field, bsw->btlSetup);
}


static GameEventReturnCode func_ov010_0216c680(GameEvent *event, u32 *state, void *work) {
    BSubwayTrainerMsgData *data = work;

    switch (*state) {
    case 0:
        if (!func_ov021_02191388(data->msgWin)) {
            break;
        }
        (*state)++;
        // fallthrough
    case 1:
        func_ov021_021914cc(data->msgWin);
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
            func_ov021_021912d8(data->msgWin);
            (*state)++;
        }
        break;
    case 2:
        if (func_ov021_021912f8(data->msgWin) == TRUE) {
            GFL_StrBufFree(data->strbuf);
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov010_0216c6e8(BSubwayScrWork *bsw, GameSystem *gsys, u32 index, u16 actorId, u8 winPos) {
    GameEvent *event;
    GameData *gameData;
    void *msgBGSys;
    Field *field;
    BSubwayTrainerMsgData *data;
    PMSData *message;
    MsgData *msgData;
    u16 messageId;
    FieldCamera *camera;
    VecFx32 offset;
    u32 winX;
    u32 winY;

    gameData = GSYS_GetGameData(gsys);
    field = GSYS_GetField(gsys);
    msgBGSys = Field_GetMsgBGSys(field);
    event = GameEvent_Create(gsys, NULL, func_ov010_0216c680, sizeof(BSubwayTrainerMsgData));
    data = GameEvent_GetData(event);
    data->gsys = gsys;
    data->actorId = actorId;
    message = &bsw->trainers[index].message;
    if (message->type == 0xffff) {
        data->strbuf = GFL_StrBufCreate(0x300, HEAPID_GAMEEVENT);
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BATTLE_SUBWAY_TRAINER_MESSAGES, HEAPID_GAMEEVENT);
        messageId = bsw->trainers[index].message.id;
        if (messageId >= 0x3ae) {
            messageId = 0;
        }
        GFL_MsgDataLoadStrbuf(msgData, messageId, data->strbuf);
        GFL_MsgDataFree(msgData);
    } else {
        data->strbuf = func_0202435c(message, HEAPID_GAMEEVENT);
    }
    CopyActorWPos(FindFieldActor(GameData_GetMMSys(gameData), data->actorId), &data->pos);
    camera = Field_GetCameraSystem(field);
    func_ov021_021b09d8(&data->pos, &offset, FieldCamera_GetG3DCamera(camera), camera, winPos);
    data->pos.x += offset.x;
    data->pos.y += offset.y;
    data->pos.z += offset.z;
    func_ov021_021b09ec(winPos, &winX, &winY);
    data->msgWin = ActorMsgWin_CheckAndCreate(msgBGSys, winX, &data->pos, data->strbuf, 0, winY);
    return event;
}

static GameEventReturnCode func_ov010_0216c7fc(GameEvent *event, u32 *state, void *work) {
    BSubwayRecordsData *data = work;
    void *bsubwaySave;

    switch (*state) {
    case 0:
        data->param.gameData = GSYS_GetGameData(data->gsys);
        bsubwaySave = func_02012efc(data->param.gameData);
        data->param.rank = func_0200df40(bsubwaySave);
        data->param.trainNumber = func_0200df4c(bsubwaySave);
        GameEvent_ChainNext(event, EventFieldSubprocessTransition_Create(data->gsys, data->field, OVERLAY_ID(69),
                                                                         &data_ov069_021ba650, &data->param));
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov010_0216c860(GameSystem *gsys) {
    Field *field = GSYS_GetField(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov010_0216c7fc, sizeof(BSubwayRecordsData));
    BSubwayRecordsData *data = GameEvent_GetData(event);

    data->gsys = gsys;
    data->field = field;
    return event;
}

BOOL func_ov010_0216c888(GameProc *proc, u32 *state, void *param, void *work) {
    BSubwayCommBattleWork *procWork;

    GFL_HeapCreateChild(HEAPID_USER, 0x76, 0x4000);
    procWork = GFL_ProcInitSubsystem(proc, sizeof(BSubwayCommBattleWork), 0x76);
    procWork->procManager = CreateGameProcManager(0x76);
    return TRUE;
}

BOOL func_ov010_0216c8b0(GameProc *proc, u32 *state, void *param, void *work) {
    BSubwayCommBattleWork *procWork = work;

    FreeGameProcManager(procWork->procManager);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x76);
    return TRUE;
}

BOOL func_ov010_0216c8cc(GameProc *proc, u32 *state, void *param, void *work) {
    BSubwayCommBattleParam *battleParam = param;
    BSubwayCommBattleWork *procWork = work;
    BOOL running = GFL_ProcMgrUpdate(procWork->procManager);

    switch (*state) {
    case 0:
        GFL_OvlLoad(OVERLAY_ID(93));
        func_0203a574(0x100, data_ov093_021f03c4, 9, NULL);
        func_0203d594(func_020399a8(), BSUBWAY_COMM_TIMING_BATTLE);
        *state = 1;
        break;
    case 1:
        if (func_0200fb24()) {
            *state = 4;
        } else if (func_0203d5a0(func_020399a8(), BSUBWAY_COMM_TIMING_BATTLE)) {
            *state = 2;
        }
        break;
    case 2:
        QueueGameProc(procWork->procManager, -1, &data_ov093_021efdac, battleParam->setup);
        *state = 3;
        break;
    case 3:
        if (running != TRUE) {
            *state = 4;
        }
        break;
    case 4:
        func_0203a5e4(0x100);
        GFL_OvlUnload(OVERLAY_ID(93));
        *state = 5;
        break;
    case 5:
        return TRUE;
    }
    return FALSE;
}


GameEventReturnCode func_ov010_0216c978(GameEvent *event, u32 *state, void *work) {
    BSubwayCommBattleData *data = work;
    GameSystem *gsys = data->gsys;
    BSubwayCommBattleParam *param;

    switch (*state) {
    case 0:
        param = &data->param;
        data->param.gameData = GSYS_GetGameData(gsys);
        param->setup = data->setup;
        param->unk08 = data->unk08;
        GSYS_QueueProc(gsys, -1, &data_ov010_02172548, param);
        (*state)++;
        break;
    case 1:
        if (GSYS_GetProcMgrState(gsys) == FALSE) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov010_0216c9cc(GameSystem *gsys, BtlSetup *setup, void *a2) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov010_0216c978, sizeof(BSubwayCommBattleData));
    BSubwayCommBattleData *data = GameEvent_GetData(event);

    data->gsys = gsys;
    data->setup = setup;
    data->unk08 = a2;
    return event;
}

GameEventReturnCode func_ov010_0216c9f4(GameEvent *event, u32 *state, void *work) {
    BSubwayBattleData *data = work;
    GameSystem *gsys = data->gsys;
    Field *field = data->field;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallEventPrepareResidentActorsForZoneChange(gsys, field));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, EventBattleBGMPlay_Create(gsys, data->setup->fieldSituation.bgm));
        (*state)++;
        break;
    case 2:
        func_ov021_021aaabc(Field_GetEncountSystem(field));
        EncEff_StartEvent(Field_GetEncEff(data->field), event, 9);
        (*state)++;
        break;
    case 3:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, field));
        (*state)++;
        break;
    case 4:
        GameEvent_ChainNext(event, func_ov010_0216c9cc(gsys, data->setup, data->unk0C));
        (*state)++;
        break;
    case 5:
        GameEvent_ChainNext(event, EventBGMFadePop_Create(gsys));
        (*state)++;
        break;
    case 6:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 7:
        GameEvent_ChainNext(event, EventBGMFadeWait_Create(gsys));
        (*state)++;
        break;
    case 8:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0));
        (*state)++;
        break;
    case 9:
        return GAMEEVENT_DONE;
    default:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *func_ov010_0216caa8(GameSystem *gsys, Field *field, BtlSetup *setup, void *a3) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov010_0216c9f4, sizeof(BSubwayBattleData));
    BSubwayBattleData *data = GameEvent_GetData(event);

    data->gsys = gsys;
    data->setup = setup;
    data->unk0C = a3;
    data->field = field;
    return event;
}

void *func_ov021_021cbd84(BSubwayScrWork *bsw);
GameEvent *func_ov010_0216cad4(BSubwayScrWork *bsw, GameSystem *gsys, Field *field) {
    return func_ov010_0216caa8(gsys, field, bsw->btlSetup, func_ov021_021cbd84(bsw));
}


static GameEventReturnCode func_ov010_0216caf4(GameEvent *event, u32 *state, void *work) {
    BSubwayLeaderMsgData *data = work;

    switch (*state) {
    case 0:
        if (func_ov021_02191388(data->msgWin) == TRUE) {
            (*state)++;
        }
        break;
    case 1:
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            func_ov021_021912d8(data->msgWin);
            (*state)++;
        }
        break;
    case 2:
        if (func_ov021_021912f8(data->msgWin) == TRUE) {
            GFL_StrBufFree(data->strbuf);
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov010_0216cb48(GameSystem *gsys, u16 index, u16 actorId) {
    GameData *gameData = GSYS_GetGameData(gsys);
    Field *field = GSYS_GetField(gsys);
    void *msgBGSys = Field_GetMsgBGSys(field);
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov010_0216caf4, sizeof(BSubwayLeaderMsgData));
    BSubwayLeaderMsgData *data = GameEvent_GetData(event);
    MMSys *actorSystem;
    FieldActor *player;
    u8 winPos;
    FieldCamera *camera;
    VecFx32 offset;
    BSubwayLeader *leaders;
    u32 winX;
    u32 winY;

    data->gsys = gsys;
    actorSystem = GameData_GetMMSys(gameData);
    player = FindPlayerFieldActor(actorSystem);
    CopyActorWPos(FindFieldActor(actorSystem, actorId), &data->pos);
    winPos = ActorMsgWin_CalcWinPosAuto(player, &data->pos);
    camera = Field_GetCameraSystem(field);
    func_ov021_021b09d8(&data->pos, &offset, FieldCamera_GetG3DCamera(camera), camera, winPos);
    data->pos.x += offset.x;
    data->pos.y += offset.y;
    data->pos.z += offset.z;
    leaders = func_0200df58(SaveControl_GetBlockPtr(GameData_GetSaveControl(gameData), 0x3b), HEAPID_GAMEEVENT);
    data->strbuf = func_0202435c(&leaders[index].message, HEAPID_GAMEEVENT);
    GFL_HeapFree(leaders);
    func_ov021_021b09ec(winPos, &winX, &winY);
    data->msgWin = ActorMsgWin_CheckAndCreate(msgBGSys, winX, &data->pos, data->strbuf, 0, winY);
    return event;
}
