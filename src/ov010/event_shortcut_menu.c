// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/event_shortcut_menu.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/pokemon.h"
#include "field/field.h"
#include "field/field_menu.h"
#include "field/field_script.h"
#include "field/field_script_event.h"
#include "field/field_actor.h"
#include "field/hidden_event.h"
#include "field/itemuse_event.h"
#include "field/player_action.h"
#include "field/player_state.h"
#include "field/shortcut_menu.h"
#include "field/subscreen.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/shortcut.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"

GameEvent *func_ov010_021606dc(GameSystem *gsys, u16 code);
void func_ov021_021c5348(void *);
void func_ov021_021c5224(void *app);
BOOL func_ov021_021c5380(void *app);
u32 func_ov021_021c5394(void *app, u32 *item);
void func_ov021_021c536c(void *app);

const u32 data_ov010_021712ac[8] = { 0xffffffff, 0, 1, 2, 3, 4, 5, 0xffffffff };

u32 func_ov010_0215fec8(u32 index) {
    return data_ov010_021712ac[index];
}

BOOL func_ov010_0215fed4(FieldAppCallInput *input, void *arg) {
    FieldMenuWork *menu = arg;
    FieldSubscreen *subscreen = Field_GetSubscreen(menu->field);

    if (input->eventType == 0) {
        func_ov021_021a12bc(subscreen, menu->unk10);
    }
    return TRUE;
}

BOOL func_ov010_0215fef0(FieldAppCallInput *input, void *arg) {
    return TRUE;
}

void func_020129b8(GameData *gameData, u8 value);
BOOL func_ov010_0215fef4(FieldAppCallInput *input, void *arg) {
    FieldMenuWork *menu = arg;
    GameData *gameData = GSYS_GetGameData(menu->gameSystem);

    switch (input->eventType) {
    case 4:
        func_020129b8(gameData, 6);
        break;
    case 0:
        func_020129b8(gameData, 1);
        break;
    case 1:
    case 2:
    case 3:
        func_020129b8(gameData, menu->screenId);
        break;
    }
    return TRUE;
}

BOOL IsExistAnyYShortcut(GameSystem *gsys) {
    GameData *gameData = GSYS_GetGameData(gsys);
    SaveControl *save = GameData_GetSaveControl(gameData);
    ShortcutSave *shortcutSave = SaveControl_GetShortcutSave(save);

    return ShortcutSave_GetShortcutCount(shortcutSave) != 0;
}

GameEvent *CallYButtonShortcutMenu(GameSystem *gsys, Field *field, u16 code) {
    GameEvent *event;
    ShortcutMenuWork *work;
    ShortcutSave *shortcut = SaveControl_GetShortcutSave(GameData_GetSaveControl(GSYS_GetGameData(gsys)));

    if (ShortcutSave_GetShortcutCount(shortcut) == 1) {
        event = GameEvent_Create(gsys, NULL, EventShortcutCallDirect_Callback, sizeof(ShortcutMenuWork));
    } else {
        event = GameEvent_Create(gsys, NULL, EventShortcutChoicePopup_Callback, sizeof(ShortcutMenuWork));
    }
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(ShortcutMenuWork));
    work->gameSystem = gsys;
    work->event = event;
    work->field = field;
    work->code = code;
    work->done = 0;
    work->input = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(FieldAppCallInput), FALSE, "event_shortcut_menu.c", 0xc2);
    MI_CpuFill8(work->input, 0, sizeof(FieldAppCallInput));
    work->input->gameSystem = work->gameSystem;
    work->input->field = work->field;
    work->input->parent = event;
    if (ShortcutSave_GetShortcutCount(shortcut) != 1) {
        work->input->canRetry = func_ov010_02160668;
        work->input->callback1 = func_ov010_021606d0;
        work->input->arg = work;
    }
    DisableAllActorsMovement(Field_GetActorSystem(work->field));
    func_020355d8(FALSE);
    return event;
}

