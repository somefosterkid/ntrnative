// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_game_clear.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/pokemon.h"
#include "constants/version.h"
#include "demo/the_end.h"
#include "field/encounter.h"
#include "field/event_game_clear.h"
#include "field/event_3d_demo.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_script.h"
#include "field/field_script_event.h"
#include "field/subscreen.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "app/unova_link.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/medal_box.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_comm.h"
#include "system/game_system.h"
#include "system/version.h"
#include "save/trainer_data_savedata.h"
#include "field/intrude_comm.h"

extern u32 OVERLAY_20_ID[];
extern u32 OVERLAY_180_ID[];
extern u32 OVERLAY_181_ID[];
extern u32 OVERLAY_182_ID[];
extern const GameProcFunctions data_ov180_021b800c;
extern const GameProcFunctions data_ov181_021bab24;
extern const GameProcFunctions data_ov205_021b9e54;
extern const GameProcFunctions data_ov182_021b9ba0;
extern u32 OVERLAY_35_ID[];

// Where the player returns to after the credits, in each version
static const VecFx32 sEndingPositionA = { 31 * 16 * FX32_ONE, 47 * 16 * FX32_ONE, 16 * FX32_ONE };
static const VecFx32 sEndingPositionB = { 43 * 16 * FX32_ONE, 16 * FX32_ONE, 757 * 16 * FX32_ONE };
void func_ov010_0215e580(GameData *gameData);
void SetGameClearGameData(GameClearWork *work);
void func_ov010_0215f920(GameClearWork *work);
void SetGameClearStatusSequence(GameClearWork *work);



