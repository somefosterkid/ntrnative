// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/game_beacon.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/btl_main.h"
#include "constants/language.h"
#include "constants/species.h"
#include "constants/version.h"
#include "field/festival.h"
#include "field/player_state.h"
#include "field/survey.h"
#include "field/unity_tower.h"
#include "field/townmap_util.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_whpipe.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "pml/item.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/ir_check.h"
#include "system/pms.h"
#include "constants/pokemon.h"
#include "gfl/rtc_cache.h"
#include "nitro/mi.h"
#include "save/hall_of_fame.h"
#include "save/playtime.h"
#include "save/pokedex.h"
#include "save/survey_savedata.h"

void GameBeaconSendSlot_SetMissionEx(GameBeaconSendSlot *slot, u16 type, u32 value, u32 extra);
u32 func_0202cfac(u32 a0, u16 a1);

// The game's beacons. The file's name is from the ROM's string

// Zones from this one on are new in Black 2 and White 2
#define ZONE_ID_BW1_MAX 0x1ab
// The last survey question
#define SURVEY_QUESTION_LAST 0x1d
// The survey question answered by play time, in tens of hours
#define SURVEY_QUESTION_PLAY_TIME 0x1d

// A beacon type's priority, against the current type's (func_02027060), and how many frames it is sent
typedef struct {
    u32 priority : 8;
    u32 lifetime : 24;
} GameBeaconTypeInfo;

void func_02026824(GameBeaconSendSlot *slot);
static void func_0202685c(GameBeaconSystem *sys, const GameBeacon *beacon);
static void GameBeaconSys_AddLog(GameBeaconSystem *sys, const GameBeacon *beacon);
static BOOL GameBeacon_IsBW1Incompatible(const GameBeacon *beacon, BOOL b2w2Only);
static void GameBeacon_PrepareSend(GameBeacon *beacon, BOOL b2w2Only);
static void GameBeacon_FilterRecent(GameBeacon *beacon, BOOL received);
static void GameBeaconSys_CheckMission(GameBeaconSystem *sys, const GameBeacon *beacon);
static void GameBeaconSys_UpdateNotices(GameBeaconSystem *sys, const GameBeacon *beacon, BOOL isNew);
static GameBeacon *func_02026bfc(GameBeaconSystem *sys, int index);
void func_02026fb8(GameBeaconSendSlot *slot);
static GameBeacon *func_02026c30(GameBeaconSystem *sys, u32 n);
void func_02026d88(GameBeacon *beacon, GameData *gameData);
static BOOL GameBeaconSys_AddAvenueVisitor(GameBeaconSystem *sys, const GameBeacon *beacon, u32 a2);
static BOOL GameBeaconSys_UpdateAvenueVisitor(GameBeaconSystem *sys);
static void GameBeaconSendSlot_ResetHeader(GameBeaconSendSlot *slot, u8 targetVersions);
void func_020270ac(const StrBuf *str, u16 *dest);
static void func_020271f8(u16 species);
void func_02027220(GameBeacon *beacon, u16 species);
void func_02027230(u16 species);
void func_02027258(GameBeacon *beacon, u16 species);
static BOOL GameBeaconSys_SendCaptureMission(u16 species, BOOL a1, BOOL a2);
void func_02027290(GameBeacon *beacon, const StrBuf *nickname);
static void func_020272d0(GameBeacon *beacon, u16 species, const StrBuf *nickname);
void func_020272ec(u16 hours);
void func_02027314(GameBeacon *beacon, u16 hours);
void func_0202734c(GameBeacon *beacon, u16 item);
static void func_0202737c(GameBeacon *beacon, u16 zoneId, GameData *gameData);
void func_02027460(GameBeacon *beacon, const PMSData *record);
void func_020274b4(const PMSData *record);
static void GameBeaconSys_SendAvenueAd(GameBeaconSystem *sys);
static BOOL GameBeaconSys_UpdateAvenueAdTimer(GameBeaconSystem *sys);

static void func_020274f8(TCB *tcb, void *data);
static void func_02027584(GameBeaconSystem *sys);

// The frames between Join Avenue advertisements, by advertisement
static const u16 sAvenueAdIntervals[7] = { 30, 30, 30, 30, 30, 30, 30 };

