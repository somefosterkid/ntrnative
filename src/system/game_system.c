// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/game_system.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "system/game_system.h"
#include "save/save_control.h"
#include "field/event_game_clear.h"
#include "field/festival.h"
#include "field/field.h"
#include "field/field_sound.h"
#include "field/intrude_work.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_irc_wireless.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/tcb.h"
#include "gfl/ui.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "system/aeabi.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/iss_sys.h"
#include "system/playtime_ctrl.h"
#include "gfl/std.h"
#include "dsprot/dsprot.h"

// The game system: the process that runs the game from the title screen's choice on, and the accessors of what it
// owns. The file's name is a guess, from the GameSystem and GSYS names: the ROM has no string for it. The functions'
// names are swan's

u32 func_02011d9c(GameProc *proc, u32 param_2, GameSystemProcData *procData);
BOOL func_02011eac(GameProc *proc, u32 state, GameSystemProcData *param, GameSystem *gsys);
BOOL GameSystem_End(GameProc *proc, u32 *state, void *param, void *work);
u32 func_02011f24(void);
void func_020139b0(HeapID heapId);
u8 func_020129fc(GameData *gameData);
extern const GameProcFunctions FIELDMAP_PROC_FUNCS;
void GSYS_Create(GameSystem *gsys, HeapID heapId, GameSystemProcData *procData);
void GSYS_Free(GameSystem *gsys);
void SetupGameSystem(GameSystem *gsys, HeapID heapId, GameSystemProcData *procData);
BOOL GSYS_Update(GameSystem *gsys);
void func_0201205c(GameSystem *gsys);
void GSYS_Terminate(GameSystem *gsys);
void GSYS_UpdateEventRunningFlag(GameSystem *gsys);
BOOL func_02012164(GameSystem *gsys);
void func_02012220(void *work);
void GSYS_SetupUICallback(GameSystem *gsys);
void func_02012258(GameSystem *gsys);
BOOL func_02012264(void *work);
void func_020122a4(GameSystem *gsys);
void func_020122b4(GameSystem *gsys);

u32 g_AntiPiracyTaskCount;
GameSystem *g_GameSystem;
GameSystemProcData g_GameSysProcData;

const GameProcFunctions GAMESYSTEM_PROC_FUNCTIONS = {
    (void *)func_02011d9c,
    (void *)func_02011eac,
    GameSystem_End,
};

// Black's code from 0x02011d28 on (the file was nsbtx_to_clwk_main.c, a guess by position)
// The callback that DS Protect's checks call with their param, a function: it runs it and returns the complement of
// its address, which the checks pass back as their u32 result (the low 32 bits of it, on both sides of the compare)
u32 func_02011d28(void (*func)(void)) {
    if (func != NULL) {
        func();
    }
    return (u32)~(uintptr_t)func;
}

void func_02011d34(TCB *tcb, void *data) {
    GFL_RandomMT();
    GFL_RandomMT();
}

void func_02011d40(void) {
    GFL_VBlankTCBAdd(func_02011d34, NULL, 0x7f);
    g_AntiPiracyTaskCount++;
}


void func_02011d60(void) {
    GFL_HeapCreateChild(9, 5, 0x1400);
    func_02011f24();
    GFL_HeapDelete(5);
}

void func_02011d7c(void) {
    GFL_VBlankTCBAdd(func_02011d34, NULL, 0x7f);
    g_AntiPiracyTaskCount++;
}

u32 func_02011d9c(GameProc *proc, u32 param_2, GameSystemProcData *procData) {
    u32 heapSize;
    u32 base;
    u32 extra;
    u32 flag;
    u32 found;
    s32 i;
    s32 badges;
    GameSystem *gsys;
    GameEvent *event;
    GameData *gameData;
    TrainerCardSave *info;

    heapSize = func_02011f24();
    base = 0xc35ffb;
    GFL_OvlLoad(OVERLAY_ID(230));
    flag = 0;
    if (func_ov230_021882a0((void *)func_02011d28, (void *)func_02011d40, 0) == (u32)~(uintptr_t)func_02011d40) {
        flag = 1;
    }
    base += flag * 0x47;
    GFL_HeapCreateChild(1, 3, 0xa000);
    found = func_ov230_02188354((void *)func_02011d28, (void *)func_02011d60, 0) == (u32)~(uintptr_t)func_02011d60;
    extra = 0;
    if (!found) {
        extra = 0x191;
    }
    GFL_HeapCreateChild(1, 4, 0x10000);
    func_ov230_02188390((void *)func_02011d28, (void *)func_02011d7c, 0);
    GFL_HeapCreateChild(9, 5, 0x1400);
    GFL_OvlUnload(OVERLAY_ID(230));
    gsys = GFL_ProcInitSubsystem(proc, heapSize, 3);
    MI_CpuFill8(gsys, 0, heapSize);
    g_GameSystem = gsys;
    SetupGameSystem(gsys, 3, procData);
    event = EventGameClear_GiveMonotypeMedals(gsys, procData);
    GSYS_SetNowEvent(gsys, event);
    if ((base + extra) % 0x12af != 0) {
        gameData = GSYS_GetGameData(gsys);
        info = getTrainerCardDataBlkAddress(gameData);
        badges = getBadgeCount(info);
        for (i = 0; i < badges; i++) {
            func_02011d40();
        }
    }
    return 1;
}


