// Black's code at ov018 0x0217f2b4-0x0217fb5c: the message windows of the Entralink's events and their script commands. No
// name is known: the module and address.

#include "types.h"
#include "field/field_script.h"
#include "field/intrude_field.h"
#include "field/fld_vreq.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "struct_decls.h"
#include "system/bmp_oam.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "system/game_comm.h"
#include "field/intrude_comm.h"
#include "system/game_data.h"
#include "field/event_mapchange.h"
#include "field/ov018_0217da54.h"
#include "gfl/tcb.h"
#include "gfl/wm_icon.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/sound.h"
#include "field/intrude_work.h"
#include "gfl/net.h"
#include "field/beacon_view.h"
#include "field/field.h"
#include "gfl/bmp.h"
#include "gfl/key.h"
#include "save/save_control.h"
#include "nitro/math.h"
#include "field/ov018_0217c458.h"
#include "gfl/touchpanel.h"
#include "field/subscreen.h"
#include "system/palanm.h"
#include "field/field_palace.h"
#include "field/field_msgbg.h"

void func_ov018_0217f390(Ov018MsgWork *work);
void func_ov018_0217f45c(Ov018MsgWork *work);
void func_ov018_0217f36c(Ov018MsgWork *work);
void func_ov018_0217f3e4(Ov018MsgWork *work);
void func_ov018_0217f470(Ov018MsgWork *work, MsgData *msgData, MsgBGSysWin *window, u32 msgId, u32 x, u32 y);
GameEvent *func_ov018_0217f67c(GameSystem *param_1, u16 *param_2);

// What the event of the Entralink's conversation keeps: its message data, window and word set, and where its result goes
struct Ov018TalkWork {
    MsgData *msgData;
    MsgBGSysWin *window;
    WordSet *wordSet;
    u16 *result;
};

u32 func_ov018_0217f6a0(GameEvent *event, s32 *state, struct Ov018TalkWork *talk);
GameEvent *func_ov018_0217f938(GameSystem *param_1);
u32 func_ov018_0217f95c(GameEvent *event, s32 *state, Ov018MsgWork *work);
u32 func_ov018_0217fa38(GameEvent *event, s32 *state);
u32 func_ov018_0217f5a0(IntrudeMissionWork *work);
void func_ov018_0217fa1c(Ov018MsgWork *work);

void func_ov018_0217f2b4(Ov018MsgWork *work, GameSystem *gsys) {
    Field *field;
    FieldMsgBG *msgBGSys;

    field = GSYS_GetField(gsys);
    msgBGSys = Field_GetMsgBGSys(field);
    if (work->wordSet == NULL) {
        work->wordSet = GFL_WordSetSystemCreate(6, 0x20, 4);
        work->strBuf = GFL_StrBufCreate(0x1f0, 4);
        work->tmpBuf = GFL_StrBufCreate(0x1d0, 4);
        work->msgData = func_ov021_021904b0(msgBGSys, 0x44);
        work->unk_10 = func_ov021_021904b0(msgBGSys, 0x42);
        work->unk_14 = func_ov021_021904b0(msgBGSys, 0x43);
        work->msgBGSys = msgBGSys;
        work->unk_28 = 0;
    }
}

void func_ov018_0217f318(Ov018MsgWork *work) {
    func_ov018_0217f390(work);
    func_ov018_0217f58c(work);
    func_ov018_0217f45c(work);
    if (work->wordSet != NULL) {
        if (work->talkWin != NULL) {
            func_ov021_02190e40(work->talkWin);
            work->talkWin = NULL;
        }
        func_ov021_021904c8(work->msgData);
        func_ov021_021904c8(work->unk_10);
        func_ov021_021904c8(work->unk_14);
        GFL_StrBufFree(work->tmpBuf);
        GFL_StrBufFree(work->strBuf);
        GFL_WordSetSystemFree(work->wordSet);
        work->wordSet = NULL;
    }
}

void func_ov018_0217f36c(Ov018MsgWork *work) {
    if (work->talkWin != NULL) {
        if (work->unk_28 == 1) {
            func_ov018_0217f3e4(work);
        }
    } else {
        work->talkWin = func_ov021_02190f64(work->msgBGSys, work->msgData);
    }
}

void func_ov018_0217f390(Ov018MsgWork *work) {
    if (work->talkWin != NULL) {
        func_ov021_02190e40(work->talkWin);
        work->talkWin = NULL;
    }
}

void func_ov018_0217f3a4(Ov018MsgWork *work, StrBuf *strBuf) {
    func_ov018_0217f36c(work);
    func_ov021_02190eb8(work->talkWin, 0, 0, strBuf);
    work->unk_28 = 1;
}

