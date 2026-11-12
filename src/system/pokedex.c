// Black's code at main 0x0200c720-0x0200d3c0. Placed by tools/gap_files.py from 17 name, 17 pokebw2.

#include "types.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "nitro/mi.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/shortcut_util.h"
#include "constants/language.h"
#include "constants/pokemon.h"
#include "constants/species.h"

// pokebw2's names, Black's values: the Pokédex counts as complete with this many caught (the mythical species left
// out), and the regional dex number of a species outside Unova's dex
#define NATIONAL_DEX_COMPLETE 634
#define UNOVA_DEX_COMPLETE 150
#define REGIONAL_DEX_NONE 999

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

typedef struct {
    u16 species;
    u16 count;
} PokeDexFormEntry;

// The species a complete Unova Pokédex may lack (pokebw2's UNOVA_DEX_COMPLETION_EXCLUSIONS)
static const u16 data_0209de20[] = {
    SPECIES_LANDORUS, SPECIES_KYUREM, SPECIES_KELDEO, SPECIES_MELOETTA, SPECIES_GENESECT, SPECIES_VICTINI,
};

// The species a complete National Pokédex may lack (pokebw2's NATIONAL_DEX_COMPLETION_EXCLUSIONS)
// clang-format off
static const u16 data_0209de2c[] = {
    SPECIES_MEW,      SPECIES_CELEBI,   SPECIES_JIRACHI, SPECIES_DEOXYS,   SPECIES_PHIONE,
    SPECIES_MANAPHY,  SPECIES_DARKRAI,  SPECIES_SHAYMIN, SPECIES_ARCEUS,   SPECIES_LANDORUS,
    SPECIES_KYUREM,   SPECIES_KELDEO,   SPECIES_MELOETTA, SPECIES_GENESECT, SPECIES_VICTINI,
};
// clang-format on

// The species with forms and their counts, the forms' bits in the Pokédex's lists in this order
// clang-format off
static const PokeDexFormEntry pokemonFormList[] = {
    { SPECIES_UNOWN, 28 },     { SPECIES_DEOXYS, 4 },     { SPECIES_SHAYMIN, 2 },  { SPECIES_GIRATINA, 2 },
    { SPECIES_ROTOM, 6 },      { SPECIES_SHELLOS, 2 },    { SPECIES_GASTRODON, 2 }, { SPECIES_BURMY, 3 },
    { SPECIES_WORMADAM, 3 },   { SPECIES_CASTFORM, 4 },   { SPECIES_CHERRIM, 2 },  { SPECIES_DEERLING, 4 },
    { SPECIES_SAWSBUCK, 4 },   { SPECIES_MELOETTA, 2 },   { SPECIES_DARMANITAN, 2 }, { SPECIES_BASCULIN, 2 },
    { 0, 0 },
};
// clang-format on
u16 func_0200d2d8(PokeDexSave *pokedex);
u16 func_0200d310(PokeDexSave *pokedex, HeapID heapId);
u16 func_0200d368(PokeDexSave *pokedex, HeapID heapId);

u32 PML_PkmIsBadMonsNo(u32 species) {
    if (species == 0 || species > SPECIES_GENESECT) {
        return TRUE;
    }
    return FALSE;
}

static inline void SetBit(u8 *bits, u16 index) {
    bits[index >> 3] |= (u8)(1 << (index & 7));
}
void func_0200c738(PokeDexSave *pokedex, u32 species, u32 language) {
    if (language > LANGUAGE_KOREAN || language == 6) {
        return;
    }
    if (species > POKEDEX_LANGUAGE_SPECIES_MAX) {
        return;
    }
    if (language >= LANGUAGE_SPANISH) {
        language--;
    }
    SetBit(pokedex->languages, (species - 1) * POKEDEX_LANGUAGE_COUNT + (language - 1));
}


