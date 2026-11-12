// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/enc_pokeset.c, ported by tools/port_file.py.
// The wild Pokémon of an encounter: the slots of the zone's encounter data and swarms, the lead Pokémon's abilities,
// the roaming Pokémon and N's Pokémon. The name is descriptive; the ROM has no string for this file.
// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "constants/abilities.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/version.h"
#include "field/enc_pokeset.h"
#include "field/encounter.h"
#include "field/field.h"
#include "field/field_wfbc.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/encounter.h"
#include "save/event_work.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/aeabi.h"
#include "system/game_data.h"


// The flag that marks the swarms, which overlay 36's swarm table lists
#define FLAG_SWARMS 0x960
// The flag that lets N's Pokémon appear
#define FLAG_N_POKEMON 0x98a

static void SetupEncountFlagsByPartyPokeAbil(EncountManager *manager, u16 weather);
static BOOL FieldEncount_CheckLevelRepelled(EncountManager *manager, u8 level);
BOOL createSwarm(EncData *encData, EncountManager *manager, u16 zoneId, WildEncSlot *slots);
u8 FieldEncount_PrepareActiveSlots(EncData *encData, EncountManager *manager, u16 zoneId, WildEncSlot *slots);
static void FieldEncount_SetupFromData(const WildEncSlot *slot, WildPkmParam *param);
static u8 GetEncountIndexForPreferredType(EncountManager *manager, const WildEncSlot *slots, u8 type);
static void FieldEncount_CheckApplyHighLevelWilds(const EncountManager *manager, const WildEncSlot *slots,
                                                  WildPkmParam *param);
static void FieldEncount_GenWildPokeParam(EncountManager *manager, const WildEncSlot *slots, WildPkmParam *param);
void FieldEncount_SetRandomHeldItem(PartyPkm *pkm, void *personal, BOOL compoundEyes, BOOL doubleBattle);
u32 FieldEncount_GenPID(const EncountManager *manager, void *personal, WildPkmParam *param);
static u32 GetEncountSlotByProbability(void);
int EncountMakeRnd12(int count);
int EncountMakeRnd5T1(int count);
int EncountMakeRnd5T2(int count);
static int EncountMakeRndFull(int count);

// The chance of each held item of the species' three, in percent: without and with Compound Eyes, in normal and in dark
// grass
static const u8 data_ov021_021d8dcc[4][3] = {
#include "asset/enc_pokeset_data_ov021_021d8dcc.inc"
};

static int (*const ENCOUNT_SLOT_RANDOMIZE_FUNCS[4])(int count) = {
    EncountMakeRnd12,
    EncountMakeRnd5T1,
    EncountMakeRnd5T2,
    EncountMakeRndFull,
};

// The chance of each slot, in percent, for each row the Lucky Encounter pass powers pick
static const u8 ENCOUNTRATE_RND5T1[4][5] = {
    {60, 30, 5, 4, 1},
    {50, 30, 10, 5, 5},
    {40, 30, 10, 10, 10},
    {30, 20, 10, 20, 20},
};

static const u8 ENCOUNTRATE_RND5T2[4][5] = {
    {40, 40, 15, 4, 1},
    {40, 35, 15, 5, 5},
    {30, 30, 20, 10, 10},
    {20, 20, 20, 20, 20},
};

static const u8 ENCOUNTRATE_RND12[4][12] = {
    {20, 20, 10, 10, 10, 10, 5, 5, 4, 4, 1, 1},
    {10, 10, 10, 10, 10, 10, 10, 10, 5, 5, 5, 5},
    {5, 5, 5, 5, 10, 10, 10, 10, 10, 10, 10, 10},
    {1, 1, 4, 4, 5, 5, 10, 10, 10, 10, 20, 20},
};

static const SwarmData data_ov021_021d8de8[17] = {
#include "asset/enc_pokeset_data_ov021_021d8de8.inc"
};