void func_ov018_0217f3c4(Ov018MsgWork *work, u16 msgId) {
    GFL_MsgDataLoadStrbuf(work->msgData, msgId, work->tmpBuf);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, work->tmpBuf);
    func_ov018_0217f3a4(work, work->strBuf);
}

void func_ov018_0217f3e4(Ov018MsgWork *work) {
    if (work->talkWin == NULL) {
        GFL_DebugAssertFail("", 0, "");
    }
    func_ov021_02190f7c(work->talkWin);
    work->unk_28 = 0;
}

BOOL func_ov018_0217f40c(Ov018MsgWork *work) {
    if (work->talkWin == NULL) {
        return 1;
    }
    return func_ov021_02190ef0(work->talkWin);
}

u32 func_ov018_0217f420(void) {
    if ((GCTX_HIDGetPressedKeys() & 0xf3) != 0) {
        return 1;
    }
    return 0;
}

void func_ov018_0217f434(Ov018MsgWork *work) {
    if (work->unk_1c == NULL) {
        work->unk_1c = FieldMsgBG_CreateMoneyWin(work->msgBGSys, work->msgData, 1, 0x13, 0x1e, 4);
    }
}

void func_ov018_0217f45c(Ov018MsgWork *work) {
    if (work->unk_1c != NULL) {
        func_ov021_02190728(work->unk_1c);
        work->unk_1c = NULL;
    }
}

void func_ov018_0217f470(Ov018MsgWork *work, MsgData *msgData, MsgBGSysWin *window, u32 msgId, u32 x, u32 y) {
    GFL_MsgDataLoadStrbuf(msgData, msgId, work->tmpBuf);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, work->tmpBuf);
    func_ov021_02190758(window, x, y, work->strBuf);
}


void func_ov018_0217f4a0(Ov018MsgWork *work, u32 msgId, u16 x, u16 y) {
    func_ov018_0217f434(work);
    func_ov018_0217f470(work, work->msgData, work->unk_1c, msgId, x, y);
}

void func_ov018_0217f4c4(Ov018MsgWork *work) {
    if (work->menu != NULL) {
        func_ov021_021909bc(work->menu);
        work->menu = NULL;
    }
}

void func_ov018_0217f4d8(Ov018MsgWork *work, GameSystem *gsys) {
    GSYS_GetField(gsys);
    if (work->menu != NULL || work->msgData == NULL) {
        GFL_DebugAssertFail("", 0, "");
    }
    work->menu = func_ov021_02190be4(work->msgBGSys, 0);
}

u32 func_ov018_0217f508(Ov018MsgWork *work) {
    if (work->menu == NULL) {
        GFL_DebugAssertFail("", 0, "");
    }
    return func_ov021_02190c78(work->menu);
}

void func_ov018_0217f528(Ov018MsgWork *work, GameSystem *gsys, u32 x, u32 y, u32 width, u32 height) {
    GSYS_GetField(gsys);
    GFL_ASSERT(work->unk_20 == NULL);
    work->unk_20 = FieldMsgBG_CreateMoneyWin(work->msgBGSys, work->msgData, x, y, width, height);
}


void func_ov018_0217f574(Ov018MsgWork *work, u32 msgId, u16 x, u16 y) {
    func_ov018_0217f470(work, work->unk_14, work->unk_20, msgId, x, y);
}

void func_ov018_0217f58c(Ov018MsgWork *work) {
    if (work->unk_20 != NULL) {
        func_ov021_02190728(work->unk_20);
        work->unk_20 = NULL;
    }
}

u32 func_ov018_0217f5a0(IntrudeMissionWork *work) {
    IntrudeMissionEntry *entry;

    if (func_ov011_02175da8(work) == TRUE) {
        return 4;
    }
    entry = func_ov011_02175b34(work);
    if (entry == NULL) {
        return 0;
    }
    if (entry->unk_30 == 0) {
        if (work->unk_1A5_2 == 1) {
            return 3;
        }
        return 1;
    }
    if (work->unk_1A5_2 == 1) {
        return 2;
    }
    return 1;
}

u32 func_ov018_0217f5f0(IntrudeComm *comm, u32 netId) {
    IntrudeCommUnk8C *status;

    if (netId == func_02012b58(getBasePlayerBlk(comm->commSys))) {
        status = &comm->unk_070;
    } else {
        status = &comm->unk_08C[netId];
    }
    switch (status->unk_18) {
    case 0:
        if (getTrainerGender(func_02012950(getBasePlayerBlk(comm->commSys), netId)) == 0) {
            return 3;
        }
        return 4;
    case 0xffff:
        break;
    }
    return status->unk_1A_0;
}