BOOL func_02011eac(GameProc *proc, u32 state, GameSystemProcData *param, GameSystem *gsys) {
    if (GSYS_Update(gsys)) {
        return TRUE;
    }
    return FALSE;
}



BOOL GameSystem_End(GameProc *proc, u32 *state, void *param, void *work) {
    GSYS_Terminate(work);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_GAMESYSTEM);
    GFL_HeapDelete(HEAPID_GAMEEVENT);
    GFL_HeapDelete(5);
    OS_ResetSystem(0);
    return TRUE;
}

GameSystemProcData *GameSystem_CreateProcData(GameEntryPoint entryPoint, u16 zoneId, VecFx32 *spawnPos,
                                              s16 unk12) {
    MI_CpuFill8(&g_GameSysProcData, 0, sizeof(g_GameSysProcData));
    g_GameSysProcData.entryPoint = entryPoint;
    g_GameSysProcData.zoneId = zoneId;
    g_GameSysProcData.spawnPos = *spawnPos;
    g_GameSysProcData.unk12 = unk12;
    return &g_GameSysProcData;
}

// pokebw2's GameSystem_GetWorkSize
u32 func_02011f24(void) {
    return sizeof(GameSystem);
}


void GSYS_Create(GameSystem *gsys, HeapID heapId, GameSystemProcData *procData) {
    gsys->heapId = heapId;
    gsys->procData = procData;
    gsys->procManager = CreateGameProcManager(gsys->heapId);
    gsys->procManagerState = 0;
    gsys->eventProvider = NULL;
    gsys->eventProviderData = NULL;
    gsys->event = NULL;
    gsys->gameData = GameData_Create(gsys->heapId);
    gsys->field = NULL;
    gsys->comm = func_020261e4(gsys->heapId, gsys->gameData);
    gsys->unk28 = 0;
    gsys->iss = ISS_Create(gsys->gameData, heapId);
    gsys->unk36 = 0;
    gsys->unk37_0 = FALSE;
    GSYS_SetupUICallback(gsys);
    func_020122a4(gsys);
    func_020139b0(heapId);
}


void GSYS_Free(GameSystem *gsys) {
    func_020122b4(gsys);
    func_02012258(gsys);
    ISS_Free(gsys->iss);
    GameData_Free(gsys->gameData);
    FreeGameComm(gsys->comm);
    FreeZoneDataSystem();
}

void SetupGameSystem(GameSystem *gsys, HeapID heapId, GameSystemProcData *procData) {
    GSYS_Create(gsys, heapId, procData);
    func_020267bc(gsys->gameData);
    GameSystemTimer_Disable();
}


BOOL GSYS_Update(GameSystem *gsys) {
    GSYS_UpdateEventRunningFlag(gsys);
    if (func_02012a24(gsys->gameData)) {
        GCTX_HIDSetUpdateRate(30);
    }
    if (!func_020129fc(gsys->gameData)) {
        GSYS_FeedEventsFromProvider(gsys, gsys->eventProvider, gsys->eventProviderData);
        func_0201205c(gsys);
        GSYS_UpdateEvents(gsys);
        func_0202627c(gsys->comm);
    }
    gsys->procManagerState = GFL_ProcMgrUpdate(gsys->procManager);
    FieldSnd_Update(GameData_GetFieldSoundSystem(gsys->gameData));
    ISS_Update(gsys->iss);
    GameSystemTimer_Update(gsys->gameData);
    if (func_02012a24(gsys->gameData)) {
        GCTX_HIDSetUpdateRate(60);
    }
    if (gsys->procManagerState == 2 || gsys->procManagerState == 0) {
        GFL_NetErrShow(0);
    }
    if (gsys->procManagerState == 0 && gsys->event == NULL) {
        return TRUE;
    }
    return FALSE;
}




// Starts the field's communication once the map is up and no event runs
// pokebw2's GSYS_UpdateGameCommBoot
void func_0201205c(GameSystem *gsys) {
    if (!GSYS_GetEventRunningFlag(gsys) && GSYS_CheckField(gsys) == TRUE
        && Field_CheckMapLoadFinished(GSYS_GetField(gsys)) == TRUE) {
        func_020121cc(gsys);
    }
}

void GSYS_Terminate(GameSystem *gsys) {
    FreeGameProcManager(gsys->procManager);
    GSYS_Free(gsys);
}

void GSYS_SetEventProvider(GameSystem *gsys, void *provider, void *data) {
    gsys->eventProvider = provider;
    gsys->eventProviderData = data;
}

void GSYS_UpdateEventRunningFlag(GameSystem *gsys) {
    gsys->eventRunning = GSYS_CheckNowEvent(gsys);
}

