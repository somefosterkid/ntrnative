// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov104/field_gimmick_gate.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "field/encounter.h"
#include "field/field.h"
#include "field/field_environment.h"
#include "field/field_exp_obj.h"
#include "field/field_gimmick_gate.h"
#include "field/gimmick_obj_elboard.h"
#include "field/field_map.h"
#include "field/field_script.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/calctool.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/hall_of_fame.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/rtc.h"
#include "system/version.h"
#include "system/wordset.h"
#include "text/system/gym_insect.h"
#include "field/move_pokemon.h"
#include "save/trainer_data_savedata.h"

struct GimmickGateMessageList {
    u8 count;
    u8 padding[3];
    u32 kinds[7];
    u32 flags[7];
};

struct GimmickGateSave {
    u32 value;
    u32 boardTime;
    u16 flag;
    u16 padding;
    GimmickGateMessageList messages;
};

struct GimmickGateZoneList {
    u16 zones[4];
    u8 weather[4];
};

struct GimmickGateWork {
    u16 heapId;
    u16 pad02;
    Field *field;
    GameData *gameData;
    GameSystem *gameSystem;
    u32 value;
    Elboard *board;
    u32 boardTime;
    GimmickGateZoneData *zoneData;
    GimmickGateBoardEntry *boardEntries;
    u8 boardEntryCount;
    u8 unk25[3];
    GimmickGateMessageList messages;
    u16 flag;
};

void func_ov048_021f37c4(GimmickGateWork *work);
void func_ov048_021f37fc(GimmickGateWork *work);
void func_ov048_021f3818(GimmickGateWork *work);
void func_ov048_021f3830(GimmickGateWork *work);
void func_ov048_021f3870(GimmickGateWork *work);
GimmickGateWork *func_ov048_021f3880(Field *field);
void func_ov048_021f3908(GimmickGateWork *work);
void func_ov048_021f39a8(GimmickGateWork *work);
void func_ov048_021f39bc(GimmickGateWork *work);
void func_ov048_021f39e4(GimmickGateWork *work);
void func_ov048_021f3a50(GimmickGateWork *work, u32 kind, u32 flag);
u32 func_ov048_021f3a70(GimmickGateWork *work);
u32 func_ov048_021f3a98(GimmickGateWork *work);
void func_ov048_021f3ad4(const GimmickGateWork *work, u32 *species, s32 *count);
void func_ov048_021f3c14(GimmickGateWork *work);
GimmickGateBoardEntry *func_ov048_021f3c2c(GimmickGateWork *work);
GimmickGateBoardEntry *func_ov048_021f3cb0(GimmickGateWork *work);
u32 func_ov048_021f3d24(GimmickGateWork *work);
void func_ov048_021f3d38(GimmickGateWork *work, ElboardMessageArg *message, u32 kind, GimmickGateBoardEntry *entry);
void func_ov048_021f3d78(GimmickGateWork *work);
void func_ov048_021f3da8(GimmickGateWork *work);
void func_ov048_021f3df0(GimmickGateWork *work);
void func_ov048_021f3e2c(GimmickGateWork *work);
void func_ov048_021f3e6c(GimmickGateWork *work);
void func_ov048_021f3ee8(GimmickGateWork *work);
void func_ov048_021f4058(GimmickGateWork *work);
void func_ov048_021f4104(GimmickGateWork *work);
void func_ov048_021f4188(GimmickGateWork *work);
void func_ov048_021f420c(GimmickGateWork *work);
void func_ov048_021f4290(GimmickGateWork *work);
void func_ov048_021f4314(GimmickGateWork *work);
void func_ov048_021f43e4(GimmickGateWork *work);
void func_ov048_021f440c(GimmickGateWork *work, GimmickGateBoardEntry *entry, u32 index);
void func_ov048_021f4454(GimmickGateWork *work);
s32 func_ov048_021f4488(const GimmickGateZoneList *list);
void func_ov048_021f44b8(GimmickGateZoneList *list);
void func_ov048_021f44d8(GimmickGateWork *work, GimmickGateZoneList *list);
void func_ov048_021f4590(GimmickGateWork *work, GimmickGateZoneList *list);
void func_ov048_021f45f0(GimmickGateWork *work, GimmickGateZoneList *list);
BOOL func_ov048_021f4650(VM *vm, FieldScriptEnv *env);
void func_ov048_021f38ec(GimmickGateWork *work);
void func_ov048_021f392c(GimmickGateWork *work);
GimmickGateSave *func_ov048_021f37a4(Field *field);

// The board's scenes
typedef enum GimmickGateScene {
    GATE_SCENE_0,
    GATE_SCENE_1,
    GATE_SCENE_COUNT,
} GimmickGateScene;

// The board advances by this each frame
// pokebw2's sBoardStep
static fx32 data_ov048_021f50c0 = FX32_ONE;