GameEventReturnCode EventShortcutChoicePopup_Callback(GameEvent *event, u32 *state, void *data) {
    ShortcutMenuWork *work = data;
    u32 item;
    u32 action;
    u32 invalid;
    u32 result;
    BOOL noRibbon = FALSE;
    BOOL checkPerms;
    BOOL blocked = FALSE;
    u32 mode;
    PlayerActionPerms perms;
    u32 id;
    GameEvent *next;

    switch (*state) {
    case 0:
        *state = 1;
        break;
    case 1:
        func_ov010_021605c0(0, work);
        *state = 2;
        break;
    case 2:
        func_ov021_021c5348(work->app);
        *state = 3;
        break;
    case 3:
        func_ov021_021c5224(work->app);
        if (!func_ov021_021c5380(work->app)) {
            *state = 4;
        }
        break;
    case 4:
        func_ov021_021c5224(work->app);
        result = func_ov021_021c5394(work->app, &item);
        if (result == 1) {
            noRibbon = FALSE;
            blocked = FALSE;
            checkPerms = ShortcutMenu_GetActionFromKeyItem(item, &action, &invalid);
            mode = ShortcutMenu_SetKeyItemID(work->input, item);
            if (invalid == 0 && GameData_IsForceSeasonSync(GSYS_GetGameData(work->gameSystem)) == TRUE) {
                blocked = TRUE;
            } else if (checkPerms == TRUE) {
                PlayerActionPerms_Create(&perms, work->gameSystem, work->field);
                if (func_02028754(&perms, action) == FALSE) {
                    blocked = TRUE;
                }
            }
            if (item == 9 && !CheckAnyRibbon(work)) {
                noRibbon = TRUE;
            }
            if (blocked == TRUE) {
                *state = 12;
            } else if (noRibbon == TRUE) {
                *state = 14;
            } else {
                switch (mode) {
                case 0:
                    *state = 9;
                    break;
                case 1:
                    *state = 11;
                    break;
                }
            }
        } else if (result == 2) {
            *state = 5;
        }
        break;
    case 5:
        func_ov021_021c536c(work->app);
        *state = 6;
        break;
    case 6:
        func_ov021_021c5224(work->app);
        if (!func_ov021_021c5380(work->app)) {
            *state = 7;
        }
        break;
    case 7:
        if (func_ov010_0216060c(work)) {
            GFL_HeapFree(work->input);
            EnableAllActorsMovement(Field_GetActorSystem(work->field));
            return GAMEEVENT_DONE;
        }
        break;
    case 8:
        func_ov010_021605c0(1, work);
        *state = 4;
        break;
    case 9:
        GameEvent_ChainNext(event, EventFieldAppCall_Create(work->input, work->code));
        *state = 10;
        break;
    case 10:
        if (work->input->eventType == 0) {
            if (work->done) {
                *state = 7;
            } else {
                *state = 8;
            }
        } else if (work->input->eventType == 1) {
            *state = 7;
        } else if (work->input->eventType == 3) {
            *state = 11;
        }
        break;
    case 11:
        if (func_ov010_0216060c(work)) {
            id = work->input->eventId;
            GFL_HeapFree(work->input);
            work->input = NULL;
            GameEvent_ChainNext(event, CallFieldCommonEventFunc(id, work->gameSystem, work->field));
            *state = 13;
        }
        break;
    case 12:
        if (func_ov010_0216060c(work)) {
            id = work->input->eventId;
            GFL_HeapFree(work->input);
            work->input = NULL;
            if (id == 0) {
                if (FieldPlayerState_GetExState(GameData_GetPlayerState(GSYS_GetGameData(work->gameSystem))) == 1) {
                    next = func_ov010_021606dc(work->gameSystem, TRUE);
                } else {
                    next = func_ov010_021606dc(work->gameSystem, FALSE);
                }
            } else {
                next = func_ov010_021606dc(work->gameSystem, FALSE);
            }
            GameEvent_ChainNext(event, next);
            *state = 13;
        }
        break;
    case 13:
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        return GAMEEVENT_DONE;
    case 14:
        if (func_ov010_0216060c(work)) {
            GFL_HeapFree(work->input);
            work->input = NULL;
            GameEvent_ChainNext(event, func_ov010_021606dc(work->gameSystem, 2));
            *state = 15;
        }
        break;
    case 15:
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode EventShortcutCallDirect_Callback(GameEvent *event, u32 *state, void *data) {
    ShortcutMenuWork *work = data;
    u32 eventId;
    GameEvent *next;

    switch (*state) {
    case 0: {
        GameData *gameData;
        SaveControl *save;
        ShortcutSave *shortcut;
        u32 item;
        u32 mode;
        u32 action;
        u32 invalid;
        BOOL checkPerms;
        BOOL noRibbon;
        BOOL blocked;
        PlayerActionPerms perms;

        gameData = GSYS_GetGameData(work->gameSystem);
        save = GameData_GetSaveControl(gameData);
        shortcut = SaveControl_GetShortcutSave(save);
        blocked = FALSE;
        item = func_0200c464(shortcut, 0);
        mode = ShortcutMenu_SetKeyItemID(work->input, item);
        noRibbon = FALSE;
        checkPerms = ShortcutMenu_GetActionFromKeyItem(item, &action, &invalid);
        if (invalid == 0 && GameData_IsForceSeasonSync(GSYS_GetGameData(work->gameSystem)) == TRUE) {
            blocked = TRUE;
        } else if (checkPerms == TRUE) {
            PlayerActionPerms_Create(&perms, work->gameSystem, work->field);
            if (func_02028754(&perms, action) == FALSE) {
                blocked = TRUE;
            }
        }
        if (item == 9 && !CheckAnyRibbon(work)) {
            noRibbon = TRUE;
        }
        if (blocked == TRUE) {
            *state = 5;
        } else if (noRibbon == TRUE) {
            *state = 7;
        } else {
            switch (mode) {
            case 0:
                *state = 2;
                break;
            case 1:
                *state = 4;
                break;
            }
        }
        break;
    }
    case 1:
        GFL_HeapFree(work->input);
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        return GAMEEVENT_DONE;
    case 2:
        if (func_ov010_0216060c(work)) {
            GameEvent_ChainNext(event, EventFieldAppCall_Create(work->input, work->code));
            *state = 3;
        }
        break;
    case 3:
        if (work->input->eventType == 0) {
            *state = 1;
        } else if (work->input->eventType == 1) {
            *state = 1;
        } else if (work->input->eventType == 3) {
            *state = 4;
        }
        break;
    case 4:
        eventId = work->input->eventId;
        GFL_HeapFree(work->input);
        work->input = NULL;
        GameEvent_ChainNext(event, CallFieldCommonEventFunc(eventId, work->gameSystem, work->field));
        *state = 6;
        break;
    case 5:
        eventId = work->input->eventId;
        GFL_HeapFree(work->input);
        work->input = NULL;
        if (eventId == 0) {
            if (FieldPlayerState_GetExState(GameData_GetPlayerState(GSYS_GetGameData(work->gameSystem))) == 1) {
                next = func_ov010_021606dc(work->gameSystem, TRUE);
            } else {
                next = func_ov010_021606dc(work->gameSystem, FALSE);
            }
        } else {
            next = func_ov010_021606dc(work->gameSystem, FALSE);
        }
        GameEvent_ChainNext(event, next);
        *state = 6;
        break;
    case 6:
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        return GAMEEVENT_DONE;
    case 7:
        GFL_HeapFree(work->input);
        work->input = NULL;
        GameEvent_ChainNext(event, func_ov010_021606dc(work->gameSystem, 2));
        *state = 8;
        break;
    case 8:
        EnableAllActorsMovement(Field_GetActorSystem(work->field));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


u32 ShortcutMenu_SetKeyItemID(FieldAppCallInput *input, u32 item) {
    switch (item) {
    case 0:
        input->eventId = 0;
        return 1;
    case 1:
        input->appId = 7;
        return 0;
    case 2:
        input->appId = 11;
        return 0;
    case 3:
        input->appId = 8;
        return 0;
    case 4:
        input->eventId = 4;
        return 1;
    case 5:
        input->eventId = 5;
        return 1;
    case 6:
        input->appId = 0;
        input->appParam = 5;
        return 0;
    case 7:
        input->appId = 6;
        input->appParam = 0;
        return 0;
    case 8:
        input->appId = 6;
        input->appParam = 1;
        return 0;
    case 9:
        input->appId = 6;
        input->appParam = 2;
        return 0;
    case 10:
        input->appId = 2;
        input->appParam = 0;
        return 0;
    case 11:
        input->appId = 2;
        input->appParam = 1;
        return 0;
    case 12:
        input->appId = 2;
        input->appParam = 2;
        return 0;
    case 13:
        input->appId = 2;
        input->appParam = 3;
        return 0;
    case 14:
        input->appId = 2;
        input->appParam = 4;
        return 0;
    case 15:
        input->appId = 1;
        input->appParam = 1;
        return 0;
    case 16:
        input->appId = 1;
        input->appParam = 6;
        return 0;
    case 17:
        input->appId = 1;
        input->appParam = 2;
        return 0;
    case 18:
        input->appId = 1;
        input->appParam = 3;
        return 0;
    case 19:
        input->appId = 1;
        input->appParam = 4;
        return 0;
    case 20:
        input->appId = 1;
        input->appParam = 5;
        return 0;
    case 21:
        input->appId = 3;
        input->appParam = 1;
        return 0;
    case 22:
        input->appId = 3;
        input->appParam = 2;
        return 0;
    case 23:
        input->appId = 3;
        input->appParam = 3;
        return 0;
    case 24:
        input->appId = 5;
        return 0;
    default:
        input->appId = 1;
        return 0;
    }
}

BOOL ShortcutMenu_GetActionFromKeyItem(s32 item, u32 *action, u32 *invalid) {
    *invalid = 0;
    switch (item) {
    case 0:
        *action = 0;
        return TRUE;
    case 1:
        *action = 1;
        return TRUE;
    case 2:
        *action = 6;
        return TRUE;
    case 3:
        *action = 2;
        return TRUE;
    case 4:
        *action = 5;
        return TRUE;
    case 5:
        *action = 9;
        return TRUE;
    case 6:
        return FALSE;
    default:
        *invalid = 1;
        return FALSE;
    }
}

void *func_ov021_021c4f7c(GameData *gameData, u32 arg1, u16 *arg2, HeapID arg3, HeapID arg4);
void func_ov010_021605c0(u32 arg0, ShortcutMenuWork *work) {
    GameData *gameData;
    FieldMsgBG *msgBG;
    void *param;

    if (work->app == NULL && work->done == 0) {
        GFL_BGSysSetBGEnabled(1, 0);
        msgBG = Field_GetMsgBGSys(work->field);
        if (msgBG) {
            func_ov021_02190344(msgBG);
        }
        gameData = GSYS_GetGameData(work->gameSystem);
        param = func_02012c0c(gameData);
        work->app = func_ov021_021c4f7c(gameData, arg0, param, 0x50, 0x15);
    }
}

void func_ov021_021c51a0(void *arg);
BOOL func_ov010_0216060c(ShortcutMenuWork *work) {
    if (work->done == 0) {
        switch (work->state) {
        case 0:
            if (work->app) {
                GFL_BGSysSetBGEnabled(1, 0);
                func_ov021_021c51a0(work->app);
                work->app = NULL;
                work->state = 1;
            } else {
                work->state = 2;
            }
            break;
        case 1:
            func_ov021_02190398(Field_GetMsgBGSys(work->field));
            work->state = 2;
            break;
        case 2:
            work->state = 0;
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov021_021c5334(void *app);
BOOL func_ov010_02160668(FieldAppCallInput *input, void *arg) {
    ShortcutMenuWork *work = arg;
    GameData *gameData;
    SaveControl *save;
    ShortcutSave *shortcut;

    gameData = GSYS_GetGameData(work->gameSystem);
    save = GameData_GetSaveControl(gameData);
    shortcut = SaveControl_GetShortcutSave(save);
    switch (work->state) {
    case 0:
        if (ShortcutSave_GetShortcutCount(shortcut) == 0) {
            work->done = 1;
            work->state = 2;
        } else if (input->eventType - 1 <= 2) {
            work->state = 2;
        } else {
            func_ov010_021605c0(1, work);
            work->state = 1;
        }
        break;
    case 1:
        if (func_ov021_021c5334(work->app)) {
            work->state = 2;
        }
        break;
    case 2:
        work->state = 0;
        return TRUE;
    }
    return FALSE;
}




BOOL func_ov010_021606d0(FieldAppCallInput *input, void *arg) {
    return func_ov010_0216060c(arg);
}

GameEvent *func_ov010_021606dc(GameSystem *gsys, u16 code) {
    GameEvent *event;
    ScriptWork *work;

    event = EventScriptCall_Create(gsys, 0x7d6, NULL, 0x15);
    work = EventScriptCall_GetWork(event);
    ScriptWork_SetParams(work, code, 0, 0, 0);
    return event;
}


BOOL CheckAnyRibbon(ShortcutMenuWork *work) {
    GameData *gameData = GSYS_GetGameData(work->gameSystem);
    PokeParty *party = GameData_GetParty(gameData);
    int i;

    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);

        if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL) != 0 && PokeParty_CheckAnyRibbon(pkm) != 0) {
            return TRUE;
        }
    }
    return FALSE;
}
