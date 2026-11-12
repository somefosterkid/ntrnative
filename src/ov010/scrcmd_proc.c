// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_proc.c, ported by tools/port_file.py.
#include "types.h"
#include "app/bag.h"
#include "app/mailbox.h"
#include "app/marine_tube_board.h"
#include "app/monolith.h"
#include "app/psel.h"
#include "app/subway_map.h"
#include "app/zukan_award.h"
#include "field/entree_forest.h"
#include "field/event_3d_demo.h"
#include "field/event_battle_video.h"
#include "field/event_wifibattlematch.h"
#include "field/field_event.h"
#include "field/field_script.h"
#include "field/game_beacon_set.h"
#include "field/intrude_work.h"
#include "field/scrcmd_proc.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "pml/move_reminder.h"
#include "pml/poke_party.h"
#include "save/high_link.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "app/monolith/monolith_main.h"
#include "field/intrude_comm.h"
#include "ov037_021f36c0.h"

extern u32 OVERLAY_2_ID[];
extern u32 OVERLAY_3_ID[];
extern u32 OVERLAY_136_ID[];
extern u32 OVERLAY_173_ID[];
extern u32 OVERLAY_71_ID[];
extern GameProcFunctions data_ov071_021ba758;
extern u32 OVERLAY_221_ID[];
extern u32 OVERLAY_222_ID[];
extern u32 OVERLAY_224_ID[];
extern GameProcFunctions data_ov221_021ba1a4;
extern GameProcFunctions data_ov222_021ba250;
extern GameProcFunctions data_ov224_021b9bf8;
extern GameProcFunctions data_ov173_021b81d0;


struct BagScriptResult {
    u16 *hasSelection;
    u16 *item;
};


BOOL s014A_FieldOpen(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    ScriptWork_CallEvent(work, EventFieldOpen_CreateHeadless(gsys));
    return TRUE;
}

BOOL s014B_FieldClose(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    Field *field = GSYS_GetField(gsys);
    ScriptWork_CallEvent(work, CreateFieldCloseEvent(gsys, field));
    return TRUE;
}

BOOL func_ov010_0215c5ec(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    ScriptWork_CallEvent(work, EventFieldOpen_CreateHeadless(gsys));
    return TRUE;
}

BOOL func_ov010_0215c610(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    Field *field = GSYS_GetField(gsys);
    ScriptWork_CallEvent(work, CreateFieldCloseEvent(gsys, field));
    return TRUE;
}

void func_ov010_0215c63c(VM *vm, FieldScriptEnv *env, s32 overlayId, const GameProcFunctions *functions, void *resource,
                         void (*cleanup)(ScriptOverlayWork *), void *data) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    ScriptWork *scriptWork = FieldScriptEnv_GetScriptWork(env);
    void **heapPtr = ScriptWork_GetUserHeapPtr(scriptWork);
    ScriptOverlayWork *work = GFL_HeapAllocate(4, sizeof(ScriptOverlayWork), TRUE, "scrcmd_proc.c", 0x8b);

    work->resource = resource;
    work->data = data;
    work->cleanup = cleanup;
    GSYS_QueueProc(gsys, overlayId, functions, resource);
    *heapPtr = work;
    VM_SetNativeCallback(vm, func_ov010_0215c69c);
}



BOOL func_ov010_0215c69c(VM *vm, void *data) {
    FieldScriptEnv *env = data;
    GameSystem *gsys;
    ScriptWork *scriptWork;
    ScriptOverlayWork *work;

    gsys = FieldScriptEnv_GetGameSystem(env);
    scriptWork = FieldScriptEnv_GetScriptWork(env);
    work = ScriptWork_GetUserHeap(scriptWork);
    if (GSYS_GetProcMgrState(gsys) != 0) {
        return FALSE;
    }
    if (work->cleanup != NULL) {
        work->cleanup(work);
    } else {
        if (work->resource != NULL) {
            GFL_HeapFree(work->resource);
        }
        if (work->data != NULL) {
            GFL_HeapFree(work->data);
        }
    }
    ScriptWork_FreeUserHeap(scriptWork);
    return TRUE;
}

BOOL s014C_RTFreeUserHeap(VM *vm, FieldScriptEnv *env) {
    ScriptWork_FreeUserHeap(FieldScriptEnv_GetScriptWork(env));
    return TRUE;
}