u32 PML_PkmIsNationalDexExclude(u16 species) {
    u32 i;

    for (i = 0; i < NELEMS(data_0209de2c); i++) {
        if (species == data_0209de2c[i]) {
            return 0;
        }
    }
    return 1;
}

u32 func_0200c7a0(u32 species) {
    u32 i;

    for (i = 0; i < NELEMS(data_0209de20); i++) {
        if (species == data_0209de20[i]) {
            return 0;
        }
    }
    return 1;
}

s32 func_0200c7c0(u16 species) {
    s32 index = 0;
    s32 i = 0;

    do {
        if (species == pokemonFormList[i].species) {
            return index;
        }
        index += pokemonFormList[i].count;
        i++;
    } while (pokemonFormList[i].species != 0);
    return -1;
}


s32 getIndexNumOfPkmForm(u32 species) {
    s32 i = 0;

    do {
        if (species == pokemonFormList[i].species) {
            return i;
        }
        i++;
    } while (pokemonFormList[i].species != 0);
    return -1;
}

static inline void SetSpeciesBit(u8 *bits, u16 species) {
    u16 bit = species - 1;

    bits[bit >> 3] |= (u8)(1 << (bit & 7));
}

static inline void SetSeen(PokeDexSave *pokedex, u16 kind, u16 species) {
    SetSpeciesBit(pokedex->seen + kind * POKEDEX_SPECIES_BYTES, species);
}
void func_0200c818(PokeDexSave *pokedex, u32 species, u32 sex, BOOL rare) {
    if (sex == 0) {
        if (rare == TRUE) {
            SetSeen(pokedex, POKEDEX_KIND_RARE_MALE, species);
        } else {
            SetSeen(pokedex, POKEDEX_KIND_MALE, species);
        }
    } else if (sex == 1) {
        if (rare == TRUE) {
            SetSeen(pokedex, POKEDEX_KIND_RARE_FEMALE, species);
        } else {
            SetSeen(pokedex, POKEDEX_KIND_FEMALE, species);
        }
    } else {
        if (rare == TRUE) {
            SetSeen(pokedex, POKEDEX_KIND_RARE_MALE, species);
        } else {
            SetSeen(pokedex, POKEDEX_KIND_MALE, species);
        }
    }
}


void func_0200c914(PokeDexSave *pokedex, u32 species, u32 sex, BOOL rare, u32 form) {
    int index = func_0200c7c0(species);
    u32 count;
    u16 i;

    if (index == -1) {
        return;
    }
    if (species == SPECIES_CASTFORM) {
        form = 0;
        count = 4;
    } else if (species == SPECIES_CHERRIM) {
        form = 0;
        count = 2;
    } else if (species == SPECIES_DARMANITAN) {
        form = 0;
        count = 2;
    } else if (species == SPECIES_MELOETTA) {
        form = 0;
        count = 2;
    } else {
        if (form >= getNumberOfForms(species)) {
            form = 0;
        }
        count = 1;
    }
    if (rare == TRUE) {
        for (i = 0; i < count; i++) {
            SetBit(pokedex->seenForms + POKEDEX_FORM_BYTES, i + (index + form));
        }
    } else {
        for (i = 0; i < count; i++) {
            SetBit(pokedex->seenForms, i + (index + form));
        }
    }
}


u32 getSizeofPokedexData(void) {
    return sizeof(PokeDexSave);
}

void func_0200c9f4(PokeDexSave *pokedex) {
    MI_CpuFill8(pokedex, 0, sizeof(PokeDexSave));
    pokedex->magic = POKEDEX_MAGIC;
}

PokeDexSave *func_0200ca10(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, 0x37);
}

PokeDexSave *GameData_GetPokedex(GameData *gameData) {
    return func_0200ca10(GameData_GetSaveControl(gameData));
}

void func_0200ca28(PokeDexSave *pokedex) {
    pokedex->nationalObtained = TRUE;
}

