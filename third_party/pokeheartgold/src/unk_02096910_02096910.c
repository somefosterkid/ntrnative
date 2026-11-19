#include "global.h"

#include "unk_02033AE0.h"
#include "unk_02096910.h"
#include "constants/heap.h"
#include "field_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "overlay_80_frontier_opponent.h"

extern s32 _02108594;

struct BattleTowerSave;
struct FieldBattleDTO;
struct WifiBattleTowerRecord;
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
u16 sub_0203769C(void);
BOOL sub_02037030(int cmd, const void *data, int size);
void sub_02096924(int param0, int param1, void *param2, void *param3);
BOOL sub_02096998(BattleTower *battleTower);
void sub_020969C4(int netID, int unused, void *data, void *context);
void sub_020969F8(int param0, int param1, void *param2, void *param3);

void sub_02096910(void *data) {
    sub_0203410C(&_02108594, 0x34, data);
}

void sub_02096924(int param0, int param1, void *param2, void *param3) {
    u16 v0;
    int v1;
    BattleTower *battleTower = param3;
    const u16 *v3 = param2;

    v0 = 0;
    v1 = 0;
    battleTower->msgsReceived++;

    if (sub_0203769C() == param0) {
        return;
    }

    battleTower->partnerGender = (u8)v3[0];
    battleTower->unk_16[0] = v3[1];
    battleTower->unk_16[1] = v3[2];
    battleTower->unk_14 = v3[3];
    battleTower->partnerID = 5 + battleTower->partnerGender;

    if ((battleTower->selectedSpecies[0] == battleTower->unk_16[0]) || (battleTower->selectedSpecies[0] == battleTower->unk_16[1])) {
        v0 += 1;
    }

    if ((battleTower->selectedSpecies[1] == battleTower->unk_16[0]) || (battleTower->selectedSpecies[1] == battleTower->unk_16[1])) {
        v0 += 2;
    }

    battleTower->unk_8D8 = v0;
    return;
}

BOOL sub_02096998(BattleTower *battleTower) {
    int dataSize = 7 * 2 * sizeof(u16);
    MI_CpuCopy8(battleTower->trainerIDs, battleTower->commPayload, dataSize);

    return sub_02037030(63, battleTower->commPayload, dataSize) == 1;
}

void sub_020969C4(int netID, int unused, void *data, void *context) {
    BattleTower *battleTower = context;
    const u16 *trainerIDs = data;

    battleTower->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    if (sub_0203769C() == 0) {
        return;
    }

    MI_CpuCopy8(trainerIDs, battleTower->trainerIDs, 7 * 2 * sizeof(u16));
}

void sub_020969F8(int param0, int param1, void *param2, void *param3) {
    int v0;
    BattleTower *battleTower = param3;
    const u16 *v2 = param2;

    v0 = 0;

    battleTower->unk_8D8 = 0;
    battleTower->msgsReceived++;

    if (sub_0203769C() == param0) {
        return;
    }

    if (battleTower->isRetiring || v2[0]) {
        battleTower->unk_8D8 = 1;
    }

    return;
}