// pokebw2's sScene0Animations
static const G3DSceneAnimationSetup data_ov048_021f4eb4[] = {
#include "asset/field_gimmick_gate_data_ov048_021f4eb4.inc"
};

// pokebw2's sScene1Animations
static const G3DSceneAnimationSetup data_ov048_021f4f04[] = {
#include "asset/field_gimmick_gate_data_ov048_021f4f04.inc"
};

// pokebw2's sScene0Actors
static const G3DSceneActorSetup data_ov048_021f4e64[] = {
    { 0, 0, 1, 0, data_ov048_021f4eb4, NELEMS(data_ov048_021f4eb4) },
};

// The zones whose weather the boards report, and the weathers they report
// pokebw2's sWeatherZones
static const u16 data_ov048_021f4e5c[] = { 0x71 };

// pokebw2's sScene1Actors
static const G3DSceneActorSetup data_ov048_021f4e74[] = {
    { 0, 0, 1, 0, data_ov048_021f4f04, NELEMS(data_ov048_021f4f04) },
};

// pokebw2's sMessageKinds
static u16 data_ov048_021f50c4[7] = { 0, 1, 2, 3, 4, 5, 6 };

// pokebw2's sBoardNames
static const char *data_ov048_021f50ec[7] = {
    "gelboard_1", "gelboard_2", "gelboard_3", "gelboard_4", "gelboard_5", "gelboard_6", "gelboard_7",
};
// pokebw2's sBoardPlNames
static const char *data_ov048_021f5108[7] = {
    "gelboard_1_pl", "gelboard_2_pl", "gelboard_3_pl", "gelboard_4_pl",
    "gelboard_5_pl", "gelboard_6_pl", "gelboard_7_pl",
};

// Messages for the weathers, as GimmickGateZoneList gives them
// pokebw2's sWeatherMessages
static u32 data_ov048_021f5124[15] = {
#include "asset/field_gimmick_gate_data_ov048_021f5124.inc"
};

// pokebw2's sEntrySlots
static const u32 data_ov048_021f4e84[4] = { 2, 3, 4, 5 };

// pokebw2's sReportedWeathers
static const u8 data_ov048_021f4e5e[] = { 6, 7, 8 };

// pokebw2's sSceneResources
static const G3DSceneResourceSetup data_ov048_021f4f54[] = {
#include "asset/field_gimmick_gate_data_ov048_021f4f54.inc"
};

// pokebw2's sBoardAnimations
static u16 data_ov048_021f50d2[13] = {
#include "asset/field_gimmick_gate_data_ov048_021f50d2.inc"
};

// pokebw2's sSceneSetups
static const G3DSceneSetup data_ov048_021f4e94[GATE_SCENE_COUNT] = {
    { data_ov048_021f4f54, NELEMS(data_ov048_021f4f54), data_ov048_021f4e64, NELEMS(data_ov048_021f4e64) },
    { data_ov048_021f4f54, NELEMS(data_ov048_021f4f54), data_ov048_021f4e74, NELEMS(data_ov048_021f4e74) },
};

void func_ov048_021f3bc0(GimmickGateWork *work);

void func_ov048_021f36c0(Field *field) {
    FieldExpObjSystem *expObj = Field_GetExpObjSystem(field);
    GimmickGateWork *work;
    GimmickGateScene scene;

    switch (func_020083e4((Config *)getTrainerDataBlkAddress(GameData_GetSaveControl(GSYS_GetGameData(Field_GetGameSystem(field)))))) {
    case 1:
        scene = GATE_SCENE_0;
        break;
    case 0:
        scene = GATE_SCENE_1;
        break;
    }
    LoadFieldExpandObjData(expObj, &data_ov048_021f4e94[scene], 1);
    work = func_ov048_021f3880(field);
    func_ov048_021f392c(work);
    func_ov048_021f3bc0(work);
    func_ov048_021f37fc(work);
    if (work->value == 0) {
        func_ov048_021f3908(work);
    } else {
        func_ov048_021f3818(work);
        func_ov048_021f3830(work);
    }
    func_ov048_021f3d78(work);
    func_ov048_021f3870(work);
    func_ov048_021f39e4(work);
    func_ov048_021f39bc(work);
}

void func_ov048_021f374c(Field *field) {
    GimmickGateWork *work;

    work = Field_GetGimmickWorkBlock(field, 0);
    func_ov048_021f37c4(work);
    if (work->board != 0) {
        func_ov048_021f474c(work->board);
    }
    func_ov048_021f38ec(work);
}

void func_ov048_021f376c(Field *field) {
    GimmickGateWork *work;

    work = Field_GetGimmickWorkBlock(field, 0);
    func_ov048_021f4784(work->board, data_ov048_021f50c0);
    FieldExpObj_StepAllAnimations(Field_GetExpObjSystem(field));
}

u32 func_ov048_021f3790(Field *field) {
    GimmickGateWork *work;
    GimmickGateZoneData *zoneData;

    work = Field_GetGimmickWorkBlock(field, 0);
    zoneData = work->zoneData;
    return (u8)zoneData->direction;
}

