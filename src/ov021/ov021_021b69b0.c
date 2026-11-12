// Black's code at ov021 0x021b69b0-0x021b70d8, a file pokebw2 lacks: script commands of pokebw2's scrcmd_misc.c (the
// trainer-log and elevator commands), then the mystery-gift command and the gift handlers that pokebw2 has in its
// pdw_postman.c. The handlers' table sorts first in .rodata before scrcmd_misc.c's tables, so it is this file's; Black's
// pdw_postman.c is the list window after scrcmd_misc.c.

#include "types.h"
#include "fdlibm.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/ev_time.h"
#include "field/event_data.h"
#include "field/field_actor_internal.h"
#include "field/field_async_proc.h"
#include "field/field_internal.h"
#include "field/field_map.h"
#include "field/field_script.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "gfl/overlay.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/trainer_log.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/shortcut_util.h"
#include "system/vm.h"
#include "system/wordset.h"
#include "save/mystery_gift.h"
#include "field/field_rail.h"
#include "field/pdw_postman.h"
#include "save/trainer_card.h"
#include "system/game_system.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "gfl/heap.h"
#include "save/pokedex.h"

struct TCB {
    TCBManager *manager;
    TCB *prev;
    TCB *next;
    u32 priority;
    void *data;
    TCBFunc func;
    BOOL addedDuringUpdate;
};
struct TCBManager {
    u32 count;
    u32 used;
    TCB head;
    TCB **ready;
    TCB *tasks;
    BOOL adding;
    TCB *current;
    TCB *next;
};
struct FieldAsyncProc {
    FieldAsyncProcManager *manager;
    TCB *updateTask;
    TCB *drawTask;
    const FieldAsyncProcDef *def;
    void *work;
    u32 overlayId;
};
struct FieldAsyncProcManager {
    Field *field;
    u32 heapId;
    int count;
    FieldAsyncProc *procs;
    TCBManager *updateTasks;
    TCBManager *drawTasks;
};
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3_0 : 7;
    u8 unk3_7 : 1;
    u8 unk4;
} WordSetAttr;
typedef struct {
    WordSetAttr attr;
    StrBuf *strbuf;
} WordSetBuf;
struct WordSet {
    u32 count;
    u32 heapId;
    WordSetBuf *bufs;
    StrBuf *tmp;
};
struct StrBuf {
    u16 size;
    u16 length;
    u32 magic;
    u16 chars[1];
};
struct CommPlayerSupport {
    u32 type;
    u32 usedType;
    u32 lastType;
    PlayerInfo supporter;
    PlayerInfo usedSupporter;
};
struct BoxSaveAccessor {
    SaveControl *save;
};
struct GameData {
    SaveControl *saveControl;
    PlayerState playerStates[5];
    EventData *eventData;
    PlayerSave *playerSave;
    ZoneSpawnInfo *nowSpawn;
    ZoneSpawnInfo *outboundWarpSpawn;
    ZoneSpawnInfo *nextSpawn;
    ZoneSpawnInfo *escapeRopeSpawn;
    ZoneSpawnInfo entralinkParentSpawn;
    void *bagCursor;
    BagSave *bag;
    PokeParty *party;
    Config *config;
    BoxSaveAccessor *boxes;
    WifiList *wifiList;
    BGMInfo *bgmInfo;
    MMSys *mmSys;
    EventWork *eventWork;
    MapMatrix *mapMatrix;
    FieldSound *fieldSound;
    FieldStatus *fieldStatus;
    u32 lastBtlResult;
    u32 unk1C0;
    u8 season;
    u8 weather;
    u8 unk1C6;
    u8 unk1C7;
    u8 counter;
    u8 counterEnabled;
    u8 lastSubscreen;
    u8 netId;
    u8 forceSeasonSync;
    u8 savedCGearPowerRequest;
    u8 saving;
    u8 is30FPSMode;
    u8 seasons[4];
    PassPowerLevel levels[4];
    CityState cityStates[2];
    EncountState *encountState;
    ShortcutMenuPos shortcutMenuPos;
    GimmickState gimmickState;
    PleasureBoat *pleasureBoat;
    CommPlayerSupport *commPlayerSupport;
    u32 unk688;
    void *unk68C;
    Calendar *calendar;
    UnityTowerFloor unityTowerFloor;
    TrialHouseWork *trialHouseWork;
    u32 unk6A8;
    PlayTime *playTime;
};
struct BGMInfo {
    u8 count;
    u16 *bgms;
    u8 *subsystems;
};

