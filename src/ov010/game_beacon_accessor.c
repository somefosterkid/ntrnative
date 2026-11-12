// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/game_beacon_accessor.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "system/game_beacon.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/version.h"
#include "constants/zones.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "save/high_link.h"
#include "save/survey_savedata.h"
#include "system/pms_data.h"
#include "system/wordset.h"

// The tables of received beacons and the readers of a beacon's fields, which give a default for a value out of range.
// The file's name is from the ROM's string.

// What a beacon's message shows and its payload holds, GameBeaconTypeFormat's format
enum {
    BEACON_FORMAT_NONE,
    BEACON_FORMAT_TRAINER,
    BEACON_FORMAT_SPECIES,
    BEACON_FORMAT_NICKNAME,
    BEACON_FORMAT_SPECIES_NICKNAME,
    BEACON_FORMAT_ITEM,
    BEACON_FORMAT_NUMBER3,
    BEACON_FORMAT_NUMBER3_LONG,
    BEACON_FORMAT_GIFT_SPECIES,
    BEACON_FORMAT_GIFT_ITEM,
    BEACON_FORMAT_MOVE,
    BEACON_FORMAT_NUMBER1,
    BEACON_FORMAT_BATTLE_INSTITUTE,
    BEACON_FORMAT_PASS_POWER,
    BEACON_FORMAT_TEXT,
    BEACON_FORMAT_GIFT_PASS_POWER,
    BEACON_FORMAT_COUNT
};

#define BEACON_FORMAT_TYPE_MAX 0x33
// The kinds of a recent event, with GAME_BEACON_RECENT_NONE
#define BEACON_RECENT_KIND_COUNT 6
#define BEACON_PLAY_HOURS_MAX 999
#define BEACON_PLAY_MINUTES_MAX 59
#define BEACON_ZONE_COUNT (ZONE_PLEDGE_GROVE + 1)
#define BEACON_MOVE_COUNT (MOVE_FUSION_BOLT + 1)

static int GameBeaconInfoTable_Add(GameBeaconInfoTable *table, const GameBeacon *beacon, const GameBeaconTime *time,
                         BOOL overwrite);
u16 func_ov010_02165e34(const GameBeacon *beacon);
u16 func_ov010_02165e78(const GameBeacon *beacon);
u16 func_ov010_02165e8c(const GameBeacon *beacon);
u16 func_ov010_02165e9c(const GameBeacon *beacon);
u16 func_ov010_02165eac(const GameBeacon *beacon);
void func_ov010_02165ecc(const GameBeacon *beacon, StrBuf *dest);
u16 func_ov010_02165f30(const GameBeacon *beacon);
u32 func_ov010_02165f64(const GameBeacon *beacon);
u16 func_ov010_02165f74(const GameBeacon *beacon);
u32 func_ov010_02165f84(const GameBeacon *beacon);
u8 func_ov010_02165f94(const GameBeacon *beacon);
u16 func_ov010_02165fa4(const GameBeacon *beacon);
u8 func_ov010_02165fb4(u16 type);
void loadTrainerNamesToStrbuf(WordSet *wordSet, u32 index, u32 trainerId);
void func_0201f1e0(WordSet *wordSet, u32 index, u32 value);
BOOL func_ov010_021661cc(const GameBeacon *beacon);
BOOL func_ov010_021661ec(const GameBeacon *beacon);
BOOL func_ov010_02166200(const GameBeacon *beacon);
BOOL func_ov010_02166218(const GameBeacon *beacon);
BOOL func_ov010_02166230(const GameBeacon *beacon);
BOOL func_ov010_02166248(const GameBeacon *beacon);
BOOL func_ov010_0216625c(const GameBeacon *beacon);

BOOL func_ov010_02166164(const GameBeacon *beacon);
BOOL func_ov010_02166168(const GameBeacon *beacon);
BOOL func_ov010_02166180(const GameBeacon *beacon);
BOOL func_ov010_02166198(const GameBeacon *beacon);
BOOL func_ov010_0216619c(const GameBeacon *beacon);
BOOL func_ov010_021661b4(const GameBeacon *beacon);
BOOL func_ov010_0216626c(const GameBeacon *beacon);
BOOL func_ov010_0216627c(const GameBeacon *beacon);