GimmickGateSave *func_ov048_021f37a4(Field *field) {
    GimmickState *state;
    u32 id;

    state = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    id = GimmickState_GetID(state);
    return GimmickState_GetUserData(state, id);
}

void func_ov048_021f37c4(GimmickGateWork *work) {
    GimmickGateSave *data;

    data = func_ov048_021f37a4(work->field);
    data->value = work->value;
    data->boardTime = func_ov048_021f47b0(work->board) >> 12;
    data->flag = work->flag;
    data->messages = work->messages;
}

void func_ov048_021f37fc(GimmickGateWork *work) {
    GimmickGateSave *data;

    data = func_ov048_021f37a4(work->field);
    work->value = data->value;
    work->boardTime = data->boardTime;
    work->flag = data->flag;
}

void func_ov048_021f3818(GimmickGateWork *work) {
    SaveControl *save;
    TrainerCardSave *card;

    save = GameData_GetSaveControl(work->gameData);
    card = getTrainerGameInfoAddress(save);
    func_0200c3b8(card, work->flag);
}

void func_ov048_021f3830(GimmickGateWork *work) {
    EventWork *eventWork;
    s32 i;
    GimmickGateMessageList *list;

    eventWork = GameData_GetEventWork(work->gameData);
    list = &func_ov048_021f37a4(work->field)->messages;
    for (i = 0; i < list->count; i++) {
        if (list->kinds[i] == 8) {
            EventWork_FlagSet(eventWork, list->flags[i]);
        }
    }
}

void func_ov048_021f3870(GimmickGateWork *work) {
    func_ov048_021f47bc(work->board, work->boardTime << 12);
}

GimmickGateWork *func_ov048_021f3880(Field *field) {
    u16 heapId;
    FieldExpObjSystem *system;
    GimmickGateWork *work;
    ElboardInit init;

    heapId = Field_GetHeapID(field);
    system = Field_GetExpObjSystem(field);
    work = Field_AllocGimmickWorkBlock(field, 0, heapId, sizeof(GimmickGateWork));
    MI_CpuFill8(work, 0, sizeof(GimmickGateWork));
    work->heapId = heapId;
    work->field = field;
    work->gameSystem = Field_GetGameSystem(field);
    work->gameData = GSYS_GetGameData(work->gameSystem);
    work->value = 0;
    init.heapId = heapId;
    init.capacity = 7;
    init.unk03 = 8;
    init.unk04 = 3;
    init.actor = FieldExpObj_GetActor(system, 1, 0);
    work->board = func_ov048_021f4698(&init);
    return work;
}

void func_ov048_021f38ec(GimmickGateWork *work) {
    if (work != 0) {
        func_ov048_021f39a8(work);
        func_ov048_021f3c14(work);
        Field_DeleteGimmickWorkBlock(work->field, 0);
    }
}

void func_ov048_021f3908(GimmickGateWork *work) {
    SaveControl *save;
    TrainerCardSave *card;

    if (work->value == 0) {
        save = GameData_GetSaveControl_(work->gameData);
        card = getTrainerCardData_wrapper(save);
        work->flag = func_0200c3b0(card);
        work->value = 1;
    }
}

void func_ov048_021f392c(GimmickGateWork *work) {
    GimmickGateZoneData temp;
    s32 i;
    u32 count;
    u32 zone;

    if (work->zoneData == NULL) {
        zone = Field_GetPlayerStateZoneID(work->field);
        work->zoneData = GFL_HeapAllocate(work->heapId, sizeof(GimmickGateZoneData), FALSE, "field_gimmick_gate.c", 0x387);
        count = GFL_ArcSysGetDataMax(0xae);
        for (i = 0; i < count; i++) {
            func_ov048_021f4c10(&temp, 0xae, i);
            if (temp.zoneId == zone) {
                if (temp.version == 0 || temp.version == getGameVersion()) {
                    *work->zoneData = temp;
                    return;
                }
            }
        }
    }
}


void func_ov048_021f39a8(GimmickGateWork *work) {
    if (work->zoneData != 0) {
        GFL_HeapFree(work->zoneData);
        work->zoneData = 0;
    }
}

void func_ov048_021f39bc(GimmickGateWork *work) {
    FieldExpObjSystem *system;
    u32 anm;

    system = Field_GetExpObjSystem(work->field);
    anm = work->zoneData->anmIndex;
    FieldExpObj_SetAnm(system, 1, 0, data_ov048_021f50d2[anm], TRUE);
}