BOOL PokeDex_IsNationalObtained(PokeDexSave *pokedex) {
    return pokedex->nationalObtained;
}

void func_0200ca40(PokeDexSave *pokedex) {
    pokedex->given = 1;
}

u32 func_0200ca54(PokeDexSave *pokedex) {
    return pokedex->given;
}

void func_0200ca5c(PokeDexSave *pokedex, u32 mode) {
    pokedex->mode = mode;
}

u32 func_0200ca70(PokeDexSave *pokedex) {
    return pokedex->mode;
}

void func_0200ca78(PokeDexSave *pokedex, u16 species) {
    pokedex->lastSpecies = species;
}

u16 func_0200ca8c(PokeDexSave *pokedex) {
    return pokedex->lastSpecies;
}

static inline void ClearBit(u8 *bits, u16 index) {
    bits[index >> 3] &= (u8)((1 << (index & 7)) ^ 0xff);
}

static inline void ClearSpeciesBit(u8 *bits, u16 species) {
    species--;
    bits[species >> 3] &= (u8)((1 << (species & 7)) ^ 0xff);
}

static inline BOOL CheckBit(const u8 *bits, u16 index) {
    if (bits[index >> 3] & (1 << (index & 7))) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL CheckSpeciesBit(const u8 *bits, u16 species) {
    u16 bit = species - 1;

    if (bits[bit >> 3] & (1 << (bit & 7))) {
        return TRUE;
    }
    return FALSE;
}

static inline void SetShown(PokeDexSave *pokedex, u16 kind, u16 species) {
    SetSpeciesBit(pokedex->shown + kind * POKEDEX_SPECIES_BYTES, species);
}
void addToDex(PokeDexSave *pokedex, u16 species, u32 sex, BOOL rare, u32 form) {
    u32 i;
    u32 j;
    u32 index;

    index = 0;
    i = 0;
    do {
        if (species == pokemonFormList[i].species) {
            for (j = 0; j < pokemonFormList[i].count; j++) {
                ClearBit((u8 *)pokedex->shownForms, index + j);
                ClearBit((u8 *)(pokedex->shownForms + POKEDEX_FORM_BYTES), index + j);
            }
            if (form >= pokemonFormList[i].count) {
                form = 0;
            }
            if (rare == TRUE) {
                SetBit(pokedex->shownForms + POKEDEX_FORM_BYTES, index + form);
            } else {
                SetBit(pokedex->shownForms, index + form);
            }
            break;
        }
        index += pokemonFormList[i].count;
        i++;
    } while (pokemonFormList[i].species != 0);

    for (i = 0; i < POKEDEX_KIND_COUNT; i++) {
        ClearSpeciesBit((u8 *)(pokedex->shown + i * POKEDEX_SPECIES_BYTES), species);
    }
    if (sex == 0) {
        if (rare == TRUE) {
            SetShown(pokedex, POKEDEX_KIND_RARE_MALE, species);
        } else {
            SetShown(pokedex, POKEDEX_KIND_MALE, species);
        }
    } else if (sex == 1) {
        if (rare == TRUE) {
            SetShown(pokedex, POKEDEX_KIND_RARE_FEMALE, species);
        } else {
            SetShown(pokedex, POKEDEX_KIND_FEMALE, species);
        }
    } else {
        if (rare == TRUE) {
            SetShown(pokedex, POKEDEX_KIND_RARE_MALE, species);
        } else {
            SetShown(pokedex, POKEDEX_KIND_MALE, species);
        }
    }
}


u16 getNumberOfForms(u16 species) {
    s32 entry = getIndexNumOfPkmForm(species);

    if (entry != -1) {
        return pokemonFormList[entry].count;
    }
    return 1;
}

static inline BOOL CheckShown(const PokeDexSave *pokedex, u16 kind, u16 species) {
    return CheckSpeciesBit(pokedex->shown + kind * POKEDEX_SPECIES_BYTES, species);
}
void func_0200cc40(PokeDexSave *pokedex, u16 species, u32 *sex, u32 *rare, u32 *form, HeapID heapId) {
    s32 entry = getIndexNumOfPkmForm(species);
    void *personal;

    if (entry != -1) {
        s32 index = func_0200c7c0(species);
        u32 i;

        for (i = 0; i < pokemonFormList[entry].count; i++) {
            if (CheckBit(pokedex->shownForms, index + i)) {
                *form = i;
                *rare = FALSE;
                break;
            }
            if (CheckBit(pokedex->shownForms + POKEDEX_FORM_BYTES, index + i)) {
                *form = i;
                *rare = TRUE;
                break;
            }
        }
        if (CheckShown(pokedex, POKEDEX_KIND_MALE, species) || CheckShown(pokedex, POKEDEX_KIND_RARE_MALE, species)) {
            *sex = 0;
        }
        if (CheckShown(pokedex, POKEDEX_KIND_FEMALE, species) || CheckShown(pokedex, POKEDEX_KIND_RARE_FEMALE, species)) {
            *sex = 1;
        }
    } else {
        *form = 0;
        if (CheckShown(pokedex, POKEDEX_KIND_MALE, species)) {
            *sex = 0;
            *rare = FALSE;
        }
        if (CheckShown(pokedex, POKEDEX_KIND_FEMALE, species)) {
            *sex = 1;
            *rare = FALSE;
        }
        if (CheckShown(pokedex, POKEDEX_KIND_RARE_MALE, species)) {
            *sex = 0;
            *rare = TRUE;
        }
        if (CheckShown(pokedex, POKEDEX_KIND_RARE_FEMALE, species)) {
            *sex = 1;
            *rare = TRUE;
        }
    }
    personal = PML_PersonalLoad(species, *form, heapId);
    if (PML_PersonalGetParam(personal, 0x14) == 0xff) {
        *sex = 2;
    }
    PML_PersonalFree(personal);
}

void addPkmToDex(PokeDexSave *pokedex, PartyPkm *pkm) {
    BOOL wasEncrypted = PokeParty_DecryptPkm(pkm);
    BOOL rare;
    u32 personality;
    u32 language;
    u32 sex;
    u32 form;
    u16 species;

    if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL) == TRUE) {
        PokeParty_EncryptPkm(pkm, wasEncrypted);
        return;
    }
    species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    personality = PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL);
    form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    language = PokeParty_GetParam(pkm, PKM_PARAM_REGION, NULL);
    sex = PokeParty_GetSex(pkm);
    rare = PokeParty_IsRare(pkm);
    PokeParty_EncryptPkm(pkm, wasEncrypted);
    if (PML_PkmIsBadMonsNo(species)) {
        return;
    }
    if (!PokeDex_IsSeen(pokedex, species)) {
        addToDex(pokedex, species, sex, rare, form);
    }
    if (species == SPECIES_SPINDA && pokedex->spindaPersonality == 0) {
        pokedex->spindaPersonality = personality;
    }
    func_0200c818(pokedex, species, sex, rare);
    func_0200c914(pokedex, species, sex, rare, form);
    func_0200c738(pokedex, species, language);
    SetSpeciesBit(pokedex->caught, species);
}