void CreateEncountManager(EncountManager *manager, GameData *gameData, int encType, u32 mode, u16 weather) {
    SaveControl *saveControl = GameData_GetSaveControl(gameData);
    PokeParty *party = GameData_GetParty(gameData);
    PartyPkm *pkm;

    MI_CpuFill8(manager, 0, sizeof(EncountManager));
    manager->encType = encType;
    manager->mode = mode;
    manager->gameData = gameData;
    manager->encountSave = SaveControl_GetEncountSave(saveControl);
    if (encType == ENCTYPE_FISHING || encType == ENCTYPE_FISHING_RARE) {
        manager->isFishing = TRUE;
    }
    manager->playerInfo = GetGameDataPlayerInfo(gameData);
    manager->trainerId = getIDAsUInt(manager->playerInfo);
    pkm = PokeParty_GetPkm(party, 0);
    manager->leadIsEgg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    if (manager->leadIsEgg == FALSE) {
        manager->leadSpecies = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        manager->leadItem = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
        manager->leadAbility = PokeParty_GetParam(pkm, PKM_PARAM_ABILITY, NULL);
        manager->leadSex = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
        manager->leadNature = PokeParty_GetNature(pkm);
        SetupEncountFlagsByPartyPokeAbil(manager, weather);
    }
    manager->swarmsEnabled = EventWork_FlagGet(GameData_GetEventWork(gameData), FLAG_SWARMS);
    if (!EncountSave_IsRepelDepleted(SaveControl_GetEncountSave(saveControl))) {
        manager->repelActive = TRUE;
    }
    if (manager->mode == 2) {
        manager->forced = TRUE;
        manager->mode = 0;
    }
    if (manager->mode == 1) {
        manager->forced = TRUE;
    }
    manager->pairActive = FALSE;
    manager->doubleBattle = FALSE;
    if (manager->encType == ENCTYPE_GRASS_RARE) {
        if (howManyPokesAreAbleToFight(party) >= 2 && GetEncountSlotByProbability() < 40) {
            manager->doubleBattle = TRUE;
        }
    }
    if (manager->repelsWeaker || manager->repelActive) {
        pkm = PokeParty_GetPkm(party, PokeParty_GetFirstBattleReady(party));
        manager->leadLevel = PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL);
    }
    manager->pokeCount = manager->doubleBattle + 1;
}

u32 FieldEncount_CalcEncountRate(EncountManager *manager, GameData *gameData, u32 baseRate) {
    u32 rate = baseRate;

    if (manager->isFishing) {
        if (manager->fishingRateUp) {
            rate *= 2;
        }
        if (rate > 100) {
            rate = 100;
        }
        return rate;
    }
    if (manager->rateUp) {
        rate *= 2;
    } else if (manager->rateDown) {
        rate /= 2;
    }
    if (manager->leadItem == ITEM_CLEANSE_TAG || manager->leadItem == ITEM_PURE_INCENSE) {
        rate = baseRate * 2 / 3;
    }
    if (rate > 100) {
        rate = 100;
    }
    return PassPower_ApplyEncounter(rate);
}

int FieldEncount_GenWildsCore(EncountManager *manager, const WildEncSlot *slots, WildPkmParam *params) {
    int count = 0;
    int i;

    for (i = 0; i < manager->pokeCount; i++) {
        WildPkmParam *param = &params[count];

        FieldEncount_GenWildPokeParam(manager, slots, param);
        if (!FieldEncount_CheckLevelRepelled(manager, param->level)) {
            count++;
        }
    }
    return count;
}

u32 FieldEncount_GenWilds(EncData *encData, EncountManager *manager, u16 zoneId, WildPkmParam *params) {
    WildEncSlot aWStack_48 [12];
    WildPkmParam *pWStack_18;

    pWStack_18 = params;
    MIi_CpuClear32(0, aWStack_48, 0x30);
    FieldEncount_PrepareActiveSlots(encData, manager, zoneId, aWStack_48);
    return FieldEncount_GenWildsCore(manager, aWStack_48, params);
}