void func_ov010_0215c700(ScriptOverlayWork *work) {
    BagProcessData *bag = work->resource;
    BagScriptResult *result = work->data;
    if (bag->result == 0) {
        *result->hasSelection = FALSE;
    } else {
        *result->hasSelection = TRUE;
    }
    *result->item = bag->item;
    GFL_HeapFree(work->data);
    GFL_HeapFree(work->resource);
}

extern u32 OVERLAY_70_ID[];
extern GameProcFunctions BAG_PROC_FUNCTIONS;
BOOL s014E_CallBag(VM *vm, FieldScriptEnv *env) {
    u16 mode;
    u16 *hasSelection;
    u16 *item;
    BagProcessData *bag;
    BagScriptResult *result;

    FieldScriptEnv_GetGameSystem(env);
    FieldScriptEnv_GetScriptWork(env);
    mode = ScriptReadAny(vm, env);
    hasSelection = ScriptReadVar(vm, env);
    item = ScriptReadVar(vm, env);
    if (mode == 0) {
        mode = 4;
    } else if (mode == 1) {
        mode = 5;
    } else {
        mode = 0;
    }
    bag = func_0202e474(FieldScriptEnv_GetGameData(env), NULL, mode, HEAPID_GAMEEVENT);
    result = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(BagScriptResult), TRUE, "scrcmd_proc.c", 280);
    result->hasSelection = hasSelection;
    result->item = item;
    func_ov010_0215c63c(vm, env, OVERLAY_ID(70), &BAG_PROC_FUNCTIONS, bag, func_ov010_0215c700, result);
    return TRUE;
}


void func_ov010_0215c7c4(void *arg) {
    ScriptProcCallbackWork *work = arg;
    MailboxProcessData *mailbox = work->resource;

    if (mailbox->result == 1) {
        *(u16 *)work->data = TRUE;
    } else {
        *(u16 *)work->data = FALSE;
    }
    GFL_HeapFree(work->resource);
}

BOOL s0150_CallMailbox(VM *vm, FieldScriptEnv *env) {
    ScriptWork *scriptWork = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = GSYS_GetGameData(gsys);
    Field *field = GSYS_GetField(gsys);
    MailboxProcessData *mailbox = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(MailboxProcessData), FALSE, "scrcmd_proc.c", 0x14f);
    ScriptProcCallbackWork *work = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(ScriptProcCallbackWork), FALSE, "scrcmd_proc.c", 0x150);

    mailbox->gameData = gameData;
    work->resource = mailbox;
    work->data = ScriptReadVar(vm, env);
    ScriptWork_CallEvent(scriptWork, func_02014840(gsys, field, OVERLAY_ID(136), &data_ov136_021c78a4, mailbox,
                                                   func_ov010_0215c7c4, work));
    return TRUE;
}


void func_ov010_0215c870(void *arg) {
    ScriptProcCallbackWork *work = arg;
    MoveReminderProcessData *data;
    u16 result;

    data = work->resource;
    switch (data->status) {
    case 0:
    default:
        result = FALSE;
        break;
    case 1:
        result = TRUE;
        break;
    }
    *(u16 *)work->data = result;
    GFL_HeapFree(((MoveReminderProcessData *)work->resource)->moves);
    func_ov010_0216f940(work->resource);
}

extern GameProcFunctions data_ov173_021b81d0;
BOOL s01D6_MoveReminderCallMoveSelect(VM *vm, FieldScriptEnv *env) {
    u16 index;
    ScriptWork *work;
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    u16 *ptr;
    PokeParty *party;
    PartyPkm *pkm;
    PlayerInfo *pPVar3;
    u16 *moves;
    MoveReminderProcessData *param;
    ScriptProcCallbackWork *arg;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    gameData = GSYS_GetGameData(gsys);
    field = GSYS_GetField(gsys);
    ptr = ScriptReadVar(vm, env);
    index = ScriptReadAny(vm, env);
    party = GameData_GetParty(gameData);
    pkm = PokeParty_GetPkm(party, (u32)index);
    pPVar3 = GetGameDataPlayerInfo(gameData);
    moves = PokeParty_GetRememberableMoves(pkm, 4);
    param = func_ov010_0216f918(4);
    param->pkm = pkm;
    param->playerInfo = pPVar3;
    param->unk08 = NULL;
    param->gsys = gsys;
    param->moves = moves;
    param->unk19 = 1;
    arg = GFL_HeapAllocate(4, sizeof(ScriptProcCallbackWork), 0, "scrcmd_proc.c", 0x19b);
    arg->resource = param;
    arg->data = ptr;
    event = func_02014808(gsys, field, OVERLAY_ID(173), &data_ov173_021b81d0, param, func_ov010_0215c870, arg);
    ScriptWork_CallEvent(work, event);
    return 1;
}

