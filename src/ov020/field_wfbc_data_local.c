// Black's code at ov020 0x02186278-0x021875f4. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/city_state.h"
#include "field/field_wfbc.h"
#include "field/field_wfbc_data_local.h"
#include "field/field_g3d_mapper.h"
#include "field/field.h"
#include "field/field_script.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/game_system.h"
#include "field/field_actor.h"
#include "field/field_player.h"
#include "field/intrude_comm.h"
#include "field/intrude_work.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "nitro/fx/fx.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/wordset.h"
#include "field/player_state.h"
#include "gfl/fade.h"
#include "gfl/key.h"
#include "gfl/random.h"
#include "nitro/rtc.h"
#include "pml/poke_party.h"
#include "gfl/rtc_cache.h"
#include "field/encounter.h"


// The work of the event func_ov020_021871f8 (0x58 bytes), fields named by offset where the code names them only
typedef struct {
    GameSystem *gsys;
    Field *field;
    HeapID heapId;
    u8 unk_0a[2];
    FieldMsgBG *msgBG;
    ActorMsgWin *unk_10;
    void *unk_14;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strBuf20;
    StrBuf *strBuf24;
    FieldWFBC *unk_28;
    FieldWFBCPerson *unk_2c;
    FieldActor *actor;
    VecFx32 pos;
    CityComm *comm;
    CityState *state44;
    CityState *state48;
    u16 unk_4c;
    u16 unk_4e;
    u16 unk_50;
    u16 unk_52;
    u8 unk_54;
    u8 unk_55;
    u8 unk_56;
} FieldWfbcEventWork;

// An entry of 8 bytes: a u16 and a u32 at 4, fields named by offset
typedef struct {
    u16 unk_0;
    u32 unk_4;
} FieldWfbcEntry;

void func_ov020_02186ac8(CityState *state, PlayerInfo *player, const CityResident *resident, BOOL hasName);
u32 func_ov020_02186a58(CityState *state, u32 id);
u32 func_ov020_02186b08(const CityResident *list);
CityResident *func_ov020_02186b2c(CityResident *list, const CityResident *resident);
void func_ov020_0218677c(CityResident *resident, PlayerInfo *info);
void *func_020127f0(GameData *gameData);

GameEvent *func_ov020_021870c4(GameSystem *param_1, Field *param_2);

s32 func_ov020_02186090(FieldActor *actor);
u32 func_ov020_021871f8(u32 param_1, u32 *param_2, FieldWfbcEventWork *work);
u32 func_ov020_02186ba8(const CityResident *resident);
u32 func_ov020_02186bb8(CityResident *resident, s32 amount);
s32 func_ov020_02186a2c(CityState *state, s32 index);
void func_ov020_0218741c(FieldWfbcEventWork *work, u32 param_2, u32 *param_3, u16 param_4);
void func_ov020_0218748c(FieldWfbcEventWork *work, u32 param_2, u32 *param_3, u16 param_4, u16 param_5);
u32 func_ov020_02187508(FieldWfbcEventWork *work);
u32 func_ov020_021875b8(FieldWfbcEventWork *work);
void func_ov020_02186be0(CityResident *resident, u8 param_2);
u32 func_ov020_02186878(u16 *ids, u16 param_2, u16 param_3);
void func_ov020_021869d4(CityResidentInfo *info, u16 value);
void func_ov020_021869dc(CityResidentInfo *info, u16 value);

static const ZoneNPC data_ov020_02187dbc = { 0 };

u8 func_ov020_021867b0(const CityResident *resident);

s32 func_ov020_02186278(CityState *state, s32 param_2) {
    BOOL BVar1;
    s32 i;
    s32 j;
    CityResident *val;

    if (param_2 == 0) {
        val = state->residents;
    }
    else {
        val = state->visitors;
    }
    j = 0;
    i = 0;
    do {
        BVar1 = func_ov010_02161ecc(&val[i]);
        if (BVar1 != 0) {
            j = j + 1;
        }
        i = i + 1;
    } while (i < 10);
    return j;
}

