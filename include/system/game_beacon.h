// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/game_beacon.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_GAME_BEACON_H
#define BWPORT_SYSTEM_GAME_BEACON_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Black 2's, not checked against Black
// How many received beacons the log keeps
#define GAME_BEACON_LOG_MAX 30

// Black 2's, not checked against Black
// The characters of a beacon's free text
#define GAME_BEACON_TEXT_LEN 10

// The beacon types are below this (Black's, the compared literal of the game's code and its table of 51)
#define GAME_BEACON_TYPE_MAX 0x33

// Black 2's, not checked against Black
// A beacon's passPower when no Pass Power is active
#define GAME_BEACON_PASS_POWER_NONE 0x30

// A beacon's recentKind when it has no recent event
#define GAME_BEACON_RECENT_NONE 5

// Black 2's; matching code confirms the offsets of value, text, message, gift, value32, value8 and named in Black
// What a beacon's type sends
typedef union {
    // A value (an item, a move, a Pass Power, a number of hours)
    u16 value;
    // Free text, such as a nickname
    u16 text[GAME_BEACON_TEXT_LEN];
    // Type 0x18: a message to one trainer
    struct {
        u16 text[9];
        u16 trainerId;
    } message;
    // Types 0x19, 0x1a, 0x1b and 0x32: a distribution, which key identifies
    struct {
        u16 species;
        u16 item;
        u16 passPower;
        u16 unk06;
        u32 key;
    } gift;
    // The other views overlay 12's game_beacon_set.c fills its messages through
    u32 value32;
    u8 value8;
    struct {
        u16 name[9];
        u16 value;
    } named;
} GameBeaconPayload;

// Black 2's; matching code confirms the offsets of country, region, passPower, gender, unk04, zoneId, language, unk08,
// parentZoneId, version, trainerView, surveyRank, favoriteColor, playHours, playMinutes, trainerId, name, greeting,
// type, arg, payload, recentKind, cgear0, cgear1, cgear2, cgear3, recentValue and surveyAnswers in Black
struct GameBeacon {
    // A bit for each game version that may receive it, (version - VERSION_WHITE)
    u8 targetVersions;
    u8 country;
    u8 region;
    u8 passPower : 7;
    u8 gender : 1;
    u32 unk04 : 17;
    u32 zoneId : 10;
    u32 language : 5;
    u32 unk08 : 17;
    // Incremented each time the beacon changes, so that receivers tell a new beacon from one already seen
    u32 serial : 5;
    u32 parentZoneId : 10;
    u32 version : 6;
    u32 trainerView : 3;
    u32 surveyRank : 3;
    u32 favoriteColor : 4;
    u32 playHours : 10;
    u32 playMinutes : 6;
    u16 trainerId;
    u16 name[7];
    u16 greeting[8];
    u16 type;
    union {
        u16 value;
        struct {
            u16 missionId : 14;
            u16 unk : 2;
        } mission;
    } arg;
    GameBeaconPayload payload;
    // The last notable event: a species (kinds 0 and 1), an item (kinds 2 to 4), or GAME_BEACON_RECENT_NONE
    u16 recentKind : 8;
    u16 cgear0 : 3;
    u16 cgear1 : 5;
    u16 cgear2;
    u16 cgear3;
    u16 recentValue;
    u8 surveyAnswers[10];
    u8 medalCount;
    u8 unk5B;
};

// Black 2's, not checked against Black
// When a received beacon arrived; copied as one halfword
typedef union {
    u16 raw;
    struct {
        u8 minute;
        u8 hour : 7;
        // Whether the sender's survey answers were counted
        u8 surveyCounted : 1;
    };
} GameBeaconTime;

// Black 2's, not checked against Black
typedef struct {
    GameBeacon beacon;
    // Frames since it was received, up to 0xffff
    u16 age;
    GameBeaconTime time;
} GameBeaconLogEntry;

// Black 2's; matching code confirms the offsets of beacon and updated in Black
// The beacon this game sends
typedef struct {
    GameBeacon beacon;
    // Frames in the current type, which reverts to 0 after its lifetime
    s16 timer;
    // Set by every change; GameBeaconSys_SendIfUpdated sends the beacon and clears it
    u8 updated;
} GameBeaconSendSlot;

// Black 2's without its Join Avenue, B2W2-only and name fields (0xbb4 bytes in Black, from func_020266e0);
// matching code confirms the offsets of mine in Black
struct GameBeaconSystem {
    // Black has one pointer here where Black 2 has its GameSystem and GameData
    GameData *gameData;
    GameBeaconSendSlot mine;
    GameBeaconLogEntry log[GAME_BEACON_LOG_MAX];
    // A bit for each log entry not yet shown
    u32 newMask;
    u32 recvCount;
    s8 logOldest;
    s8 logNewest;
    s8 logCount;
    u8 surveyUpdated;
    u16 playHours;
    u8 enabled;
    u8 unkBB3;
};