BOOL PokeDex_IsCaught(PokeDexSave *pokedex, u16 species) {
    if (PML_PkmIsBadMonsNo(species)) {
        return FALSE;
    }
    return CheckSpeciesBit(pokedex->caught, species);
}

u32 func_0200cf08(PokeDexSave *pokedex, HeapID heapId) {
    if (PokeDex_IsNationalObtained(pokedex)) {
        return PokeDex_GetCaughtNoNational(pokedex);
    }
    return func_0200cf54(pokedex, heapId);
}

u16 PokeDex_GetCaughtNoNational(PokeDexSave *pokedex) {
    int count = 0;
    int i;

    for (i = 1; i <= SPECIES_GENESECT; i++) {
        if (PokeDex_IsCaught(pokedex, i) == TRUE) {
            count++;
        }
    }
    return count;
}

u16 func_0200cf54(PokeDexSave *pokedex, HeapID heapId) {
    u16 i;
    u16 count = 0;
    u16 *table = PML_PersonalLoadRegionalDexTable(heapId, NULL);

    for (i = 1; i <= SPECIES_GENESECT; i++) {
        if (PokeDex_IsCaught(pokedex, i) == TRUE && table[i] != REGIONAL_DEX_NONE) {
            count++;
        }
    }
    GFL_HeapFree(table);
    return count;
}