void func_ov020_021862a8(GameData *gameData, s32 count, u16 param_3) {
    CityState *state;
    u16 *ids;
    CityResidentInfo *info;
    int zoneId;
    u32 total;
    u32 remainder;
    CityResidentDataHandle *handle;
    u32 rnd;
    PlayerState *playerState;
    CityResidentData *data;
    s32 total2;
    u32 cum;
    s32 ret;
    u16 r100;
    u16 r10;
    s32 i;
    u32 n;

    playerState = GameData_GetPlayerState(gameData);
    zoneId = PlayerState_GetZoneID(playerState);
    state = func_020127c4(gameData);
    ids = func_020127e0(gameData);
    info = func_020127f0(gameData);
    if (count <= 0) {
        count = 1;
    }
    if (count > 0x15) {
        count = 0x15;
    }
    total = func_ov020_02186278(state, 1);
    n = 0;
    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        if (func_ov010_02161ecc(&state->residents[i])) {
            state->residents[i].unk3 = 7;
            ret = func_ov020_02186bb8(&state->residents[i], count * -5);
            if (ret != 0 && total + n >= 10) {
                func_ov020_02186be0(&state->residents[i], 1);
            } else if (ret != 0) {
                if (func_02013de0(zoneId) == FALSE) {
                    func_ov020_02186b2c(state->visitors, &state->residents[i]);
                    func_ov010_02161e78(&state->residents[i]);
                } else {
                    state->residents[i].unk3 = 1;
                }
                n++;
            }
        }
    }
    func_ov010_02161e60(state, 0);
    func_ov010_02161eec(ids);
    handle = func_ov020_021867c4(0, (param_3 & 0x7FFF) | 0x8000);
    if (state->city == 1) {
        for (i = 0; i < CITY_RESIDENT_MAX; i++) {
            if (func_ov010_02161ecc(&state->residents[i])) {
                func_ov020_02186818(handle, state->residents[i].id);
                data = func_ov020_0218682c(handle);
                r100 = GFL_RandomLC(100);
                if (r100 < data->unk26) {
                    r10 = GFL_RandomLC(10);
                    func_ov020_02186878(ids, data->unk24, r10);
                }
            }
        }
        total2 = func_ov020_02186278(state, 0);
        if (total2 != 0) {
            rnd = GFL_RandomLC(total2 * 3);
            cum = 0;
            for (i = 0; i < CITY_RESIDENT_MAX; i++) {
                if (func_ov010_02161ecc(&state->residents[i])) {
                    cum += 3;
                    if (cum > rnd) {
                        func_ov020_02186818(handle, state->residents[i].id);
                        data = func_ov020_0218682c(handle);
                        remainder = rnd % 3;
                        func_ov020_021869d4(info, (&data->unk8)[remainder]);
                        func_ov020_021869dc(info, data->unk24);
                        break;
                    }
                }
            }
        }
    }
    func_ov020_02186804(handle);
}


void func_ov020_02186470(GameData *gameData) {
    PlayerState *playerState = GameData_GetPlayerState(gameData);
    int zoneId = PlayerState_GetZoneID(playerState);
    CityState *state = func_020127c4(gameData);
    int i;
    CityState *origState;
    origState = state;
    for (i = 0; i < 10; i++) {
        if (func_ov010_02161ecc(&state->residents[i]) && func_ov020_02186ba8(&state->residents[i]) && !func_02013de0(zoneId)) {
            func_ov020_02186b2c(origState->visitors, &origState->residents[i]);
            func_ov010_02161e78(&state->residents[i]);
        }
    }
    func_ov010_02161e60(state, 0);
}

void func_ov020_021864dc(CityState *state) {
    int i;

    for (i = 0; i < 10; i++) {
        if (func_ov010_02161ecc(&state->residents[i]) && (state->residents[i].unk3 & 2)) {
            func_ov020_02186bb8(&state->residents[i], 3);
            state->residents[i].unk3 = state->residents[i].unk3 & ~2;
        }
    }
}



void func_ov020_02186520(CityState *state, PlayerInfo *player, const CityResident *resident) {
    if (state->city == 0) {
        func_ov020_02186ac8(state, player, resident, 1);
        return;
    }
    func_ov020_02186ac8(state, player, resident, 0);
}

ZoneNPC *func_ov020_02186538(CityState *state, s32 param_2, u16 param_3) {
    u16 heapId;
    CityResident *list;
    s32 i;
    s32 count;
    s32 used;
    ZoneNPC *npcs;
    CityResidentData *data;
    CityResidentDataHandle *handle;
    u8 *table;

    heapId = (param_3 & 0x7FFF) | 0x8000;
    count = func_ov020_02186278(state, param_2);
    if (count == 0) {
        return NULL;
    }
    if (param_2 == 0) {
        list = state->residents;
    } else {
        list = state->visitors;
    }
    npcs = GFL_HeapAllocate(heapId, count * sizeof(ZoneNPC), 1, "field_wfbc_data_local.c", 0x2E3);
    used = 0;
    handle = func_ov020_021867c4(0, heapId);
    table = func_ov020_02186830(handle, state->city, heapId);
    for (i = 0; i < 10; i++) {
        ZoneNPC template = data_ov020_02187dbc;
        if (func_ov010_02161ecc(&list[i])) {
            ZoneNPC *npc;
            npc = &npcs[used];
            *npc = template;
            func_ov020_02186818(handle, func_ov020_021867b0(&list[i]));
            data = func_ov020_0218682c(handle);
            npc->uid = func_ov020_021867b0(&list[i]) + 0x80;
            npc->modelId = data->unk0[0];
            npc->moveCode = 3;
            if (param_2 == 0) {
                if (state->city == 0) {
                    npc->scrId = data->unk1E;
                } else {
                    npc->scrId = data->unk1C;
                }
            } else {
                if (state->city == 0) {
                    npc->scrId = data->unk22;
                } else {
                    npc->scrId = data->unk20;
                }
            }
            func_ov010_0216e618(npc, table[i * 2], table[i * 2 + 1], 0);
            used++;
        }
    }
    func_ov020_0218683c(table);
    func_ov020_02186804(handle);
    return npcs;
}