u32 func_ov018_0217f648(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *pGVar1;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    pGVar1 = FieldScriptEnv_GetGameSystem(param_2);
    event = func_ov018_0217f67c(pGVar1, ScriptReadVar(param_1, param_2));
    ScriptWork_CallEvent(work, event);
    return 1;
}

GameEvent *func_ov018_0217f67c(GameSystem *param_1, u16 *param_2) {
    GameEvent *event;
    struct Ov018TalkWork *talk;

    event = GameEvent_Create(param_1, (GameEvent *)0x0, (void *)func_ov018_0217f6a0, sizeof(struct Ov018TalkWork));
    talk = GameEvent_GetData(event);
    talk->result = param_2;
    *param_2 = 0;
    return event;
}

u32 func_ov018_0217f6a0(GameEvent *event, s32 *state, struct Ov018TalkWork *talk) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    GameData *gameData = GSYS_GetGameData(gsys);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    Field *field = GSYS_GetField(gsys);
    BOOL printed = FALSE;
    IntrudeComm *work = func_ov010_02158344(commSys);
    void *msgBGSys;
    StrBuf *dest;
    StrBuf *src;

    if (work == NULL && *state > 0 && *state < 5) {
        *state = 5;
    }
    switch (*state) {
    case 0:
        if (func_0200fb24() != 0 || (work == NULL && func_02012b80(gameData) != func_02012b58(gameData))) {
            *state = 7;
            break;
        }
        if (work != NULL) {
            func_ov011_021757a8(&work->unk_1AC);
            func_ov018_0217bf1c(&work->unk_914);
        }
        msgBGSys = Field_GetMsgBGSys(field);
        talk->msgData = func_ov021_021904b0(msgBGSys, 0x2d);
        talk->window = func_ov021_021907a8(msgBGSys, talk->msgData);
        talk->wordSet = GFL_WordSetSystemCreateDefault(4);
        GFL_SndSEPlay(0x547);
        if (work == NULL || func_ov010_0215841c(gameData) == func_02012b58(gameData)) {
            func_ov021_02190740(talk->window, 0, 0, 0x49);
            printed = TRUE;
        } else {
            dest = GFL_StrBufCreate(0x100, 0x15);
            src = GFL_MsgDataLoadStrbufNew(talk->msgData, 0x4a);
            copyVarForText(talk->wordSet, 0, func_ov010_021583ec(gameData, func_ov010_0215841c(gameData)));
            GFL_WordSetFormatStrbuf(talk->wordSet, dest, src);
            func_ov021_02190758(talk->window, 0, 0, dest);
            GFL_StrBufFree(src);
            GFL_StrBufFree(dest);
        }
        if (func_0203d1b4() <= 1) {
            if (printed == FALSE) {
                *talk->result = 0;
            } else {
                *talk->result = 1;
            }
            *state = 5;
            break;
        }
        *state = *state + 1;
    case 1:
        if (func_ov011_02173d2c(work, func_ov010_0215841c(gameData)) == 1) {
            *state = *state + 1;
        }
        break;
    case 2:
        if (func_ov011_02175fb4(&work->unk_1AC.players[func_ov010_0215841c(gameData)]) == 1) {
            *state = *state + 1;
        }
        break;
    case 3:
        if (func_ov011_02173c6c(work, func_ov010_0215841c(gameData)) == 1) {
            *state = *state + 1;
        }
        break;
    case 4:
        if (func_ov018_0217bf28(&work->unk_914) == 1) {
            *talk->result = 1;
            *state = 5;
        }
        break;
    case 5:
        if (func_ov021_0219077c(talk->window) == 1) {
            *state = *state + 1;
        }
        break;
    case 6:
        if ((GCTX_HIDGetPressedKeys() & 0xf3) != 0) {
            GFL_WordSetSystemFree(talk->wordSet);
            func_ov021_02190728(talk->window);
            GFL_MsgDataFree(talk->msgData);
            *state = *state + 1;
        }
        break;
    case 7:
        return 1;
    default:
        break;
    }
    return 0;
}