void func_ov048_021f39e4(GimmickGateWork *work) {
    FieldExpObjSystem *system;
    SRTMatrix *matrix;
    GimmickGateZoneData *state;
    u32 angle;

    system = Field_GetExpObjSystem(work->field);
    matrix = FieldExpObj_GetActorMatrixPtr(system, 1, 0);
    state = work->zoneData;
    matrix->translation.x = state->x << 12;
    matrix->translation.y = state->y << 12;
    matrix->translation.z = state->z << 12;
    switch (state->direction) {
    case 1:
        angle = 0;
        break;
    case 3:
        angle = 90;
        break;
    case 0:
        angle = 180;
        break;
    case 2:
        angle = 270;
        break;
    default:
        angle = 0;
        break;
    }
    MAT3_RotationEulerZYX(0, (u16)(angle * 182), 0, &matrix->rotation);
}

void func_ov048_021f3a50(GimmickGateWork *work, u32 kind, u32 flag) {
    GimmickGateMessageList *list;
    s32 count;

    list = &work->messages;
    count = list->count;
    if (count < 7) {
        list->kinds[count] = kind;
        list->flags[count] = flag;
        list->count = count + 1;
    }
}

u32 func_ov048_021f3ab4(GimmickGateWork *work);

u32 func_ov048_021f3a70(GimmickGateWork *work) {
    if (func_ov048_021f3a98(work) == 1 || func_ov048_021f3ab4(work) == 1) {
        if (work->flag != 0) {
            return 1;
        }
    }
    return 0;
}

u32 func_ov048_021f3a98(GimmickGateWork *work) {
    EventWork *eventWork;

    eventWork = GameData_GetEventWork(work->gameData);
    if (EventWork_FlagGet(eventWork, 0x960) == 1) {
        return 1;
    }
    return 0;
}

u32 func_ov048_021f3ab4(GimmickGateWork *work) {
    EventWork *eventWork = GameData_GetEventWork(work->gameData);

    if (EventWork_FlagGet(eventWork, 0x97b) == 1) {
        return 1;
    }
    return 0;
}


void func_ov048_021f3ad4(const GimmickGateWork *work, u32 *species, s32 *count) {
    GameData *gameData = work->gameData;
    SaveControl *save;
    HallOfFameSave *hallOfFame;
    HallOfFamePokemon pokemon;
    s32 i = 0;
    u32 result;

    *count = 0;
    save = GameData_GetSaveControl(gameData);
    result = func_0200725c(save, 8, work->heapId);
    if (result == 1 || result == 2) {
        hallOfFame = getAddressOfExtraSaveBlk(save, 8, 0);
        if (func_0200ecb4(hallOfFame) != 0) {
            *count = func_0200ecd0(hallOfFame, 0);
            for (i = 0; i < *count; i++) {
                pokemon.nickname = GFL_StrBufCreate(0x40, work->heapId);
                pokemon.trainerName = GFL_StrBufCreate(0x40, work->heapId);
                func_0200ecf0(hallOfFame, 0, i, &pokemon);
                species[i] = pokemon.species;
                GFL_StrBufFree(pokemon.nickname);
                GFL_StrBufFree(pokemon.trainerName);
            }
        }
    }
    freeIntermediateSaveExtraBlksAfterLoad(save, 8);
}

void func_ov048_021f3b64(GimmickGateWork *work, u32 *species, s32 *count) {
    HallOfFameTeam *team;
    HallOfFamePokemon pokemon;
    s32 i;

    team = func_02012f68(work->gameData);
    *count = func_0200ec14(team);
    for (i = 0; i < *count; i++) {
        pokemon.nickname = GFL_StrBufCreate(0x40, work->heapId);
        pokemon.trainerName = GFL_StrBufCreate(0x40, work->heapId);
        func_0200ec2c(team, i, &pokemon);
        species[i] = pokemon.species;
        GFL_StrBufFree(pokemon.nickname);
        GFL_StrBufFree(pokemon.trainerName);
    }
}

void func_ov048_021f3bc0(GimmickGateWork *work) {
    s32 count;
    s32 i;

    if (work->boardEntries == NULL) {
        count = GFL_ArcSysGetDataMax(0xa6);
        work->boardEntries = GFL_HeapAllocate(work->heapId, count * sizeof(GimmickGateBoardEntry), FALSE, "field_gimmick_gate.c", 0x4cb);
        for (i = 0; i < count; i++) {
            func_ov048_021f4de4(&work->boardEntries[i], 0xa6, i);
        }
        work->boardEntryCount = count;
    }
}



void func_ov048_021f3c14(GimmickGateWork *work) {
    if (work->boardEntries != 0) {
        GFL_HeapFree(work->boardEntries);
        work->boardEntries = 0;
        work->boardEntryCount = 0;
    }
}

GimmickGateBoardEntry *func_ov048_021f3c2c(GimmickGateWork *work) {
    u16 zone;
    EventWork *eventWork;
    s32 i;
    BOOL flag;
    BOOL zoneMatch;

    zone = Field_GetPlayerStateZoneID(work->field);
    eventWork = GameData_GetEventWork(work->gameData);
    if (work->boardEntries == 0) {
        return 0;
    }
    for (i = 0; i < work->boardEntryCount; i++) {
        flag = EventWork_FlagGet(eventWork, work->boardEntries[i].flagId);
        zoneMatch = func_ov048_021f4df4(&work->boardEntries[i], zone);
        if (func_ov048_021f4e3c(&work->boardEntries[i]) && flag &&
            zoneMatch) {
            return &work->boardEntries[i];
        }
    }
    return 0;
}