void *func_ov021_021a957c(EncData *encData, EncountManager *manager, u16 zoneId) {
    void *roaming[2];
    u8 count = 0;
    u8 i;

    for (i = 0; i < 2; i++) {
        if (EncountSave_GetRoamingPkmStatus(manager->encountSave, i)) {
            void *roamingPkm = EncountSave_GetRoamingPkm(manager->encountSave, i);

            if (zoneId == EncountSave_GetRoamingPkmParam(roamingPkm, 1) &&
                !FieldEncount_CheckLevelRepelled(manager, EncountSave_GetRoamingPkmParam(roamingPkm, 6))) {
                roaming[count++] = roamingPkm;
            }
        }
    }
    if (count == 0 || GFL_RandomLCAlt(1000) < 1000 / 2) {
        return NULL;
    }
    return roaming[GFL_RandomLCAlt(count)];
}


BOOL func_ov021_021a9604(void *mapEncount, EncountManager *manager, WildPkmParam *param) {
    u32 slotIndex;
    u32 sum;
    u32 randomResult;
    u32 runningSum;
    int i;
    FieldWFBCPerson *slot;
    u8 *personal;

    switch (manager->encType) {
    case 0:
        slotIndex = 0;
        break;
    case 1:
        slotIndex = 1;
        break;
    case 3:
        slotIndex = 2;
        break;
    case 0xff:
        return FALSE;
    }

    sum = 0;
    for (i = 0; i < 0x1e; i++) {
        slot = func_ov021_0219390c(mapEncount, i);
        if (slot != NULL) {
            personal = (u8 *)func_ov021_02193c00(slot);
            sum += personal[slotIndex + 0x14];
        }
    }

    randomResult = GFL_RandomLC(sum);

    runningSum = 0;
    for (i = 0; i < 0x1e; i++) {
        slot = func_ov021_0219390c(mapEncount, i);
        if (slot != NULL) {
            personal = (u8 *)func_ov021_02193c00(slot);
            runningSum += personal[slotIndex + 0x14];
            if (randomResult < runningSum) {
                MI_CpuFill8(param, 0, sizeof(WildPkmParam));
                param->species = ((u16 *)(personal + 8))[slotIndex];
                param->level = (personal + slotIndex)[0x10];
                return TRUE;
            }
        }
    }
    return FALSE;
}

void func_ov021_021a96b8(PartyPkm *pkm, const EncountManager *manager, WildPkmParam *param) {
    void *personal = PML_PersonalLoad(param->species, param->form, 4);
    u32 pid = FieldEncount_GenPID(manager, personal, param);

    PokeParty_CreatePkm(pkm, param->species, param->level, manager->trainerId, PKM_IVS_RANDOM, pid);
    PokeParty_SetParam(pkm, PKM_PARAM_FORM, param->form);
    if (param->hiddenAbility) {
        PokeParty_SetHiddenAbil(pkm, param->species, param->form);
    }
    if (param->item <= ITEM_LAST) {
        if (param->item != 0) {
            PokeParty_SetParam(pkm, PKM_PARAM_ITEM, param->item);
        } else {
            FieldEncount_SetRandomHeldItem(pkm, personal, manager->compoundEyes, manager->encType == ENCTYPE_GRASS_RARE);
        }
    }
    PokeParty_SetParam(pkm, PKM_PARAM_OT_GENDER, getTrainerGender(manager->playerInfo));
    PokeParty_SetParam(pkm, PKM_PARAM_OT_NAME_RAW, (uintptr_t)GetPlayerName(manager->playerInfo));
    if (manager->synchronize) {
        PokeParty_SetParam(pkm, PKM_PARAM_NATURE, manager->leadNature);
    }
    PokeParty_RecalcStats(pkm);
    PML_PersonalFree(personal);
}


void func_ov021_021a9780(PartyPkm *pkm, const EncountManager *manager, void *roamingPkm) {
    u16 species = EncountSave_GetRoamingPkmParam(roamingPkm, 4);
    u8 level = EncountSave_GetRoamingPkmParam(roamingPkm, 6);
    u32 ivs = EncountSave_GetRoamingPkmParam(roamingPkm, 2);

    PokeParty_CreatePkm(pkm, species, level, manager->trainerId, ivs, EncountSave_GetRoamingPkmParam(roamingPkm, 3));
    PokeParty_SetParam(pkm, PKM_PARAM_NATURE, EncountSave_GetRoamingPkmParam(roamingPkm, 10));
    PokeParty_SetParam(pkm, PKM_PARAM_STATUS, EncountSave_GetRoamingPkmParam(roamingPkm, 7));
    PokeParty_SetParam(pkm, PKM_PARAM_HP, EncountSave_GetRoamingPkmParam(roamingPkm, 5));
    PokeParty_RecalcStats(pkm);
}