// Black 2's first 14, without its legendaries of other regions
// pokebw2's sSpecialSpecies
static const u16 data_0209fc04[14] = {
    SPECIES_ZOROARK,   SPECIES_COBALION, SPECIES_TERRAKION, SPECIES_VIRIZION, SPECIES_TORNADUS,
    SPECIES_THUNDURUS, SPECIES_RESHIRAM, SPECIES_ZEKROM,    SPECIES_LANDORUS, SPECIES_KYUREM,
    SPECIES_KELDEO,    SPECIES_MELOETTA, SPECIES_GENESECT,  SPECIES_VICTINI,
};

// pokebw2's sGameBeaconTypeInfo
static const GameBeaconTypeInfo data_0209fc20[GAME_BEACON_TYPE_MAX] = {
    { 0, 600 },    // 0x00
    { 0, 600 },    // 0x01
    { 0, 600 },    // 0x02
    { 0, 600 },    // 0x03
    { 0, 600 },    // 0x04
    { 0, 600 },    // 0x05
    { 0, 600 },    // 0x06
    { 0, 600 },    // 0x07
    { 0, 900 },    // 0x08
    { 0, 900 },    // 0x09
    { 0, 900 },    // 0x0a
    { 0, 900 },    // 0x0b
    { 0, 900 },    // 0x0c
    { 0, 900 },    // 0x0d
    { 0, 600 },    // 0x0e
    { 1, 900 },    // 0x0f
    { 0, 600 },    // 0x10
    { 0, 600 },    // 0x11
    { 5, 1200 },   // 0x12
    { 0, 600 },    // 0x13
    { 2, 900 },    // 0x14
    { 2, 900 },    // 0x15
    { 2, 900 },    // 0x16
    { 0, 600 },    // 0x17
    { 3, 600 },    // 0x18
    { 5, 1200 },   // 0x19
    { 5, 1200 },   // 0x1a
    { 5, 1200 },   // 0x1b
    { 1, 600 },    // 0x1c
    { 1, 600 },    // 0x1d
    { 4, 600 },    // 0x1e
    { 2, 600 },    // 0x1f
    { 1, 600 },    // 0x20
    { 3, 600 },    // 0x21
    { 1, 600 },    // 0x22
    { 0, 600 },    // 0x23
    { 0, 600 },    // 0x24
    { 0, 600 },    // 0x25
    { 0, 600 },    // 0x26
    { 0, 600 },    // 0x27
    { 1, 600 },    // 0x28
    { 2, 600 },    // 0x29
    { 3, 900 },    // 0x2a
    { 1, 600 },    // 0x2b
    { 2, 600 },    // 0x2c
    { 1, 600 },    // 0x2d
    { 1, 600 },    // 0x2e
    { 1, 600 },    // 0x2f
    { 2, 600 },    // 0x30
    { 3, 1200 },   // 0x31
    { 5, 1200 },   // 0x32
};

// pokebw2's GameBeaconSys
GameBeaconSystem *data_02146940;

// pokebw2's GameBeaconSys_Create
void func_020266e0(HeapID heapId) {
    GFL_ASSERT(data_02146940 == NULL);
    data_02146940 = GFL_HeapAllocate(heapId, sizeof(GameBeaconSystem), TRUE, "game_beacon.c", 90);
    data_02146940->logNewest = -1;
}

// pokebw2's GameBeaconSys_Update, without its Join Avenue
void func_02026728(void) {
    GameBeaconSystem *sys = data_02146940;
    u16 hours;
    int i;

    if (sys == NULL || func_0203cd98() == FALSE) {
        return;
    }
    if (sys->gameData != NULL) {
        hours = func_0200864c(func_02012fcc(sys->gameData));
        if (sys->playHours != hours) {
            switch (hours) {
            case 10:
            case 30:
            case 50:
            case 100:
                func_020272ec(hours);
                break;
            }
        }
        sys->playHours = hours;
    }
    func_02026824(&sys->mine);
    for (i = 0; i < GAME_BEACON_LOG_MAX; i++) {
        if (sys->log[i].beacon.targetVersions != 0 && sys->log[i].age < 0xffff) {
            sys->log[i].age++;
        }
    }
}

// pokebw2's GameBeaconSys_SetGameData
void func_020267bc(GameData *gameData) {
    GameBeacon *beacon = &data_02146940->mine.beacon;

    data_02146940->gameData = gameData;
    func_02026d88(beacon, gameData);
    data_02146940->playHours = func_0200864c(func_02012fcc(gameData));
}