void PokeDex_RegistPkm(PokeDexSave *pokedex, PartyPkm *pkm) {
    BOOL wasEncrypted = PokeParty_DecryptPkm(pkm);
    BOOL rare;
    u32 personality;
    u32 sex;
    u32 form;
    u16 species;

    if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL) == TRUE) {
        PokeParty_EncryptPkm(pkm, wasEncrypted);
        return;
    }
    species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    personality = PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL);
    form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    sex = PokeParty_GetSex(pkm);
    rare = PokeParty_IsRare(pkm);
    PokeParty_EncryptPkm(pkm, wasEncrypted);
    if (PML_PkmIsBadMonsNo(species)) {
        return;
    }
    if (!PokeDex_IsSeen(pokedex, species)) {
        addToDex(pokedex, species, sex, rare, form);
    }
    if (species == SPECIES_SPINDA && pokedex->spindaPersonality == 0) {
        pokedex->spindaPersonality = personality;
    }
    func_0200c818(pokedex, species, sex, rare);
    func_0200c914(pokedex, species, sex, rare, form);
}


static inline const u8 *SeenList(const PokeDexSave *pokedex, u16 kind) {
    return pokedex->seen + kind * POKEDEX_SPECIES_BYTES;
}

static inline BOOL CheckSeen(const PokeDexSave *pokedex, u16 kind, u16 species) {
    return CheckSpeciesBit(SeenList(pokedex, kind), species);
}
BOOL PokeDex_IsSeen(PokeDexSave *pokedex, u16 species) {
    u32 i;

    if (PML_PkmIsBadMonsNo(species)) {
        return FALSE;
    }
    for (i = 0; i < POKEDEX_KIND_COUNT; i++) {
        if (CheckSeen(pokedex, i, species)) {
            return TRUE;
        }
    }
    return FALSE;
}


u32 func_0200d0ac(PokeDexSave *pokedex, HeapID heapId) {
    if (PokeDex_IsNationalObtained(pokedex)) {
        return PokeDex_GetSeenNoNational(pokedex);
    }
    return func_0200d0f8(pokedex, heapId);
}

u16 PokeDex_GetSeenNoNational(PokeDexSave *pokedex) {
    int count = 0;
    int i;

    for (i = 1; i <= SPECIES_GENESECT; i++) {
        if (PokeDex_IsSeen(pokedex, i) == TRUE) {
            count++;
        }
    }
    return count;
}

u16 func_0200d0f8(PokeDexSave *pokedex, HeapID heapId) {
    u16 i;
    u16 count = 0;
    u16 *table = PML_PersonalLoadRegionalDexTable(heapId, NULL);

    for (i = 1; i <= SPECIES_GENESECT; i++) {
        if (PokeDex_IsSeen(pokedex, i) == TRUE && table[i] != REGIONAL_DEX_NONE) {
            count++;
        }
    }
    GFL_HeapFree(table);
    return count;
}

