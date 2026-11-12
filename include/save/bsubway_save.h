// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/bsubway_save.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_BSUBWAY_SAVE_H
#define BWPORT_SAVE_BSUBWAY_SAVE_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/rtc.h"
#include "struct_decls.h"

#define BSUBWAY_WIFI_RANK_MAX 10
#define BSUBWAY_WIFI_ROOM_MAX 200

// Black 2's, not checked against Black
#define BSUBWAY_WIFI_PLAYER_MAX 7

#define BSUBWAY_WIFI_LEADER_MAX 30
#define BSUBWAY_BATTLE_POINTS_MAX 9999

// Black 2's, not checked against Black
// The play modes the scores are kept for
#define BSUBWAY_SCORE_MODE_MAX 9

// The values of the current play, which BSubwayPlayData_GetData reads and BSubwayPlayData_SetData sets
enum {
    BSUBWAY_PLAY_MODE,
    BSUBWAY_PLAY_ROUND,
    BSUBWAY_PLAY_WIFI_DOWN,
    BSUBWAY_PLAY_WIFI_TURN,
    BSUBWAY_PLAY_WIFI_DAMAGE,
    BSUBWAY_PLAY_UNK8,
    BSUBWAY_PLAY_UNK28,
    BSUBWAY_PLAY_UNK0_0,
    BSUBWAY_PLAY_UNKC,
    BSUBWAY_PLAY_UNK0_6,
    BSUBWAY_PLAY_UNK0_9,
    BSUBWAY_PLAY_UNK0_10,
};

// What BSubwayScoreData_SetFlag does with a flag of the scores
enum {
    BSUBWAY_FLAG_GET,
    BSUBWAY_FLAG_SET,
    BSUBWAY_FLAG_RESET,
    // Adds 1 to the count of BSubwayScoreData_SetWifiLoseCount
    BSUBWAY_FLAG_INC,
};

// Block 0x38: the play the player saved in the middle of
struct BSubwayPlayData {
    u16 unk0_0 : 1;
    // Whether the play was saved
    u16 saved : 1;
    u16 playMode : 4;
    u16 unk0_6 : 3;
    u16 unk0_9 : 1;
    u16 unk0_10 : 1;
    u16 : 5;
    u8 round;
    // The Wi-Fi train's record: the Pokémon lost, the turns taken and the damage taken
    u8 wifiDown;
    u16 wifiTurn;
    u16 wifiDamage;
    u8 unk8[4];
    u8 unkC[0x1c];
    u8 unk28[0x14];
};

// Black 2's; twinned code confirms the offsets of battlePoints, wifiLoseCount, wifiRank, flags, streaks, maxStreaks,
// stages, wifiScore, team1 and team0, and its size in Black
// Block 0x39: the scores
struct BSubwayScoreData {
    u16 battlePoints;
    // How many Wi-Fi plays in a row were lost
    u8 wifiLoseCount;
    u8 wifiRank;
    u32 flags;
    // By play mode: the current streak, the longest one, and a count that starts at 1 once the mode is played
    u16 streaks[BSUBWAY_SCORE_MODE_MAX];
    u16 maxStreaks[BSUBWAY_SCORE_MODE_MAX];
    u16 stages[BSUBWAY_SCORE_MODE_MAX];
    u16 wifiScore;
    u8 unk40[4];
    // Two teams of three BSubwayPokemon
    u8 team1[0xb4];
    u8 team0[0xb4];
};

// Black 2's; twinned code confirms the offsets of pokemon, name, trainerClass, message, winWords and loseWords in Black
// A player of the Wi-Fi train, of whom BSubwayWifiData_GetTrainer makes a trainer
typedef struct {
    u8 pokemon[0xb4];
    u16 name[8];
    u8 unkC4[0x11];
    u8 trainerClass;
    u16 message[4];
    u16 winWords[4];
    u16 loseWords[4];
    u8 paddingEE[2];
} BSubwayWifiPlayer;

// Black 2's; twinned code confirms the offsets of name and id in Black
typedef struct {
    u16 name[8];
    u8 unk10[4];
    // A 32-bit ID, lowest byte first
    u8 id[4];
    u8 unk18[10];
} BSubwayWifiLeader;