// pokebw2's GameBeaconSys_GetSendBeacon
void func_020267e4(GameBeacon *dest) {
    MI_CpuCopy8(&data_02146940->mine.beacon, dest, sizeof(GameBeacon));
}

// pokebw2's GameBeaconSys_SendIfUpdated
void func_020267fc(void) {
    if (data_02146940->mine.updated == TRUE && func_0203cd98() == TRUE) {
        func_ov014_0217778c();
        data_02146940->mine.updated = FALSE;
    }
}

// pokebw2's GameBeaconSendSlot_UpdateTimer
void func_02026824(GameBeaconSendSlot *slot) {
    if (slot->beacon.type != 1) {
        slot->timer++;
        if (slot->timer > data_0209fc20[slot->beacon.type].lifetime) {
            slot->timer = 0;
            slot->beacon.type = 1;
        }
    }
}

static void func_0202685c(GameBeaconSystem *sys, const GameBeacon *beacon) {
    SurveySave *survey;
    SurveySave *survey2;
    SaveControl *save;
    TrainerGameInfoSave *info;
    GameBeaconLogEntry *entry;
    int i;
    int j;
    u16 question;
    u32 answer;

    sys->logNewest++;
    if (sys->logNewest >= GAME_BEACON_LOG_MAX) {
        sys->logNewest = 0;
    }
    entry = &sys->log[sys->logNewest];
    if (sys->logCount > 0 && sys->logNewest == sys->logOldest) {
        sys->logOldest++;
        if (sys->logOldest >= GAME_BEACON_LOG_MAX) {
            sys->logOldest = 0;
        }
    }

    survey = func_0200e394(GameData_GetSaveControl(sys->gameData));
    for (i = 0; i < 3; i++) {
        question = func_0200e44c(survey, i);
        if (question == 0xff) {
            continue;
        }
        if (question == SURVEY_QUESTION_PLAY_TIME) {
            answer = beacon->playHours / 10 + 1;
            if (answer > func_ov010_02167394(question)) {
                answer = func_ov010_02167394(question);
            }
        } else {
            answer = func_0200e3a4(beacon->surveyAnswers, question);
        }
        if (answer != 0 && func_0200e49c(survey, question, answer) == FALSE) {
            func_0200e460(survey, question, 1);
            func_0200e4cc(survey, question, answer, 1);
            sys->surveyUpdated = TRUE;
        }
    }

    save = GameData_GetSaveControl(sys->gameData);
    survey2 = func_0200e394(save);
    info = getTrainerGameInfoAddress(save);
    for (i = 0; i < 3; i++) {
        question = func_0200c33c(info, i);
        if (question == 0xff) {
            continue;
        }
        for (j = 0; j < 3; j++) {
            if (question == func_0200e44c(survey2, j)) {
                if (question == SURVEY_QUESTION_PLAY_TIME) {
                    answer = beacon->playHours / 10 + 1;
                    if (answer > func_ov010_02167394(question)) {
                        answer = func_ov010_02167394(question);
                    }
                } else {
                    answer = func_0200e3a4(beacon->surveyAnswers, question);
                }
                if (answer != 0) {
                    func_0200c364(info, i, 1);
                }
            }
        }
    }

    MI_CpuFill8(entry, 0, sizeof(GameBeaconLogEntry));
    entry->beacon = *beacon;
    entry->age = 0;
    sys->newMask |= 1 << sys->logNewest;
    if (sys->logCount < GAME_BEACON_LOG_MAX) {
        sys->logCount++;
    }
}


