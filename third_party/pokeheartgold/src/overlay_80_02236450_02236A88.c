#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "overlay_80_02236450.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"

struct BattleTowerSave;
struct FieldBattleDTO;
struct WifiBattleTowerRecord;
enum BattleTowerMode {
    BATTLE_TOWER_MODE_SINGLE = 0,
    BATTLE_TOWER_MODE_DOUBLE = 1,
    BATTLE_TOWER_MODE_MULTI = 2,
    BATTLE_TOWER_MODE_LINK_MULTI = 3,
    BATTLE_TOWER_MODE_WIFI = 4,
    BATTLE_TOWER_MODE_5 = 5,
    BATTLE_TOWER_MODE_6 = 6,
};
enum PokemonStat {
    STAT_HP = 0,
    STAT_ATTACK = 1,
    STAT_DEFENSE = 2,
    STAT_SPEED = 3,
    STAT_SPECIAL_ATTACK = 4,
    STAT_SPECIAL_DEFENSE = 5,
    STAT_MAX = 6,
};
typedef struct WifiBattleTowerRecord WifiBattleTowerRecord;
typedef struct BattleTowerPartnerData {
    u32 otID;
    u16 monSetIDs[2];
    u32 personalities[2];
} BattleTowerPartnerData;
typedef struct BattleTowerSave BattleTowerSave;
typedef struct BattleTower {
    int sentinel;
    enum HeapID heapID;
    u32 rng;
    u8 nextOpponentNum;
    u8 currentBattle;
    u8 partySize;
    u8 challengeMode;
    u8 defeatedSevenTrainers : 1;
    u8 beatPalmer : 2;
    u8 isRetiring : 1;
    u8 ribbonsEarned : 1;
    u8 partnerID : 3;
    u8 playerGender;
    u8 partnerGender;
    u16 unk_14;
    u16 unk_16[2];
    u16 startingStreak;
    u16 roomNum;
    u32 victoryRecord;
    u16 unk_24;
    u16 unk_26;
    u16 unk_28;
    u8 partySlots[4];
    u16 selectedSpecies[4];
    u16 selectedItems[4];
    u16 trainerIDs[7 * 2];
    Location unused;
    BattleTowerSave *towerSave;
    WifiBattleTowerRecord *wifiRecord;
    FrontierOpponent opponents[2];
    FrontierOpponent partners[5];
    BattleTowerPartnerData unk_7E8[5];
    u8 partnerConflicts[5];
    u16 commPayload[35];
    u16 commBuffer[35];
    int wonBattle;
    FieldBattleDTO *dto;
    u8 msgsReceived;
    u8 unk_8D5;
    u8 unk_8D6;
    u8 unused2;
    u16 unk_8D8;
    u16 unk_8DA;
} BattleTower;

void ov80_02236ABC(BattleTower *battleTower, BOOL shouldRetire);
u16 ov80_02236AD8(BattleTower *battleTower, const u16 *payload);

u32 ov80_02236A88(u8 challengeMode) {
    switch (challengeMode) {
    case BATTLE_TOWER_MODE_SINGLE:
    case BATTLE_TOWER_MODE_WIFI:
        return (1 << 7) | ((0 << 0) | (1 << 0));
    case BATTLE_TOWER_MODE_DOUBLE:
        return (1 << 7) | ((1 << 1) | (1 << 0));
    case BATTLE_TOWER_MODE_MULTI:
        return (1 << 7) | ((1 << 0) | ((1 << 1) | (1 << 3) | (1 << 6)));
    case BATTLE_TOWER_MODE_LINK_MULTI:
    case BATTLE_TOWER_MODE_6:
        return ((1 << 2) | (1 << 7)) | ((1 << 1) | (1 << 0)) | (1 << 3);
    }

    return (1 << 7) | ((0 << 0) | (1 << 0));
}

void ov80_02236ABC(BattleTower *battleTower, BOOL shouldRetire) {
    battleTower->isRetiring = shouldRetire;
    battleTower->commPayload[0] = shouldRetire;
}

u16 ov80_02236AD8(BattleTower *battleTower, const u16 *payload) {
    if (battleTower->isRetiring || payload[0]) {
        return 1;
    }
    return 0;
}