GimmickGateBoardEntry *func_ov048_021f3cb0(GimmickGateWork *work) {
    u16 zone;
    EventWork *eventWork;
    s32 i;
    GimmickGateBoardEntry *entry;
    BOOL flag;
    BOOL zoneMatch;

    zone = Field_GetPlayerStateZoneID(work->field);
    eventWork = GameData_GetEventWork(work->gameData);
    if (work->boardEntries == 0) {
        return 0;
    }
    for (i = 0; i < work->boardEntryCount; i++) {
        flag = EventWork_FlagGet(eventWork, work->boardEntries[i].flagId);
        zoneMatch = func_ov048_021f4df4(&work->boardEntries[i], zone);
        if (flag && zoneMatch) {
            entry = &work->boardEntries[i];
            if (entry->type == 2) {
                return entry;
            }
        }
    }
    return 0;
}

u32 func_ov048_021f3d24(GimmickGateWork *work) {
    if (func_ov048_021f3c2c(work) != 0) {
        return 1;
    }
    return 0;
}

void func_ov048_021f3d38(GimmickGateWork *work, ElboardMessageArg *message, u32 kind, GimmickGateBoardEntry *entry) {
    func_ov048_021f472c(work->board, message);
    if (kind == 8) {
        func_ov048_021f3a50(work, kind, entry->flagId);
    } else {
        func_ov048_021f3a50(work, kind, 0);
    }
    if (kind == 8 && entry->unk08 == 1) {
        EventWork_FlagReset(GameData_GetEventWork(work->gameData), entry->flagId);
    }
}

void func_ov048_021f3d78(GimmickGateWork *work) {
    if (func_ov048_021f3a70(work) == 1) {
        func_ov048_021f3df0(work);
        return;
    }
    if (func_ov048_021f3d24(work) != 0) {
        func_ov048_021f3e2c(work);
        return;
    }
    func_ov048_021f3da8(work);
}

void func_ov048_021f3da8(GimmickGateWork *work) {
    if (!func_ov048_021f47b8(work->board) && work->zoneData && work->zoneData->enabled) {
        func_ov048_021f4058(work);
        func_ov048_021f3e6c(work);
        func_ov048_021f3ee8(work);
        func_ov048_021f4104(work);
        func_ov048_021f4188(work);
        func_ov048_021f420c(work);
        func_ov048_021f4290(work);
    }
}

void func_ov048_021f3df0(GimmickGateWork *work) {
    if (!func_ov048_021f47b8(work->board) && work->zoneData && work->zoneData->enabled) {
        func_ov048_021f4058(work);
        func_ov048_021f3e6c(work);
        func_ov048_021f3ee8(work);
        func_ov048_021f4314(work);
        func_ov048_021f4290(work);
    }
}

void func_ov048_021f3e2c(GimmickGateWork *work) {
    if (!func_ov048_021f47b8(work->board) && work->boardEntries && work->zoneData && work->zoneData->enabled) {
        func_ov048_021f4058(work);
        func_ov048_021f3e6c(work);
        func_ov048_021f3ee8(work);
        func_ov048_021f43e4(work);
        func_ov048_021f4290(work);
    }
}

void func_ov048_021f3e6c(GimmickGateWork *work) {
    WordSet *wordSet;
    ElboardMessageArg arg;
    RTCDate date;

    wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    RTC_GetDate(&date);
    WordSetNumber(wordSet, 2, date.year, 2, 2, 1);
    loadMonthToStrbuf(wordSet, 0, date.month);
    WordSetNumber(wordSet, 1, date.day, 2, 0, 1);
    arg.kind = data_ov048_021f50c4[0];
    arg.name = data_ov048_021f50ec[0];
    arg.plName = data_ov048_021f5108[0];
    arg.messageArc = 2;
    arg.messageFile = 0x26;
    arg.messageId = work->zoneData->messageIds[0];
    arg.wordSet = wordSet;
    func_ov048_021f3d38(work, &arg, 1, 0);
    GFL_WordSetSystemFree(wordSet);
}

