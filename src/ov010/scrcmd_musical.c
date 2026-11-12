// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_musical.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "app/ircbattlematch.h"
#include "constants/version.h"
#include "field/event_poke_status.h"
#include "field/field.h"
#include "field/field_script.h"
#include "field/musical.h"
#include "field/musical_dressup_sys.h"
#include "field/pdw_postman.h"
#include "field/scrcmd_musical.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "pml/poke_party.h"
#include "save/dream_world.h"
#include "save/musical_savedata.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "system/wordset.h"
#include "app/musical/musical_shot_sys.h"
#include "app/musical/musical_system.h"
#include "field/mus_comm_func.h"
#include "field/union_oneself.h"

// The event that runs a musical
typedef struct {
    GameEvent *event;
    MusicalCommWork *work;
    BOOL online;
} MusicalCallEvent;

typedef struct {
    MusicalShotParam *param;
} MusicalShotHolder;

typedef struct {
    MusicalDressUpParam *param;
} MusicalDressUpEvent;

typedef struct {
    FieldScriptEnv *env;
    MusicalCommWork *work;
} MusicalIrcMatchHolder;

typedef struct {
    u16 *a0;
    u16 *a1;
    u32 unk8;
} MusicalOv020Args;

void func_ov010_0215de38(MusicalCommWork *work, GameSystem *gsys, GameCommSys *comm, u16 value);
void func_ov010_0215de44(MusicalCommWork *work);
GameEventReturnCode func_ov010_0215de58(GameEvent *event, u32 *state, void *arg);
GameEventReturnCode func_ov010_0215def0(GameEvent *event, u32 *state, void *data);
void func_ov010_0215df6c(void *arg);
void func_ov010_0215ddc8(void *unused, GameData *gameData);
void func_ov010_0215de0c(void *ptr, GameData *gameData);
extern const GameProcFunctions data_ov100_021bb6a4;
void func_ov010_0215df78(void *arg);
BOOL func_ov010_0215dfb4(VM *vm, void *arg);
BOOL func_ov010_0215e010(VM *vm, void *arg);
BOOL func_ov010_0215e068(VM *vm, void *arg);
BOOL func_ov010_0215e0b0(VM *vm, void *arg);
BOOL func_ov010_0215e0ec(VM *vm, void *arg);
BOOL func_ov010_0215e130(VM *vm, void *arg);
BOOL func_ov010_0215e184(VM *vm, void *arg);


BOOL func_ov010_0215d34c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    Field *field = GSYS_GetField(gsys);
    u16 arg;
    u8 kind;
    MusicalCommWork *commWork;
    BOOL online;
    GameEvent *event;
    MusicalCallEvent *data;
    MusicalSave *save;
    MusicalShotHolder *holder;

    ScriptWork_GetFieldWork(work);
    arg = ScriptReadAny(vm, env);
    kind = VM_Read8(vm);
    if (kind <= 1) {
        commWork = func_02012f80(gameData);
        if (kind == 0) {
            online = FALSE;
        } else {
            online = TRUE;
        }
        event = GameEvent_Create(gsys, NULL, func_ov010_0215de58, sizeof(MusicalCallEvent));
        data = GameEvent_GetData(event);
        MI_CpuFill8(data, 0, sizeof(MusicalCallEvent));
        data->event = func_ov010_02155a98(gsys, gameData, arg, online, commWork);
        data->work = commWork;
        data->online = online;
        ScriptWork_SetPostEvent(work, event);
        GameData_GetFieldSoundSystem(gameData);
        VM_Halt(vm);
        return TRUE;
    }
    save = getMusicalInfoBlkAddress(gameData);
    holder = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(MusicalShotHolder), TRUE, "scrcmd_musical.c", 0xa9);
    holder->param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(MusicalShotParam), TRUE, "scrcmd_musical.c", 0xaa);
    holder->param->askSave = FALSE;
    holder->param->shot = func_0200a640(save);
    holder->param->loadComm = TRUE;
    ScriptWork_CallEvent(work, func_02014808(gsys, field, OVERLAY_ID(112), &data_ov112_021f8360, holder->param, func_ov010_0215df6c, holder));
    return TRUE;
}

