// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/field_encount.c, ported by tools/port_file.py.
// Wild encounters: the encounter system, the encounter rate while the player walks, and the battles it starts, wild,
// static, fishing and trainers'. The name is the ROM's own, from GFL_HeapAllocate's file argument. Function names
// from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "battle/btl_setup.h"
#include "battle/trainer_data.h"
#include "constants/species.h"
#include "field/enc_pokeset.h"
#include "field/encounter.h"
#include "field/event_battle.h"
#include "field/event_data.h"
#include "field/event_wild_battle.h"
#include "field/field.h"
#include "field/field_wfbc.h"
#include "field/field_actor.h"
#include "field/field_environment.h"
#include "field/field_event.h"
#include "field/field_map.h"
#include "field/field_player.h"
#include "field/pair_sys.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "nitro/rtc.h"
#include "pml/poke_party.h"
#include "save/records.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "gfl/rtc_cache.h"
#include "nitro/fx/fx_cp.h"



int FieldEncount_DecideTypeByTerrain(EncountSystem *system, u32 mode, u8 *rateBonus);
static int EncSys_GetEncountRateConfig(EncountSystem *system, int encType, int adjust);
BOOL EncSys_CheckRNGEncounter(EncountSystem *system, EncountState *state, u32 rate);
BOOL FieldEncount_SetupNormal(EncountState *state, void *mapEncount);
void FieldEncount_SetupRoaming(EncountSystem *system, EncountManager *manager, BtlSetup *setup, HeapID heapId, WildPkmParam *params);
void FieldEncount_SetupNPoke(EncountSystem *system, EncountManager *manager, BtlSetup *setup, HeapID heapId, void *roamingPkm);
u32 EncountState_SyncToPlayer(void);
void EncountState_UpdateTerrain(EncountState *state, FieldPlayer *player);
void EncSys_UpdateEncountRate(EncountState *state, FieldPlayer *player);
u32 EncountState_GetEncountRateStepCounter(EncountState *state);

EncountSystem *EncSys_Create(Field *field) {
    HeapID heapId = Field_GetHeapID(field);
    EncountSystem *system = GFL_HeapAllocate(heapId, sizeof(EncountSystem), TRUE, "field_encount.c", 98);

    system->field = field;
    system->gsys = Field_GetGameSystem(field);
    system->gameData = GSYS_GetGameData(system->gsys);
    system->encData = GetEncountData(GameData_GetEventData(system->gameData));
    system->effectEncountState = EffectEncountState_Create(heapId);
    PrepareFieldEncountSystem(system, system->effectEncountState);
    return system;
}

void EncSys_Free(EncountSystem *system) {
    func_ov021_021aa7cc(system, system->effectEncountState);
    EffectEncountState_Free(system->effectEncountState);
    GFL_HeapFree(system);
}

GameEvent *EventWildBattleCall_CreateRandom(EncountSystem *system, u32 mode) {
    WildPkmParam params[2];
    EncountManager manager;
    u8 rateBonus;
    int rate;
    int encType;
    EncountState *state;
    FieldPlayer *player;
    BOOL forced = FALSE;
    void *nIndex = NULL;
    BtlSetup *setup;

    player = Field_GetPlayer(system->field);
    state = GameData_GetEncountState(system->gameData);
    if (mode == 1 || mode == 2) {
        forced = TRUE;
    } else {
        EncSys_UpdateEncountRate(state, player);
    }
    if (system->encData == NULL || !system->encData->fishingEnable) {
        return NULL;
    }
    rateBonus = 0;
    encType = FieldEncount_DecideTypeByTerrain(system, mode, &rateBonus);
    rate = EncSys_GetEncountRateConfig(system, encType, rateBonus);
    if (encType == 0xff || rate == 0) {
        return NULL;
    }
    CreateEncountManager(&manager, system->gameData, encType, mode,
                        func_ov021_021a1bf4(Field_GetWeatherSystem(system->field)));
    if (!forced && !EncSys_CheckRNGEncounter(system, state, FieldEncount_CalcEncountRate(&manager, system->gameData, rate))) {
        return NULL;
    }
    if (mode == 1) {
        RecordAddOne(GameData_GetRecords(system->gameData), 0x70);
    } else {
        nIndex = func_ov021_021a957c(system->encData, &manager, Field_GetPlayerStateZoneID(system->field));
    }
    if (nIndex != 0) {
        setup = BtlSetup_Create(4);
        FieldEncount_SetupNPoke(system, &manager, setup, 4, nIndex);
    } else {
        u32 count;

        MI_CpuFill8(params, 0, sizeof(params));
        count = FieldEncount_GenWilds(system->encData, &manager, Field_GetPlayerStateZoneID(system->field), params);
        if (count == 0) {
            return NULL;
        }
        if (manager.doubleBattle && count == 1) {
            manager.doubleBattle = FALSE;
            manager.pokeCount = count;
        }
        setup = BtlSetup_Create(4);
        FieldEncount_SetupRoaming(system, &manager, setup, 4, params);
    }
    EncountState_UpdateTerrain(state, player);
    return EventWildBattleCall_Create(system->gsys, system->field, setup, 0, mode);
}