void func_ov048_021f3ee8(GimmickGateWork *work) {
    ElboardMessageArg arg;
    GimmickGateZoneList list;
    u32 message;
    WordSet *wordSet;
    WordSet *formatSet;
    MsgData *placeMessages;
    MsgData *weatherMessages;
    StrBuf *placeName;
    StrBuf *weatherName;
    StrBuf *template;
    StrBuf *formatted;
    s32 i;
    u8 weather;

    func_ov048_021f44d8(work, &list);
    wordSet = GFL_WordSetSystemCreate(4, 0x100, work->heapId);
    placeMessages = GFL_MsgSysLoadData(0, ARCID_SYSTEM_MESSAGE, TEXT_BANK_PLACE_NAMES, work->heapId);
    weatherMessages = GFL_MsgSysLoadData(0, ARCID_SYSTEM_MESSAGE, TEXT_BANK_GYM_INSECT, work->heapId);
    for (i = 0; i < 4; i++) {
        if (list.zones[i] == 0x1ab) {
            continue;
        }
        weather = list.weather[i];
        placeName = GFL_MsgDataLoadStrbufNew(placeMessages, ZoneData_GetPlaceNameID(list.zones[i]));
        weatherName = GFL_MsgDataLoadStrbufNew(weatherMessages, data_ov048_021f5124[weather]);
        formatSet = GFL_WordSetSystemCreate(2, 0x100, work->heapId);
        template = GFL_MsgDataLoadStrbufNew(weatherMessages, GymInsect_Text_192);
        formatted = GFL_StrBufCreate(0x40, work->heapId);
        func_0201ee0c(formatSet, 0, placeName, 0, 1, 0);
        func_0201ee0c(formatSet, 1, weatherName, 0, 1, 0);
        GFL_WordSetFormatStrbuf(formatSet, formatted, template);
        GFL_StrBufFree(template);
        GFL_WordSetSystemFree(formatSet);
        func_0201ee0c(wordSet, i, formatted, 0, 1, 0);
        GFL_StrBufFree(placeName);
        GFL_StrBufFree(weatherName);
        GFL_StrBufFree(formatted);
    }
    GFL_MsgDataFree(weatherMessages);
    GFL_MsgDataFree(placeMessages);
    switch (func_ov048_021f4488(&list)) {
    case 1:
        message = 0xc1;
        break;
    case 2:
        message = 0xc2;
        break;
    case 3:
        message = 0xc3;
        break;
    case 4:
        message = 0xc4;
        break;
    }
    arg.kind = data_ov048_021f50c4[1];
    arg.name = data_ov048_021f50ec[1];
    arg.plName = data_ov048_021f5108[1];
    arg.messageArc = 2;
    arg.messageFile = 0x26;
    arg.messageId = message;
    arg.wordSet = wordSet;
    func_ov048_021f3d38(work, &arg, 2, 0);
    GFL_WordSetSystemFree(wordSet);
}

void func_ov048_021f4058(GimmickGateWork *work) {
    u16 zone;
    WordSet *wordSet;
    MsgData *msgData;
    StrBuf *zoneName;
    ElboardMessageArg arg;

    zone = getSwarmLevelRangeFromData(work->gameData);
    if (zone == 0xffff || (zone == 0x159 && GameData_GetSeason(work->gameData) == 3)) {
        return;
    }
    wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    msgData = GFL_MsgSysLoadData(0, ARCID_SYSTEM_MESSAGE, TEXT_BANK_PLACE_NAMES, work->heapId);
    zoneName = GFL_MsgDataLoadStrbufNew(msgData, ZoneData_GetPlaceNameID(zone));
    func_0201ee0c(wordSet, 0, zoneName, 0, 1, 0);
    GFL_StrBufFree(zoneName);
    GFL_MsgDataFree(msgData);
    arg.kind = data_ov048_021f50c4[2];
    arg.name = data_ov048_021f50ec[2];
    arg.plName = data_ov048_021f5108[2];
    arg.messageArc = 2;
    arg.messageFile = 0x26;
    arg.messageId = work->zoneData->messageIds[2];
    arg.wordSet = wordSet;
    func_ov048_021f3d38(work, &arg, 3, 0);
    GFL_WordSetSystemFree(wordSet);
}

void func_ov048_021f4104(GimmickGateWork *work) {
    ElboardMessageArg arg;
    RTCDate date;

    RTC_GetDate(&date);
    switch (date.week) {
    case 0:
        arg.messageId = work->zoneData->messageIds[9];
        break;
    case 1:
        arg.messageId = work->zoneData->messageIds[6];
        break;
    case 2:
        arg.messageId = work->zoneData->messageIds[3];
        break;
    case 3:
        arg.messageId = work->zoneData->messageIds[7];
        break;
    case 4:
        arg.messageId = work->zoneData->messageIds[4];
        break;
    case 5:
        arg.messageId = work->zoneData->messageIds[8];
        break;
    case 6:
        arg.messageId = work->zoneData->messageIds[5];
        break;
    }
    arg.kind = data_ov048_021f50c4[3];
    arg.name = data_ov048_021f50ec[3];
    arg.plName = data_ov048_021f5108[3];
    arg.messageArc = 2;
    arg.messageFile = 0x26;
    arg.wordSet = 0;
    func_ov048_021f3d38(work, &arg, 4, 0);
}