void func_ov020_02186670(CityState *state, FieldWfbcEntry *out, u16 *count, s32 start, HeapID heapId) {
    u32 total = func_ov020_02186278(state, 0);
    s32 first = start * 2;
    s32 end = first + 2;
    s32 n;
    CityResidentDataHandle *handle;
    CityResidentData *data;
    s32 i;
    if (end > total) {
        end = total;
    }
    n = end - first;
    handle = func_ov020_021867c4(0, heapId);
    data = func_ov020_0218682c(handle);
    *count = 0;
    for (i = 0; i < n; i++) {
        func_ov020_02186818(handle, state->residents[first + i].id);
        out[*count].unk_0 = data->unk28;
        out[*count].unk_4 = data->unk2A;
        (*count)++;
    }
    func_ov020_02186804(handle);
}

void func_ov020_021866f8(CityResident *resident) {
    int i;
    int found;
    u16 terminator = GFL_StrBufGetTerminator();
    if (resident->id >= 0x1e) {
        func_ov010_02161e78(resident);
    }
    if (resident->unk2 > 100) {
        func_ov010_02161e78(resident);
    }
    if (resident->hasName) {
        found = 0;
        for (i = 0; i < 8; i++) {
            if (terminator == resident->name[i]) {
                found = 1;
            }
        }
        if (!found) {
            func_ov010_02161e78(resident);
        }
    }
}



void func_ov020_02186748(CityResident *resident) {
    if (resident->unk3 & 4) {
        func_ov020_02186bb8(resident, 10);
        resident->unk3 = resident->unk3 & ~4;
    }
}

void func_ov020_02186764(CityResident *resident) {
    resident->unk3 = resident->unk3 & ~1;
}



BOOL func_ov020_02186770(CityResident *resident) {
    return (resident->unk3 & 1) != 0;
}

void func_ov020_0218677c(CityResident *resident, PlayerInfo *info) {
    resident->hasName = 1;
    resident->unk14 = getIDAsUInt(info);
    MI_CpuCopy8(GetPlayerName(info), resident->name, 0x10);
}


u8 func_ov020_021867a8(const CityResident *resident) {
    return resident->hasName;
}


u8 func_ov020_021867b0(const CityResident *resident) {
    return resident->id;
}

BOOL func_ov020_021867b4(CityResident *resident) {
    return resident->unk2 <= 20;
}


CityResidentDataHandle *func_ov020_021867c4(u32 id, HeapID heapId) {
    CityResidentDataHandle *handle;
    ArcTool *arc;

    handle = GFL_HeapAllocate(heapId, sizeof(CityResidentDataHandle), 1, "field_wfbc_data_local.c", 0x448);
    arc = GFL_ArcSysCreateFileHandle(0x9a, heapId);
    handle->arc = arc;
    GFL_ArcToolRead(arc, id, &handle->data);
    handle->id = id;
    return handle;
}

void func_ov020_02186804(CityResidentDataHandle *handle) {
    GFL_ArcToolFree(handle->arc);
    GFL_HeapFree(handle);
}

void func_ov020_02186818(CityResidentDataHandle *handle, u32 id) {
    GFL_ArcToolRead(handle->arc, id, &handle->data);
    handle->id = id;
}

CityResidentData *func_ov020_0218682c(CityResidentDataHandle *handle) {
    return &handle->data;
}

u8 *func_ov020_02186830(CityResidentDataHandle *handle, s32 city, HeapID heapId) {
    return GFL_ArcToolReadHeapNew(handle->arc, city + 0x22, heapId);
}

void func_ov020_0218683c(void *table) {
    GFL_HeapFree(table);
}


u8 *func_ov020_02186844(u8 *table, s32 index) {
    return table + index * 2;
}

u8 *func_ov020_0218684c(u8 *table, s32 index) {
    return table + (index + 10) * 2;
}

s32 func_ov020_02186854(u16 *ids) {
    s32 i;
    s32 j;

    j = 0;
    i = 0;
    do {
        if (ids[i] != 0xffff) {
            j = j + 1;
        }
        i = i + 1;
    } while (i < 10);
    return j;
}

u32 func_ov020_02186878(u16 *ids, u16 param_2, u16 param_3) {
    if (ids[param_3] == 0xffff) {
        ids[param_3] = param_2;
        return 1;
    }
    return 0;
}

u16 func_ov020_02186898(u16 *ids, s32 index) {
    return ids[index];
}

u32 func_ov020_021868a0(u16 *ids, s32 index) {
    if (ids[index] != 0xffff) {
        return 1;
    }
    return 0;
}

static const ZoneNPC data_ov020_02187de0 = { 0, 0x6e, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, { 0 } };

ZoneNPC *func_ov020_021868b8(u16 *param_1, s32 param_2, s32 param_3, u16 param_4) {
    ZoneNPC template;
    ZoneNPC *npcs;
    ZoneNPC *npc;
    CityResidentDataHandle *handle;
    u8 *table;
    u8 *entry;
    u16 heapId;
    s32 count;
    s32 used;
    s32 i;

    if (param_2 != 0) {
        return NULL;
    }
    if (param_3 == 0) {
        return NULL;
    }
    heapId = (param_4 & 0x7FFF) | 0x8000;
    count = func_ov020_02186854(param_1);
    used = 0;
    npcs = GFL_HeapAllocate(heapId, count * sizeof(ZoneNPC), 1, "field_wfbc_data_local.c", 0x561);
    handle = func_ov020_021867c4(0, heapId);
    table = func_ov020_02186830(handle, param_3, heapId);
    i = 0;
    template = data_ov020_02187de0;
    while (i < 10) {
        if (func_ov020_021868a0(param_1, i) != 0) {
            entry = func_ov020_0218684c(table, i);
            npc = &npcs[used];
            *npc = template;
            npc->uid = i + 0xa0;
            npc->scrId = func_ov020_02186898(param_1, i);
            npc->spawnFlag = i + 0xaa0;
            func_ov010_0216e618(npc, entry[0], entry[1], 0);
            used++;
        }
        i++;
    }
    func_ov020_0218683c(table);
    func_ov020_02186804(handle);
    return npcs;
}