void GSYS_QueueProc(GameSystem *gsys, s32 overlayId, const GameProcFunctions *functions, void *param) {
    QueueGameProc(gsys->procManager, overlayId, functions, param);
}

void GSYS_StartFieldmapProc(GameSystem *gsys) {
    GSYS_QueueProc(gsys, OVERLAY_ID(21), &FIELDMAP_PROC_FUNCS, gsys);
}


BOOL GSYS_GetEventRunningFlag(GameSystem *gsys) {
    if (GSYS_CheckNowEvent(gsys)) {
        return TRUE;
    }
    return gsys->eventRunning;
}

BOOL GSYS_GetProcMgrState(GameSystem *gsys) {
    return gsys->procManagerState;
}

GameData *GSYS_GetGameData(GameSystem *gsys) {
    return gsys->gameData;
}

PlayerState *GSYS_GetPlayerState(GameSystem *gsys) {
    return GameData_GetPlayerState(gsys->gameData);
}

GameEvent *GSYS_GetNowEvent(GameSystem *gsys) {
    return gsys->event;
}

void GSYS_SetNowEvent(GameSystem *gsys, GameEvent *event) {
    gsys->event = event;
}

Field *GSYS_GetField(GameSystem *gsys) {
    return gsys->field;
}

void GSYS_SetField(GameSystem *gsys, Field *field) {
    gsys->field = field;
}

BOOL GSYS_CheckField(GameSystem *gsys) {
    if (gsys->field != NULL) {
        return TRUE;
    }
    return FALSE;
}

void func_02012124(GameSystem *gsys, u32 a1) {
    gsys->unk34 = a1;
}

u8 func_0201212c(GameSystem *gsys) {
    return gsys->unk34;
}



ISS *GameSystem_GetISS(GameSystem *gsys) {
    return gsys->iss;
}

GameCommSys *GSYS_GetGameCommSystem(GameSystem *gsys) {
    return gsys->comm;
}

void func_0201213c(GameSystem *gsys, u32 value) {
    gsys->unk36 = value;
}

u8 func_02012144(GameSystem *gsys) {
    return gsys->unk36;
}

u32 func_0201214c(GameSystem *gsys) {
    return func_02012bb0(gsys->gameData);
}

void func_02012158(GameSystem *gsys, u32 value) {
    func_02012bc8(gsys->gameData, value);
}

void *func_0200921c(GameData *gameData);





BOOL func_02012164(GameSystem *gsys) {
    u16 zoneId;

    GSYS_GetGameCommSystem(gsys);
    zoneId = PlayerState_GetZoneID(GSYS_GetPlayerState(gsys));
    if (gsys->unk36 == 0 && !func_02013d34(zoneId) && !func_0200fb24() && func_0201214c(gsys) == 1
        && func_ov010_02158318(gsys) == 1 && (func_02035390() != 1 || GetIsShouldUICallback() != 1)
        && func_020090e0(func_0200921c(gsys->gameData))) {
        return TRUE;
    }
    return FALSE;
}






u32 func_0201214c(GameSystem *gsys);

// pokebw2's GSYS_TryBootGameComm
BOOL func_020121cc(GameSystem *gsys) {
    GameCommSys *comm = GSYS_GetGameCommSystem(gsys);

    if (!func_0203cd98() && !GameCommSys_BootCheck(comm) && func_02012164(gsys) == TRUE) {
        func_020263d8(comm, 1, gsys);
        return TRUE;
    }
    return FALSE;
}

BOOL func_02012204(GameSystem *gsys) {
    return gsys->unk37_0;
}

void func_02012210(GameSystem *gsys) {
    gsys->unk37_0 = FALSE;
}

void func_02012220(void *work) {
    GameSystem *gsys = work;

    if (func_0201214c(gsys)) {
        func_02012158(gsys, FALSE);
        gsys->unk37_0 = TRUE;
    }
}


void GSYS_SetupUICallback(GameSystem *gsys) {
    SetUICallbackStatic(func_02012220, gsys);
}

// pokebw2's GSYS_ClearUICallback
void func_02012258(GameSystem *gsys) {
    SetUICallbackStatic(NULL, NULL);
}

// The sleep check: whether closing the lid must not put the console to sleep
// pokebw2's GSYS_SleepCheckCallback
BOOL func_02012264(void *work) {
    GameSystem *gsys = work;

    if (GameCommSys_BootCheck(GSYS_GetGameCommSystem(gsys))) {
        return TRUE;
    }
    if (func_0203da4c()) {
        return TRUE;
    }
    if (GetIsShouldUICallback()) {
        return FALSE;
    }
    if (func_0201214c(gsys)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GSYS_SetupSleepCheckCallback
void func_020122a4(GameSystem *gsys) {
    func_02035064(func_02012264, gsys);
}

// pokebw2's GSYS_ClearSleepCheckCallback
void func_020122b4(GameSystem *gsys) {
    func_02035064(NULL, NULL);
}