u16 getSwarmLevelRangeFromData(GameData *gameData) {
    const SwarmData *swarm = getSwarmDataPtr(gameData);

    if (swarm == NULL) {
        return 0xffff;
    }
    return swarm->zoneId;
}

static void SetupEncountFlagsByPartyPokeAbil(EncountManager *manager, u16 weather) {
    switch (manager->leadAbility) {
    case ABILITY_ILLUMINATE:
    case ABILITY_ARENA_TRAP:
    case ABILITY_NO_GUARD:
        manager->rateUp = TRUE;
        return;
    case ABILITY_STENCH:
    case ABILITY_WHITE_SMOKE:
    case ABILITY_QUICK_FEET:
        manager->rateDown = TRUE;
        return;
    case ABILITY_SAND_VEIL:
        if (weather == 3 || weather == 12) {
            manager->rateDown = TRUE;
        }
        return;
    case ABILITY_SNOW_CLOAK:
        if (weather == 1 || weather == 4 || weather == 5) {
            manager->rateDown = TRUE;
        }
        return;
    case ABILITY_STICKY_HOLD:
    case ABILITY_SUCTION_CUPS:
        if (manager->isFishing) {
            manager->fishingRateUp = TRUE;
        }
        return;
    case ABILITY_COMPOUNDEYES:
        manager->compoundEyes = TRUE;
        return;
    }
    if (manager->leadAbility == ABILITY_CUTE_CHARM && GetEncountSlotByProbability() < 67) {
        manager->cuteCharm = TRUE;
        return;
    }
    if (GetEncountSlotByProbability() < 50) {
        return;
    }
    switch (manager->leadAbility) {
    case ABILITY_MAGNET_PULL:
        manager->magnetPull = TRUE;
        break;
    case ABILITY_STATIC:
        manager->staticAbility = TRUE;
        break;
    case ABILITY_PRESSURE:
    case ABILITY_HUSTLE:
    case ABILITY_VITAL_SPIRIT:
        manager->higherLevel = TRUE;
        break;
    case ABILITY_INTIMIDATE:
    case ABILITY_KEEN_EYE:
        manager->repelsWeaker = TRUE;
        break;
    case ABILITY_SYNCHRONIZE:
        manager->synchronize = TRUE;
        break;
    }
}

static BOOL FieldEncount_CheckLevelRepelled(EncountManager *manager, u8 level) {
    if (manager->forced) {
        return FALSE;
    }
    if (manager->repelsWeaker && manager->leadLevel >= level + 5) {
        return TRUE;
    }
    if (!manager->isFishing && manager->repelActive && level < manager->leadLevel) {
        return TRUE;
    }
    return FALSE;
}

const SwarmData *getSwarmDataPtr(GameData *gameData) {
    SaveControl *save = GameData_GetSaveControl(gameData);
    u8 location;

    if (!EventWork_FlagGet(GameData_GetEventWork(gameData), FLAG_SWARMS)) {
        return NULL;
    }
    location = EncountSave_GetSwarmLocation(SaveControl_GetEncountSave(save));
    if (location >= 17) {
        location = 0;
    }
    return &data_ov021_021d8de8[location];
}

BOOL createSwarm(EncData *encData, EncountManager *manager, u16 zoneId, WildEncSlot *slots) {
    const SwarmData *swarm = getSwarmDataPtr(manager->gameData);

    if (swarm == NULL || swarm->zoneId != zoneId) {
        return FALSE;
    }
    if (manager->encType != ENCTYPE_GRASS) {
        return FALSE;
    }
    if (GFL_RandomLCAlt(1000) > 400) {
        return FALSE;
    }
    slots[0].species = swarm->species;
    slots[0].minLevel = swarm->minLevel;
    slots[0].maxLevel = swarm->maxLevel;
    slots[0].form = swarm->form;
    manager->slotFuncIndex = 3;
    manager->slotCount = 1;
    return TRUE;
}