BOOL func_02026a60(const GameBeacon *beacon) {
    GameBeaconSystem *sys = data_02146940;
    BOOL found = FALSE;
    u16 type;
    int i;

    if ((beacon->targetVersions & GAME_VERSION_FLAG) == 0) {
        return FALSE;
    }
    type = beacon->type;
    if (type == 0 || type >= GAME_BEACON_TYPE_MAX) {
        return FALSE;
    }
    if (type == 0x18 && sys->mine.beacon.trainerId != beacon->payload.message.trainerId) {
        return FALSE;
    }
    if (sys->logOldest <= sys->logNewest) {
        for (i = sys->logOldest; i <= sys->logNewest; i++) {
            if (beacon->trainerId == sys->log[i].beacon.trainerId) {
                if (type == 1 || beacon->serial == sys->log[i].beacon.serial) {
                    return FALSE;
                }
                found = TRUE;
                break;
            }
        }
    } else {
        for (i = sys->logOldest; i < GAME_BEACON_LOG_MAX; i++) {
            if (beacon->trainerId == sys->log[i].beacon.trainerId) {
                if (type == 1 || beacon->serial == sys->log[i].beacon.serial) {
                    return FALSE;
                }
                found = TRUE;
                break;
            }
        }
        for (i = 0; i <= sys->logNewest; i++) {
            if (beacon->trainerId == sys->log[i].beacon.trainerId) {
                if (type == 1 || beacon->serial == sys->log[i].beacon.serial) {
                    return FALSE;
                }
                found = TRUE;
                break;
            }
        }
    }
    if (type == 0x19) {
        if (beacon->payload.gift.key != 0x2932013a) {
            return FALSE;
        }
    } else if (type == 0x1a) {
        if (beacon->payload.gift.key != 0x48841dc1) {
            return FALSE;
        }
    } else if (type == 0x1b) {
        if (beacon->payload.gift.key != 0x701ddc92) {
            return FALSE;
        }
    } else if (type == 0x32) {
        if (beacon->payload.gift.key != 0xfee48201) {
            return FALSE;
        }
    }
    if (found == TRUE) {
        sys->log[i].beacon = *beacon;
        sys->log[i].age = 0;
        sys->newMask |= 1 << i;
    } else {
        func_0202685c(sys, beacon);
        sys->recvCount++;
    }
    return TRUE;
}


// pokebw2's GameBeaconSys_GetLogEntry
static GameBeacon *func_02026bfc(GameBeaconSystem *sys, int index) {
    if (sys->logCount == 0 || sys->logCount - index <= 0) {
        return NULL;
    }
    return &sys->log[index].beacon;
}

// pokebw2's GameBeaconSys_GetLog
GameBeacon *func_02026c1c(int index) {
    return func_02026bfc(data_02146940, index);
}

// pokebw2's GameBeaconSys_GetRecentEntry
static GameBeacon *func_02026c30(GameBeaconSystem *sys, u32 n) {
    int index;

    if (sys->recvCount == 0 || sys->recvCount - n == 0) {
        return NULL;
    }
    index = sys->logNewest - (sys->recvCount - 1 - n);
    if (MATH_ABS(index) >= GAME_BEACON_LOG_MAX) {
        return NULL;
    }
    if (index < 0) {
        index += GAME_BEACON_LOG_MAX;
    }
    return &sys->log[index].beacon;
}

// pokebw2's GameBeaconSys_GetRecent
GameBeacon *func_02026c78(u32 n) {
    return func_02026c30(data_02146940, n);
}

// pokebw2's GameBeaconSys_GetReceiveCount
u32 func_02026c8c(void) {
    return data_02146940->recvCount;
}

// pokebw2's GameBeaconSys_GetNextNew
int func_02026ca0(int *index) {
    for (; *index < GAME_BEACON_LOG_MAX; (*index)++) {
        if (data_02146940->newMask & (1 << *index)) {
            (*index)++;
            return *index - 1;
        }
    }
    return GAME_BEACON_LOG_MAX;
}

// pokebw2's GameBeaconSys_ClearNew
void func_02026ce0(int index) {
    data_02146940->newMask &= 0xffffffff ^ (1 << index);
}