GameEvent *func_ov021_021aa0a0(EncountSystem *system, u16 species, u8 level, u16 flags) {
    BtlSetup *setup;
    WildPkmParam params[2];
    EncountManager manager;
    WildPkmParam *param;

    Field_GetPlayer(system->field);
    CreateEncountManager(&manager, system->gameData, 0xff, 0, func_ov021_021a1bf4(Field_GetWeatherSystem(system->field)));
    param = &params[0];
    MI_CpuFill8(params, 0, sizeof(params));
    param->species = species;
    param->level = level;
    if (flags & 4) {
        param->item = 0xffff;
    }
    if (flags & 2) {
        param->shinyLock = 1;
    } else if (flags & 0x10) {
        param->shinyLock = 2;
    } else {
        param->shinyLock = 0;
    }
    if (flags & 8) {
        param->hiddenAbility = TRUE;
    }
    if (flags & 0x20) {
        param->sex = 1;
    } else if (flags & 0x40) {
        param->sex = 2;
    }
    setup = BtlSetup_Create(4);
    FieldEncount_SetupRoaming(system, &manager, setup, 4, params);
    if (flags & 1) {
        BtlSetup_SetFlag(setup, 4);
    } else if (flags & 0x80) {
        BtlSetup_SetFlag(setup, 0x80);
    }
    if (flags & 0x100) {
        BtlSetup_SetFlag(setup, 0x200);
    }
    return EventWildBattleCall_Create(system->gsys, system->field, setup, 1, 0);
}


BtlSetup *BtlSetup_CreateFishing(EncountSystem *system, BOOL rare) {
    WildPkmParam params[2];
    EncountManager manager;
    int encType;
    int rate;
    u32 encountRate;
    BtlSetup *setup;

    if (system->encData == NULL || !system->encData->fishingEnable) {
        return NULL;
    }
    encType = ENCTYPE_FISHING_RARE;
    if (rare != TRUE) {
        encType = ENCTYPE_FISHING;
    }
    rate = EncSys_GetEncountRateConfig(system, encType, 0);
    if (rate == 0) {
        return NULL;
    }
    CreateEncountManager(&manager, system->gameData, encType, rare, func_ov021_021a1bf4(Field_GetWeatherSystem(system->field)));
    if (rare != TRUE) {
        encountRate = FieldEncount_CalcEncountRate(&manager, system->gameData, rate);
        if (EncountState_SyncToPlayer() > encountRate) {
            return NULL;
        }
    }
    MI_CpuFill8(params, 0, sizeof(params));
    if (FieldEncount_GenWilds(system->encData, &manager, Field_GetPlayerStateZoneID(system->field), params) < manager.pokeCount) {
        return NULL;
    }
    setup = BtlSetup_Create(4);
    FieldEncount_SetupRoaming(system, &manager, setup, 4, params);
    BtlSetup_SetFlag(setup, 1);
    setup->fieldSituation.env.terrain = 6;
    return setup;
}

GameEvent *func_ov021_021aa248(EncountSystem *system, void *mapEncount) {
    FieldPlayer *player = Field_GetPlayer(system->field);
    EncountState *state = GameData_GetEncountState(system->gameData);
    u8 rateBonus;
    WildPkmParam params[2];
    EncountManager manager;
    int encType;
    BtlSetup *setup;

    EncSys_UpdateEncountRate(state, player);
    if (func_ov021_02193920(mapEncount) == 1) {
        return NULL;
    }
    if (!FieldEncount_SetupNormal(state, mapEncount)) {
        return NULL;
    }
    rateBonus = 0;
    encType = FieldEncount_DecideTypeByTerrain(system, 3, &rateBonus);
    CreateEncountManager(&manager, system->gameData, encType, 3, func_ov021_021a1bf4(Field_GetWeatherSystem(system->field)));
    MI_CpuFill8(params, 0, sizeof(params));
    if (!func_ov021_021a9604(mapEncount, &manager, params)) {
        return NULL;
    }
    setup = BtlSetup_Create(4);
    FieldEncount_SetupRoaming(system, &manager, setup, 4, params);
    EncountState_UpdateTerrain(state, player);
    return EventWildBattleCall_Create(system->gsys, system->field, setup, 0, 3);
}