BOOL func_0200d144(PokeDexSave *pokedex, u16 species, u32 sex, BOOL rare, u32 form) {
    s32 index;

    if (PML_PkmIsBadMonsNo(species)) {
        return -1;
    }
    index = func_0200c7c0(species);
    if (index != -1) {
        return CheckBit(pokedex->seenForms + rare * POKEDEX_FORM_BYTES, index + form);
    }
    if (sex == 0) {
        if (rare == TRUE) {
            return CheckSeen(pokedex, POKEDEX_KIND_RARE_MALE, species);
        } else {
            return CheckSeen(pokedex, POKEDEX_KIND_MALE, species);
        }
    } else if (sex == 1) {
        if (rare == TRUE) {
            return CheckSeen(pokedex, POKEDEX_KIND_RARE_FEMALE, species);
        } else {
            return CheckSeen(pokedex, POKEDEX_KIND_FEMALE, species);
        }
    } else {
        if (rare == TRUE) {
            if (CheckSeen(pokedex, POKEDEX_KIND_RARE_MALE, species) || CheckSeen(pokedex, POKEDEX_KIND_RARE_FEMALE, species)) {
                return TRUE;
            }
        } else {
            if (CheckSeen(pokedex, POKEDEX_KIND_MALE, species) || CheckSeen(pokedex, POKEDEX_KIND_FEMALE, species)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}


u32 func_0200d288(PokeDexSave *pokedex, BOOL flag) {
    if (flag == 0) {
        return pokedex->spindaPersonality;
    }
    return 0;
}

u32 PokeDex_IsCompleteNational(PokeDexSave *pokedex) {
    if (func_0200d2d8(pokedex) >= NATIONAL_DEX_COMPLETE) {
        return TRUE;
    }
    return FALSE;
}

u32 PokeDex_IsCompleteUnova(PokeDexSave *pokedex, HeapID heapId) {
    if (func_0200d310(pokedex, heapId) >= UNOVA_DEX_COMPLETE) {
        return TRUE;
    }
    return FALSE;
}

u32 PokeDex_IsSeenAllUnova(PokeDexSave *pokedex, HeapID heapId) {
    if (func_0200d368(pokedex, heapId) >= UNOVA_DEX_COMPLETE) {
        return TRUE;
    }
    return FALSE;
}

u16 func_0200d2d8(PokeDexSave *pokedex) {
    s32 i;
    u16 count = 0;

    for (i = 1; i <= 0x289; i++) {
        if (PokeDex_IsCaught(pokedex, i) == TRUE && PML_PkmIsNationalDexExclude(i) == TRUE) {
            count++;
        }
    }
    return count;
}


u16 func_0200d310(PokeDexSave *pokedex, HeapID heapId) {
    u16 i;
    u16 count = 0;
    u16 *table = PML_PersonalLoadRegionalDexTable(heapId, NULL);

    for (i = 1; i <= SPECIES_GENESECT; i++) {
        if (PokeDex_IsCaught(pokedex, i) == TRUE && table[i] != REGIONAL_DEX_NONE && func_0200c7a0(i) == TRUE) {
            count++;
        }
    }
    GFL_HeapFree(table);
    return count;
}

u16 func_0200d368(PokeDexSave *pokedex, HeapID heapId) {
    u16 i;
    u16 count = 0;
    u16 *table = PML_PersonalLoadRegionalDexTable(heapId, NULL);

    for (i = 1; i <= SPECIES_GENESECT; i++) {
        if (PokeDex_IsSeen(pokedex, i) == TRUE && table[i] != REGIONAL_DEX_NONE && func_0200c7a0(i) == TRUE) {
            count++;
        }
    }
    GFL_HeapFree(table);
    return count;
}

BOOL PokeDex_HasLanguage(PokeDexSave *pokedex, u16 species, u32 language) {
    if (language > LANGUAGE_KOREAN || language == 6) {
        return FALSE;
    }
    if (species > POKEDEX_LANGUAGE_SPECIES_MAX) {
        return FALSE;
    }
    if (language >= LANGUAGE_SPANISH) {
        language--;
    }
    return CheckBit(pokedex->languages, (species - 1) * POKEDEX_LANGUAGE_COUNT + (language - 1));
}
