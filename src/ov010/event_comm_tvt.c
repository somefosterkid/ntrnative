// Black's code at ov010 0x0215fa54-0x0215fc40: pokebw2's ov017 event_comm_tvt.c (EventCommTvt_Callback and
// EventCommTvt_Create), the C-Gear's Xtransceiver event, which runs overlay 122's comm_tvt_field proc and then overlay
// 172's comm_tvt proc. Here it follows event_game_clear.c, in no file before (the callback was event_game_clear.c's
// last function).
#include "types.h"
#include "field/event_comm_tvt.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/intrude_comm.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

extern u32 OVERLAY_122_ID[];
extern u32 OVERLAY_172_ID[];
extern const GameProcFunctions data_ov122_021d5764;
extern const GameProcFunctions data_ov172_021c7240;

typedef struct EventCommTvt {
    GameSystem *gsys;
    GameData *gameData;
    SaveControl *save;
    // Overlay 172's proc param
    u32 commParam[5];
    u32 mode;
    u32 bgm;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
} EventCommTvt;

GameEventReturnCode func_ov010_0215fa54(GameEvent *event, u32 *state, void *data) {
    EventCommTvt *work = data;
    GameSystem *gsys = work->gsys;
    GameCommSys *comm = GSYS_GetGameCommSystem(gsys);
    GameData *gameData = GSYS_GetGameData(gsys);
    Field *field = GSYS_GetField(gsys);

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(gsys, field));
        *state = 2;
        break;
    case 2:
        (*state)++;
    case 3:
        GSYS_QueueProc(gsys, OVERLAY_ID(122), &data_ov122_021d5764, work);
        (*state)++;
        break;
    case 4:
        if (GSYS_GetProcMgrState(gsys)) {
            break;
        }
        switch (work->mode) {
        case 0:
            if (func_ov010_02158344(comm) && work->unk28 == 2) {
                func_020129b8(gameData, 3);
            }
            *state = 6;
            break;
        case 1:
            work->bgm = GFL_SndBGMGetID();
            GSYS_QueueProcAsEvent(event, OVERLAY_ID(172), &data_ov172_021c7240, work->commParam);
            *state = 5;
            break;
        default:
            *state = 6;
            break;
        }
        break;
    case 5:
        if (GSYS_GetProcMgrState(gsys)) {
            break;
        }
        GFL_SndBGMPlay(work->bgm, 0xFFFF);
        GFL_SndBGMFadeIn(60);
        *state = 6;
        break;
    case 6:
        work->commParam[4] = 0;
        (*state)++;
        break;
    case 7:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(gsys));
        (*state)++;
        break;
    case 8:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0));
        (*state)++;
        break;
    case 9:
        if (work->mode == 0) {
            if (work->unk28 == 0) {
                if (func_02026460(comm)) {
                    break;
                }
                if (GameCommSys_BootCheck(comm)) {
                    func_02026400(comm);
                    break;
                }
                GameEvent_ChainNext(event, func_ov018_0217ce08(gsys, field, 0x117, 0));
                *state = 10;
            } else if (work->unk28 == 3) {
                GameEvent_ChainNext(event, func_ov018_0217fa24(gsys));
                *state = 10;
            } else {
                if (work->unk28 != 2) {
                    GameEvent_ChainNext(event, func_ov018_0217cdbc(gsys));
                }
                *state = 10;
            }
        } else {
            *state = 10;
        }
        break;
    case 10:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov010_0215fbf4(GameSystem *gsys, Field *field, GameEvent *event, BOOL create) {
    EventCommTvt *work;

    Field_GetSubscreen(field);
    if (create) {
        event = GameEvent_Create(gsys, NULL, func_ov010_0215fa54, sizeof(EventCommTvt));
    } else {
        GameEvent_Transplant(event, func_ov010_0215fa54, sizeof(EventCommTvt));
    }
    work = GameEvent_GetData(event);
    work->gameData = GSYS_GetGameData(gsys);
    work->save = GameData_GetSaveControl(work->gameData);
    work->gsys = gsys;
    return event;
}