// Whether the payload of a beacon is invalid, by format
// pokebw2's sFormatChecks
static BOOL (*const data_ov010_02171ca8[BEACON_FORMAT_COUNT])(const GameBeacon *beacon) = {
    func_ov010_02166164, func_ov010_02166168, func_ov010_02166180, func_ov010_02166198,
    func_ov010_0216619c, func_ov010_021661b4, func_ov010_021661cc, func_ov010_021661ec,
    func_ov010_02166200, func_ov010_02166218, func_ov010_02166230, func_ov010_02166248,
    func_ov010_0216625c, func_ov010_0216626c, func_ov010_02166164, func_ov010_0216627c,
};

// By beacon type; the types from BEACON_FORMAT_TYPE_MAX on have no format
// pokebw2's sTypeFormats
static const GameBeaconTypeFormat data_ov010_02171ce8[BEACON_FORMAT_TYPE_MAX] = {
#include "asset/game_beacon_accessor_data_ov010_02171ce8.inc"
};

// Stores a beacon in the list, over the one of the same trainer or in the first free entry, and returns the entry.
// When the list is full, it drops the first beacon if overwrite is set, and else returns GAME_BEACON_LOG_MAX
static int GameBeaconInfoTable_Add(GameBeaconInfoTable *table, const GameBeacon *beacon, const GameBeaconTime *time,
                         BOOL overwrite) {
    int i;

    for (i = 0; i < GAME_BEACON_LOG_MAX; i++) {
        if (table->beacons[i].targetVersions == 0 || beacon->trainerId == table->beacons[i].trainerId) {
            table->beacons[i] = *beacon;
            table->times[i] = (time->hour << 8) | time->minute;
            return i;
        }
    }
    if (overwrite == TRUE) {
        MIi_CpuCopy32(&table->beacons[1], &table->beacons[0], sizeof(GameBeacon) * (GAME_BEACON_LOG_MAX - 1));
        MIi_CpuCopy16(&table->times[1], &table->times[0], sizeof(u16) * (GAME_BEACON_LOG_MAX - 1));
        table->beacons[GAME_BEACON_LOG_MAX - 1] = *beacon;
        table->times[GAME_BEACON_LOG_MAX - 1] = (time->hour << 8) | time->minute;
        return GAME_BEACON_LOG_MAX - 1;
    }
    return GAME_BEACON_LOG_MAX;
}

// pokebw2's GameBeacon_IsGiftType