void SaveBtlFieldStatus(BtlFieldStatus *status, GameData *gameData, Field *field) {
    u16 zoneId = Field_GetPlayerStateZoneID(field);
    FieldPlayer *player = Field_GetPlayer(field);
    RTCTime time;

    status->bgId = GetZoneBattleBGID(zoneId);
    status->terrain = GetTileEncountType(GetTileClass(FieldPlayer_GetTileTypeUnder(player)));
    status->zoneId = zoneId;
    RTC_GetCachedTime(&time);
    status->hour = time.hour;
    status->minute = time.minute;
    status->weather = ConvFieldWeatherToBtl(field);
    status->season = GameData_GetSeason(gameData);
}

// The encounter type of the tile under the player, 0xff for none; tall grass raises the rate by the bonus
int FieldEncount_DecideTypeByTerrain(EncountSystem *system, u32 mode, u8 *rateBonus) {
    FieldPlayer *player = Field_GetPlayer(system->field);
    Field_GetG3DMapper(system->field);
    u32 tileType;
    u32 flags;
    u16 tileClass;

    *rateBonus = 0;
    tileType = FieldPlayer_GetTileTypeUnder(player);
    if (!MapTile_IsValid(tileType)) {
        return 0xff;
    }
    flags = GetTileFlags(tileType);
    if (mode != 1 && !(flags & 4)) {
        return 0xff;
    }
    tileClass = GetTileClass(tileType);
    if (MapTile_IsReallyTallGrass(tileClass)) {
        *rateBonus += 10;
    }
    if (flags & 2) {
        if (mode == 1) {
            return ENCTYPE_SURF_RARE;
        }
        return ENCTYPE_SURF;
    }
    if (mode == 1) {
        return ENCTYPE_GRASS_SHAKING;
    }
    if (MapTile_IsTallGrassDoubleBtl(tileClass)) {
        return ENCTYPE_GRASS_RARE;
    }
    return ENCTYPE_GRASS;
}

// The encounter rate of the type, raised by the adjustment where the zone has any
static int EncSys_GetEncountRateConfig(EncountSystem *system, int encType, int adjust) {
    int rate;

    if (encType >= ENCTYPE_MAX) {
        return 0;
    }
    rate = system->encData->userData[encType];
    if (rate > 0) {
        rate += adjust;
    }
    if (rate < 0) {
        rate = 0;
    }
    return rate;
}

BOOL EncSys_CheckRNGEncounter(EncountSystem *system, EncountState *state, u32 rate) {
    u32 stepCounter;

    if (rate > 100) {
        rate = 100;
    }
    stepCounter = EncountState_GetEncountRateStepCounter(state);
    if (stepCounter == 0) {
        return FALSE;
    }
    if (stepCounter == 1) {
        rate = 1;
    }
    if (EncountState_SyncToPlayer() <= rate) {
        return TRUE;
    }
    return FALSE;
}

u32 PassPower_ApplyEncounter(u32 value);
BOOL FieldEncount_SetupNormal(EncountState *state, void *mapEncount) {
    u32 stepCounter = EncountState_GetEncountRateStepCounter(state);
    u32 value = func_ov021_021938f4(mapEncount);
    int random = GFL_RandomLC(100);
    fx32 rate;

    rate = FX_Mul((stepCounter - 1) << FX32_SHIFT, FX_Sqrt(value << FX32_SHIFT));
    rate = FX_Mul(rate, FX32_CONST(4));
    rate = PassPower_ApplyEncounter(FX_Whole(rate));
    return rate > random;
}

void func_ov021_021a96b8(PartyPkm *pkm, const EncountManager *manager, WildPkmParam *param);
void FieldEncount_SetupRoaming(EncountSystem *system, EncountManager *manager, BtlSetup *setup, HeapID heapId,
                               WildPkmParam *params) {
    PokeParty *party = PokeParty_Create(heapId);
    PartyPkm *pkm = GFL_HeapAllocate(heapId, PokeParty_GetPkmRawSize(), TRUE, "field_encount.c", 645);
    BtlFieldStatus status;
    int i;

    for (i = 0; i < (u8)manager->doubleBattle + 1; i++) {
        func_ov021_021a96b8(pkm, manager, &params[i]);
        PokeParty_AddPkm(party, pkm);
    }
    SaveBtlFieldStatus(&status, system->gameData, system->field);
    BtlSetup_SetWildNormal(setup, system->gameData, party, &status, manager->doubleBattle, heapId);
    if (manager->encType == 1) {
        BtlSetup_SetFlag(setup, 0x20);
    }
    GFL_HeapFree(pkm);
    GFL_HeapFree(party);
}