u8 FieldEncount_PrepareActiveSlots(EncData *encData, EncountManager *manager, u16 zoneId, WildEncSlot *slots) {
    u8 count = 0;
    const WildEncSlot *src;
    u32 funcIndex = 0;

    if (createSwarm(encData, manager, zoneId, slots)) {
        return manager->slotCount;
    }
    if (manager->encType >= ENCTYPE_MAX) {
        manager->encType = ENCTYPE_GRASS;
    }
    switch (manager->encType) {
    case ENCTYPE_GRASS:
        count = 12;
        src = encData->grass;
        break;
    case ENCTYPE_GRASS_RARE:
        count = 12;
        src = encData->grassRare;
        break;
    case ENCTYPE_GRASS_SHAKING:
        count = 12;
        src = encData->grassShaking;
        break;
    case ENCTYPE_SURF:
        funcIndex = 1;
        count = 5;
        src = encData->surf;
        break;
    case ENCTYPE_SURF_RARE:
        funcIndex = 1;
        count = 5;
        src = encData->surfRare;
        break;
    case ENCTYPE_FISHING:
        funcIndex = 2;
        count = 5;
        src = encData->fishing;
        break;
    case ENCTYPE_FISHING_RARE:
        funcIndex = 2;
        count = 5;
        src = encData->fishingRare;
        break;
    }
    manager->slotFuncIndex = funcIndex;
    manager->slotCount = count;
    MI_CpuCopy8(src, slots, count * sizeof(WildEncSlot));
    return count;
}

static void FieldEncount_SetupFromData(const WildEncSlot *slot, WildPkmParam *param) {
    s16 range = slot->maxLevel - slot->minLevel;
    u8 formCount;

    MI_CpuFill8(param, 0, sizeof(WildPkmParam));
    if (range < 0) {
        range = 0;
    }
    param->level = slot->minLevel + GetEncountSlotByProbability() % (range + 1);
    param->species = slot->species;
    formCount = PML_PersonalGetParamSingle(slot->species, 0, 0x20);
    if (slot->form == 0x1f) {
        param->form = GFL_RandomLCAlt(formCount);
    } else if (slot->form >= formCount) {
        param->form = 0;
    } else {
        param->form = slot->form;
    }
}

static u8 GetEncountIndexForPreferredType(EncountManager *manager, const WildEncSlot *slots, u8 type) {
    u8 candidates[12];
    int i;
    int count = 0;

    for (i = 0; i < manager->slotCount; i++) {
        if (slots[i].form != 0x1f) {
            u8 type1 = PML_PersonalGetParamSingle(slots[i].species, slots[i].form, 6);
            u8 type2 = PML_PersonalGetParamSingle(slots[i].species, slots[i].form, 7);

            if (type1 == type || type2 == type) {
                candidates[count++] = i;
            }
        }
    }
    if (count == 0) {
        return 0xff;
    }
    return candidates[GetEncountSlotByProbability() % count];
}

static void FieldEncount_CheckApplyHighLevelWilds(const EncountManager *manager, const WildEncSlot *slots,
                                                  WildPkmParam *param) {
    u8 level = param->level;
    int i;
    u8 maxLevel = level;
    BOOL hasRange = FALSE;

    if (!manager->higherLevel) {
        return;
    }
    for (i = 0; i < manager->slotCount; i++) {
        if (!hasRange && slots[i].minLevel != slots[i].maxLevel) {
            hasRange = TRUE;
        }
        if (param->species == slots[i].species && maxLevel < slots[i].maxLevel) {
            maxLevel = slots[i].maxLevel;
        }
    }
    if (level + 5 <= maxLevel && hasRange == TRUE) {
        param->level += 5;
    } else {
        param->level = maxLevel;
    }
}

static void FieldEncount_GenWildPokeParam(EncountManager *manager, const WildEncSlot *slots, WildPkmParam *param) {
    u8 index = 0xff;

    if (manager->magnetPull) {
        index = GetEncountIndexForPreferredType(manager, slots, 8);
    } else if (manager->staticAbility) {
        index = GetEncountIndexForPreferredType(manager, slots, 12);
    }
    if (index == 0xff) {
        index = ENCOUNT_SLOT_RANDOMIZE_FUNCS[manager->slotFuncIndex](manager->slotCount);
    }
    FieldEncount_SetupFromData(&slots[index], param);
    FieldEncount_CheckApplyHighLevelWilds(manager, slots, param);
}