void func_ov048_021f4188(GimmickGateWork *work) {
    ElboardMessageArg arg;
    RTCDate date;

    RTC_GetDate(&date);
    switch (date.week) {
    case 0:
        arg.messageId = work->zoneData->messageIds[10];
        break;
    case 1:
        arg.messageId = work->zoneData->messageIds[7];
        break;
    case 2:
        arg.messageId = work->zoneData->messageIds[4];
        break;
    case 3:
        arg.messageId = work->zoneData->messageIds[8];
        break;
    case 4:
        arg.messageId = work->zoneData->messageIds[5];
        break;
    case 5:
        arg.messageId = work->zoneData->messageIds[9];
        break;
    case 6:
        arg.messageId = work->zoneData->messageIds[6];
        break;
    }
    arg.kind = data_ov048_021f50c4[4];
    arg.name = data_ov048_021f50ec[4];
    arg.plName = data_ov048_021f5108[4];
    arg.messageArc = 2;
    arg.messageFile = 0x26;
    arg.wordSet = 0;
    func_ov048_021f3d38(work, &arg, 5, 0);
}

void func_ov048_021f420c(GimmickGateWork *work) {
    ElboardMessageArg arg;
    RTCDate date;

    RTC_GetDate(&date);
    switch (date.week) {
    case 0:
        arg.messageId = work->zoneData->messageIds[11];
        break;
    case 1:
        arg.messageId = work->zoneData->messageIds[8];
        break;
    case 2:
        arg.messageId = work->zoneData->messageIds[5];
        break;
    case 3:
        arg.messageId = work->zoneData->messageIds[9];
        break;
    case 4:
        arg.messageId = work->zoneData->messageIds[6];
        break;
    case 5:
        arg.messageId = work->zoneData->messageIds[10];
        break;
    case 6:
        arg.messageId = work->zoneData->messageIds[7];
        break;
    }
    arg.kind = data_ov048_021f50c4[5];
    arg.name = data_ov048_021f50ec[5];
    arg.plName = data_ov048_021f5108[5];
    arg.messageArc = 2;
    arg.messageFile = 0x26;
    arg.wordSet = 0;
    func_ov048_021f3d38(work, &arg, 6, 0);
}

void func_ov048_021f4290(GimmickGateWork *work) {
    ElboardMessageArg arg;
    RTCDate date;

    RTC_GetDate(&date);
    switch (date.week) {
    case 0:
        arg.messageId = work->zoneData->messageIds[18];
        break;
    case 1:
        arg.messageId = work->zoneData->messageIds[12];
        break;
    case 2:
        arg.messageId = work->zoneData->messageIds[13];
        break;
    case 3:
        arg.messageId = work->zoneData->messageIds[14];
        break;
    case 4:
        arg.messageId = work->zoneData->messageIds[15];
        break;
    case 5:
        arg.messageId = work->zoneData->messageIds[16];
        break;
    case 6:
        arg.messageId = work->zoneData->messageIds[17];
        break;
    }
    arg.kind = data_ov048_021f50c4[6];
    arg.name = data_ov048_021f50ec[6];
    arg.plName = data_ov048_021f5108[6];
    arg.messageArc = 2;
    arg.messageFile = 0x26;
    arg.wordSet = 0;
    func_ov048_021f3d38(work, &arg, 7, 0);
}

void func_ov048_021f4314(GimmickGateWork *work) {
    s32 count;
    ElboardMessageArg arg;
    u32 species[6];
    WordSet *wordSet;
    u32 message;
    s32 i;

    wordSet = GFL_WordSetSystemCreateDefault(func_ov048_021f47b4(work->board));
    if (func_ov048_021f3ab4(work)) {
        func_ov048_021f3ad4(work, species, &count);
    } else {
        func_ov048_021f3b64(work, species, &count);
    }
    switch (count) {
    case 1:
        message = 0xcd;
        break;
    case 2:
        message = 0xce;
        break;
    case 3:
        message = 0xcf;
        break;
    case 4:
        message = 0xd0;
        break;
    case 5:
        message = 0xd1;
        break;
    case 6:
        message = 0xd2;
        break;
    }
    for (i = 0; i < count; i++) {
        WordSet_LoadSpeciesName(wordSet, i, (u16)species[i]);
    }
    copyVarForText(wordSet, 6, GetGameDataPlayerInfo(work->gameData));
    arg.kind = data_ov048_021f50c4[3];
    arg.name = data_ov048_021f50ec[3];
    arg.plName = data_ov048_021f5108[3];
    arg.messageArc = 2;
    arg.messageFile = 0x26;
    arg.messageId = message;
    arg.wordSet = wordSet;
    func_ov048_021f3d38(work, &arg, 9, 0);
    GFL_WordSetSystemFree(wordSet);
}


void func_ov048_021f43e4(GimmickGateWork *work) {
    GimmickGateBoardEntry *entry;

    entry = func_ov048_021f3c2c(work);
    switch (entry->type) {
    case 0:
        func_ov048_021f440c(work, entry, 3);
        break;
    case 2:
        func_ov048_021f4454(work);
        break;
    }
}