// Set the beacon's recent event, of kinds 0 to 4
void func_020273c4(GameBeacon *beacon, u16 species);
void func_020273dc(GameBeacon *beacon, u16 species);
void func_020273f8(GameBeacon *beacon, u16 item);
void func_02027414(GameBeacon *beacon, u16 item);
void func_02027430(GameBeacon *beacon, u16 item);
// pokebw2's GameBeacon_ClearRecent
void func_0202744c(GameBeacon *beacon);

// Black 2's, not checked against Black
// A table of beacons with the times they were received at. It is used either as a list, filled from the front
// (GameBeaconInfoTable_TakeNew to GameBeaconInfoTable_MarkAllNew), or as a ring that head is the newest entry of (GameBeaconInfoTable_RingIndex to GameBeaconInfoTable_GetCount).
// BeaconStatus starts with one, the ring of the beacons that the game keeps
typedef struct {
    GameBeacon beacons[GAME_BEACON_LOG_MAX];
    // GameBeaconTime's minute and hour
    u16 times[GAME_BEACON_LOG_MAX];
    u8 head;
    u8 count;
} GameBeaconInfoTable;

// Black 2's, not checked against Black
typedef struct {
    // What the type's message shows and its payload holds, the index of the checks and of GameBeacon_SetMessageWords's cases
    u8 format;
    u8 unk1;
} GameBeaconTypeFormat;