void FieldEncount_SetRandomHeldItem(PartyPkm *pkm, void *personal, BOOL compoundEyes, BOOL doubleBattle) {
    u16 items[3];
    const u8 *odds;
    u8 roll;
    u8 sum;
    int i;

    for (i = 0; i < 3; i++) {
        items[i] = PML_PersonalGetParam(personal, 0x11 + i);
    }
    if (items[0] == items[1]) {
        PokeParty_SetParam(pkm, PKM_PARAM_ITEM, items[0]);
        return;
    }
    odds = data_ov021_021d8dcc[doubleBattle * 2 + compoundEyes];
    roll = GetEncountSlotByProbability();
    sum = 0;
    for (i = 0; i < 3; i++) {
        sum += odds[i];
        if (roll < sum) {
            PokeParty_SetParam(pkm, PKM_PARAM_ITEM, items[i]);
            return;
        }
    }
}

u32 FieldEncount_GenPID(const EncountManager *manager, void *personal, WildPkmParam *param) {
    u32 sex = 2;
    u32 shiny;
    u32 pid;
    u8 genderRatio = PML_PersonalGetParam(personal, 0x14);

    if (isGenderlessOrSetGender(genderRatio) != TRUE) {
        if (manager->cuteCharm) {
            if (manager->leadSex == 0) {
                sex = 1;
            } else if (manager->leadSex == 1) {
                sex = 0;
            }
        }
        if (param->sex == 1) {
            sex = 0;
        } else if (param->sex == 2) {
            sex = 1;
        }
    }
    if (param->shinyLock == 2) {
        shiny = 0;
    } else {
        shiny = 2;
    }
    pid = PML_GenPID(manager->trainerId, param->species, param->form, sex, 2, shiny);
    if (param->shinyLock != 2) {
        if ((((manager->trainerId >> 16) ^ (manager->trainerId & 0xffff)) ^ (pid & 0xffff)) & 1) {
            return pid | 0x80000000;
        }
        return pid & 0x7fffffff;
    }
    return pid;
}

u32 GetEncountSlotByProbability(void) {
    return GFL_RandomLC(0xFFFF) / 656;
}

int EncountMakeRnd12(int count) {
    u32 r = GetEncountSlotByProbability();
    if (r < 0x14) {
        return 0;
    }
    if (r >= 0x14 && r < 0x28) {
        return 1;
    }
    if (r >= 0x28 && r < 0x32) {
        return 2;
    }
    if (r >= 0x32 && r < 0x3c) {
        return 3;
    }
    if (r >= 0x3c && r < 0x46) {
        return 4;
    }
    if (r >= 0x46 && r < 0x50) {
        return 5;
    }
    if (r >= 0x50 && r < 0x55) {
        return 6;
    }
    if (r >= 0x55 && r < 0x5a) {
        return 7;
    }
    if (r >= 0x5a && r < 0x5e) {
        return 8;
    }
    if (r >= 0x5e && r < 0x62) {
        return 9;
    }
    if (r == 0x62) {
        return 10;
    }
    return 11;
}

int EncountMakeRnd5T1(int count) {
    u32 slot = GetEncountSlotByProbability();
    if (slot < 0x3c)
        return 0;
    if (slot >= 0x3c && slot < 0x5a)
        return 1;
    if (slot >= 0x5a && slot < 0x5f)
        return 2;
    if (slot >= 0x5f && slot < 0x63)
        return 3;
    return 4;
}

int EncountMakeRnd5T2(int count) {
    u32 result = GetEncountSlotByProbability();
    if (result < 0x28)
        return 0;
    if (result < 0x50)
        return 1;
    if (result < 0x5f)
        return 2;
    if (result < 0x63)
        return 3;
    return 4;
}


static int EncountMakeRndFull(int count) {
    return GetEncountSlotByProbability() % count;
}