extern const FieldAsyncProcDef data_ov055_021f45f0;
extern u32 OVERLAY_55_ID[];

u32 func_ov021_021b69b0(u32 param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    FieldAsyncProcManager *mgr;

    work = FieldScriptEnv_GetScriptWork(param_2);
    mgr = Field_GetAsyncProcMgr(((ScriptFieldWork *)ScriptWork_GetFieldWork(work))->field);
    FieldAsyncProcManager_AddProc(OVERLAY_ID(55), mgr, &data_ov055_021f45f0);
    return 0;
}

u32 func_ov021_021b69d8(VM *param_1, FieldScriptEnv *param_2) {
    OSOwnerInfo spC;
    u32 sp8;
    GameData *gameData;
    u32 result;
    RTCDate *date;
    StrBuf *strbuf;
    TrainerLogSave *log;
    WordSet *wordSet;
    const u16 *src;
    u32 index;
    u32 flag;

    gameData = FieldScriptEnv_GetGameData(param_2);
    sp8 = (u32) ScriptReadAny(param_1, param_2);
    log = func_0200a8a0(GameData_GetSaveControl(gameData));
    date = getAddressAdventureTimeBlk(gameData);
    OS_GetOwnerInfo(&spC);
    index = func_0200a940(log, spC.birthday.month, spC.birthday.day, date);
    result = func_0200a8f8(log, index);
    flag = 1;
    if (result == 1) {
        flag = 0;
    }
    src = func_0200a8c8(log, index);
    wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(param_2));
    strbuf = GFL_StrBufCreate(8U, 0x8001U);
    GFL_StrBufLoadString(strbuf, src);
    func_0201ee0c(wordSet, sp8, strbuf, flag, 1U, 2U);
    GFL_StrBufFree(strbuf);
    return 0U;
}

u32 func_ov021_021b6a78(VM *param_1, FieldScriptEnv *param_2) {
    GameData *gameData;
    u16 *ptr;
    SaveControl *save;
    TrainerLogSave *log;

    gameData = FieldScriptEnv_GetGameData(param_2);
    ptr = ScriptReadVar(param_1, param_2);
    save = GameData_GetSaveControl(gameData);
    log = func_0200a8a0(save);
    *ptr = (u16)func_0200a8ac(log);
    return 0;
}

// A floor of an elevator: its name, and where it goes (pokebw2's). A script's table of them follows the command
// 021b6ae0
typedef struct ElevatorFloor {
    u16 messageId;
    u16 zoneId;
    u16 x;
    u16 y;
    u16 z;
} ElevatorFloor;

ElevatorFloor *ElevatorTable_GetEntry(ElevatorFloor *table, s32 index) {
    return &table[index];
}

void func_ov021_021b6aac(void) {
}

s32 ElevatorTable_CountEntries(ElevatorFloor *table) {
    u16 val;
    ElevatorFloor *entry;
    s32 i;

    i = 0;
    val = -1;
    do {
        entry = ElevatorTable_GetEntry(table, i);
        func_ov021_021b6aac();
        if (entry->messageId == val) {
            return i;
        }
        i = i + 1;
    } while (i < 8);
    return 0;
}

