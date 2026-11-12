// Written from Black's assembly. EncountSave_GetRoamingPkmZone is the same code as Black 2's, in pokebw2 (GPL-3.0,
// https://github.com/fuddlesworth/pokebw2) src/ov012/field_encount_st.c.
#include "types.h"
#include "battle/btl_setup.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "field/move_pokemon.h"
#include "field/player_state.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "pml/poke_party.h"
#include "save/encounter.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/rtc.h"
#include "system/zone_weather.h"

// The places in ROAMING_POKEMON_ZONES a roaming Pokémon can move to, by day period
typedef struct {
    u16 count;
    u8 clocks[4];
} RoamingPkmMoves;

const RoamingPkmMoves data_ov010_021711a8[] = {
    {3, {0, 1, 15}},
    {3, {2, 3, 14}},
    {3, {4, 5, 6}},
    {3, {7, 8, 9}},
    {4, {10, 11, 12, 13}},
};

// The zones a roaming Pokémon moves between, then 0xffff while it hides: 17 entries, as the readers' index > 16 test
// says (the zero word after them is hiden_event.c's)
const u16 ROAMING_POKEMON_ZONES[17] = {
    0x13f, 0x141, 0x146, 0x149, 0x14b, 0x151, 0x159, 0x15c, 0x163, 0x16d, 0x170, 0x172, 0x176, 0x17a, 0x17f, 0x183,
    0xffff,
};

static u8 func_ov010_0215e724(u16 species);
static void func_ov010_0215e740(EncountSave *save, u8 season, u8 slot, u8 prev);
static void func_ov010_0215e7dc(EncountSave *save, u8 slot, u8 OS_GetTick, u16 zoneId);
static void *func_ov010_0215e804(EncountSave *save, u16 species);

// Whether the roaming Pokémon of the slot is in the zone (a descriptive name)
static inline BOOL IsRoamingInZone(EncountSave *save, u8 slot, u16 zoneId) {
    return EncountSave_GetRoamingPkmStatus(save, slot) && zoneId == EncountSave_GetRoamingPkmZone(save, slot);
}

void func_ov010_0215e358(EncountSave *save, u8 season, u8 slot) {
    func_ov010_0215e740(save, season, slot, 0xff);
}

void func_ov010_0215e364(GameData *gameData) {
    u8 i;
    EncountSave *save = SaveControl_GetEncountSave(GameData_GetSaveControl(gameData));
    u8 season = GameData_GetSeason(gameData);

    for (i = 0; i < 2; i++) {
        if (EncountSave_GetRoamingPkmStatus(save, i)) {
            func_ov010_0215e358(save, season, i);
        }
    }
}

void func_ov010_0215e3a0(GameData *gameData) {
    u8 i;
    EncountSave *save = SaveControl_GetEncountSave(GameData_GetSaveControl(gameData));
    u8 season = GameData_GetSeason(gameData);

    for (i = 0; i < 2; i++) {
        EncountSave_GetRoamingPkm(save, i);
        if (EncountSave_GetRoamingPkmStatus(save, i)) {
            func_ov010_0215e358(save, season, i);
        }
    }
}

u16 EncountSave_GetRoamingPkmZone(EncountSave *save, u8 slot) {
    u32 OS_GetTick = EncountSave_GetRoamingPkmZoneClock(save, slot);

    if (OS_GetTick > 16) {
        return 319;
    }
    return ROAMING_POKEMON_ZONES[OS_GetTick];
}

BOOL func_ov010_0215e404(EncountSave *save) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (EncountSave_GetRoamingPkmStatus(save, i)) {
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov010_0215e428(EncountSave *save, u16 zoneId) {
    if (func_ov010_0215e404(save)) {
        func_0200d45c(save, zoneId);
    }
}

void func_ov010_0215e440(GameData *gameData, u8 slot) {
    void *roamingPkm;
    EncountSave *save;
    SaveControl *saveControl;
    u32 species;
    u8 level;
    u8 unk9;
    PlayerInfo *info;
    PartyPkm *pkm;

    saveControl = GameData_GetSaveControl(gameData);
    save = SaveControl_GetEncountSave(saveControl);
    roamingPkm = EncountSave_GetRoamingPkm(save, slot);

    // Each legend's data, set per case: the two share their level and parameter 9, which the code still holds in
    // variables (a stack slot and a saved register, set in the cases' merged tail)
    switch (slot) {
    case 0:
        species = SPECIES_THUNDURUS;
        level = 40;
        unk9 = 1;
        break;
    case 1:
        species = SPECIES_TORNADUS;
        level = 40;
        unk9 = 1;
        break;
    default:
        return;
    }
    func_0200d50c(roamingPkm, ROAMING_PKM_PARAM_SPECIES, species);
    func_0200d50c(roamingPkm, ROAMING_PKM_PARAM_LEVEL, level);
    func_0200d50c(roamingPkm, ROAMING_PKM_PARAM_UNK13, unk9);
    info = GetGameDataPlayerInfo(gameData);
    pkm = GFL_HeapAllocate(HEAPID_TAIL(HEAPID_GAMESYSTEM), PokeParty_GetPkmRawSize(), TRUE, "move_pokemon.c", 302);
    PokeParty_CreatePkm(pkm, species, level, getIDAsUInt(info), PKM_IVS_RANDOM, PKM_PID_RANDOM);
    func_0200d50c(roamingPkm, ROAMING_PKM_PARAM_STATUS, 0);
    func_0200d50c(roamingPkm, ROAMING_PKM_PARAM_ACTIVE, TRUE);
    func_0200d50c(roamingPkm, ROAMING_PKM_PARAM_IVS, PokeParty_GetParam(pkm, PKM_PARAM_IVS_ALL, NULL));
    func_0200d50c(roamingPkm, ROAMING_PKM_PARAM_PERSONALITY, PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL));
    func_0200d50c(roamingPkm, ROAMING_PKM_PARAM_NATURE, PokeParty_GetParam(pkm, PKM_PARAM_NATURE, NULL));
    func_0200d50c(roamingPkm, ROAMING_PKM_PARAM_HP, PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL));
    PokeDex_RegistPkm(func_0200ca10(saveControl), pkm);
    GFL_HeapFree(pkm);
    func_ov010_0215e740(save, GameData_GetSeason(gameData), slot, 5);
    func_0200d470(save, slot, 1);
}