BOOL func_ov010_0215c95c(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    GameData *gameData = GSYS_GetGameData(gsys);
    MonolithParam *param;
    IntrudeComm *work;
    u8 index;

    FieldScriptEnv_GetScriptWork(env);
    work = func_ov010_02158344(commSys);
    param = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(MonolithParam), TRUE, "scrcmd_proc.c", 0x1bc);
    if (work != NULL && func_02012b58(gameData) != work->unk_070.unk_16_0 &&
        func_ov011_02175fb4(&work->unk_1AC.players[func_ov010_0215841c(gameData)]) == 1 && work->unk_914.unk_0E == 1) {
        index = func_ov010_0215841c(gameData);
        param->commPlayer = *func_ov010_0215840c(work, index);
        param->highLink = work->unk_914;
        param->unk_3D = 1;
    } else {
        getHighLinkBlockAddress(GameData_GetSaveControl(gameData));
        func_ov011_02175648(gameData, &param->highLink);
        index = 0;
        param->unk_3D = 0;
    }
    param->gsys = gsys;
    param->netId = index;
    func_ov010_0215c63c(vm, env, OVERLAY_ID(71), &data_ov071_021ba758, param, NULL, NULL);
    return TRUE;
}



BOOL s0154_Call3DDemo(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameSystem *gsys;
    u16 demoId;
    u16 param;
    GameEvent *parent;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    demoId = ScriptReadAny(vm, env);
    param = ScriptReadAny(vm, env);
    parent = ScriptWork_GetEvent(work);
    event = Event3DDemo_Create(gsys, parent, demoId, param, 0);
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

extern u32 OVERLAY_174_ID[];
extern GameProcFunctions data_ov174_021b74a0;

struct ScrCmdProcData_0x0AE {
    u16 unk0;
    GameData *unk4;
};
BOOL func_ov010_0215cab4(VM *vm, FieldScriptEnv *env) {
    u16 value;
    struct ScrCmdProcData_0x0AE *resource;
    GameData *gameData;

    value = ScriptReadAny(vm, env);
    resource = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(struct ScrCmdProcData_0x0AE), TRUE, "scrcmd_proc.c", 0x205);
    resource->unk0 = value;
    gameData = FieldScriptEnv_GetGameData(env);
    resource->unk4 = gameData;
    func_ov010_0215c63c(vm, env, OVERLAY_ID(174), &data_ov174_021b74a0, resource, NULL, NULL);
    return TRUE;
}

extern void func_ov010_02165344(void);
extern u32 OVERLAY_221_ID[];
extern u32 OVERLAY_222_ID[];
extern u32 OVERLAY_224_ID[];
extern GameProcFunctions data_ov221_021ba1a4;
extern GameProcFunctions data_ov222_021ba250;
extern GameProcFunctions data_ov224_021b9bf8;

struct ScrCmdProcData_0x0DD {
    GameData *gameData;
    u32 value;
};
BOOL func_ov010_0215cb08(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = GSYS_GetGameData(gsys);
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(gameData);
    u16 value1 = ScriptReadAny(vm, env);
    u16 value2 = ScriptReadAny(vm, env);
    struct ScrCmdProcData_0x0DD *resource;
    PlayerInfo **info;

    if (value1 != 2) {
        func_ov010_02165344();
    }
    switch (value1) {
    case 0:
        resource = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(struct ScrCmdProcData_0x0DD), TRUE, "scrcmd_proc.c", 0x22b);
        resource->gameData = gameData;
        resource->value = value2;
        func_ov010_0215c63c(vm, env, OVERLAY_ID(221), &data_ov221_021ba1a4, resource, NULL, NULL);
        break;
    case 1:
        resource = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(struct ScrCmdProcData_0x0DD), TRUE, "scrcmd_proc.c", 0x233);
        resource->gameData = gameData;
        resource->value = value2;
        func_ov010_0215c63c(vm, env, OVERLAY_ID(222), &data_ov222_021ba250, resource, NULL, NULL);
        break;
    case 2:
        info = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(PlayerInfo *), TRUE, "scrcmd_proc.c", 0x23c);
        *info = playerInfo;
        func_ov010_0215c63c(vm, env, OVERLAY_ID(224), &data_ov224_021b9bf8, info, NULL, NULL);
        break;
    }
    return TRUE;
}