u32 func_ov021_021b6ae0(VM *param_1, FieldScriptEnv *param_2) {
    FieldScriptEnv_SetElevatorTable(param_2, (ElevatorFloor *)(param_1->pc + (s32)VM_Read32(param_1)));
    return 0;
}

u32 func_ov021_021b6afc(u32 param_1, FieldScriptEnv *param_2) {
    u16 scriptEnvZoneID;
    ElevatorFloor *elevatorTable;
        ElevatorFloor *val;
    ScriptWork *work;
    StrBuf *expanded;
    StrBuf *temp;
        int result;
        int val2;
    ElevatorFloor *entry;
    u32 value;

    elevatorTable = FieldScriptEnv_GetElevatorTable(param_2);
    work = FieldScriptEnv_GetScriptWork(param_2);
    expanded = ScriptWork_GetMainStrBuf(work);
        val = elevatorTable;
    temp = ScriptWork_GetAltStrBuf(work);
    scriptEnvZoneID = GetScriptEnvZoneID(param_2);
        result = ElevatorTable_CountEntries(val);
    value = 0;
    if (result != 0) {
        do {
                        val2 = scriptEnvZoneID;
                        entry = ElevatorTable_GetEntry(val, value);
                        if (val2 != entry->zoneId) {
                AddItemToListMenu(param_2, entry->messageId, 0xffff, value, expanded, temp);
            }
            value = value + 1;
        } while (value < result);
    }
    return 0;
}

#define GRID_TO_FX32(n) ((n) * FX32_CONST(16))
BOOL func_ov021_021b6b6c(VM *vm, FieldScriptEnv *env) {
    VecFx32 pos;
    u32 index = ScriptReadAny(vm, env);
    ElevatorFloor *table = FieldScriptEnv_GetElevatorTable(env);

    GetScriptEnvZoneID(env);
    if (index < ElevatorTable_CountEntries(table)) {
        ElevatorFloor *floor = ElevatorTable_GetEntry(table, index);
        ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
        GameSystem *gsys = ScriptWork_GetGameSystem(work);
        Field *field = GSYS_GetField(gsys);
        u16 zoneId = floor->zoneId;
        u16 x = floor->x;
        u16 y = floor->y;
        u16 z = floor->z;

        pos.x = GRID_TO_FX32(x);
        pos.y = GRID_TO_FX32(y);
        pos.z = GRID_TO_FX32(z);
        ScriptWork_CallEvent(work, EventMapChange_CreateGridDefault(gsys, field, zoneId, &pos, 1));
    }
    return TRUE;
}