u16 func_ov020_021869ac(CityResidentInfo *info) {
    return info->unk0;
}

void func_ov020_021869b0(CityResidentInfo *info) {
    info->unk0 = info->unk0 + 1;
}

u16 func_ov020_021869b8(CityResidentInfo *info) {
    return info->unk2;
}

void func_ov020_021869bc(CityResidentInfo *info) {
    if (info->unk2 < 10000) {
        info->unk2 = info->unk2 + 10;
    }
}

u16 func_ov020_021869d0(CityResidentInfo *info) {
    return info->unk2;
}

void func_ov020_021869d4(CityResidentInfo *info, u16 value) {
    info->unk2 = value;
}

u16 func_ov020_021869d8(CityResidentInfo *info) {
    return info->unk0;
}

void func_ov020_021869dc(CityResidentInfo *info, u16 value) {
    info->unk0 = value;
}

CityResidentData *func_ov020_021869e0(CityState *state, u32 limit, CityResidentDataHandle *handle, u32 *out) {
    s32 i;
    u32 total = 0;
    CityResidentData *data = func_ov020_0218682c(handle);
    for (i = 0; i < 30; i++) {
        s32 id = func_ov020_02186a2c(state, i);
        func_ov020_02186818(handle, id);
        total += data->unk2C;
        if (total > limit) {
            *out = id;
            return data;
        }
    }
    return data;
}


s32 func_ov020_02186a2c(CityState *state, s32 index) {
    u32 id;
    s32 i;

    i = 0;
    id = 0;
    do {
        if (func_ov020_02186a58(state, id) == 0) {
            if (i == index) {
                return id;
            }
            i = i + 1;
        }
        id = id + 1;
    } while ((s32)id < 0x1e);
    return 0;
}

u32 func_ov020_02186a58(CityState *state, u32 id) {
    int i;
    for (i = 0; i < 10; i++) {
        if (func_ov010_02161ecc(&state->residents[i]) && id == state->residents[i].id) {
            return 1;
        }
    }
    for (i = 0; i < 10; i++) {
        if (func_ov010_02161ecc(&state->visitors[i]) && id == state->visitors[i].id) {
            return 1;
        }
    }
    return 0;
}


void func_ov020_02186ac8(CityState *state, PlayerInfo *player, const CityResident *resident, BOOL hasName) {
    CityResident *added;

    if (func_ov020_02186b08(state->residents) == 1) {
        func_ov020_02186b2c(state->visitors, &state->residents[CITY_RESIDENT_MAX - 1]);
        func_ov010_02161e78(&state->residents[CITY_RESIDENT_MAX - 1]);
    }
    added = func_ov020_02186b2c(state->residents, resident);
    if (hasName != 0) {
        func_ov020_0218677c(added, player);
    }
}

u32 func_ov020_02186b08(const CityResident *list) {
    BOOL BVar1;
    s32 i;

    i = 0;
    do {
        BVar1 = func_ov010_02161ecc(&list[i]);
        if (BVar1 == 0) {
            return 0;
        }
        i = i + 1;
    } while (i < 10);
    return 1;
}

CityResident *func_ov020_02186b2c(CityResident *list, const CityResident *resident) {
    int i;
    for (i = 9; i > 0; i--) {
        MI_CpuCopy8(&list[i - 1], &list[i], 0x18);
    }
    MI_CpuCopy8(resident, list, 0x18);
    return list;
}


CityResident *func_ov020_02186b60(CityResident *list) {
    BOOL BVar1;
    s32 i;

    i = 0;
    do {
        BVar1 = func_ov010_02161ecc(&list[i]);
        if (BVar1 == 0) {
            return &list[i];
        }
        i = i + 1;
    } while (i < 10);
    return list;
}

void func_ov020_02186b84(CityResident *resident, u8 id, PlayerInfo *info, u8 unk2, s32 flag) {
    resident->exists = 1;
    resident->id = id;
    resident->unk3 = 7;
    resident->unk2 = unk2;
    if (flag) {
        func_ov020_0218677c(resident, info);
    }
}


u32 func_ov020_02186ba8(const CityResident *resident) {
    if (resident->unk2 == 0) {
        return 1;
    }
    return 0;
}

u32 func_ov020_02186bb8(CityResident *resident, s32 amount) {
    u32 result = 0;
    s32 sum = resident->unk2 + amount;
    if (sum > 100) {
        resident->unk2 = 100;
    } else if (sum < 0) {
        resident->unk2 = 0;
        result = 1;
    } else {
        resident->unk2 += amount;
    }
    return result;
}