// pokebw2's GameBeacon_IsGiftType
// Whether the beacon's type is one of a distribution, 0x19, 0x1a, 0x1b or 0x32
BOOL func_ov010_02165c10(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetName
// Loads the beacon's trainer name into a StrBuf
void func_ov010_02165c34(const GameBeacon *beacon, StrBuf *dest);
// pokebw2's GameBeacon_GetVersion
// The game version of the beacon's sender
u32 func_ov010_02165c48(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetLanguage
u32 func_ov010_02165c50(const GameBeacon *beacon);
// The sender's answers to the survey's questions 0x1a and 0x19, or 0xff for none
u8 func_ov010_02165c60(GameBeacon *beacon);
u8 func_ov010_02165c78(GameBeacon *beacon);
// pokebw2's GameBeacon_GetCGearRecord
// The sender's C-Gear record
void func_ov010_02165c90(const GameBeacon *beacon, PMSData *dest);
// pokebw2's GameBeacon_GetSurveyRank
u8 func_ov010_02165cb4(const GameBeacon *beacon);
void func_ov010_02165cc0(u16 *dest, const GameBeacon *beacon);
// pokebw2's GameBeacon_GetFavoriteColor
u8 func_ov010_02165cd8(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetTrainerViewWithGender
// The sender's trainer view, plus 8 for a female trainer
u8 func_ov010_02165ce8(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetGender
u32 func_ov010_02165d08(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetPlayTime
void func_ov010_02165d10(const GameBeacon *beacon, u16 *hours, u16 *minutes);
// pokebw2's GameBeacon_GetPassPower
u32 func_ov010_02165d20(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetZone
u16 func_ov010_02165d28(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetParentZone
u16 func_ov010_02165d34(const GameBeacon *beacon);
u32 func_ov010_02165d40(const GameBeacon *beacon);
u32 func_ov010_02165d48(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetGreeting
void func_ov010_02165d50(const GameBeacon *beacon, StrBuf *dest);
// The message of a beacon of type 0x18 and the text of one of type 0x31; empty for other types
void func_ov010_02165d6c(const GameBeacon *beacon, StrBuf *dest);
void func_ov010_02165d8c(const GameBeacon *beacon, StrBuf *dest);
// pokebw2's GameBeacon_GetRecentKind
// The beacon's recent event: its kind, GAME_BEACON_RECENT_NONE when invalid, its trainer (kinds 2 to 4) and its
// species (kinds 0 and 1)
u32 func_ov010_02165dac(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetRecentSpecies
u16 func_ov010_02165df0(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetGiftPassPower
// The Pass Power of a distribution of type 0x32, else 0
u16 func_ov010_02165ebc(const GameBeacon *beacon);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GameBeaconSys_SendEvolution
void func_020272a4(u16 species, const StrBuf *nickname);
// Sends a beacon of type 0x23 with an item, or the Funfest mission's
void func_02027324(u16 item);
// pokebw2's GameBeacon_SetZone
void func_0202735c(u16 zoneId, GameData *gameData);
// Sends the Funfest mission beacon of type 0x3c
void func_020274c8(void);
// pokebw2's BeaconStatus_Create (beacon_status.c)
BeaconStatus *func_0202751c(HeapID heapId, HeapID tempHeapId);
// pokebw2's BeaconStatus_GetLogTop
u8 func_020275ac(BeaconStatus *status);
// pokebw2's BeaconStatus_GetInfoTable: the beacon status's ring of the beacons the game keeps, which it starts with
GameBeaconInfoTable *func_020275a8(BeaconStatus *status);
// How many beacons the ring has
u8 func_ov010_02165b04(GameBeaconInfoTable *table);
// pokebw2's GameBeacon_GetCountry
u32 func_ov010_02165c58(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetRegion
u8 func_ov010_02165c5c(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetTrainerId
u16 func_ov010_02165ce4(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetRecentTrainer
u16 func_ov010_02165dbc(const GameBeacon *beacon);
// pokebw2's GameBeacon_GetType
// The beacon's type, or 1 for a type of 0 or of GAME_BEACON_TYPE_MAX or more
u16 func_ov010_02165e24(const GameBeacon *beacon);
// The icon a type of beacon shows
u8 func_ov010_02165fc0(u32 type);
// Sets the beacon's words in a word set
void func_ov010_02165fcc(const GameBeacon *beacon, WordSet *wordSet, HeapID heapId);
void func_ov010_02165374(u32 count);
// pokebw2's GameBeaconInfoTable_RingGet
// Copies the ring's beacon at offset from the newest into dest and its time into time, and returns whether there is
// one
BOOL func_ov010_02165ac4(GameBeaconInfoTable *table, GameBeacon *dest, u16 *time, int offset);
// The index in the game's log of the beacon at offset from the newest
u32 func_ov010_021659f0(GameBeaconInfoTable *table, s32 offset);
// pokebw2's GameBeaconSys_GetNextNew
// The first log entry at or after *index not yet shown, which it advances past that entry, or 30 when there is none
int func_02026ca0(int *index);
// pokebw2's GameBeaconSys_Create
void func_020266e0(HeapID heapId);
// pokebw2's GameBeaconSys_Update
// Called once a frame
void func_02026728(void);
// pokebw2's GameBeaconSys_SetGameData
// Builds the game's beacon from the save
void func_020267bc(GameData *gameData);
// pokebw2's GameBeaconSys_GetSendBeacon
// Copies the game's beacon as it is sent
void func_020267e4(GameBeacon *dest);
// pokebw2's GameBeaconSys_ToggleB2W2Only
void func_020267fc(void);
// pokebw2's GameBeaconSys_Receive
// Logs a received beacon; FALSE if it was rejected or already seen
BOOL func_02026a60(const GameBeacon *beacon);
// pokebw2's GameBeaconSys_GetLog
// The log's entry at index, and when it was received
GameBeacon *func_02026c1c(int index);
// pokebw2's GameBeaconSys_GetRecent
// The beacon received n-th, counting from the first
GameBeacon *func_02026c78(u32 n);
// pokebw2's GameBeaconSys_GetReceiveCount
u32 func_02026c8c(void);
// pokebw2's GameBeaconSys_ClearNew
void func_02026ce0(int index);
// pokebw2's GameBeaconSys_MarkNew
// Marks the log's entry from beacon's sender as not yet shown
BOOL func_02026d04(const GameBeacon *beacon);
// pokebw2's GameBeaconSys_GetAge
// The age of the logged beacon of a trainer, or 0xffff
u16 func_02026d48(u16 trainerId);
// pokebw2's GameBeaconSys_CanSendType
// Whether a beacon of type may replace the current one, by their priorities
BOOL func_02027060(u16 type);
// pokebw2's GameBeacon_IsSpecialSpecies
// Whether species is one of the special Pokémon (legendaries and such)
BOOL func_0202708c(u16 species);
// pokebw2's GameBeaconSys_PopSurveyUpdated
// Whether a received beacon changed the survey's results since the last call
u8 func_020270e0(void);
// pokebw2's GameBeaconSys_SetSurveyAnswers
void func_02027100(const void *answers);
// pokebw2's GameBeaconSys_SetCountryRegion
void func_02027120(u8 country, u8 region);
// pokebw2's GameBeaconSys_SetSurveyRank
void func_02027138(u8 rank);
// pokebw2's GameBeaconSys_SetCGearRecord
// Sets the C-Gear record, a sentence (CGearPhrases_Get)
void func_0202715c(const PMSData *record);
// pokebw2's GameBeaconSys_SetTrainerView
void func_02027174(u32 trainerView);
// pokebw2's GameBeaconSys_UpdateGreeting
// Sets the greeting from the save, after the player changed it
void func_02027198(void);
// pokebw2's GameBeaconSys_SendCapture
// Sends that the player caught species, of a wild battle of the kinds a1 and a2
void func_020271dc(u16 species);
// Sends a beacon of type 0x10 with a battle Pokémon's nickname
void func_02027268(const StrBuf *nickname);
// pokebw2's GameBeacon_StoreText
void func_020270ac(const StrBuf *str, u16 *dest);

#endif // BWPORT_SYSTEM_GAME_BEACON_H