extern u32 OVERLAY_223_ID[];
extern GameProcFunctions data_ov223_021bc8c4;

struct ScrCmdProcData_0x0DF {
    u16 *value;
    u32 unk4;
};
BOOL func_ov010_0215cbf0(VM *vm, FieldScriptEnv *env) {
    u16 *result;
    struct ScrCmdProcData_0x0DF *resource;

    result = ScriptReadVar(vm, env);
    resource = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(struct ScrCmdProcData_0x0DF), TRUE, "scrcmd_proc.c", 0x254);
    resource->value = result;
    func_ov010_0215c63c(vm, env, OVERLAY_ID(223), &data_ov223_021bc8c4, resource, NULL, NULL);
    return TRUE;
}




BOOL func_ov010_0215cc38(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    FieldScriptEnv_GetGameSystem(env);
    return TRUE;
}

BOOL s0160_NetConnectWiFiBattle(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameSystem *gsys;
    u16 battleType;
    u16 mode;
    u32 type;
    u32 option;
    EventWifiBattleMatchArgs args;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    battleType = ScriptReadAny(vm, env);
    mode = ScriptReadAny(vm, env);
    switch (battleType) {
    case 15:
        type = 0;
        break;
    case 16:
        type = 1;
        break;
    case 17:
        type = 2;
        break;
    case 18:
        type = 3;
        break;
    case 19:
        type = 4;
        break;
    }
    switch (mode) {
    case 0:
        option = 0;
        break;
    case 1:
        option = 1;
        break;
    }
    args.field = GSYS_GetField(gsys);
    args.unk4 = 1;
    args.unk8 = option;
    args.unkC = type;
    event = GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(2), func_ov002_02154438, &args);
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL s0161_NetConnectBattleVideo(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameSystem *gsys;
    u16 mode;
    EventBattleVideoArgs args;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    mode = ScriptReadAny(vm, env);
    if (mode == 0) {
        mode = 1;
    } else {
        mode = 2;
    }
    args.field = GSYS_GetField(gsys);
    args.mode = mode;
    event = GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(3), func_ov003_021543bc, &args);
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

extern u32 OVERLAY_37_ID[];
BOOL func_ov010_0215cd3c(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u16 *var1;
    u16 *var2;

    gsys = FieldScriptEnv_GetGameSystem(env);
    var1 = ScriptReadVar(vm, env);
    var2 = ScriptReadVar(vm, env);
    GFL_OvlLoad(OVERLAY_ID(37));
    func_ov037_021f36c0(gsys, *var1, *var2);
    GFL_OvlUnload(OVERLAY_ID(37));
    return FALSE;
}

BOOL func_ov010_0215cd80(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u16 value;

    gsys = FieldScriptEnv_GetGameSystem(env);
    value = ScriptReadAny(vm, env);
    GFL_OvlLoad(OVERLAY_ID(37));
    func_ov037_021f36e8(gsys, value);
    GFL_OvlUnload(OVERLAY_ID(37));
    return FALSE;
}

BOOL func_ov010_0215cdb8(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u16 value;

    gsys = FieldScriptEnv_GetGameSystem(env);
    value = ScriptReadAny(vm, env);
    GFL_OvlLoad(OVERLAY_ID(37));
    func_ov037_021f3724(gsys, value);
    GFL_OvlUnload(OVERLAY_ID(37));
    return FALSE;
}

BOOL func_ov010_0215cdf0(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    u16 *vars[8];

    gsys = FieldScriptEnv_GetGameSystem(env);
    vars[0] = ScriptReadVar(vm, env);
    vars[1] = ScriptReadVar(vm, env);
    vars[2] = ScriptReadVar(vm, env);
    vars[3] = ScriptReadVar(vm, env);
    vars[4] = ScriptReadVar(vm, env);
    vars[5] = ScriptReadVar(vm, env);
    vars[6] = ScriptReadVar(vm, env);
    vars[7] = ScriptReadVar(vm, env);
    GFL_OvlLoad(OVERLAY_ID(37));
    func_ov037_021f3768(gsys, vars);
    GFL_OvlUnload(OVERLAY_ID(37));
    return FALSE;
}


BOOL func_ov010_0215ce70(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;

    gsys = FieldScriptEnv_GetGameSystem(env);
    GFL_OvlLoad(OVERLAY_ID(37));
    func_ov037_021f3800(gsys);
    GFL_OvlUnload(OVERLAY_ID(37));
    return FALSE;
}