void func_ov020_02186be0(CityResident *resident, u8 param_2) {
    resident->unk2 = param_2;
}

u32 func_ov020_02186be4(u32 param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    ScriptFieldWork *fieldWork;
    FieldG3DMapper *mapper;
    FieldWFBC *ptr;
    FieldPlayer *player;
    MMSys *system;
    s16 sStack_16;
    s16 sStack_18;
    FieldActor *actor;
    s16 asStack_14 [2];


    work = FieldScriptEnv_GetScriptWork(param_2);
    fieldWork = ScriptWork_GetFieldWork(work);
    mapper = Field_GetG3DMapper(fieldWork->field);
    ptr = func_ov021_0218dc70(mapper);
    player = Field_GetPlayer(fieldWork->field);
    system = Field_GetActorSystem(fieldWork->field);
    FieldPlayer_GetGPosInFront(player, asStack_14, &sStack_16, &sStack_18);
    actor = GetFirstActorOnGPos(system, asStack_14[0], sStack_18, 0);
    func_ov021_02193b40(ptr, func_ov020_02186090(actor));
    return 0;
}

u32 func_ov020_02186c44(u32 param_1, FieldScriptEnv *env) {
    ScriptWork *work;
    ScriptFieldWork *fieldWork;
    FieldG3DMapper *mapper;
    FieldWFBC *handle;
    GameData *gameData;
    CityResidentInfo *info;
    FieldPlayer *player;
    MMSys *system;
    FieldActor *actor;
    s16 x;
    s16 y;
    s16 z;
    u32 id;

    work = FieldScriptEnv_GetScriptWork(env);
    fieldWork = ScriptWork_GetFieldWork(work);
    mapper = Field_GetG3DMapper(fieldWork->field);
    handle = func_ov021_0218dc70(mapper);
    gameData = FieldScriptEnv_GetGameData(env);
    info = func_020127f0(gameData);
    player = Field_GetPlayer(fieldWork->field);
    system = Field_GetActorSystem(fieldWork->field);
    FieldPlayer_GetGPosInFront(player, &x, &y, &z);
    actor = GetFirstActorOnGPos(system, x, z, 0);
    id = func_ov020_02186090(actor);
    if (func_ov021_02193be4(handle, id)) {
        func_ov021_02193bc4(handle, id);
        func_ov020_021869b0(info);
    }
    return 0;
}


u32 func_ov020_02186cc4(VM *param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    ScriptFieldWork *fieldWork;
    FieldG3DMapper *mapper;
    FieldWFBC *ptr;
    FieldPlayer *player;
    MMSys *system;
    WordSet *pWVar5;
    u16 result;
    HeapID HVar2;
    s16 sStack_1e;
    s16 sStack_20;
    FieldActor *actor;
    s16 asStack_1c [2];


    work = FieldScriptEnv_GetScriptWork(param_2);
    fieldWork = ScriptWork_GetFieldWork(work);
    mapper = Field_GetG3DMapper(fieldWork->field);
    ptr = func_ov021_0218dc70(mapper);
    player = Field_GetPlayer(fieldWork->field);
    system = Field_GetActorSystem(fieldWork->field);
    pWVar5 = ScriptWork_GetWordSet(work);
    result = ScriptReadAny(param_1, param_2);
    HVar2 = FieldScriptEnv_GetHeapID(param_2);
    FieldPlayer_GetGPosInFront(player, asStack_1c, &sStack_1e, &sStack_20);
    actor = GetFirstActorOnGPos(system, asStack_1c[0], sStack_20, 0);
    func_ov021_02193b68(ptr, pWVar5, func_ov020_02186090(actor), (u32)result, HVar2);
    return 0;
}

u32 func_ov020_02186d48(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    GameData *gameData;
    CityState *state;
    u16 kind;
    u16 *result;
    ScriptFieldWork *fieldWork;
    FieldG3DMapper *mapper;
    FieldWFBC *mapEncount;
    FieldPlayer *player;
    MMSys *system;
    CityResidentInfo *info;
    FieldActor *actor;
    u32 id;
    u16 uid;
    FieldWFBCPerson *slot;
    CityResident *entry;
    s16 x;
    s16 y;
    s16 z;

    work = FieldScriptEnv_GetScriptWork(env);
    gameData = FieldScriptEnv_GetGameData(env);
    state = func_020127c4(gameData);
    kind = ScriptReadAny(vm, env);
    result = ScriptReadVar(vm, env);
    if (kind == 3) {
        *result = func_ov020_02186278(state, 0);
    } else {
        fieldWork = ScriptWork_GetFieldWork(work);
        mapper = Field_GetG3DMapper(fieldWork->field);
        mapEncount = func_ov021_0218dc70(mapper);
        player = Field_GetPlayer(fieldWork->field);
        system = Field_GetActorSystem(fieldWork->field);
        info = func_020127f0(gameData);
        u32 *slotA;
        FieldPlayer_GetGPosInFront(player, &x, &y, &z);
        actor = GetFirstActorOnGPos(system, x, z, 0);
        if (actor != NULL) {
            id = func_ov020_02186090(actor);
            uid = GetActorUID(actor);
            func_ov021_02193904(mapEncount);
            slot = func_ov021_0219390c(mapEncount, id);
            if (slot != NULL) {
                slotA = (u32 *)func_ov021_02193c00(slot);
                entry = func_ov021_02193c04(slot);
            }
        }
        switch (kind) {
        case 0:
            *result = slotA[6];
            break;
        case 1:
            *result = func_ov020_021867b4(entry);
            break;
        case 2:
            *result = func_ov020_021867a8(entry);
            break;
        case 4:
            *result = func_ov020_021869ac(info);
            break;
        case 5:
            *result = func_ov020_021869b8(info);
            break;
        case 6:
            if (func_ov021_021938f4(mapEncount) == 0) {
                *result = 0;
            } else {
                *result = 1;
            }
            break;
        case 7:
            *result = func_ov020_021869d8(info);
            break;
        case 8:
            *result = func_ov020_021869d0(info);
            break;
        case 9:
            *result = id;
            break;
        case 10:
            *result = func_ov021_02193be4(mapEncount, id);
            break;
        case 11:
            *result = uid;
            break;
        }
    }
    return 0;
}