PartyPkm *func_ov010_02157e9c(MysteryGift *gift, u32 heapId, GameData *gameData);
void func_0200bf18(HighLinkSave *save, s32 param_2);
BOOL func_ov021_021b6f00(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
void func_ov021_021b6f18(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
u32 func_ov021_021b6f5c(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
u32 func_ov021_021b6f88(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env);
u32 func_ov021_021b6fe8(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env);
BOOL func_ov021_021b6fec(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
void func_ov021_021b6ff0(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
u32 func_ov021_021b7020(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
u32 func_ov021_021b7038(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env);
u32 func_ov021_021b7068(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env);
BOOL func_ov021_021b707c(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
void func_ov021_021b7080(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
u32 func_ov021_021b7098(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
u32 func_ov021_021b709c(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env);
u32 func_ov021_021b70d4(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env);

// What the postman does for each kind of mystery gift (MysteryGift.kind): pokebw2's sGiftHandlers, without its kind 4
static const PdwPostmanGiftHandler sGiftHandlers[4] = {
    { 0, NULL, NULL, NULL, NULL, NULL },
    { 1, func_ov021_021b6f00, func_ov021_021b6f18, func_ov021_021b6f5c, func_ov021_021b6f88, func_ov021_021b6fe8 },
    { 2, func_ov021_021b6fec, func_ov021_021b6ff0, func_ov021_021b7020, func_ov021_021b7038, func_ov021_021b7068 },
    { 3, func_ov021_021b707c, func_ov021_021b7080, func_ov021_021b7098, func_ov021_021b709c, func_ov021_021b70d4 },
};

u32 func_ov021_021b6bdc(VM *vm, FieldScriptEnv *env) {
    u32 type;
    u16 *result;
    GameData *gameData;
    HeapID heapId;
    MysteryGiftSave *save;
    MysteryGift *gift;
    s32 slot;
    MysteryGift giftBuf;
    u32 current;
    TrainerCardSave *trainerCard;

    type = ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    gameData = FieldScriptEnv_GetGameData(env);
    heapId = FieldScriptEnv_GetHeapID(env);
    save = mysteryGiftBlock(GameData_GetSaveControl(gameData), 0, heapId);
    gift = func_ov021_021b7834(save, &slot, &giftBuf);
    current = func_ov021_021b7864(save);
    if (current != sGiftHandlers[current].kind) {
        func_0200a338(save);
        return 0;
    }
    switch (type) {
    case 0:
        if (current != 0) {
            *result = 1;
        } else {
            *result = 0;
        }
        break;
    case 1:
        *result = func_ov021_021b6d0c(current, env, gameData, gift);
        break;
    case 2:
        *result = func_ov021_021b6d30(current, gift, env);
        break;
    case 3:
        *result = func_ov021_021b6d64(current, gift, env);
        break;
    case 4:
        if (func_ov021_021b6d98(current, env, gameData, gift) == 1) {
            func_ov021_021b785c(save, slot);
            if (func_ov021_021b6eb0(gift, current) == 1) {
                trainerCard = getTrainerCardDataBlkAddress(gameData);
                setOneShotDRObtained(trainerCard, 1, GetGameDataPlayerInfo(gameData));
            }
        }
        *result = 0;
        break;
    case 5:
        *result = func_ov021_021b6de4(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)), gameData);
        break;
    case 6:
        *result = func_ov021_021b6e0c(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)));
        break;
    case 7:
        *result = func_ov021_021b6dc0(current, env, gameData, gift);
        break;
    }
    func_0200a338(save);
    return 0;
}

u32 func_ov021_021b6d0c(u32 kind, FieldScriptEnv *env, GameData *gameData, MysteryGift *gift) {
    BOOL (*canReceive)(FieldScriptEnv *, GameData *, MysteryGift *) = sGiftHandlers[kind].canReceive;

    if (gift != NULL && canReceive != NULL) {
        return canReceive(env, gameData, gift);
    }
    return 0;
}

u32 func_ov021_021b6d30(u32 kind, MysteryGift *gift, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u32 (*getMessage)(WordSet *, MysteryGift *, FieldScriptEnv *) = sGiftHandlers[kind].unk10;

    if (gift != NULL && getMessage != NULL) {
        return getMessage(wordSet, gift, env);
    }
    return 7;
}

u32 func_ov021_021b6d64(u32 kind, MysteryGift *gift, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u32 (*getMessage)(WordSet *, MysteryGift *, FieldScriptEnv *) = sGiftHandlers[kind].unk14;

    if (gift != NULL && getMessage != NULL) {
        return getMessage(wordSet, gift, env);
    }
    return 8;
}

u32 func_ov021_021b6d98(u32 kind, FieldScriptEnv *env, GameData *gameData, MysteryGift *gift) {
    void (*receive)(FieldScriptEnv *, GameData *, MysteryGift *) = sGiftHandlers[kind].receive;

    if (gift != NULL && receive != NULL) {
        receive(env, gameData, gift);
        return 1;
    }
    return 0;
}

u32 func_ov021_021b6dc0(u32 kind, FieldScriptEnv *env, GameData *gameData, MysteryGift *gift) {
    u32 (*handler)(FieldScriptEnv *, GameData *, MysteryGift *) = sGiftHandlers[kind].unk0C;

    if (gift != NULL && handler != NULL) {
        return handler(env, gameData, gift);
    }
    return 0;
}

u32 func_ov021_021b6de4(Field *param_1, GameData *param_2) {
    FieldActor *actor;
    u32 result;
    actor = FindMysteryGiftDeliveryManActor(param_1);
    if (actor != NULL) {
        return (u32)GetActorUID(actor);
    }
    result = FindMysteryGiftDeliveryManNPCID(param_2);
    if ((s32)result >= 0) {
        return result & 0xffff;
    }
    return 0;
}

s32 func_ov021_021b6e0c(Field *param_1) {
    u16 flag;

    if (FindMysteryGiftDeliveryManActor(param_1) != NULL) {
        flag = 1;
    } else {
        flag = 0;
    }
    return (s32) flag;
}

#define MYSTERY_GIFT_DELIVERY_MAN_OBJ_CODE 0x46
FieldActor *FindMysteryGiftDeliveryManActor(Field *field) {
    u32 index = 0;
    FieldActor *actor;
    MMSys *actorSystem = Field_GetActorSystem(field);

    while (NextActor(actorSystem, &actor, &index) == TRUE) {
        if (FldAct_GetObjCode(actor) == MYSTERY_GIFT_DELIVERY_MAN_OBJ_CODE) {
            return actor;
        }
    }
    return NULL;
}

s32 FindMysteryGiftDeliveryManNPCID(GameData *gameData) {
    EventData *eventData;
    const ZoneNPC *npcs;
    s32 count;
    s32 i;

    eventData = GameData_GetEventData(gameData);
    npcs = GetZoneNPCs(eventData);
    count = GetZoneNPCsCount(eventData);

    if (npcs == NULL || count == 0) {
        return -1;
    }
    for (i = 0; i < count; i++) {
        if (npcs[i].modelId == MYSTERY_GIFT_DELIVERY_MAN_OBJ_CODE) {
            return npcs[i].uid;
        }
    }
    return -1;
}

u32 func_ov021_021b6eb0(MysteryGift *param_1, s32 param_2) {
    if (param_2 != 2) {
        return 0;
    }
    if (param_1->id != 0x7fe) {
        return 0;
    }
    if (param_1->value == 0x23e) {
        return 1;
    }
    return 0;
}

PartyPkm *func_ov021_021b6ee0(FieldScriptEnv *param_1, MysteryGift *param_2) {
    HeapID HVar1;
    GameData *pGVar2;

    HVar1 = FieldScriptEnv_GetHeapID(param_1);
    pGVar2 = FieldScriptEnv_GetGameData(param_1);
    return func_ov010_02157e9c(param_2, (u32)HVar1, pGVar2);
}

BOOL func_ov021_021b6f00(FieldScriptEnv *param_1, GameData *param_2, MysteryGift *param_3) {
    PokeParty *party;

    party = GameData_GetParty(param_2);
    if (PokeParty_GetPkmCount(party) < 6) {
        return 1;
    }
    return 0;
}

void func_ov021_021b6f18(FieldScriptEnv *param_1, GameData *param_2, MysteryGift *param_3) {
    PokeParty *party;
    PartyPkm *pkm;
    PokeDexSave *pokedex;

    party = GameData_GetParty(param_2);
    pkm = func_ov021_021b6ee0(param_1, param_3);
    if ((PokeParty_GetPkmCount(party) < 6) && (pkm != NULL)) {
        PokeParty_AddPkm(party, pkm);
        pokedex = GameData_GetPokedex(param_2);
        addPkmToDex(pokedex, pkm);
        GFL_HeapFree(pkm);
    }
}

u32 func_ov021_021b6f5c(FieldScriptEnv *param_1, GameData *param_2, MysteryGift *param_3) {
    PartyPkm *pkm;
    u32 result;
    u32 val;

    pkm = func_ov021_021b6ee0(param_1, param_3);
    if (pkm == NULL) {
        return 1;
    }
    result = PokeParty_GetParam(pkm, 0x4c, (void *)0x0);
    GFL_HeapFree(pkm);
    val = 2;
    if (result != 1) {
        val = 1;
    }
    return val;
}

u32 func_ov021_021b6f88(WordSet *param_1, MysteryGift *param_2, FieldScriptEnv *param_3) {
    GameData *gameData;
    PlayerInfo *playerInfo;
    PartyPkm *pkm;
    u32 val;

    gameData = FieldScriptEnv_GetGameData(param_3);
    playerInfo = GetGameDataPlayerInfo(gameData);
    pkm = func_ov021_021b6ee0(param_3, param_2);
    val = 5;
    copyVarForText(param_1, 0, playerInfo);
    if (pkm == NULL) {
        loadPokemonTextNameToStrbuf(param_1, 1, 0);
    }
    else {
        if (PokeParty_GetParam(pkm, 0x4c, (void *)0x0) == 1) {
            val = 0xb;
        }
        else {
            loadPokemonSpeciesTextNameToStrbuf(param_1, 1, pkm);
        }
        GFL_HeapFree(pkm);
    }
    return val;
}

u32 func_ov021_021b6fe8(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env) {
    return 6;
}

BOOL func_ov021_021b6fec(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift) {
    return 1;
}

void func_ov021_021b6ff0(FieldScriptEnv *param_1, GameData *param_2, MysteryGift *param_3) {
    BagSave *bag;
    u16 item;
    u16 heapId;

    heapId = FieldScriptEnv_GetHeapID(param_1);
    bag = GameData_GetBag(param_2);
    item = (u16)param_3->value;
    if ((item != 0) && ((u32) item <= 0x272U)) {
        BagSave_AddItem(bag, item, 1U, heapId);
    }
}

u32 func_ov021_021b7020(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift) {
    if (func_ov021_021b6eb0(gift, gift->kind) == 1) {
        return 4;
    }
    return 3;
}

u32 func_ov021_021b7038(WordSet *param_1, MysteryGift *param_2, FieldScriptEnv *param_3) {
    int item;
    GameData *gameData;
    PlayerInfo *playerInfo;

    gameData = FieldScriptEnv_GetGameData(param_3);
    playerInfo = GetGameDataPlayerInfo(gameData);
    item = param_2->value & 0xffff;
    copyVarForText(param_1, 0, playerInfo);
    loadItemNameToStrbuf(param_1, 1, item);
    return 7;
}

u32 func_ov021_021b7068(WordSet *param_1, MysteryGift *param_2, FieldScriptEnv *param_3) {
    loadItemNameToStrbuf(param_1, 0, param_2->value & 0xffff);
    return 8;
}

BOOL func_ov021_021b707c(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift) {
    return 1;
}

void func_ov021_021b7080(FieldScriptEnv *param_1, GameData *param_2, MysteryGift *param_3) {
    SaveControl *save;
    HighLinkSave *save_00;

    save = GameData_GetSaveControl(param_2);
    save_00 = getHighLinkBlockAddress(save);
    func_0200bf18(save_00, param_3->value);
}

u32 func_ov021_021b7098(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift) {
    return 5;
}

u32 func_ov021_021b709c(WordSet *param_1, MysteryGift *param_2, FieldScriptEnv *param_3) {
    GameData *gameData;
    SaveControl *save;
    PlayerInfo *playerInfo;
    s32 passPower;

    gameData = FieldScriptEnv_GetGameData(param_3);
    save = GameData_GetSaveControl(gameData);
    getHighLinkBlockAddress(save);
    playerInfo = GetGameDataPlayerInfo(gameData);
    passPower = param_2->value;
    copyVarForText(param_1, 0, playerInfo);
    loadPassPowerToStrbuf(param_1, 1, passPower);
    return 9;
}

u32 func_ov021_021b70d4(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env) {
    return 10;
}