// Block 0x3a: the Wi-Fi train's downloaded data
typedef struct {
    // The day the rooms were cleared on, packed as year, month, day and week bytes
    u32 date;
    // A bit for each room of each rank that was cleared on that day
    u8 roomFlags[BSUBWAY_WIFI_RANK_MAX * BSUBWAY_WIFI_ROOM_MAX / 8];
    u8 hasPlayers;
    u8 hasLeaders;
    u8 playerRoom;
    u8 playerRank;
    u8 leaderRoom;
    u8 leaderRank;
    BSubwayWifiPlayer players[BSUBWAY_WIFI_PLAYER_MAX];
    BSubwayWifiLeader leaders[BSUBWAY_WIFI_LEADER_MAX];
} BSubwayWifiData;

// pokebw2's BSubwayPlayData_GetSize
// Block 0x38
u32 func_0200d858(void);
// pokebw2's BSubwayPlayData_SetSaveFlag
void func_0200d868(BSubwayPlayData *data, u32 value);
// pokebw2's BSubwayPlayData_GetSaveFlag
u32 func_0200d87c(BSubwayPlayData *data);
// pokebw2's BSubwayPlayData_GetData
// Reads a value, into the buffer when it is an array
u32 func_0200d884(BSubwayPlayData *data, u32 id, void *buffer);
// pokebw2's BSubwayPlayData_SetData
void func_0200d914(BSubwayPlayData *data, u32 id, const void *value);
// pokebw2's BSubwayPlayData_AddWifiRecord
// Adds to the Wi-Fi train's record
void func_0200d9e8(BSubwayPlayData *data, u8 down, u8 turn, u16 damage);
// pokebw2's BSubwayPlayData_ResetRound
void func_0200da14(BSubwayPlayData *data);
// pokebw2's BSubwayPlayData_IncRound
void func_0200da28(BSubwayPlayData *data);
// pokebw2's BSubwayPlayData_GetRound
u16 func_0200da54(BSubwayPlayData *data);
// pokebw2's BSubwayScoreData_GetSize
u32 func_0200da64(void);
// pokebw2's BSubwayScoreData_Init
void func_0200da6c(BSubwayScoreData *score);
// pokebw2's BSubwayScoreData_AddBattlePoints
void func_0200da80(BSubwayScoreData *score, u16 amount);
// pokebw2's BSubwayScoreData_SubBattlePoints
// Takes Battle Points, down to 0
void func_0200da98(BSubwayScoreData *score, u16 amount);
// pokebw2's BSubwayScoreData_GetStreak
u16 func_0200dac4(BSubwayScoreData *score, u32 mode);
// pokebw2's BSubwayScoreData_GetMaxStreak
u16 func_0200dad8(BSubwayScoreData *score, u32 mode);
// pokebw2's BSubwayScoreData_UpdateMaxStreak
// Raises the longest streak
void func_0200daec(BSubwayScoreData *score, u32 mode, u32 value);
// pokebw2's BSubwayScoreData_SetStreak
void func_0200db08(BSubwayScoreData *score, u32 mode, u32 value);
// pokebw2's BSubwayScoreData_ResetStage
void func_0200db1c(BSubwayScoreData *score, u32 mode);
// pokebw2's BSubwayScoreData_StartStage
void func_0200db30(BSubwayScoreData *score, u32 mode);
// pokebw2's BSubwayScoreData_HasStage
BOOL func_0200db44(BSubwayScoreData *score, u32 mode);
// pokebw2's BSubwayScoreData_IncStage
void func_0200db60(BSubwayScoreData *score, u32 mode);
// pokebw2's BSubwayScoreData_GetStage
u16 func_0200db80(BSubwayScoreData *score, u32 mode);
// pokebw2's BSubwayScoreData_SetFlag
// Reads, sets or clears one of the 11 flags (a BSUBWAY_FLAG_* operation)
BOOL func_0200dba0(BSubwayScoreData *score, int id, u32 op);
// pokebw2's BSubwayScoreData_IncWifiRank
void func_0200dbf0(BSubwayScoreData *score);
// pokebw2's BSubwayScoreData_DecWifiRank
void func_0200dbfc(BSubwayScoreData *score);
// pokebw2's BSubwayScoreData_SetWifiLoseCount
// Resets or counts the Wi-Fi plays lost in a row (BSUBWAY_FLAG_RESET or BSUBWAY_FLAG_INC), and returns the count
u32 func_0200dc0c(BSubwayScoreData *score, u32 op);
// pokebw2's BSubwayScoreData_SetTeam
void func_0200dc50(BSubwayScoreData *score, u32 flag, const void *team);
// pokebw2's BSubwayScoreData_GetTeam
void func_0200dc74(BSubwayScoreData *score, u32 flag, void *team);
// pokebw2's BSubwayScoreData_CalcWifiScore
// Scores a Wi-Fi play from its rounds and its record
void func_0200dc94(BSubwayScoreData *score, BSubwayPlayData *play);
// pokebw2's BSubwayScoreData_GetWifiScore
u16 func_0200dcf8(BSubwayScoreData *score);
// pokebw2's BSubwayScoreData_GetWifiScoreRounds
u8 func_0200dcfc(BSubwayScoreData *score);
// pokebw2's BSubwayWifiData_GetSize
// Block 0x3a
u32 func_0200dd10(void);
// pokebw2's BSubwayWifiData_Init
void func_0200dd18(BSubwayWifiData *data);
// pokebw2's BSubwayWifiData_SetRoomFlag
void func_0200dd28(BSubwayWifiData *data, u8 rank, u8 room, const RTCDate *date);
// pokebw2's BSubwayWifiData_GetRoomFlag
BOOL func_0200ddd0(BSubwayWifiData *data, u8 rank, u8 room, const RTCDate *today);
// pokebw2's BSubwayWifiData_HasPlayers
u16 func_0200de5c(BSubwayWifiData *data);
// pokebw2's BSubwayWifiData_HasLeaders
u16 func_0200de64(BSubwayWifiData *data);
// pokebw2's BSubwayWifiData_SetPlayers
void func_0200de6c(BSubwayWifiData *data, const void *players, u8 rank, u8 room);
// pokebw2's BSubwayWifiData_GetPlayerRank
u16 func_0200de94(BSubwayWifiData *data);
// pokebw2's BSubwayWifiData_GetPlayerRoom
u16 func_0200dea0(BSubwayWifiData *data);
// pokebw2's BSubwayWifiData_GetTrainer
void func_0200dea8(BSubwayWifiData *data, BSubwayTrainer *trainer, u8 round, u16 id);
// pokebw2's BSubwayWifiData_SetLeaders
void func_0200df10(BSubwayWifiData *data, const void *leaders, u8 rank, u8 room);
// pokebw2's BSubwayWifiData_GetLeaderRank
u16 func_0200df40(BSubwayWifiData *data);
// pokebw2's BSubwayWifiData_GetLeaderRoom
u16 func_0200df4c(BSubwayWifiData *data);
// pokebw2's BSubwayWifiData_AllocLeaders
// Allocates a copy of the block's list of 0x22-byte leaders
void *func_0200df58(BSubwayWifiData *data, HeapID heapId);
// pokebw2's BSubwayWifiLeader_CountList
int func_0200df94(void *list);
// pokebw2's BSubwayWifiLeader_GetID
u32 func_0200dfb4(void *entry);

// Black's (near twins)
#define SAVE_BLOCK_BSUBWAY_SCORE 0x3a
// Black's (near twins)
#define SAVE_BLOCK_BSUBWAY_3A 0x3a
// Black's: the block func_ov021_021c9934 keeps in unk78
#define SAVE_BLOCK_BSUBWAY_3B 0x3b

// Black 2's, not checked against Black
// The Battle Subway's save blocks: 0x38 the current play, 0x39 the scores, which include the Battle Points, and 0x3a
#define SAVE_BLOCK_BSUBWAY_PLAY 0x39 // Black's

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BSubwayPlayData_Init
void func_0200d85c(BSubwayPlayData *data);
// pokebw2's BSubwayScoreData_GetBattlePoints
u16 func_0200dabc(BSubwayScoreData *score);
// pokebw2's BSubwayScoreData_GetWifiRank
u32 func_0200dc08(BSubwayScoreData *score);

#endif // BWPORT_SAVE_BSUBWAY_SAVE_H
