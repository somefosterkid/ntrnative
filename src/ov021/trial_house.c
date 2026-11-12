// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/trial_house.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/version.h"
#include "app/name_entry.h"
#include "constants/pokemon.h"
#include "field/battle_facility.h"
#include "field/field_effect.h"
#include "field/fld_btl_inst_event.h"
#include "field/trial_house.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "gfl/net.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/trial_house.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/version.h"
#include "field/delivery_beacon.h"

#define HEAPID_TRIAL_HOUSE 0x5

struct TrialHouseEventData {
    DeliveryInit init;
    void *subwork;
    GameSystem *gsys;
    TrialHouseWork *work;
    u16 *result;
    s32 timeout;
};

struct TrialHouseEffectEvent {
    GameSystem *gsys;
    void *buffer;
    u32 mode;
    void *effect;
};

struct TrialHouseWork *CreateTrialHouseWk(GameSystem *gsys) {
    u32 saveSize = func_0200e588();
    GameData *gameData = GSYS_GetGameData(gsys);
    struct TrialHouseWork *work;

    GameData_GetSaveControl(gameData);
    work = GFL_HeapAllocate(HEAPID_TRIAL_HOUSE, sizeof(struct TrialHouseWork), TRUE, "trial_house.c", 0x5d);
    work->heapId = HEAPID_TRIAL_HOUSE;
    work->saveBuffer = GFL_HeapAllocate(HEAPID_TRIAL_HOUSE, saveSize, TRUE, "trial_house.c", 0x5f);
    func_ov021_021d09d0(gsys, work);
    work->party = PokeParty_Create(work->heapId);
    return work;
}

void freeIntermediateSaveExtraBlksAfterLoad2(SaveControl *save, u32 slot);
void func_ov021_021d09d0(GameSystem *gsys, TrialHouseWork *work) {
    u32 state;
    SaveControl *save;
    void *buffer;
    void *extraSave;

    save = GameData_GetSaveControl(GSYS_GetGameData(gsys));
    buffer = GFL_HeapAllocate(0x8004, 0x1000, TRUE, "trial_house.c", 0x79);
    func_020072cc(save, 5, 0x8004, buffer, 0x1000);
    extraSave = getAddressOfExtraSaveBlk(save, 5, 0);
    state = 0;
    if (func_0200e5a0(extraSave)) {
        if (func_0200e5cc(extraSave)) {
            state = 1;
        } else {
            state = 2;
        }
    }
    freeIntermediateSaveExtraBlksAfterLoad2(save, 5);
    GFL_HeapFree(buffer);
    work->initState = state;
}

void TrialHouseWorkDelete(void *unused, struct TrialHouseWork **workPtr) {
    if (*workPtr != NULL) {
        GFL_HeapFree((*workPtr)->saveBuffer);
        GFL_HeapFree((*workPtr)->party);
        GFL_HeapFree(*workPtr);
        *workPtr = NULL;
    }
}

void func_ov021_021d0a74(TrialHouseWork *work, u32 mode) {
    u32 battleType;

    if (work != NULL) {
        if (mode <= 1) {
            if (mode == 0) {
                battleType = 0;
                work->capacity = 3;
            } else {
                battleType = 1;
                work->capacity = 4;
            }
            work->battleType = battleType;
        } else {
            work->battleType = 0;
            work->capacity = 3;
        }
        PokeParty_InitCore(work->party, work->capacity);
    }
}

void func_ov021_021d0ab8(TrialHouseWork *work, u32 selectionFlag) {
    work->selectionFlag = selectionFlag;
}

u32 func_ov021_021d0ac0(GameSystem *gsys, TrialHouseWork *work, u32 mode) {
    if (work->selectionFlag != 0) {
        func_ov021_021d0b58(gsys, work, mode);
    } else {
        func_ov021_021d0ae4(work, mode);
    }
    return func_ov021_021d0bcc(work);
}