u32 func_ov020_02186e94(u32 param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    ScriptFieldWork *fieldWork;
    GameSystem *gsys;
    GameData *gameData;
    CityResidentInfo *info;

    work = FieldScriptEnv_GetScriptWork(param_2);
    fieldWork = ScriptWork_GetFieldWork(work);
    gsys = Field_GetGameSystem(fieldWork->field);
    gameData = GSYS_GetGameData(gsys);
    info = func_020127f0(gameData);
    func_ov020_021869bc(info);
    return 0;
}

u32 func_ov020_02186eb8(VM *vm, FieldScriptEnv *env) {
    GameData *gameData;
    CityResidentInfo *info;
    PokeParty *party;
    u16 *ptr1;
    u16 *ptr2;
    u16 *ptr3;
    u32 count;
    u32 zoneId;
    RTCDate date;
    u32 index;
    PartyPkm *pkm;
    u32 result;
    u32 result2;
    u32 result3;
    u32 result4;
    u32 result5;
    u32 result6;
    u32 result7;
    u32 result8;
    u32 result9;

    FieldScriptEnv_GetScriptWork(env);
    gameData = FieldScriptEnv_GetGameData(env);
    func_020127c4(gameData);
    info = func_020127f0(gameData);
    party = GameData_GetParty(gameData);
    ptr1 = ScriptReadVar(vm, env);
    ptr2 = ScriptReadVar(vm, env);
    ptr3 = ScriptReadVar(vm, env);
    count = PokeParty_GetPkmCount(party);
    zoneId = func_ov020_021869d0(info);
    RTC_GetCachedDate(&date);
    *ptr1 = 0;
    *ptr2 = 0;
    *ptr3 = 0xFFFF;
    for (index = 0; index < count; index++) {
        pkm = PokeParty_GetPkm(party, index);
        result = PokeParty_GetParam(pkm, 5, NULL);
        result2 = PokeParty_GetParam(pkm, 0x92, NULL);
        result3 = PokeParty_GetParam(pkm, 0x93, NULL);
        result4 = PokeParty_GetParam(pkm, 0x94, NULL);
        result5 = PokeParty_GetParam(pkm, 0x96, NULL);
        result6 = PokeParty_GetParam(pkm, 0x4c, NULL);
        result7 = PokeParty_GetParam(pkm, 0x8f, NULL);
        result8 = PokeParty_GetParam(pkm, 0x90, NULL);
        result9 = PokeParty_GetParam(pkm, 0x91, NULL);
        if (result == zoneId && result6 == 0 && result5 == 0x33 && result7 + result9 + result8 == 0) {
            *ptr1 = 1;
            if (*ptr3 == 0xFFFF) {
                *ptr3 = index;
            }
            if (result2 == date.year && result3 == date.month && result4 == date.day) {
                *ptr2 = 1;
                *ptr3 = index;
            }
        }
    }
    return 0;
}


u32 func_ov020_02187000(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    ScriptFieldWork *fieldWork;
    FieldG3DMapper *mapper;
    FieldWFBC *handle;
    FieldPlayer *player;
    MMSys *system;
    u16 *var;
    u16 value;
    s16 x;
    s16 y;
    s16 z;
    FieldActor *actor;
    s32 id;

    work = FieldScriptEnv_GetScriptWork(env);
    fieldWork = ScriptWork_GetFieldWork(work);
    mapper = Field_GetG3DMapper(fieldWork->field);
    handle = func_ov021_0218dc70(mapper);
    player = Field_GetPlayer(fieldWork->field);
    system = Field_GetActorSystem(fieldWork->field);
    ScriptWork_GetWordSet(work);
    var = ScriptReadVar(vm, env);
    value = ScriptReadAny(vm, env);
    FieldPlayer_GetGPosInFront(player, &x, &y, &z);
    actor = GetFirstActorOnGPos(system, x, z, 0);
    id = func_ov020_02186090(actor);
    value--;
    if (func_ov021_0219391c(handle) == 0) {
        *var = value + (id * 4 + 1);
    } else {
        *var = value + (id * 4 + 9);
    }
    return 0;
}