// pokebw2's GameBeaconSys_MarkNew
BOOL func_02026d04(const GameBeacon *beacon) {
    GameBeaconSystem *sys = data_02146940;
    int i;

    for (i = 0; i < GAME_BEACON_LOG_MAX; i++) {
        if (beacon->trainerId == sys->log[i].beacon.trainerId) {
            sys->newMask |= 1 << i;
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's GameBeaconSys_GetAge
u16 func_02026d48(u16 trainerId) {
    GameBeaconSystem *sys = data_02146940;
    int i;

    for (i = 0; i < GAME_BEACON_LOG_MAX; i++) {
        if (sys->log[i].beacon.targetVersions != 0 && trainerId == sys->log[i].beacon.trainerId) {
            return sys->log[i].age;
        }
    }
    return 0xffff;
}

void func_02026d88(GameBeacon *beacon, GameData *gameData) {
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(gameData);
    SaveControl *save = GameData_GetSaveControl(gameData);
    TrainerGameInfoSave *trainerCard = getTrainerGameInfoAddress(save);
    void *cgear = func_0200e6e8(save);
    PlayTime *time;
    PMSData sentence;
    OSOwnerInfo ownerInfo;
    u16 terminator;
    const u16 *name;
    const u16 *greeting;
    int i;

    OS_GetOwnerInfo(&ownerInfo);
    beacon->targetVersions = 0xff;
    beacon->zoneId = PlayerState_GetZoneID(GameData_GetPlayerState(gameData));
    if (GetZoneIsUnionRoom(beacon->zoneId) == TRUE || IsZone150Or151(beacon->zoneId) == TRUE) {
        beacon->zoneId = GameData_GetNextZone(gameData)->zoneId;
    }
    beacon->parentZoneId = func_ov010_02166c7c(gameData, beacon->zoneId);
    beacon->passPower = 0x30;
    beacon->trainerId = getTrainerID(playerInfo);
    beacon->favoriteColor = ownerInfo.favoriteColor;
    beacon->gender = getTrainerGender(playerInfo);
    if (beacon->gender == 0) {
        beacon->trainerView = func_02008554(playerInfo);
    } else {
        beacon->trainerView = func_02008554(playerInfo) - 8;
    }
    beacon->version = GAME_VERSION;
    beacon->language = GAME_LANGUAGE;
    beacon->country = UnityTowerVisitor_GetCountry(playerInfo);
    beacon->region = UnityTowerVisitor_GetProvince(playerInfo);
    beacon->surveyRank = func_0200c224(trainerCard);
    beacon->unk08 = func_0200c1c4(trainerCard);
    beacon->unk04 = func_0200c1dc(trainerCard);
    time = func_02012fcc(gameData);
    beacon->playHours = func_0200864c(time);
    beacon->playMinutes = func_02008650(time);
    MI_CpuCopy8(func_0200e3a0(func_0200e394(save)), beacon->surveyAnswers, 10);
    func_0200e6fc(cgear, 0, &sentence);
    func_020274b4(&sentence);
    terminator = GFL_StrBufGetTerminator();
    name = GetPlayerName(playerInfo);
    for (i = 0; i < 7; i++) {
        beacon->name[i] = name[i];
        if (terminator == name[i]) {
            break;
        }
    }
    greeting = func_0200c1f4(trainerCard);
    for (i = 0; i < 8; i++) {
        beacon->greeting[i] = greeting[i];
        if (terminator == greeting[i]) {
            break;
        }
    }
    func_0202744c(beacon);
    beacon->type = 0;
}


// pokebw2's GameBeaconSendSlot_ResetHeader
void func_02026fb8(GameBeaconSendSlot *slot) {
    SaveControl *save = GameData_GetSaveControl(data_02146940->gameData);
    PlayTime *playTime = func_02012fcc(data_02146940->gameData);
    TrainerGameInfoSave *info = getTrainerGameInfoAddress(save);

    slot->beacon.playHours = func_0200864c(playTime);
    slot->beacon.playMinutes = func_02008650(playTime);
    slot->beacon.unk08 = func_0200c1c4(info);
    slot->beacon.unk04 = func_0200c1dc(info);
    slot->beacon.serial++;
    slot->timer = 0;
    slot->updated = TRUE;
}

// pokebw2's GameBeaconSys_CanSendType
BOOL func_02027060(u16 type) {
    if (data_0209fc20[type].priority >= data_0209fc20[data_02146940->mine.beacon.type].priority) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GameBeacon_IsSpecialSpecies
BOOL func_0202708c(u16 species) {
    u32 i;

    for (i = 0; i < NELEMS(data_0209fc04); i++) {
        if (species == data_0209fc04[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

void GameBeaconSendSlot_Reset(GameBeaconSendSlot *slot);

void GameBeaconSendSlot_SetMission(GameBeaconSendSlot *slot, u16 type, u32 value);

BOOL func_02027060(u16 type);

// pokebw2's GameBeacon_StoreTextN
void func_020270ac(const StrBuf *str, u16 *dest) {
    u16 buf[GAME_BEACON_TEXT_LEN + 2];
    u16 terminator = GFL_StrBufGetTerminator();
    int i;

    GFL_StrBufStoreString(str, buf, NELEMS(buf));
    for (i = 0; i < GAME_BEACON_TEXT_LEN; i++) {
        dest[i] = buf[i];
        if (terminator == buf[i]) {
            break;
        }
    }
}

// pokebw2's GameBeaconSys_PopSurveyUpdated
u8 func_020270e0(void) {
    GameBeaconSystem *sys = data_02146940;
    u8 updated;

    if (sys == NULL) {
        return FALSE;
    }
    updated = sys->surveyUpdated;
    sys->surveyUpdated = FALSE;
    return updated;
}

// pokebw2's GameBeaconSys_SetSurveyAnswers
void func_02027100(const void *answers) {
    GameBeaconSendSlot *slot = &data_02146940->mine;

    MI_CpuCopy8(answers, slot->beacon.surveyAnswers, sizeof(slot->beacon.surveyAnswers));
    slot->updated = TRUE;
}

// pokebw2's GameBeaconSys_SetCountryRegion
void func_02027120(u8 country, u8 region) {
    GameBeaconSendSlot *slot = &data_02146940->mine;

    slot->beacon.country = country;
    slot->beacon.region = region;
    slot->updated = TRUE;
}

// pokebw2's GameBeaconSys_SetSurveyRank
void func_02027138(u8 rank) {
    GameBeaconSendSlot *slot = &data_02146940->mine;

    slot->beacon.surveyRank = rank;
    slot->updated = TRUE;
}

// pokebw2's GameBeaconSys_SetCGearRecord
void func_0202715c(const PMSData *record) {
    GameBeaconSendSlot *slot = &data_02146940->mine;

    func_020274b4(record);
    slot->updated = TRUE;
}

// pokebw2's GameBeaconSys_SetTrainerView
void func_02027174(u32 trainerView) {
    GameBeaconSendSlot *slot = &data_02146940->mine;

    slot->beacon.trainerView = trainerView;
    slot->updated = TRUE;
}

// pokebw2's GameBeaconSys_UpdateGreeting
void func_02027198(void) {
    GameBeaconSendSlot *slot = &data_02146940->mine;
    TrainerGameInfoSave *info = getTrainerGameInfoAddress(GameData_GetSaveControl(data_02146940->gameData));
    u16 terminator = GFL_StrBufGetTerminator();
    const u16 *greeting = func_0200c1f4(info);
    int i;

    for (i = 0; i < 8; i++) {
        slot->beacon.greeting[i] = greeting[i];
        if (terminator == greeting[i]) {
            break;
        }
    }
    slot->updated = TRUE;
}

// pokebw2's GameBeaconSys_SendCapture
void func_020271dc(u16 species) {
    if (func_0202708c(species)) {
        func_02027230(species);
    } else {
        func_020271f8(species);
    }
}

// pokebw2's GameBeaconSys_SendCaptureWild
static void func_020271f8(u16 species) {
    if (func_02027060(0xe)) {
        func_02027220(&data_02146940->mine.beacon, species);
        func_02026fb8(&data_02146940->mine);
    }
}


// pokebw2's GameBeacon_SetCaptureWild
void func_02027220(GameBeacon *beacon, u16 species) {
    beacon->type = 0xe;
    beacon->arg.value = species;
    func_0202744c(beacon);
}

// pokebw2's GameBeaconSys_SendCaptureSpecial
void func_02027230(u16 species) {
    if (func_02027060(0xf)) {
        func_02027258(&data_02146940->mine.beacon, species);
        func_02026fb8(&data_02146940->mine);
    }
}

// pokebw2's GameBeacon_SetCaptureSpecial
void func_02027258(GameBeacon *beacon, u16 species) {
    beacon->type = 0xf;
    beacon->arg.value = species;
    func_0202744c(beacon);
}

// pokebw2's GameBeaconSys_SendCaptureSpecial (type 0x10 variant)
void func_02027268(const StrBuf *nickname) {
    if (func_02027060(0x10)) {
        func_02027290(&data_02146940->mine.beacon, nickname);
        func_02026fb8(&data_02146940->mine);
    }
}


// pokebw2's GameBeacon_SetType10
void func_02027290(GameBeacon *beacon, const StrBuf *nickname) {
    beacon->type = 0x10;
    func_020270ac(nickname, beacon->payload.text);
}

// pokebw2's GameBeaconSys_SendEvolution
void func_020272a4(u16 species, const StrBuf *nickname) {
    if (func_02027060(0x11)) {
        func_020272d0(&data_02146940->mine.beacon, species, nickname);
        func_02026fb8(&data_02146940->mine);
    }
}

// pokebw2's GameBeacon_SetEvolution
static void func_020272d0(GameBeacon *beacon, u16 species, const StrBuf *nickname) {
    beacon->type = 0x11;
    beacon->arg.value = species;
    func_020270ac(nickname, beacon->payload.text);
    func_0202744c(beacon);
}

// pokebw2's GameBeaconSys_SendPlayTime
void func_020272ec(u16 hours) {
    if (func_02027060(0x14)) {
        func_02027314(&data_02146940->mine.beacon, hours);
        func_02026fb8(&data_02146940->mine);
    }
}

// pokebw2's GameBeacon_SetPlayTime
void func_02027314(GameBeacon *beacon, u16 hours) {
    beacon->type = 0x14;
    beacon->payload.value = hours;
    func_0202744c(beacon);
}

// pokebw2's GameBeaconSys_SendItem
void func_02027324(u16 item) {
    if (func_02027060(0x23)) {
        func_0202734c(&data_02146940->mine.beacon, item);
        func_02026fb8(&data_02146940->mine);
    }
}


// pokebw2's GameBeacon_SetItem
void func_0202734c(GameBeacon *beacon, u16 item) {
    beacon->type = 0x23;
    beacon->payload.value = item;
    func_0202744c(beacon);
}

// pokebw2's GameBeaconSys_SendFlashbackMemoryPostFX
void func_0202735c(u16 zoneId, GameData *gameData) {
    func_0202737c(&data_02146940->mine.beacon, zoneId, gameData);
    data_02146940->mine.updated = TRUE;
}

static void func_0202737c(GameBeacon *beacon, u16 zoneId, GameData *gameData) {
    beacon->zoneId = zoneId;
    beacon->parentZoneId = func_ov010_02166c7c(gameData, zoneId);
    beacon->recentKind = GAME_BEACON_RECENT_NONE;
    func_0202744c(beacon);
}



void func_020273c4(GameBeacon *beacon, u16 species) {
    beacon->recentKind = 0;
    beacon->recentValue = species;
}

void func_020273dc(GameBeacon *beacon, u16 species) {
    beacon->recentKind = 1;
    beacon->recentValue = species;
}

void func_020273f8(GameBeacon *beacon, u16 item) {
    beacon->recentKind = 2;
    beacon->recentValue = item;
}

void func_02027414(GameBeacon *beacon, u16 item) {
    beacon->recentKind = 3;
    beacon->recentValue = item;
}

void func_02027430(GameBeacon *beacon, u16 item) {
    beacon->recentKind = 4;
    beacon->recentValue = item;
}

// pokebw2's GameBeacon_ClearRecent
void func_0202744c(GameBeacon *beacon) {
    beacon->recentKind = GAME_BEACON_RECENT_NONE;
}

// Sets the game's own beacon, whichever beacon it is given; its only caller gives that one
// pokebw2's GameBeacon_SetCGearRecordFields
void func_02027460(GameBeacon *beacon, const PMSData *record) {
    GameBeacon *mine = &data_02146940->mine.beacon;

    mine->cgear0 = record->type;
    mine->cgear1 = record->id;
    mine->cgear2 = record->words[0];
    mine->cgear3 = record->words[1];
}

// pokebw2's GameBeacon_SetMyCGearRecord
void func_020274b4(const PMSData *record) {
    func_02027460(&data_02146940->mine.beacon, record);
}

void func_020274c8(void) {
    BOOL ok;

    GFL_OvlLoad(OVERLAY_ID(231));
    ok = func_ov231_021809d8();
    GFL_OvlUnload(OVERLAY_ID(231));
    if (!ok) {
        GFL_HBlankTCBAdd(func_020274f8, NULL, 3);
    }
}

// pokebw2's GameBeaconSys_SetEnabled
static void func_020274f8(TCB *tcb, void *data) {
    if (data_02146940 != NULL) {
        data_02146940->newMask = 0;
        data_02146940->mine.updated = FALSE;
        data_02146940->newMask = 0;
    }
}