BOOL func_ov010_0215d468(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    u16 slot;
    MusicalSave *save;
    MusicalPoke *poke;
    GameEvent *event;
    MusicalDressUpEvent *data;

    GSYS_GetField(gsys);
    slot = ScriptReadAny(vm, env);
    save = getMusicalInfoBlkAddress(gameData);
    poke = func_ov112_021f675c(PokeParty_GetPkm(GameData_GetParty(gameData), slot), HEAPID_GAMEEVENT);
    event = GameEvent_Create(gsys, NULL, func_ov010_0215def0, sizeof(MusicalDressUpEvent));
    data = GameEvent_GetData(event);
    MI_CpuFill8(data, 0, sizeof(MusicalDressUpEvent));
    data->param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(MusicalDressUpParam), TRUE, "scrcmd_musical.c", 215);
    data->param->poke = poke;
    data->param->save = save;
    data->param->comm = NULL;
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov010_0215d504(VM *vm, FieldScriptEnv *env) {
    GameData *gameData;
    u32 mostIndex;
    u32 fewestIndex;
    u32 firstIndex;
    u8 topIndex;
    u8 secondValue;
    u32 second;
    PokeParty *party;
    u8 kind;
    u16 arg;
    u16 *result;
    MusicalSave *save;
    MusicalEventWork *event;
    MusicalCommWork *commWork;
    u8 i;
    u8 j;
    u8 most;
    u8 count;
    u8 value;
    u8 best;
    MusicalSaveUnk1B0 *entries;

    FieldScriptEnv_GetScriptWork(env);
    kind = VM_Read8(vm);
    arg = ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    gameData = FieldScriptEnv_GetGameData(env);
    save = getMusicalInfoBlkAddress(gameData);
    event = NULL;
    if (kind >= 30) {
        commWork = func_02012f80(gameData);
        if (commWork == NULL) {
            *result = 0;
            return FALSE;
        }
        event = commWork->event;
    }
    switch (kind) {
    case 0:
        *result = func_0200a75c(save);
        break;
    case 1:
        *result = func_0200a780(save);
        break;
    case 2:
        *result = func_0200a7b8(save);
        break;
    case 3:
        best = 0;
        mostIndex = 4;
        for (i = 0; i < 4; i++) {
            value = func_0200a7a0(save, i);
            if (value > best) {
                best = value;
                mostIndex = i;
            } else if (value == best) {
                mostIndex = 4;
            }
        }
        *result = mostIndex;
        break;
    case 4:
        *result = func_0200a7c8(save);
        break;
    case 5:
        func_0200a7d4(save, arg);
        break;
    case 6:
        *result = 0;
        for (i = 0; i < 4; i++) {
            if (func_0200a7a0(save, i) != 0) {
                *result = 1;
            }
        }
        break;
    case 7:
        if (func_0200a640(save)->month == 0) {
            *result = FALSE;
        } else {
            *result = TRUE;
        }
        break;
    case 8:
        best = 0xff;
        fewestIndex = 4;
        for (i = 0; i < 4; i++) {
            value = func_0200a7a0(save, i);
            if (value <= best) {
                best = value;
                fewestIndex = i;
            }
        }
        *result = fewestIndex;
        break;
    case 9:
        best = 0;
        firstIndex = 4;
        for (i = 0; i < 4; i++) {
            value = func_0200a7a0(save, i);
            if (value > best) {
                best = value;
                firstIndex = i;
            }
        }
        *result = firstIndex;
        break;
    case 10:
        topIndex = 4;
        secondValue = 0;
        most = 0;
        second = 4;
        for (j = 0; j < 4; j++) {
            value = func_0200a7a0(save, j);
            if (value > most) {
                if (secondValue == most) {
                    second = 4;
                } else {
                    secondValue = most;
                    second = topIndex;
                }
                most = value;
                topIndex = j;
            }
        }
        *result = second;
        break;
    case 11:
        count = 0;
        party = GameData_GetParty(gameData);
        value = PokeParty_GetPkmCount(party);
        for (i = 0; i < value; i++) {
            if (func_ov112_021f6744(PokeParty_GetPkm(party, i)) == TRUE) {
                count++;
            }
        }
        *result = count;
        break;
    case 12:
        if (func_ov112_021f6744(PokeParty_GetPkm(GameData_GetParty(gameData), arg)) == TRUE) {
            *result = TRUE;
        } else {
            *result = FALSE;
        }
        break;
    case 13:
        entries = func_0200a628(save);
        *result = 0;
        for (i = 0; i < 8; i++) {
            if (entries[i].unk0 != 10) {
                *result = 1;
            }
        }
        break;
    case 14:
        if (func_0200a7e0(save) == TRUE) {
            *result = TRUE;
        } else {
            *result = FALSE;
        }
        break;
    case 15:
        if (func_0200a644(save, arg) == TRUE) {
            *result = TRUE;
        } else {
            *result = FALSE;
        }
        break;
    case 30:
        *result = func_ov010_021568e0(event);
        break;
    case 31:
        *result = func_ov010_02156954(event);
        break;
    case 32:
        *result = func_ov010_02156978(event);
        break;
    case 33:
        *result = func_ov010_02156934(event, arg);
        break;
    case 35:
        *result = func_ov010_021568e8(event, arg);
        break;
    case 36:
        *result = func_ov010_02156908(event);
        break;
    case 37:
        *result = func_ov010_0215699c(event, arg);
        break;
    case 38:
        *result = func_ov010_021569a4(event, arg);
        break;
    case 39:
        *result = func_ov010_021569ec(event, arg);
        break;
    }
    return FALSE;
}