BOOL func_ov010_02165b5c(const GameBeacon *beacon) {
    if (beacon->type == 0 || beacon->type >= BEACON_FORMAT_TYPE_MAX) {
        return TRUE;
    }
    if (beacon->recentKind >= 6) {
        return TRUE;
    }
    if (beacon->surveyRank >= 6) {
        return TRUE;
    }
    if (beacon->playHours > 999 || beacon->playMinutes > 59) {
        return TRUE;
    }
    if (beacon->passPower > 0x2d && beacon->passPower != GAME_BEACON_PASS_POWER_NONE) {
        return TRUE;
    }
    if (beacon->zoneId >= 0x1ab || beacon->parentZoneId >= 0x1ab) {
        return TRUE;
    }
    if (beacon->unk04 > 99999 || beacon->unk08 > 99999) {
        return TRUE;
    }
    if (data_ov010_02171ca8[func_ov010_02165fb4(beacon->type)](beacon)) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_02165c10(const GameBeacon *beacon) {
    switch (beacon->type) {
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x32:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GameBeacon_GetName
void func_ov010_02165c34(const GameBeacon *beacon, StrBuf *dest) {
    GFL_StrBufLoadFixedString(dest, beacon->name, 8);
}

// pokebw2's GameBeacon_GetVersion
u32 func_ov010_02165c48(const GameBeacon *beacon) {
    return beacon->version;
}

// pokebw2's GameBeacon_GetLanguage
u32 func_ov010_02165c50(const GameBeacon *beacon) {
    return beacon->language;
}

// pokebw2's GameBeacon_GetCountry
u32 func_ov010_02165c58(const GameBeacon *beacon) {
    return beacon->country;
}

// pokebw2's GameBeacon_GetRegion
u8 func_ov010_02165c5c(const GameBeacon *beacon) {
    return beacon->region;
}

u8 func_ov010_02165c60(GameBeacon *beacon) {
    u32 answer = func_0200e3a4(beacon->surveyAnswers, 0x1a);

    if (answer == 0) {
        return 0xff;
    }
    return answer;
}

u8 func_ov010_02165c78(GameBeacon *beacon) {
    u32 answer = func_0200e3a4(beacon->surveyAnswers, 0x19);

    if (answer == 0) {
        return 0xff;
    }
    return answer;
}

// pokebw2's GameBeacon_GetCGearRecord
void func_ov010_02165c90(const GameBeacon *beacon, PMSData *dest) {
    dest->type = beacon->cgear0;
    dest->id = beacon->cgear1;
    dest->words[0] = beacon->cgear2;
    dest->words[1] = beacon->cgear3;
}

// pokebw2's GameBeacon_GetSurveyRank
u8 func_ov010_02165cb4(const GameBeacon *beacon) {
    return beacon->surveyRank;
}

void func_ov010_02165cc0(u16 *dest, const GameBeacon *beacon) {
    u32 color = beacon->favoriteColor;
    const u16 *colors = OS_GetFavoriteColorTable();

    *dest = colors[color & 0xf];
}

// pokebw2's GameBeacon_GetFavoriteColor
u8 func_ov010_02165cd8(const GameBeacon *beacon) {
    return beacon->favoriteColor;
}

// pokebw2's GameBeacon_GetTrainerId
u16 func_ov010_02165ce4(const GameBeacon *beacon) {
    return beacon->trainerId;
}

// pokebw2's GameBeacon_GetTrainerViewWithGender
u8 func_ov010_02165ce8(const GameBeacon *beacon) {
    if (beacon->gender == 0) {
        return beacon->trainerView;
    }
    return beacon->trainerView + 8;
}

// pokebw2's GameBeacon_GetGender
u32 func_ov010_02165d08(const GameBeacon *beacon) {
    return beacon->gender;
}

// pokebw2's GameBeacon_GetPlayTime
void func_ov010_02165d10(const GameBeacon *beacon, u16 *hours, u16 *minutes) {
    *hours = beacon->playHours;
    *minutes = beacon->playMinutes;
}

// pokebw2's GameBeacon_GetPassPower
u32 func_ov010_02165d20(const GameBeacon *beacon) {
    return beacon->passPower;
}

// pokebw2's GameBeacon_GetZone
u16 func_ov010_02165d28(const GameBeacon *beacon) {
    return beacon->zoneId;
}

// pokebw2's GameBeacon_GetParentZone
u16 func_ov010_02165d34(const GameBeacon *beacon) {
    return beacon->parentZoneId;
}

u32 func_ov010_02165d40(const GameBeacon *beacon) {
    return beacon->unk08;
}

u32 func_ov010_02165d48(const GameBeacon *beacon) {
    return beacon->unk04;
}

// pokebw2's GameBeacon_GetGreeting
void func_ov010_02165d50(const GameBeacon *beacon, StrBuf *dest) {
    GFL_StrBufClear(dest);
    GFL_StrBufLoadFixedString(dest, beacon->greeting, 9);
}

void func_ov010_02165d6c(const GameBeacon *beacon, StrBuf *dest) {
    if (beacon->type != 0x18) {
        GFL_StrBufClear(dest);
        return;
    }
    GFL_StrBufLoadString(dest, beacon->payload.message.text);
}

void func_ov010_02165d8c(const GameBeacon *beacon, StrBuf *dest) {
    if (beacon->type != 0x31) {
        GFL_StrBufClear(dest);
        return;
    }
    GFL_StrBufLoadString(dest, beacon->payload.text);
}

// pokebw2's GameBeacon_GetRecentKind
u32 func_ov010_02165dac(const GameBeacon *beacon) {
    u32 kind = beacon->recentKind;

    if (kind >= BEACON_RECENT_KIND_COUNT) {
        kind = GAME_BEACON_RECENT_NONE;
    }
    return kind;
}

// pokebw2's GameBeacon_GetRecentTrainer
u16 func_ov010_02165dbc(const GameBeacon *beacon) {
    u32 trainerId = beacon->recentValue;

    switch (beacon->recentKind) {
    case 2:
    case 3:
    case 4:
        if (trainerId == 0 || trainerId >= 0x268) {
            trainerId = 1;
        }
        return trainerId;
    }
    return 1;
}

// pokebw2's GameBeacon_GetRecentSpecies
u16 func_ov010_02165df0(const GameBeacon *beacon) {
    u32 species = beacon->recentValue;

    switch (beacon->recentKind) {
    case 0:
    case 1:
        if (species == SPECIES_NONE || species > SPECIES_GENESECT) {
            species = SPECIES_PATRAT;
        }
        return species;
    }
    return SPECIES_PATRAT;
}

// pokebw2's GameBeacon_GetType
u16 func_ov010_02165e24(const GameBeacon *beacon) {
    u16 type = beacon->type;

    if (type == 0 || type >= 0x33) {
        type = 1;
    }
    return type;
}

// The readers of a type's value: it, or a default for another type

u16 func_ov010_02165e34(const GameBeacon *beacon) {
    switch (beacon->type) {
    case 0x2:
    case 0x3:
    case 0x4:
    case 0x5:
    case 0xe:
    case 0xf:
    case 0x11:
    case 0x26:
        return beacon->arg.value;
    }
    return SPECIES_PATRAT;
}

u16 func_ov010_02165e78(const GameBeacon *beacon) {
    switch (beacon->type) {
    case 0x13:
    case 0x23:
        return beacon->payload.value;
    }
    return ITEM_POTION;
}

u16 func_ov010_02165e8c(const GameBeacon *beacon) {
    if (beacon->type == 0x24) {
        return beacon->payload.value;
    }
    return MOVE_POUND;
}

u16 func_ov010_02165e9c(const GameBeacon *beacon) {
    if (beacon->type == 0x19) {
        return beacon->payload.gift.species;
    }
    return SPECIES_PATRAT;
}

u16 func_ov010_02165eac(const GameBeacon *beacon) {
    if (beacon->type == 0x1a) {
        return beacon->payload.gift.item;
    }
    return ITEM_POTION;
}

// pokebw2's GameBeacon_GetGiftPassPower
u16 func_ov010_02165ebc(const GameBeacon *beacon) {
    if (beacon->type == 0x32) {
        return beacon->payload.gift.passPower;
    }
    return 0;
}

// Loads the nickname of the types that send one. The switch does nothing; its cases are the types whose format has
// a nickname, and 0x17 is a guess, since the code only shows that the cases from 0x10 to 0x26 are dense
void func_ov010_02165ecc(const GameBeacon *beacon, StrBuf *dest) {
    GFL_StrBufClear(dest);
    GFL_StrBufLoadFixedString(dest, beacon->payload.text, GAME_BEACON_TEXT_LEN + 1);
    switch (beacon->type) {
    case 0x10:
    case 0x11:
    case 0x17:
    case 0x1c:
    case 0x1d:
    case 0x1f:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x26:
    case 0x2f:
        return;
    }
}

u16 func_ov010_02165f30(const GameBeacon *beacon) {
    switch (beacon->type) {
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    case 0xa:
    case 0xb:
        return beacon->payload.value;
    }
    return 1;
}

u32 func_ov010_02165f64(const GameBeacon *beacon) {
    if (beacon->type == 0x16) {
        return beacon->payload.value32;
    }
    return 1;
}

u16 func_ov010_02165f74(const GameBeacon *beacon) {
    if (beacon->type == 0x14) {
        return beacon->payload.value;
    }
    return 1;
}

u32 func_ov010_02165f84(const GameBeacon *beacon) {
    if (beacon->type == 0x29) {
        return beacon->payload.value32;
    }
    return 1;
}

u8 func_ov010_02165f94(const GameBeacon *beacon) {
    if (beacon->type == 0x2c) {
        return beacon->payload.value8;
    }
    return 0;
}

u16 func_ov010_02165fa4(const GameBeacon *beacon) {
    if (beacon->type == 0x30) {
        return beacon->payload.value;
    }
    return 0;
}

u8 func_ov010_02165fb4(u16 type) {
    return data_ov010_02171ce8[type].format;
}

u8 func_ov010_02165fc0(u32 type) {
    return data_ov010_02171ce8[type].unk1;
}


void func_ov010_02165fcc(const GameBeacon *beacon, WordSet *wordSet, HeapID heapId) {
    StrBuf *dest;
    u8 kind;
    u16 trainerId;

    GFL_WordSetClearAll(wordSet);
    dest = GFL_StrBufCreate(0x21, heapId);
    if (dest != NULL) {
        func_ov010_02165c34(beacon, dest);
        func_0201ee0c(wordSet, 0, dest, 0, 1, 2);
        kind = func_ov010_02165fb4(func_ov010_02165e24(beacon));
        switch (kind) {
        case 0:
        case 15:
            break;
        case 1:
            trainerId = func_ov010_02165f30(beacon);
            loadTrainerTypeToStrbuf(wordSet, 1, trainerId);
            loadTrainerNamesToStrbuf(wordSet, 2, trainerId);
            break;
        case 2:
        case 4:
            WordSet_LoadSpeciesName(wordSet, 1, func_ov010_02165e34(beacon));
            if (kind == 2) {
                break;
            }
            /* fall through */
        case 3:
            func_ov010_02165ecc(beacon, dest);
            func_0201ee0c(wordSet, 2, dest, 0, 1, 2);
            break;
        case 5:
            loadItemTextNameToStrbuf(wordSet, 1, func_ov010_02165e78(beacon));
            break;
        case 6:
            WordSetNumber(wordSet, 1, func_ov010_02165f74(beacon), 3, 0, 1);
            break;
        case 7:
            WordSetNumber(wordSet, 1, func_ov010_02165f64(beacon), 3, 0, 1);
            break;
        case 8:
            WordSet_LoadSpeciesName(wordSet, 1, func_ov010_02165e9c(beacon));
            break;
        case 9:
            loadItemNameToStrbuf(wordSet, 1, func_ov010_02165eac(beacon));
            break;
        case 10:
            loadMoveNameToStrbuf(wordSet, 1, func_ov010_02165e8c(beacon));
            break;
        case 11:
            WordSetNumber(wordSet, 1, func_ov010_02165f84(beacon), 1, 0, 1);
            break;
        case 12:
            func_0201f1e0(wordSet, 1, func_ov010_02165f94(beacon));
            break;
        case 13:
            loadPassPowerToStrbuf(wordSet, 1, func_ov010_02165fa4(beacon));
            break;
        case 14:
            func_ov010_02165d8c(beacon, dest);
            func_0201ee0c(wordSet, 1, dest, func_ov010_02165d08(beacon), 1, 2);
            break;
        }
        GFL_StrBufFree(dest);
    }
}


BOOL func_ov010_02166164(const GameBeacon *beacon) {
    return FALSE;
}

BOOL func_ov010_02166168(const GameBeacon *beacon) {
    if (beacon->payload.value == 0 || beacon->payload.value >= 0x268) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_02166180(const GameBeacon *beacon) {
    if (beacon->arg.value == SPECIES_NONE || beacon->arg.value > SPECIES_GENESECT) {
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov010_02166198(const GameBeacon *beacon) {
    return FALSE;
}

BOOL func_ov010_0216619c(const GameBeacon *beacon) {
    if (beacon->arg.value == SPECIES_NONE || beacon->arg.value > SPECIES_GENESECT) {
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov010_021661b4(const GameBeacon *beacon) {
    if (beacon->payload.value == ITEM_NONE || beacon->payload.value > 0x272) {
        return TRUE;
    }
    return FALSE;
}


// The checks of a beacon's payload, by format: whether it is invalid

BOOL func_ov010_021661cc(const GameBeacon *beacon) {
    if (beacon->playHours > BEACON_PLAY_HOURS_MAX || beacon->playMinutes > BEACON_PLAY_MINUTES_MAX) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_021661ec(const GameBeacon *beacon) {
    if (beacon->payload.value32 > 99999) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_02166200(const GameBeacon *beacon) {
    if (beacon->payload.gift.species == SPECIES_NONE || beacon->payload.gift.species > SPECIES_GENESECT) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_02166218(const GameBeacon *beacon) {
    if (beacon->payload.gift.item == ITEM_NONE || beacon->payload.gift.item > ITEM_LAST) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_02166230(const GameBeacon *beacon) {
    if (beacon->payload.value == MOVE_NONE || beacon->payload.value >= BEACON_MOVE_COUNT) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_02166248(const GameBeacon *beacon) {
    if (beacon->payload.value32 == 0 || beacon->payload.value32 > 7) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_0216625c(const GameBeacon *beacon) {
    if (beacon->payload.value8 > 6) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_0216626c(const GameBeacon *beacon) {
    if (beacon->payload.value < 0x30) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov010_0216627c(const GameBeacon *beacon) {
    if (beacon->payload.text[2] < 0x30) {
        return FALSE;
    }
    return TRUE;
}