u32 func_ov020_02187098(u32 param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    Field *pFVar1;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    pFVar1 = GSYS_GetField(gsys);
    event = func_ov020_021870c4(gsys, pFVar1);
    ScriptWork_CallEvent(work, event);
    return 1;
}

GameEvent *func_ov020_021870c4(GameSystem *param_1, Field *param_2) {
    u16 heapId;
    void *manager;
    GameData *pGVar2;
    GameCommSys *commSys;
    IntrudeComm *work;
    void *ptr;
    GameEvent *event;
    FieldWfbcEventWork *data;
    StrBuf *pSVar5;
    MsgData *pMVar6;
    WordSet *pWVar7;
    CityState *pCVar8;
    FieldG3DMapper *mapper;
    FieldPlayer *player;
    MMSys *system;
    s16 sStack_1a;
    s16 sStack_1c;
    FieldActor *actor;
    u32 index;
    FieldWFBCPerson *result;
    s16 asStack_18 [2];


    heapId = Field_GetHeapID(param_2);
    manager = Field_GetMsgBGSys(param_2);
    pGVar2 = GSYS_GetGameData(param_1);
    commSys = GSYS_GetGameCommSystem(param_1);
    work = func_ov010_02158344(commSys);
    if (work == (IntrudeComm *)0x0) {
        return (GameEvent *)0x0;
    }
    ptr = func_ov010_02158434(work);
    event = GameEvent_Create(param_1, (GameEvent *)0x0, (void *)func_ov020_021871f8, sizeof(FieldWfbcEventWork));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    pSVar5 = GFL_StrBufCreate(0x100, heapId);
    data->strBuf20 = pSVar5;
    pSVar5 = GFL_StrBufCreate(0x100, heapId);
    data->strBuf24 = pSVar5;
    pMVar6 = GFL_MsgSysLoadData(0, 3, 0x142, heapId);
    data->msgData = pMVar6;
    pWVar7 = GFL_WordSetSystemCreateDefault(heapId);
    data->wordSet = pWVar7;
    data->msgBG = manager;
    data->comm = ptr;
    data->unk_56 = 1;
    data->heapId = heapId & 0x7fff | 0x8000;
    pCVar8 = func_020127d0(pGVar2, 1);
    data->state44 = pCVar8;
    pCVar8 = func_020127d0(pGVar2, 0);
    data->state48 = pCVar8;
    mapper = Field_GetG3DMapper(param_2);
    manager = func_ov021_0218dc70(mapper);
    player = Field_GetPlayer(param_2);
    system = Field_GetActorSystem(param_2);
    pGVar2 = GSYS_GetGameData(param_1);
    func_020127f0(pGVar2);
    FieldPlayer_GetGPosInFront(player, asStack_18, &sStack_1a, &sStack_1c);
    actor = GetFirstActorOnGPos(system, asStack_18[0], sStack_1c, 0);
    CopyActorWPos(actor, &data->pos);
    index = func_ov020_02186090(actor);
    result = func_ov021_0219390c(manager, index);
    data->unk_28 = manager;
    data->unk_2c = result;
    data->actor = actor;
    data->unk_4c = (s16)index;
    return event;
}

u32 func_ov020_021871f8(u32 param_1, u32 *param_2, FieldWfbcEventWork *work) {
    GameCommSys *commSys;
    GameData *gameData;
    PlayerInfo *player;
    u32 count;

    commSys = GSYS_GetGameCommSystem(work->gsys);
    if (func_ov010_02158344(commSys) == NULL && work->unk_56 != 0) {
        *param_2 = 0x11;
    }
    switch (*param_2) {
    case 0:
        func_ov010_021620a0(work->comm);
        func_ov010_02162048(work->comm, work->unk_4c, 0);
        *param_2 = 1;
        break;
    case 1:
        if (func_ov010_0216208c(work->comm, work->unk_4c) != FALSE) {
            if (func_ov010_0216209c(work->comm) == 0) {
                *param_2 = 3;
            } else {
                *param_2 = 2;
            }
        }
        break;
    case 2:
        func_ov020_0218741c(work, 0, param_2, 0x11);
        break;
    case 3:
        if (work->state44->city != work->state48->city) {
            *param_2 = 5;
        } else {
            *param_2 = 4;
        }
        break;
    case 4:
        func_ov020_0218741c(work, 1, param_2, 0x11);
        break;
    case 5:
        count = func_ov020_02186278(work->state44, 0);
        if (count >= 10) {
            *param_2 = 6;
        } else if (func_ov010_02161e4c(work->state44, work->unk_4c, 2)) {
            *param_2 = 6;
        } else {
            *param_2 = 7;
        }
        break;
    case 6:
        func_ov020_0218741c(work, 2, param_2, 0x11);
        break;
    case 7:
        func_ov020_0218748c(work, 3, param_2, 9, 8);
        break;
    case 8:
        func_ov020_0218741c(work, 4, param_2, 0x11);
        break;
    case 9:
        func_ov010_021620a0(work->comm);
        func_ov010_02162048(work->comm, work->unk_4c, 1);
        *param_2 = 10;
        break;
    case 10:
        if (func_ov010_0216208c(work->comm, work->unk_4c) != FALSE) {
            if (func_ov010_0216209c(work->comm) == 2) {
                *param_2 = 0xc;
            } else {
                *param_2 = 0xb;
            }
        }
        break;
    case 0xb:
        work->unk_56 = 0;
        func_ov020_0218741c(work, 5, param_2, 0x11);
        break;
    case 0xc:
        func_ov020_0218741c(work, 7, param_2, 0xd);
        func_ov010_021620a0(work->comm);
        func_ov010_02162048(work->comm, work->unk_4c, 2);
        work->unk_56 = 0;
        break;
    case 0xd:
        GFL_FadeSet(1, 0, 0x10, 0);
        *param_2 = 0xe;
        break;
    case 0xe:
        if (!GFL_FadeIsRunning()) {
            gameData = GSYS_GetGameData(work->gsys);
            player = GetGameDataPlayerInfo(gameData);
            func_ov010_02161f90(work->comm, work->state44, work->state48, player, work->unk_4c, work->heapId);
            func_ov021_02193b20(work->unk_28, work->unk_4c);
            DeleteActor(work->actor);
            GFL_FadeSet(1, 0x10, 0, 0);
            *param_2 = 0xf;
        }
        break;
    case 0xf:
        if (!GFL_FadeIsRunning()) {
            *param_2 = 0x11;
        }
        break;
    case 0x10:
        if (func_ov020_02187508(work) != FALSE) {
            if (work->unk_54 != 0) {
                *param_2 = work->unk_4e;
            } else {
                *param_2 = work->unk_50;
            }
        }
        break;
    case 0x11:
        if (func_ov020_021875b8(work) != 0) {
            GFL_StrBufFree(work->strBuf20);
            GFL_StrBufFree(work->strBuf24);
            GFL_MsgDataFree(work->msgData);
            GFL_WordSetSystemFree(work->wordSet);
            return 1;
        }
        break;
    }
    return 0;
}