GameEventReturnCode EventGameClear_Callback(GameEvent *event, u32 *state, void *data) {
    GameClearWork *work = data;
    GameSystem *gsys = work->gameSystem;
    GameData_GetSaveControl(work->gameData);
    Field *field = GSYS_GetField(work->gameSystem);
    GameCommSys *comm = GSYS_GetGameCommSystem(work->gameSystem);

    switch (work->current) {
    case 0:
        if (work->unk08 == 1) {
            func_ov010_0215f9ec(work);
        }
        GFL_SndBGMFadeOut(30);
        EventGameClear_NextState(work, state);
        break;
    case 1:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        EventGameClear_NextState(work, state);
        break;
    case 2:
        if (GameCommSys_BootCheck(comm)) {
            func_02026400(comm);
        }
        EventGameClear_NextState(work, state);
        break;
    case 3:
        if (!GameCommSys_BootCheck(comm)) {
            EventGameClear_NextState(work, state);
        }
        break;
    case 4:
        GameEvent_ChainNext(event, EventFieldCloseKeepSound_Create(gsys, field));
        EventGameClear_NextState(work, state);
        break;
    case 5:
        GSYS_QueueProc(gsys, OVERLAY_ID(180), &data_ov180_021b800c, &work->ov265Param);
        EventGameClear_NextState(work, state);
        break;
    case 6:
        if (!GSYS_GetProcMgrState(gsys)) {
            EventGameClear_NextState(work, state);
        }
        break;
    case 7:
        GSYS_QueueProc(gsys, OVERLAY_ID(181), &data_ov181_021bab24, &work->ov266Param);
        EventGameClear_NextState(work, state);
        break;
    case 8:
        if (!GSYS_GetProcMgrState(gsys)) {
            EventGameClear_NextState(work, state);
        }
        break;
    case 9:
        GameEvent_ChainNext(event, Event3DDemo_Create(gsys, event, EventGameClear_Get3DDemoID(), 0, 0));
        EventGameClear_NextState(work, state);
        break;
    case 10:
        FieldScript_CallPlayerPostHOFSetup(gsys, 4);
        EventGameClear_NextState(work, state);
        break;
    case 11:
        GSYS_QueueProcAsEvent(event, OVERLAY_ID(205), &data_ov205_021b9e54, &work->ov295Param);
        EventGameClear_NextState(work, state);
        break;
    case 12:
        if (work->counter > 60) {
            work->counter = 0;
            EventGameClear_NextState(work, state);
        }
        work->counter++;
        break;
    case 13:
        GSYS_QueueProcAsEvent(event, OVERLAY_ID(182), &data_ov182_021b9ba0, &work->ov267Param);
        func_02012158(gsys, 0);
        EventGameClear_NextState(work, state);
        break;
    case 14:
        return GAMEEVENT_DONE;
    case 15:
        return GAMEEVENT_CONTINUE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *EventGameClear_Create(GameSystem *gsys, u32 param) {
    GameData *gameData = GSYS_GetGameData(gsys);
    GameEvent *event = GameEvent_Create(gsys, NULL, EventGameClear_Callback, sizeof(GameClearWork));
    GameClearWork *work = GameEvent_GetData(event);

    work->gameSystem = gsys;
    work->gameData = gameData;
    work->unk08 = param;
    work->unk10 = GetGameDataPlayerInfo(gameData);
    work->counter = 0;
    work->ov295Param.gsys = gsys;
    work->ov295Param.param = param;
    func_ov010_0215e580(gameData);
    SetGameClearGameData(work);
    func_ov010_0215f920(work);
    SetGameClearStatusSequence(work);
    return event;
}

void SetGameClearGameData(GameClearWork *work) {
    work->ov265Param.party = GameData_GetParty(work->gameData);
    work->ov265Param.playerInfo = GetGameDataPlayerInfo(work->gameData);
    work->ov265Param.playTime = func_02012fcc(work->gameData);
}

void func_ov010_0215f920(GameClearWork *work) {
    u32 value;

    work->ov266Param.unk00 = work->unk08 == 1;
    value = func_020083e4((Config *)getTrainerDataBlkAddress(GameData_GetSaveControl(work->gameData)));
    work->ov266Param.unk04 = value;
    work->ov267Param.unk00 = value;
}


void SetGameClearStatusSequence(GameClearWork *work) {
    switch (work->unk08) {
    case 0:
    default:
        work->states[0] = 0;
        work->states[1] = 2;
        work->states[2] = 3;
        work->states[3] = 4;
        work->states[4] = 7;
        work->states[5] = 8;
        work->states[6] = 12;
        work->states[7] = 9;
        work->states[8] = 12;
        work->states[9] = 10;
        work->states[10] = 11;
        work->states[11] = 12;
        work->states[12] = 13;
        work->states[13] = 14;
        break;
    case 1:
        work->states[0] = 0;
        work->states[1] = 2;
        work->states[2] = 1;
        work->states[3] = 3;
        work->states[4] = 4;
        work->states[5] = 5;
        work->states[6] = 6;
        work->states[7] = 10;
        work->states[8] = 11;
        work->states[9] = 7;
        work->states[10] = 8;
        work->states[11] = 13;
        work->states[12] = 14;
        break;
    }
    work->current = work->states[0];
}

u32 EventGameClear_Get3DDemoID(void) {
    switch (getGameVersion()) {
    case VERSION_WHITE:
    default:
        return 8;
    case VERSION_BLACK:
        return 7;
    }
}


void EventGameClear_NextState(GameClearWork *work, u32 *state) {
    ++*state;
    work->current = work->states[*state];
}

void func_ov010_0215f9ec(GameClearWork *work) {
    func_0200c3b8(getTrainerCardDataBlkAddress(work->gameData), 0x5a0);
}

GameEvent *EventGameClear_GiveMonotypeMedals(GameSystem *gsys, GameSystemProcData *procData) {
    GFL_OvlLoad(OVERLAY_ID(20));
    return CreateGameEntryPointEvent(gsys, procData);
}


GameEvent *EventMapChangeBlackout_CreateExternal(GameSystem *gsys) {
    GFL_OvlLoad(OVERLAY_ID(20));
    return EventMapChangeBlackout_Create(gsys);
}


BOOL LoadFieldGlueOverlay(void) {
    return GFL_OvlLoad(OVERLAY_ID(20));
}


void UnloadFieldGlueOverlay(void) {
    GFL_OvlUnload(OVERLAY_ID(20));
}