void func_ov021_021d0ae4(TrialHouseWork *work, u32 mode) {
    u32 base;
    u32 range;
    u32 value;

    switch (mode) {
    case 0:
        base = 0x32;
        range = 0x14;
        break;
    case 1:
        base = 0x6e;
        range = 0x32;
        break;
    case 2:
        base = 0xa0;
        range = 0x14;
        break;
    case 3:
        base = 0xf0;
        range = 0x1e;
        break;
    case 4:
        base = 0x10e;
        range = 0x1e;
        break;
    default:
        base = 0x32;
        range = 0x14;
        break;
    }
    value = base + GFL_RandomLC(range);
    func_ov010_02168878(&work->trainer, value, work->capacity, NULL, NULL, NULL, HEAPID_TAIL(work->heapId));
}


void func_ov021_021d0b58(GameSystem *gsys, TrialHouseWork *work, u32 mode) {
    SaveControl *save;
    void *buffer;
    void *extra;

    save = GameData_GetSaveControl(GSYS_GetGameData(gsys));
    buffer = GFL_HeapAllocate(0x8004, 0x1000, TRUE, "trial_house.c", 0x14c);
    if (func_020072cc(save, 5, 0x8004, buffer, 0x1000) == 1) {
        extra = getAddressOfExtraSaveBlk(save, 5, 0);
        work->trainer = *func_0200e5f8(extra, mode);
    }
    freeIntermediateSaveExtraBlksAfterLoad2(save, 5);
    GFL_HeapFree(buffer);
}

u32 func_ov021_021d0bcc(TrialHouseWork *work) {
    return func_ov010_02168aec(work->trainer.trainerId);
}

GameEvent *func_ov021_021d0bd8(GameSystem *gsys, TrialHouseWork *work, u32 index, u32 actorId) {
    return func_ov010_021680c8(gsys, &work->trainer, index, (u16)actorId);
}

GameEvent *func_ov021_021d0be4(GameSystem *gsys, TrialHouseWork *work, u16 *result) {
    GameEvent *event;
    struct TrialHouseEventData *data;

    event = GameEvent_Create(gsys, NULL, func_ov021_021d0c58, sizeof(struct TrialHouseEventData));
    data = GameEvent_GetData(event);
    data->gsys = gsys;
    data->result = result;
    data->work = work;
    data->timeout = 0;
    data->init.code = 0x2e;
    data->init.data[0].datasize = func_0200e588();
    data->init.data[0].pData = work->saveBuffer;
    data->init.data[0].region = region;
    data->init.data[0].mask = 1 << GAME_VERSION;
    data->init.dataNum = 1;
    data->init.flag4 = 0;
    data->init.heapId = HEAPID_TAIL(HEAPID_FIELDMAP);
    return event;
}