void func_ov010_0215e580(GameData *gameData) {
    EncountSave *save = SaveControl_GetEncountSave(GameData_GetSaveControl(gameData));
    int i;

    for (i = 0; i < 2; i++) {
        if (func_0200d468(save, i) == 2) {
            func_ov010_0215e440(gameData, i);
        }
    }
}

void func_ov010_0215e5b0(GameData *gameData, BtlSetup *setup) {
    u16 zoneId;
    PartyPkm *pkm;
    u16 species;
    u16 hp;
    u8 status;
    u8 slot;
    EncountSave *save;
    void *roamingPkm;

    if (setup->battleStyle != 0 || setup->battleType != 0) {
        return;
    }
    zoneId = PlayerState_GetZoneID(GameData_GetPlayerState(gameData));
    pkm = PokeParty_GetPkm(BtlSetup_GetParty(setup, 1), 0);
    species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    hp = PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL);
    status = PokeParty_GetParam(pkm, PKM_PARAM_STATUS, NULL);
    save = SaveControl_GetEncountSave(GameData_GetSaveControl(gameData));
    roamingPkm = func_ov010_0215e804(save, species);
    if (roamingPkm == NULL) {
        if (GFL_RandomLCAlt(100) < 30) {
            func_ov010_0215e364(gameData);
        }
        return;
    }
    slot = func_ov010_0215e724(species);
    if ((setup->unk8C == 1 || setup->unk8C == 2) && hp == 0) {
        func_0200d494(roamingPkm);
        func_0200d480(save, slot, 16);
        func_0200d470(save, slot, 2);
        return;
    }
    if (setup->unk8C == 5) {
        func_0200d494(roamingPkm);
        func_0200d480(save, slot, 16);
        func_0200d470(save, slot, 3);
        return;
    }
    func_0200d50c(roamingPkm, 5, hp);
    func_0200d50c(roamingPkm, 7, status);
    func_ov010_0215e364(gameData);
}

u32 GetDefaultWeatherValue(GameData *gameData, u16 zoneId) {
    EncountSave *save = SaveControl_GetEncountSave(GameData_GetSaveControl(gameData));

    if (IsRoamingInZone(save, 0, zoneId)) {
        return 6;
    }
    if (IsRoamingInZone(save, 1, zoneId)) {
        return 7;
    }
    return WEATHER_NONE;
}

BOOL func_ov010_0215e6f8(GameData *gameData) {
    EncountSave *save = SaveControl_GetEncountSave(GameData_GetSaveControl(gameData));

    if (EncountSave_GetRoamingPkmStatus(save, 0) || EncountSave_GetRoamingPkmStatus(save, 1)) {
        return TRUE;
    }
    return FALSE;
}

static u8 func_ov010_0215e724(u16 species) {
    switch (species) {
    case SPECIES_THUNDURUS:
        return 0;
    case SPECIES_TORNADUS:
        return 1;
    }
    return 2;
}

static void func_ov010_0215e740(EncountSave *save, u8 season, u8 slot, u8 prev) {
    const RoamingPkmMoves *moves;
    u8 candidates[4];
    u32 OS_GetTick = EncountSave_GetRoamingPkmZoneClock(save, slot);
    u16 period = GetRealTimeDayPeriod(season);
    int count;
    int i;
    u8 next;
    u16 zoneId;

    moves = &data_ov010_021711a8[period];

    if (OS_GetTick != 16 && GFL_RandomLCAlt(100) < 20) {
        func_ov010_0215e7dc(save, slot, 16, 0xffff);
        return;
    }
    count = 0;
    for (i = 0; i < moves->count; i++) {
        u8 clockI = moves->clocks[i];
        if (clockI != OS_GetTick && clockI != prev) {
            candidates[count++] = clockI;
        }
    }
    next = candidates[GFL_RandomLCAlt(count)];
    if (next > 16) {
        zoneId = 319;
    } else {
        zoneId = ROAMING_POKEMON_ZONES[next];
    }
    func_ov010_0215e7dc(save, slot, next, zoneId);
}

static void func_ov010_0215e7dc(EncountSave *save, u8 slot, u8 OS_GetTick, u16 zoneId) {
    void *roamingPkm = EncountSave_GetRoamingPkm(save, slot);

    func_0200d480(save, slot, OS_GetTick);
    func_0200d50c(roamingPkm, 1, zoneId);
}

static void *func_ov010_0215e804(EncountSave *save, u16 species) {
    u8 i;
    void *roamingPkm;

    for (i = 0; i < 2; i++) {
        if (EncountSave_GetRoamingPkmStatus(save, i)) {
            roamingPkm = EncountSave_GetRoamingPkm(save, i);
            if (species == EncountSave_GetRoamingPkmParam(roamingPkm, 4)) {
                return roamingPkm;
            }
        }
    }
    return NULL;
}