void func_ov048_021f440c(GimmickGateWork *work, GimmickGateBoardEntry *entry, u32 index) {
    ElboardMessageArg arg;

    arg.kind = data_ov048_021f50c4[index];
    arg.name = data_ov048_021f50ec[index];
    arg.plName = data_ov048_021f5108[index];
    arg.messageArc = 2;
    arg.messageFile = 0x26;
    arg.messageId = entry->unk0c;
    arg.wordSet = NULL;
    func_ov048_021f3d38(work, &arg, 8, entry);
}

void func_ov048_021f4454(GimmickGateWork *work) {
    s32 index;
    GimmickGateBoardEntry *entry;

    index = 0;
    entry = func_ov048_021f3cb0(work);
    while (entry && index < 4) {
        func_ov048_021f440c(work, entry, data_ov048_021f4e84[index++]);
        entry = func_ov048_021f3cb0(work);
    }
}

s32 func_ov048_021f4488(const GimmickGateZoneList *list) {
    s32 index;
    s32 count;

    count = 0;
    for (index = 0; index < 4; index++) {
        if (list->zones[index] != 0x1ab && list->weather[index] != 0xffff) {
            count++;
        }
    }
    return count;
}

void func_ov048_021f44b8(GimmickGateZoneList *list) {
    s32 index;

    for (index = 0; index < 4; index++) {
        list->zones[index] = 0x1ab;
        list->weather[index] = 0xff;
    }
}

u32 Field_GetWeatherForZone(Field *field, u16 zone);






void func_ov048_021f44d8(GimmickGateWork *work, GimmickGateZoneList *list) {
    GimmickGateZoneList second;
    GimmickGateZoneList first;
    GimmickGateZoneData *zoneData;
    s32 count;
    s32 i;

    count = 0;
    func_ov048_021f44b8(list);
    func_ov048_021f4590(work, &second);
    func_ov048_021f45f0(work, &first);
    for (i = 0; i < 4; i++) {
        if (first.zones[i] == 0x1ab || first.weather[i] == 0xffff || count >= 2) {
            break;
        }
        list->zones[count] = first.zones[i];
        list->weather[count] = first.weather[i];
        count++;
    }
    for (i = 0; i < 4; i++) {
        if (second.zones[i] == 0x1ab || second.weather[i] == 0xffff || count >= 2) {
            break;
        }
        list->zones[count] = second.zones[i];
        list->weather[count] = second.weather[i];
        count++;
    }
    if (count == 0) {
        zoneData = work->zoneData;
        list->zones[0] = zoneData->fallbackZones[0];
        list->zones[1] = zoneData->fallbackZones[1];
        list->zones[2] = zoneData->fallbackZones[2];
        list->zones[3] = zoneData->fallbackZones[3];
    }
    for (i = 0; i < 4; i++) {
        list->weather[i] = Field_GetWeatherForZone(work->field, list->zones[i]);
    }
}

void func_ov048_021f4590(GimmickGateWork *work, GimmickGateZoneList *list) {
    int i;
    int j;
    s32 count;
    u16 zone;
    u8 weather;

    func_ov048_021f44b8(list);
    count = 0;
    for (i = 0; i < NELEMS(data_ov048_021f4e5c); i++) {
        zone = data_ov048_021f4e5c[i];
        weather = Field_GetWeatherForZone(work->field, zone);
        for (j = 0; j < NELEMS(data_ov048_021f4e5e); j++) {
            if (weather == data_ov048_021f4e5e[j]) {
                if (count < 4) {
                    list->zones[count] = zone;
                    list->weather[count] = weather;
                    count++;
                } else {
                    break;
                }
            }
        }
    }
}




void func_ov048_021f45f0(GimmickGateWork *work, GimmickGateZoneList *list) {
    EncountSave *save;
    u32 version;
    u32 slot;
    u32 weather;
    u16 zone;

    save = SaveControl_GetEncountSave(GameData_GetSaveControl(work->gameData));
    func_ov048_021f44b8(list);
    if (func_ov010_0215e404(save)) {
        version = getGameVersion();
        switch (version) {
        case 0x14:
            slot = 0;
            break;
        case 0x15:
            slot = 1;
            break;
        }
        switch (slot) {
        case 0:
            weather = 6;
            break;
        case 1:
            weather = 7;
            break;
        }
        zone = EncountSave_GetRoamingPkmZone(save, (u8)slot);
        if (zone != 0xffff) {
            list->zones[0] = zone;
            list->weather[0] = weather;
        }
    }
}

BOOL func_ov048_021f4650(VM *vm, FieldScriptEnv *env) {
    ScriptWork *scriptWork;
    GameSystem *gsys;
    Field *field;
    u32 id;
    GameEvent *event;

    scriptWork = FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    field = GSYS_GetField(gsys);
    id = ScriptReadAny(vm, env);
    GFL_SndSEPlay(0x547);
    event = func_ov048_021f4dbc(gsys, field, id);
    ScriptWork_CallEvent(scriptWork, event);
    return TRUE;
}
