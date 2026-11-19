#ifndef POKEHEARTGOLD_SAVE_FRONTIER_H
#define POKEHEARTGOLD_SAVE_FRONTIER_H

#include "global.h"

#include "battle_castle_save.h"
#include "battle_frontier_stats.h"
#include "mail_message_types.h"
#include "frontier_pokemon.h"
#include "wifi_battle_tower_match_candidate.h"

// The Battle Frontier save block (SAVE_TABLE_ENTRY_FRONTIER), one header for the whole block: the per-facility run saves (a
// union: only the facility being played is live), the Wi-Fi Battle Tower data and the persistent streak flags.

typedef struct BattleHallSave {
    u8 challengeType : 3;
    u8 didSave : 1;
    u8 currentBattleNum;
    u8 partySlots[2];
    u16 trainerIDs[10 * 2];
    u16 monIndices[10 * 2];
} BattleHallSave;

typedef struct BattleHallStreakFlags {
    u8 streakActiveFlags;
    u8 unused[3];
} BattleHallStreakFlags;

typedef struct WifiTrainerRecord {
    FrontierPokemon mons[3];
    u16 trainerName[7 + 1];
    u8 unk_B8;
    u8 unk_B9;
    u8 unk_BA;
    u8 unk_BB;
    u32 unk_BC;
    u16 unk_C0[4];
    union {
        struct {
            u8 isAnonymous : 1;
            u8 anonymousNameIdx : 1;
            u8 : 6;
        };
        u8 unk_C8_val2;
    };
    u8 trainerType;
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
    u16 unk_E2;
} WifiTrainerRecord;

typedef struct WifiBattleTowerDownloadData {
    u32 lastDownloadDate;
    u8 downloadedOpponents[250];
    u8 hasOpponentData;
    u8 hasMatchListData;
    u8 storedOpponentIdx;
    u8 storedRank;
    u8 storedMatchOpponentIdx;
    u8 storedMatchRank;
    WifiTrainerRecord trainerRecords[7];
    WifiBattleTowerMatchCandidate matchCandidates[30];
} WifiBattleTowerDownloadData;

typedef struct WifiBattleTowerRecord {
    u16 battlePoints;
    u8 lossStreak;
    u8 rank;
    u32 rngState;
    union {
        struct {
            u16 reward2Given : 1;
            u16 reward3Given : 1;
            u16 reward2FullPC : 1;
            u16 reward3FullPC : 1;
            u16 lossStreakInitialized : 1;
            u16 unused : 2;
            u16 hasRegisteredTeam : 1;
            u16 challengeActiveFlag_1 : 1;
            u16 challengeActiveFlag_2 : 1;
            u16 challengeActiveFlag_3 : 1;
            u16 challengeActiveFlag_4 : 1;
            u16 challengeActiveFlag_5 : 1;
            u16 reward1Given : 1;
            u16 reward1FullPC : 1;
        };
        u16 flags;
    };
    u16 roomNums[6];
    u16 ratingScore;
    FrontierPokemon team1[3];
    FrontierPokemon team0[3];
} WifiBattleTowerRecord;

typedef struct BattleTowerPartnerData {
    u32 otID;
    u16 monSetIDs[2];
    u32 personalities[2];
} BattleTowerPartnerData;

typedef struct BattleTowerSave {
    u8 unk_00_0 : 1;
    u8 didSave : 1;
    u8 challengeMode : 3;
    u8 partnerID : 3;
    u8 unused_01;
    u8 nextOpponentNum;
    u8 unk_03;
    u16 unk_04;
    u16 unk_06;
    u8 partySlots[4];
    u16 trainerIDs[7 * 2];
    u32 rngSeed;
    BattleTowerPartnerData unk_2C;
} BattleTowerSave;

typedef struct BattleArcadeSave {
    u8 challengeType : 3;
    u8 didSave : 1;
    u8 cursorRandomized : 1;
    u8 currentBattle;
    u8 rouletteSpeed;
    u8 partySlots[3];
    u16 heldItems[4];
    u16 trainerIDs[14];
    u16 monSetIDs[14];
    u16 unused;
} BattleArcadeSave;

typedef struct BattleArcadeStreakFlags {
    u8 streakActiveFlags;
    u8 unused[3];
} BattleArcadeStreakFlags;

typedef struct BattleFactoryStreakFlags {
    u8 streakActiveFlags;
    u8 unused[3];
} BattleFactoryStreakFlags;

typedef struct BattleFactorySave {
    u8 isOpenLevel : 1;
    u8 challengeType : 3;
    u8 didSave : 1;
    u8 currentBattleNum;
    u16 unused2;
    u16 trainerIDs[7 * 2];
    u16 playerPartySets[(((3) >= (2 * 2)) ? (3) : (2 * 2))];
    u8 playerIVs[(((3) >= (2 * 2)) ? (3) : (2 * 2))];
    u32 playerPersonality[(((3) >= (2 * 2)) ? (3) : (2 * 2))];
    u16 opponentPartySets[(((3) >= (2 * 2)) ? (3) : (2 * 2))];
    u8 opponentIVs[(((3) >= (2 * 2)) ? (3) : (2 * 2))];
    u32 opponentPersonality[(((3) >= (2 * 2)) ? (3) : (2 * 2))];
} BattleFactorySave;

typedef struct BattleFrontierStats {
    u16 soloStats[STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS];
    u16 wfcStats[32][(STATS_NUM_STATS_TOTAL - STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS)];
} BattleFrontierStats;

typedef struct FrontierEasyChatMessages {
    MailMessage sentences[4];
} FrontierEasyChatMessages;

typedef struct FrontierSave {
    BattleFrontierStats stats;
    union {
        BattleTowerSave towerSave;
        BattleFactorySave factorySave;
        BattleHallSave hallSave;
        BattleCastleSave castleSave;
        BattleArcadeSave arcadeSave;
    };
    struct {
        WifiBattleTowerRecord wifiBattleTowerRecord;
        FrontierEasyChatMessages easyChatMessages;
        WifiBattleTowerDownloadData wifiBattleTowerDownloadData;
    } unk_950;
    struct {
        BattleFactoryStreakFlags streakFlags;
    } factory;
    struct {
        BattleHallStreakFlags streakFlags;
    } hall;
    struct {
        BattleCastlePersistentSave persistentSave;
    } castle;
    struct {
        BattleArcadeStreakFlags streakFlags;
    } arcade;
} FrontierSave; // size: 0x1628

#endif // POKEHEARTGOLD_SAVE_FRONTIER_H