BOOL func_ov010_0215d7d4(VM *vm, FieldScriptEnv *env) {
    u16 arg = ScriptReadAny(vm, env);
    u8 kind = VM_Read8(vm);
    u16 *result = ScriptReadVar(vm, env);
    MusicalSaveUnk1E0 *entry = func_0200a750(getMusicalInfoBlkAddress(FieldScriptEnv_GetGameData(env)), arg);

    switch (kind) {
    case 0:
        switch (entry->unk0) {
        case 0:
            *result = 24;
            break;
        case 1:
            *result = 15;
            break;
        case 2:
            *result = 45;
            break;
        case 3:
            *result = 12;
            break;
        case 4:
            *result = 0;
            break;
        }
        break;
    case 1:
        *result = arg + 5;
        break;
    case 2:
        *result = arg + 15;
        break;
    case 3:
        *result = entry->unk1;
        break;
    case 4:
        *result = entry->unk2;
        break;
    }
    return FALSE;
}

BOOL func_ov010_0215d864(VM *vm, FieldScriptEnv *env) {
    u16 prop = ScriptReadAny(vm, env);

    func_0200a6bc(getMusicalInfoBlkAddress(FieldScriptEnv_GetGameData(env)), prop);
    return FALSE;
}

BOOL s005D_WordSetMusicalInfo(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    u8 kind = VM_Read8(vm);
    u8 wordIndex = VM_Read8(vm);
    u16 arg = ScriptReadAny(vm, env);
    MusicalSave *save;
    MusicalEventWork *event;
    MusicalCommWork *commWork;
    MsgData *msgData;
    MusicalSave *musicalInfoBlkAddress;
    StrBuf *str;
    u8 i;
    u8 j;
    u8 next;
    u8 tmp;
    u16 messages[3];
    u8 order[4];
    u8 points[4];

    switch (kind) {
    case 0:
        save = getMusicalInfoBlkAddress(FieldScriptEnv_GetGameData(env));
        str = GFL_StrBufCreate(0x26, heapId);
        GFL_StrBufLoadFixedString(str, func_0200a7f8(save), 0x26);
        func_0201ee0c(wordSet, wordIndex, str, 0, 1, 2);
        GFL_StrBufFree(str);
        break;
    case 1:
    case 8:
        if (kind == 1) {
            msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_13, heapId);
        } else {
            msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0484, heapId);
        }
        str = GFL_MsgDataLoadStrbufNew(msgData, arg + 3);
        func_0201ee0c(wordSet, wordIndex, str, 0, 1, 2);
        GFL_StrBufFree(str);
        GFL_MsgDataFree(msgData);
        break;
    case 2:
        commWork = func_02012f80(gameData);
        event = commWork->event;
        str = func_ov010_02156914(event, heapId);
        func_0201ee0c(wordSet, wordIndex, str, 0, 1, 2);
        GFL_StrBufFree(str);
        break;
    case 3:
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_S005D_WORD_SET_MUSICAL_INFO, heapId);
        str = GFL_MsgDataLoadStrbufNew(msgData, arg);
        func_0201ee0c(wordSet, wordIndex, str, 0, 1, 2);
        GFL_StrBufFree(str);
        GFL_MsgDataFree(msgData);
        break;
    case 4:
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_S005D_WORD_SET_MUSICAL_INFO, heapId);
        str = GFL_MsgDataLoadStrbufNew(msgData, arg + 4);
        func_0201ee0c(wordSet, wordIndex, str, 0, 1, 2);
        GFL_StrBufFree(str);
        GFL_MsgDataFree(msgData);
        break;
    case 5:
        commWork = func_02012f80(gameData);
        event = commWork->event;
        func_ov010_02156a3c(event, arg, wordSet, wordIndex);
        break;
    case 6:
        commWork = func_02012f80(gameData);
        event = commWork->event;
        func_ov010_02156ad4(event, arg, wordSet, wordIndex);
        break;
    case 7:
        musicalInfoBlkAddress = getMusicalInfoBlkAddress(FieldScriptEnv_GetGameData(env));
        save = musicalInfoBlkAddress;
        for (i = 0; i < 4; i++) {
            order[i] = i;
            points[i] = func_0200a7a0(save, i);
        }
        for (i = 1; i < 4; i++) {
            for (j = 0; j < 4 - i; j++) {
                next = j + 1;
                if (points[j] < points[next]) {
                    tmp = points[j];
                    points[j] = points[next];
                    points[next] = tmp;
                    tmp = order[j];
                    order[j] = order[next];
                    order[next] = tmp;
                }
            }
        }
        if (points[0] == points[1] && points[0] == points[2]) {
            messages[0] = 9;
            messages[1] = 8;
        } else {
            messages[0] = order[0] + 4;
            if (points[1] == points[2]) {
                messages[1] = 8;
            } else {
                messages[1] = order[1] + 4;
            }
        }
        if (points[1] == points[3]) {
            messages[2] = order[1] + 4;
        } else if (points[2] == points[3]) {
            messages[2] = order[2] + 4;
        } else {
            messages[2] = order[3] + 4;
        }
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_S005D_WORD_SET_MUSICAL_INFO, heapId);
        for (i = 0; i < 3; i++) {
            str = GFL_MsgDataLoadStrbufNew(msgData, messages[i]);
            func_0201ee0c(wordSet, i, str, 0, 1, 2);
            GFL_StrBufFree(str);
        }
        GFL_MsgDataFree(msgData);
        break;
    }
    return FALSE;
}