void func_ov021_021a9780(PartyPkm *pkm, const EncountManager *manager, void *roamingPkm);
void FieldEncount_SetupNPoke(EncountSystem *system, EncountManager *manager, BtlSetup *setup, HeapID heapId,
                                    void *roamingPkm) {
    PokeParty *party = PokeParty_Create(heapId);
    PartyPkm *pkm = GFL_HeapAllocate(HEAPID_TAIL(heapId), PokeParty_GetPkmRawSize(), TRUE, "field_encount.c", 676);
    BtlFieldStatus status;

    func_ov021_021a9780(pkm, manager, roamingPkm);
    PokeParty_AddPkm(party, pkm);
    SaveBtlFieldStatus(&status, system->gameData, system->field);
    BtlSetup_SetWildNormal(setup, system->gameData, party, &status, FALSE, heapId);
    BtlSetup_SetFlag(setup, 8);
    GFL_HeapFree(pkm);
    GFL_HeapFree(party);
}

void BtlSetup_SetTrainerLocal(EncountSystem *encountSystem, BtlSetup *setup, u32 style, u32 partnerId, u32 trainerId,
                              u32 trainerId2, HeapID heapId) {
    GameData *gameData = encountSystem->gameData;
    u32 terrain;
    BtlFieldStatus status;

    BtlSetup_Reset(setup);
    SaveBtlFieldStatus(&status, encountSystem->gameData, encountSystem->field);
    terrain = func_0202a420(TrainerData_GetParam(trainerId, 1));
    if (terrain != 0x11) {
        status.terrain = terrain;
    }
    switch (style) {
    case 1:
        if (partnerId) {
            BtlSetup_SetTrainer2v2(setup, gameData, &status, partnerId, trainerId, trainerId2, heapId);
            return;
        }
        BtlSetup_SetTrainer1v1Double(setup, gameData, &status, trainerId, heapId);
        return;
    case 0:
        if (trainerId2) {
            BtlSetup_SetTrainer1v2(setup, gameData, &status, trainerId, trainerId2, heapId);
            return;
        }
        BtlSetup_SetTrainer1v1Single(setup, gameData, &status, trainerId, heapId);
        return;
    case 2:
        BtlSetup_SetTrainer3v3(setup, gameData, &status, trainerId, heapId);
        return;
    case 3:
        BtlSetup_SetTrainerRotation(setup, gameData, &status, trainerId, heapId);
        break;
    }
}





u32 ConvFieldWeatherToBtl(Field *field) {
    switch (func_ov021_021a1bf4(Field_GetWeatherSystem(field))) {
    case 2:
    case 6:
    case 7:
        return 2;
    case 3:
    case 12:
        return 4;
    case 4:
    case 5:
        return 3;
    default:
        return 0;
    }
}

u32 EncountState_SyncToPlayer(void) {
    return GFL_RandomLC(0xFFFF) / 656;
}

void EncountState_UpdateTerrain(EncountState *state, FieldPlayer *player) {
    FieldActor *actor = FieldPlayer_GetActor(player);
    MI_CpuFill8(state, 0, 0xc);
    state->gridX = GetGPosX(actor);
    state->gridY = FldAct_GetGPosY(actor);
    state->gridZ = GetGPosZ(actor);
}



void EncSys_UpdateEncountRate(EncountState *state, FieldPlayer *player) {
    FieldActor *actor = FieldPlayer_GetActor(player);

    if (state->unk_6 != 0) {
        if (state->rateStepCounter >= 10 * 0x1000) {
            return;
        }
        state->rateStepCounter++;
        return;
    }

    if (state->gridX == GetGPosX(actor) &&
        state->gridY == FldAct_GetGPosY(actor) &&
        state->gridZ == GetGPosZ(actor)) {
        return;
    }

    state->unk_6 = 1;
    state->rateStepCounter++;
}

u32 EncountState_GetEncountRateStepCounter(EncountState *state) {
    return state->rateStepCounter;
}
