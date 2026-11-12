// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov120/gimmick_league_champion.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_event.h"
#include "field/field_map.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

GameEvent *func_ov021_021c06cc(GameSystem *gsys, Field *field);
GameEventReturnCode func_ov021_021c06e0(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021c0780(GameEvent *event, u32 *state, void *data);
static BOOL func_ov021_021c0808(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov021_021c080c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov021_021c0810(GameProc *proc, u32 *state, void *param, void *work);
void func_ov010_02164fc4(void *work);

// The transition's processes: init, main and exit
const GameProcFunctions data_ov021_021da08c = {
    func_ov021_021c0808,
    func_ov021_021c080c,
    func_ov021_021c0810,
};

// pokebw2's GimmickChampion_CreateChangeEvent
GameEvent *func_ov021_021c06cc(GameSystem *gsys, Field *field) {
    return GameEvent_Create(gsys, NULL, func_ov021_021c06e0, 0);
}

// pokebw2's GimmickChampion_ChangeEvent
GameEventReturnCode func_ov021_021c06e0(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    Field *field;
    GameEvent *child;

    field = GSYS_GetField(gsys);
    switch (*state) {
    case 0:
        DisableAllActorsMovement(Field_GetActorSystem(field));
        if (func_0202647c(GSYS_GetGameCommSystem(gsys)) == 2) {
            if (GameData_IsForceSeasonSync(GSYS_GetGameData(gsys)) == TRUE) {
                func_0200fb38();
                child = EventFieldSubprocessTransition_Create(gsys, field, OVERLAY_ID(21), &data_ov021_021da08c, gsys);
            } else {
                child = GameEvent_Create(gsys, NULL, func_ov021_021c0780, 0);
            }
        } else {
            func_0200fb38();
            child = EventFieldSubprocessTransition_Create(gsys, field, OVERLAY_ID(21), &data_ov021_021da08c, gsys);
        }
        GameEvent_ChainNext(event, child);
        (*state)++;
        break;
    case 1:
        if (field != NULL && Field_CheckMapLoadFinished(field) == TRUE) {
            EnableAllActorsMovement(Field_GetActorSystem(field));
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

// pokebw2's GimmickChampion_OpenEvent
GameEventReturnCode func_ov021_021c0780(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    GameCommSys *comm = GSYS_GetGameCommSystem(gsys);
    void *work;

    switch (*state) {
    case 0:
        if (func_0203cd98()) {
            func_0203ceac(NULL);
        }
        (*state)++;
        break;
    case 1:
        if (func_0203cdbc() == TRUE) {
            func_0200fba0();
            func_02012124(gsys, 0);
            (*state)++;
        }
        break;
    case 2:
        if (func_020121cc(gsys) == TRUE) {
            (*state)++;
        } else {
            return GAMEEVENT_DONE;
        }
        break;
    case 3:
        work = func_02026474(comm);
        if (work != NULL && GameCommSys_BootCheck(comm) != 0) {
            func_ov010_02164fc4(work);
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

static BOOL func_ov021_021c0808(GameProc *proc, u32 *state, void *param, void *work) {
    return TRUE;
}

static BOOL func_ov021_021c080c(GameProc *proc, u32 *state, void *param, void *work) {
    return TRUE;
}

static BOOL func_ov021_021c0810(GameProc *proc, u32 *state, void *param, void *work) {
    func_02012124(param, 0);
    return TRUE;
}