BOOL func_ov010_0215dadc(VM *vm, FieldScriptEnv *env) {
    u16 index = ScriptReadAny(vm, env);

    func_0200a750(getMusicalInfoBlkAddress(FieldScriptEnv_GetGameData(env)), index)->unk1 = 0;
    return FALSE;
}

BOOL func_ov010_0215db00(VM *vm, FieldScriptEnv *env) {
    u16 *a0 = ScriptReadVar(vm, env);
    u16 *a1 = ScriptReadVar(vm, env);
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameEvent *event = func_ov010_0215a168(FieldScriptEnv_GetGameSystem(env), a0, a1);
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL func_ov010_0215db38(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    Field *field = GSYS_GetField(gsys);
    int kind = ScriptReadAny(vm, env);
    u16 value = ScriptReadAny(vm, env);
    MusicalCommWork *commWork;
    MusicalIrcMatchHolder *holder;
    GameCommSys *comm;
    PlayerInfo *info;
    GameEvent *event;
    BOOL flag;
    u16 *result;

    ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    commWork = NULL;
    flag = FALSE;
    if (kind != 0 && kind != 21) {
        commWork = func_02012f80(gameData);
        if (commWork->event != NULL) {
            func_ov010_02156bac(commWork->event);
            if (func_ov010_02156bcc(commWork->event) == 1) {
                flag = TRUE;
            }
        }
    }
    switch (kind) {
    case 0:
        func_ov010_0215ddc8(work, gameData);
        break;
    case 1:
        func_ov010_0215de0c(commWork, gameData);
        break;
    case 10:
        comm = GSYS_GetGameCommSystem(gsys);
        func_ov010_0215de38(commWork, gsys, comm, value);
        break;
    case 11:
        func_ov010_0215de44(commWork);
        VM_SetNativeCallback(vm, func_ov010_0215e130);
        return TRUE;
    case 12:
        commWork->result = result;
        info = GetGameDataPlayerInfo(gameData);
        commWork->menu = func_ov021_021cd48c(info, field, 2, 4, 4, 0, 1, 0);
        VM_SetNativeCallback(vm, func_ov010_0215dfb4);
        return TRUE;
    case 13:
        commWork->result = result;
        info = GetGameDataPlayerInfo(gameData);
        commWork->menu = func_ov021_021cd48c(info, field, 2, 4, 4, 1, 1, 0);
        VM_SetNativeCallback(vm, func_ov010_0215e010);
        return TRUE;
    case 14:
        if (commWork->comm != NULL && !flag) {
            commWork->value = value;
            func_ov132_021fcdec(commWork->comm, (u8)commWork->value);
            VM_SetNativeCallback(vm, func_ov010_0215e068);
            return TRUE;
        }
        break;
    case 15:
        if (commWork->comm != NULL && !flag) {
            VM_SetNativeCallback(vm, func_ov010_0215e0b0);
            return TRUE;
        }
        break;
    case 16:
        VM_SetNativeCallback(vm, func_ov010_0215e0ec);
        return TRUE;
    case 17:
        if (commWork->comm != NULL && !flag) {
            VM_SetNativeCallback(vm, func_ov010_0215e184);
            return TRUE;
        }
        break;
    case 18:
        holder = GFL_HeapAllocate(0x8004, sizeof(MusicalIrcMatchHolder), TRUE, "scrcmd_musical.c", 0x3c8);
        holder->env = env;
        holder->work = commWork;
        commWork->result = result;
        commWork->ircMatch.gameData = gameData;
        commWork->ircMatch.musicalNetInit = func_ov132_021fda6c(gameData);
        if (value == 0) {
            commWork->ircMatch.result = 9;
        } else {
            commWork->ircMatch.result = 10;
        }
        event = func_02014808(gsys, field, OVERLAY_ID(100), &data_ov100_021bb6a4, &commWork->ircMatch, func_ov010_0215df78, holder);
        ScriptWork_CallEvent(work, event);
        return TRUE;
    case 19:
        if (commWork->comm != NULL && flag == TRUE) {
            *result = 1;
        } else {
            *result = 0;
        }
        break;
    case 20:
        if (commWork->comm != NULL) {
            *result = 1;
        } else {
            *result = 0;
        }
        break;
    case 21:
        if (func_0200fb24()) {
            func_02012124(gsys, 1);
            *result = 1;
        } else {
            *result = 0;
        }
        break;
    case 100:
        break;
    }
    return FALSE;
}


typedef struct {
    void *unk00;
    void *unk04;
    u32 unk08[12];
} MusicalOv010Alloc;

void func_ov010_0215ddc8(void *unused, GameData *gameData) {
    MusicalOv010Alloc *p = GFL_HeapAllocate(4, sizeof(MusicalOv010Alloc), TRUE, "scrcmd_musical.c", 0x413);
    p->unk00 = NULL;
    p->unk04 = NULL;
    func_02012f74(gameData, (MusicalCommWork *)p);
    GFL_OvlLoad(OVERLAY_ID(132));
    GFL_OvlLoad(OVERLAY_ID(112));
}


void func_ov010_0215de0c(void *ptr, GameData *gameData) {
    GFL_OvlUnload(OVERLAY_ID(112));
    GFL_OvlUnload(OVERLAY_ID(132));
    GFL_HeapFree(ptr);
    func_02012f74(gameData, 0);
}


void func_ov010_0215de38(MusicalCommWork *work, GameSystem *gsys, GameCommSys *comm, u16 value) {
    func_ov132_021fc644(HEAPID_GAMEEVENT, gsys, comm, value);
}

void func_ov010_0215de44(MusicalCommWork *work) {
    if (work->comm != NULL) {
        func_ov132_021fc684(work->comm);
        work->comm = NULL;
    }
}

GameEventReturnCode func_ov010_0215de58(GameEvent *event, u32 *state, void *arg) {
    MusicalCallEvent *data = arg;
    GameData *gameData = GSYS_GetGameData(GameEvent_GetGameSystem(event));
    GameCommSys *comm;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, data->event);
        (*state)++;
        break;
    case 1:
        GameData_GetFieldSoundSystem(gameData);
        func_ov010_0215de44(data->work);
        (*state)++;
        break;
    case 2:
        comm = GSYS_GetGameCommSystem(GameEvent_GetGameSystem(event));
        if (data->online && GameCommSys_BootCheck(comm)) {
            break;
        }
        (*state)++;
        break;
    case 3:
        if (func_0203cdbc() == TRUE || !data->online) {
            if (data->online == TRUE) {
                func_020121cc(GameEvent_GetGameSystem(event));
            }
            func_ov010_0215de0c(data->work, gameData);
            (*state)++;
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov010_0215def0(GameEvent *event, u32 *state, void *arg) {
    MusicalDressUpEvent *data = arg;
    GameSystem *gsys = GameEvent_GetGameSystem(event);

    GSYS_GetGameData(gsys);
    GSYS_GetField(gsys);
    switch (*state) {
    case 0:
        (*state)++;
        break;
    case 1:
        (*state)++;
        break;
    case 2:
        (*state)++;
        break;
    case 3:
        GSYS_QueueProc(gsys, OVERLAY_NONE, &data_ov010_02172724, data->param);
        (*state)++;
        break;
    case 4:
        if (GSYS_GetProcMgrState(gsys)) {
            break;
        }
        (*state)++;
        break;
    case 5:
        (*state)++;
        break;
    case 6:
        GFL_HeapFree(data->param->poke);
        GFL_HeapFree(data->param);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

void func_ov010_0215df6c(void *arg) {
    MusicalShotHolder *holder = arg;

    GFL_HeapFree(holder->param);
}

void func_ov010_0215df78(void *arg) {
    MusicalIrcMatchHolder *holder = arg;
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(holder->env);

    FieldScriptEnv_GetGameData(holder->env);
    GSYS_GetGameCommSystem(gsys);
    if (holder->work->ircMatch.result != IRCBATTLEMATCH_RESULT_QUIT
        && holder->work->ircMatch.result != IRCBATTLEMATCH_RESULT_BACK) {
        *holder->work->result = 1;
        func_ov132_021fc81c(holder->work->comm);
    } else {
        *holder->work->result = 0;
    }
}


BOOL func_ov010_0215dfb4(VM *vm, void *arg) {
    FieldScriptEnv *env = arg;
    MusicalCommWork *commWork;
    BOOL done;
    u32 choice;

    FieldScriptEnv_GetScriptWork(env);
    commWork = func_02012f80(FieldScriptEnv_GetGameData(env));
    choice = func_ov021_021cd688(commWork->menu);
    done = FALSE;
    switch (choice) {
    case 1:
        *commWork->result = 0;
        func_ov132_021fc804(commWork->comm);
        done = TRUE;
        break;
    case 2:
        *commWork->result = 1;
        done = TRUE;
        break;
    case 4:
        *commWork->result = 2;
        done = TRUE;
        break;
    }
    if (done == TRUE) {
        func_ov021_021cd5a4(commWork->menu);
        commWork->menu = NULL;
    }
    return done;
}

BOOL func_ov010_0215e010(VM *vm, void *arg) {
    FieldScriptEnv *env = arg;
    MusicalCommWork *commWork;
    BOOL done;

    FieldScriptEnv_GetScriptWork(env);
    commWork = func_02012f80(FieldScriptEnv_GetGameData(env));
    done = FALSE;
    switch (func_ov021_021cd688(commWork->menu)) {
    case 1:
        *commWork->result = 3;
        func_ov132_021fc804(commWork->comm);
        done = TRUE;
        break;
    case 2:
        *commWork->result = 4;
        done = TRUE;
        break;
    case 4:
        *commWork->result = 2;
        done = TRUE;
        break;
    }
    if (done == TRUE) {
        func_ov021_021cd5a4(commWork->menu);
        commWork->menu = NULL;
    }
    return done;
}

BOOL func_ov010_0215e068(VM *vm, void *arg) {
    FieldScriptEnv *env = arg;
    MusicalCommWork *commWork;

    FieldScriptEnv_GetScriptWork(env);
    commWork = func_02012f80(FieldScriptEnv_GetGameData(env));
    if (commWork->event != NULL) {
        func_ov010_02156bac(commWork->event);
        if (func_ov010_02156bcc(commWork->event) == TRUE) {
            return TRUE;
        }
    }
    if (func_ov132_021fce00(commWork->comm, commWork->value) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_0215e0b0(VM *vm, void *arg) {
    FieldScriptEnv *env = arg;
    MusicalCommWork *commWork;

    FieldScriptEnv_GetScriptWork(env);
    commWork = func_02012f80(FieldScriptEnv_GetGameData(env));
    func_ov010_02156bac(commWork->event);
    if (func_ov010_02156bcc(commWork->event) == TRUE) {
        return TRUE;
    }
    if (func_ov132_021fd844(commWork->comm) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_0215e0ec(VM *vm, void *arg) {
    FieldScriptEnv *env = arg;
    GameSystem *gsys;
    GameData *gameData;
    GameCommSys *comm;
    MusicalCommWork *commWork;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    gameData = FieldScriptEnv_GetGameData(env);
    comm = GSYS_GetGameCommSystem(gsys);
    commWork = func_02012f80(gameData);
    if (GameCommSys_BootCheck(comm) == 4) {
        commWork->comm = func_02026474(comm);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_0215e130(VM *vm, void *arg) {
    FieldScriptEnv *env = arg;
    GameSystem *gsys;
    GameData *gameData;
    GameCommSys *comm;
    MusicalCommWork *commWork;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    gameData = FieldScriptEnv_GetGameData(env);
    comm = GSYS_GetGameCommSystem(gsys);
    commWork = func_02012f80(gameData);
    if (commWork->event != NULL) {
        func_ov010_02156bac(commWork->event);
        if (func_ov010_02156bcc(commWork->event) == TRUE) {
            return TRUE;
        }
    }
    if (GameCommSys_BootCheck(comm) == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_0215e184(VM *vm, void *arg) {
    FieldScriptEnv *env = arg;
    MusicalCommWork *commWork;

    FieldScriptEnv_GetScriptWork(env);
    commWork = func_02012f80(FieldScriptEnv_GetGameData(env));
    func_ov010_02156bac(commWork->event);
    if (func_ov010_02156bcc(commWork->event) == TRUE) {
        return TRUE;
    }
    if (func_ov132_021fd910(commWork->comm) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

// The items sent from the Dream World
BOOL func_ov010_0215e1c0(VM *vm, FieldScriptEnv *env) {
    u16 kind = ScriptReadAny(vm, env);
    u16 *result = ScriptReadVar(vm, env);
    HeapID heapId = FieldScriptEnv_GetHeapID(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    DreamWorldSave *dreamWorld = getDreamWorldStuffAddress(GameData_GetSaveControl(gameData));
    s32 count = func_02009370(dreamWorld);
    GameEvent *event;

    if (GSYS_GetField(gsys) == NULL && kind != 0) {
        return FALSE;
    }
    switch (kind) {
    case 0:
        *result = func_02009370(dreamWorld) > 0 ? TRUE : FALSE;
        break;
    case 1:
        if (func_ov021_021b76f8(gameData, heapId, dreamWorld) < count) {
            *result = TRUE;
        } else {
            *result = FALSE;
        }
        break;
    case 2:
        event = func_ov021_021b7794(gsys);
        ScriptWork_CallEvent(FieldScriptEnv_GetScriptWork(env), event);
        return TRUE;
    case 3:
        func_ov021_021b76b4(gameData, heapId, dreamWorld);
        break;
    case 5:
        loadItemsNameToStrbuf(ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env)), 0,
                              func_ov021_021b7740(gameData, heapId, dreamWorld, *result));
        break;
    case 4:
        *result = func_ov021_021b76f8(gameData, heapId, dreamWorld);
        break;
    }
    return FALSE;
}