BOOL func_ov018_0217f8a0(VM *vm, FieldScriptEnv *env) {
    u16 *var = ScriptReadVar(vm, env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    IntrudeComm *intrude = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    u16 value;
    if (intrude == NULL || (value = intrude->unk_070.unk_18) == 0) {
        *var = 0;
    } else {
        *var = value;
    }
    return FALSE;
}

BOOL func_ov018_0217f8d0(VM *vm, FieldScriptEnv *env) {
    u16 *var = ScriptReadVar(vm, env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    IntrudeComm *intrude = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    u16 value;
    if (!(intrude == NULL || intrude->unk_93F_3 != 0 || func_ov011_02175b8c(&intrude->unk_1AC) != TRUE)) {
        value = 1;
    } else {
        value = 0;
    }
    *var = value;
    return FALSE;
}



u32 func_ov018_0217f914(u32 param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *pGVar1;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    pGVar1 = FieldScriptEnv_GetGameSystem(param_2);
    event = func_ov018_0217f938(pGVar1);
    ScriptWork_CallEvent(work, event);
    return 1;
}

GameEvent *func_ov018_0217f938(GameSystem *param_1) {
    GameEvent *event;
    void *dest;

    event = GameEvent_Create(param_1, (GameEvent *)0x0, (void *)func_ov018_0217f95c, sizeof(Ov018MsgWork));
    dest = GameEvent_GetData(event);
    MI_CpuFill8(dest, 0, sizeof(Ov018MsgWork));
    return event;
}

u32 func_ov018_0217f95c(GameEvent *event, s32 *state, Ov018MsgWork *work) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    IntrudeComm *comm = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    Field *field;
    GameEvent *next;

    if (comm == NULL && *state < 3) {
        *state = 3;
    }
    switch (*state) {
    case 0:
        comm->unk_93F_3 = 1;
        func_ov018_0217f2b4(work, gsys);
        if (func_ov018_0217f5a0(&comm->unk_1AC) == 3) {
            *state = 2;
        } else {
            func_ov018_0217f4a0(work, 0x1a, 0, 0);
            *state = *state + 1;
        }
        break;
    case 1:
        switch (func_ov018_0217f5a0(&comm->unk_1AC)) {
        case 2:
            break;
        case 3:
        default:
            *state = *state + 1;
            break;
        }
        break;
    case 2:
        field = GSYS_GetField(gsys);
        next = func_ov018_0217c458(gsys, field, comm, 0x15);
        func_ov018_0217fa1c(work);
        GameEvent_Replace(event, next);
        return 0;
    case 3:
        func_ov018_0217fa1c(work);
        return 1;
    default:
        break;
    }
    return 0;
}


void func_ov018_0217fa1c(Ov018MsgWork *work) {
    func_ov018_0217f318(work);
}


GameEvent *func_ov018_0217fa24(GameSystem *gsys) {
    GameEvent *pGVar1;

    pGVar1 = GameEvent_Create(gsys, (GameEvent *)0x0, (void *)func_ov018_0217fa38, 0);
    return pGVar1;
}

u32 func_ov018_0217fa38(GameEvent *event, s32 *state) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    GameData *gameData = GSYS_GetGameData(gsys);
    Field *field = GSYS_GetField(gsys);
    IntrudeComm *work = func_ov010_02158344(commSys);
    GameEvent *next;

    switch (*state) {
    case 0:
        if (work == NULL) {
            func_02012b58(gameData);
        } else {
            func_ov011_02176230(work, &work->unk_1AC);
        }
        next = func_ov018_0217ce08(gsys, field, 0x117, 1);
        if (next == NULL) {
            return 1;
        }
        GameEvent_ChainNext(event, next);
        *state = *state + 1;
        break;
    case 1:
        if (IsZoneEntralinkHub(PlayerState_GetZoneID(GSYS_GetPlayerState(gsys))) == TRUE) {
            next = func_ov018_0217f938(gsys);
            GameEvent_ChainNext(event, next);
        }
        *state = *state + 1;
        break;
    case 2:
        return 1;
    default:
        break;
    }
    return 0;
}


BOOL func_ov018_0217fae0(VM *vm, FieldScriptEnv *env) {
    u16 *var = ScriptReadVar(vm, env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData;
    s32 diff;

    GSYS_GetGameCommSystem(gsys);
    gameData = GSYS_GetGameData(gsys);
    if (func_ov010_0215841c(gameData) == func_02012b58(gameData)) {
        *var = 0;
    } else {
        diff = func_ov010_0215841c(gameData) - func_02012b58(gameData);
        if (diff < 0) {
            diff += 3;
        }
        *var = diff;
    }
    return FALSE;
}


u32 func_ov018_0217fb2c(VM *param_1, FieldScriptEnv *param_2) {
    u16 *ptr;
    GameSystem *gsys;
    GameData *gameData;
    SaveControl *save;
    HighLinkSave *highLink;

    ptr = ScriptReadVar(param_1, param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    gameData = GSYS_GetGameData(gsys);
    save = GameData_GetSaveControl(gameData);
    highLink = getHighLinkBlockAddress(save);
    *ptr = (u16)(func_0200bf10(highLink) != 0x30);
    return 0;
}