static const u16 data_ov020_02187e04[2] = { 0x00f1, 0x0001 };

void func_ov020_0218741c(FieldWfbcEventWork *work, u32 param_2, u32 *param_3, u16 param_4) {
    u16 messageId = param_2 + (data_ov020_02187e04[work->state48->city] + work->unk_4c * 8);
    GFL_MsgDataLoadStrbuf(work->msgData, messageId, work->strBuf24);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf20, work->strBuf24);
    work->unk_10 = ActorMsgWin_CheckAndCreate(work->msgBG, 2, &work->pos, work->strBuf20, 0, 0);
    *param_3 = 0x10;
    work->unk_4e = param_4;
    work->unk_52 = 0;
    work->unk_54 = 1;
    work->unk_55 = 0;
}

void func_ov020_0218748c(FieldWfbcEventWork *work, u32 param_2, u32 *param_3, u16 param_4, u16 param_5) {
    u16 messageId = param_2 + (data_ov020_02187e04[work->state48->city] + work->unk_4c * 8);
    GFL_MsgDataLoadStrbuf(work->msgData, messageId, work->strBuf24);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf20, work->strBuf24);
    work->unk_10 = ActorMsgWin_CheckAndCreate(work->msgBG, 2, &work->pos, work->strBuf20, 0, 0);
    *param_3 = 0x10;
    work->unk_4e = param_4;
    work->unk_50 = param_5;
    work->unk_52 = 0;
    work->unk_54 = 1;
    work->unk_55 = 1;
}

u32 func_ov020_02187508(FieldWfbcEventWork *work) {
    u32 keys;
    u32 result;

    switch (work->unk_52) {
    case 0:
        if (func_ov021_02191388(work->unk_10) != FALSE) {
            work->unk_52++;
        }
        break;
    case 1:
        keys = GCTX_HIDGetPressedKeys();
        if ((keys & 0xf3) != 0) {
            if (work->unk_55 == 0) {
                work->unk_52 = 4;
            } else {
                work->unk_52 = 2;
            }
        }
        break;
    case 2:
        work->unk_14 = func_ov021_02190be4(work->msgBG, 0);
        work->unk_52++;
        break;
    case 3:
        result = func_ov021_02190c78(work->unk_14);
        if (result != 2) {
            if (result == 0) {
                work->unk_54 = 1;
            } else {
                work->unk_54 = 0;
            }
            work->unk_52 = 4;
            func_ov021_021909bc(work->unk_14);
            work->unk_14 = NULL;
        }
        break;
    case 4:
        func_ov021_021912d8(work->unk_10);
        work->unk_52++;
        break;
    case 5:
        if (func_ov021_021912f8(work->unk_10) != FALSE) {
            work->unk_10 = NULL;
            return 1;
        }
        break;
    }
    return 0;
}




u32 func_ov020_021875b8(FieldWfbcEventWork *work) {
    if (work->unk_10 != NULL) {
        if (work->unk_52 == 5) {
            if (func_ov021_021912f8(work->unk_10) == FALSE) {
                return 0;
            }
            work->unk_10 = NULL;
        } else {
            func_ov021_0219131c(work->unk_10);
            work->unk_10 = NULL;
        }
    }
    if (work->unk_14 != NULL) {
        func_ov021_021909bc(work->unk_14);
        work->unk_14 = NULL;
    }
    return 1;
}