GameEventReturnCode func_ov021_021d0c58(GameEvent *event, u32 *state, void *arg) {
    TrialHouseEventData *data;
    GameSystem *gsys;
    GameData *gameData;
    TrialHouseSave *save;
    void *saveBuffer;
    u32 a;
    u32 b;
    u32 c;

    data = arg;
    gsys = data->gsys;
    switch (*state) {
    case 0:
        data->subwork = func_ov010_021576d0(&data->init);
        if (func_ov010_021578a0(data->subwork) == 0) {
            *data->result = 0;
            *state = 4;
        } else {
            *state = 1;
        }
        break;
    case 1:
        if (GCTX_HIDGetPressedKeys() == 2) {
            *data->result = 3;
            *state = 4;
            break;
        }
        func_ov010_02157928(data->subwork);
        data->timeout++;
        if (data->timeout > 120) {
            *state = 2;
        }
        break;
    case 2:
        if (GCTX_HIDGetPressedKeys() == 2) {
            *data->result = 3;
            *state = 4;
            break;
        }
        func_ov010_02157928(data->subwork);
        if (func_ov010_021578f0(data->subwork)) {
            *state = 3;
        } else {
            *data->result = 0;
            *state = 4;
        }
        break;
    case 3:
        if (GCTX_HIDGetPressedKeys() == 2) {
            *data->result = 3;
            *state = 4;
            break;
        }
        func_ov010_02157928(data->subwork);
        if (!func_ov010_02157910(data->subwork)) {
            break;
        }
        gameData = GSYS_GetGameData(gsys);
        save = func_0200e8f8(GameData_GetSaveControl(gameData));
        saveBuffer = data->work->saveBuffer;
        a = func_0200e5e4(saveBuffer);
        b = func_0200e5a0(saveBuffer);
        c = func_ov021_021d1058(save, a);
        if (b != 0 && (c == 0 || a == 0)) {
            func_ov021_021d1080(save, a);
            func_0200e608(gameData, data->work->saveBuffer, 0x8004);
            *data->result = 1;
        } else if (c != 0) {
            *data->result = 2;
        } else {
            *data->result = 0;
        }
        *state = 4;
        break;
    case 4:
        func_ov010_02157938(data->subwork);
        *state = 5;
        break;
    case 5:
        if (func_0203d210()) {
            *state = 6;
        }
        break;
    case 6:
        func_ov021_021d09d0(gsys, data->work);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

static inline u32 TrialHouse_GetRank(u32 points) {
    if (points >= 6000) {
        return 6;
    } else if (points >= 5000) {
        return 5;
    } else if (points >= 4000) {
        return 4;
    } else if (points >= 3000) {
        return 3;
    } else if (points >= 2000) {
        return 2;
    } else if (points >= 1000) {
        return 1;
    }
    return 0;
}

// Puts the party's Pokémon in the record
static inline void TrialHouse_SetPokemon(TrialHousePokemon *mons, TrialHouseWork *work) {
    int i;
    PartyPkm *pkm;

    for (i = 0; i < work->capacity; i++) {
        TrialHousePokemon *mon = &mons[i];
        pkm = PokeParty_GetPkm(work->party, i);
        mon->species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        mon->form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
        mon->sex = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
    }
}

static inline void TrialHouse_SetRecord(TrialHouseRecord *record, TrialHouseWork *work, u32 points) {
    record->valid = TRUE;
    if (work->capacity == 3) {
        record->isDouble = FALSE;
    } else if (work->capacity == 4) {
        record->isDouble = TRUE;
    } else {
        record->isDouble = FALSE;
    }
    record->points = points;
    MI_CpuFill8(record->pokemon, 0, sizeof(record->pokemon));
    TrialHouse_SetPokemon(record->pokemon, work);
}

void TrialHouseCalcPointScore(GameSystem *gsys, TrialHouseWork *work, u16 *rankOut, u16 *pointsOut) {
    u16 *stats = work->stats;
    SaveControl *save;
    s32 total;
    u32 points;
    TrialHouseSave *thSave;
    u32 rank;
    SaveControl *extraSave;
    void *buffer;
    GameRecords *records;

    total = 0;
    total += stats[7] * 1000;
    total += stats[8] * 80;
    total += stats[1] * 5;
    total += stats[3];
    total += stats[5] * 5;
    total += stats[6] * 2;
    total += stats[11] * 15;
    total -= stats[0] * 10;
    total -= stats[2] * 10;
    total -= stats[4] * 2;
    total -= stats[9] * 80;
    total -= 500 - stats[10];
    points = total;
    if (total < 0) {
        points = 0;
    } else if (total > 9999) {
        points = 9999;
    }
    if (points >= 6000) {
        rank = 6;
    } else if (points >= 5000) {
        rank = 5;
    } else if (points >= 4000) {
        rank = 4;
    } else if (points >= 3000) {
        rank = 3;
    } else if (points >= 2000) {
        rank = 2;
    } else if (points >= 1000) {
        rank = 1;
    } else {
        rank = 0;
    }
    *rankOut = rank;
    *pointsOut = points;
    save = GameData_GetSaveControl(GSYS_GetGameData(gsys));
    thSave = func_0200e8f8(save);
    TrialHouse_SetRecord(&thSave->records[(u8)(work->initState != 0)], work, points);
    if (work->initState != 0) {
        extraSave = GameData_GetSaveControl(GSYS_GetGameData(gsys));
        buffer = GFL_HeapAllocate(0x8004, 0x1000, TRUE, "trial_house.c", 0x277);
        if (func_020072cc(extraSave, 5, 0x8004, buffer, 0x1000) == 1) {
            MI_CpuCopy8((u8 *)getAddressOfExtraSaveBlk(extraSave, 5, 0) + 0x5a0, thSave->unk38, sizeof(thSave->unk38));
        }
        GFL_HeapFree(buffer);
        freeIntermediateSaveExtraBlksAfterLoad2(extraSave, 5);
    }
    records = getTrainerCardInfoBlkAddress(save);
    func_02008e88(records, points);
    func_02008e68(records, (u8)rank);
}

u32 func_ov021_021d0fe0(GameSystem *gsys, TrialHouseWork *work) {
    return work->initState;
}

GameEvent *func_ov021_021d0fe8(GameSystem *gsys, TrialHouseWork *work, u32 mode) {
    GameEvent *event;
    TrialHouseEffectEvent *data;

    event = GameEvent_Create(gsys, NULL, func_ov021_021d10a8, sizeof(TrialHouseEffectEvent));
    data = GameEvent_GetData(event);
    data->gsys = gsys;
    data->buffer = NULL;
    data->mode = mode;
    data->effect = func_ov021_021d1164(0x8015, GSYS_GetField(gsys));
    func_ov021_021d11b0(data->effect);
    return event;
}

u32 func_ov021_021d1028(GameSystem *gsys) {
    TrialHouseSave *save;

    save = func_0200e8f8(GameData_GetSaveControl(GSYS_GetGameData(gsys)));
    if (save->records[0].valid) {
        if (save->records[1].valid) {
            return 3;
        }
        return 1;
    }
    if (save->records[1].valid) {
        return 2;
    }
    return 0;
}

u8 func_ov021_021d1058(TrialHouseSave *save, u32 index) {
    u8 byte;
    u8 bit;

    if (index < 128) {
        byte = index / 8;
        bit = index % 8;
        return (save->bits[byte] >> bit) & 1;
    }
    return TRUE;
}

void func_ov021_021d1080(TrialHouseSave *save, u32 index) {
    u8 byte;
    u8 bit;

    if (index < 128) {
        byte = index / 8;
        bit = index % 8;
        save->bits[byte] |= 1 << bit;
    }
}

GameEventReturnCode func_ov021_021d10a8(GameEvent *event, u32 *state, void *arg) {
    TrialHouseEffectEvent *data;
    GameData *gameData;
    SaveControl *save;
    void *extraSave;

    data = arg;
    gameData = GSYS_GetGameData(data->gsys);
    save = GameData_GetSaveControl(gameData);
    switch (*state) {
    case 0:
        data->buffer = GFL_HeapAllocate(0x8004, 0x1000, FALSE, "trial_house.c", 0x34c);
        func_020072cc(save, 5, 0x8004, data->buffer, 0x1000);
        extraSave = getAddressOfExtraSaveBlk(save, 5, 0);
        if (data->mode == 1) {
            func_0200e684(extraSave);
        } else {
            MI_CpuFill8(extraSave, 0, func_0200e588());
        }
        func_02012e4c(gameData, 5);
        (*state)++;
        break;
    case 1:
        if (func_02012e7c(gameData, 5) == 2) {
            freeIntermediateSaveExtraBlksAfterLoad2(save, 5);
            GFL_HeapFree(data->buffer);
            func_ov021_021d11d8(data->effect);
            func_ov021_021d1194(data->effect);
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}


