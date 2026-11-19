#include "global.h"

#include "battle_tower_mode.h"
#include "frontier_data.h"
#include "save_frontier.h"
#include "unk_0202D230.h"

void sub_0202D274(WifiBattleTowerDownloadData *downloadData) {
    MI_CpuClear8(downloadData, sizeof(WifiBattleTowerDownloadData));
}

u32 sub_0202D284(BattleTowerSave *save, int fieldId, void *outBuf) {
    switch (fieldId) {
    case 0:
        return save->challengeMode;
    case 1:
        return save->nextOpponentNum;
    case 2:
        return save->unk_03;
    case 3:
        return save->unk_04;
    case 4:
        return save->unk_06;
    case 5:
        MI_CpuCopy8(save->partySlots, outBuf, 4);
        return 0;
    case 6:
        MI_CpuCopy8(&save->unk_2C, outBuf, sizeof(BattleTowerPartnerData));
        return 0;
    case 7:
        return save->unk_00_0;
    case 8:
        MI_CpuCopy8(save->trainerIDs, outBuf, 7 * 2 * sizeof(u16));
        return 0;
    case 9:
        return save->partnerID;
    case 10:
        return save->rngSeed;
    }

    return 0;
}

void sub_0202D308(BattleTowerSave *save, int fieldId, const void *inBuf) {
    u32 *inBuf32 = (u32 *)inBuf;
    u16 *inBuf16 = (u16 *)inBuf;
    u8 *inBuf8 = (u8 *)inBuf;

    switch (fieldId) {
    case 0:
        save->challengeMode = inBuf8[0];
        break;
    case 1:
        save->nextOpponentNum = inBuf8[0];
        break;
    case 2:
        save->unk_03 = inBuf8[0];
        break;
    case 3:
        save->unk_04 = inBuf16[0];
        break;
    case 4:
        save->unk_06 = inBuf16[0];
        break;
    case 5:
        MI_CpuCopy8(inBuf8, save->partySlots, 4);
        break;
    case 6:
        MI_CpuCopy8(inBuf16, &save->unk_2C, sizeof(BattleTowerPartnerData));
        break;
    case 7:
        save->unk_00_0 = inBuf8[0];
        break;
    case 8:
        MI_CpuCopy8(inBuf16, save->trainerIDs, 2 * 14);
        break;
    case 10:
        save->rngSeed = inBuf32[0];
        break;
    case 9:
        save->partnerID = inBuf8[0];
        break;
    }
}

void sub_0202D3B0(BattleTowerSave *save, u8 param1, u16 param2, u16 param3) {
    if (save->unk_03 + param1 < (u8)-1) {
        save->unk_03 += param1;
    }

    if (save->unk_04 + param2 < (u16)-1) {
        save->unk_04 += param2;
    }

    if (save->unk_06 + param3 < (u16)-1) {
        save->unk_06 += param3;
    }
}

BOOL sub_0202D3DC(BattleTowerSave *save) {
    return save->didSave;
}

void sub_0202D3E4(BattleTowerSave *save, BOOL didSave) {
    save->didSave = didSave;
}

u16 FrontierData_BattlePointAction(FrontierData *record, u16 value, int func) {
    switch (func) {
    case 1:
        if (value > 9999) {
            record->battlePoints = 9999;
        } else {
            record->battlePoints = value;
        }
        break;
    case 5:
        if (record->battlePoints + value > 9999) {
            record->battlePoints = 9999;
        } else {
            record->battlePoints += value;
        }
        break;
    case 6:
        if (record->battlePoints < value) {
            record->battlePoints = 0;
        } else {
            record->battlePoints -= value;
        }
    case 0:
    default:
        break;
    }

    return record->battlePoints;
}

u8 sub_0202D450(FrontierData *record, int op) {
    switch (op) {
    case 2:
        record->lossStreak = 0;
        record->lossStreakInitialized = 0;
        break;
    case 3:
        if (record->lossStreakInitialized) {
            record->lossStreak += 1;
        } else {
            record->lossStreak = 1;
            record->lossStreakInitialized = 1;
        }
        break;
    }

    return record->lossStreak;
}

u8 sub_0202D488(FrontierData *record, int op) {
    switch (op) {
    case 2:
        record->rank = 1;
        break;
    case 3:
        if (record->rank < 10) {
            record->rank += 1;
        }
        break;
    case 4:
        if (record->rank > 1) {
            record->rank -= 1;
        }
        break;
    }

    return record->rank;
}

void sub_0202D4B8(FrontierData *record, int teamIdx, FrontierPokemon *mons) {
    if (teamIdx == 0) {
        MI_CpuCopy8(mons, record->team0, sizeof(FrontierPokemon) * 3);
    } else {
        MI_CpuCopy8(mons, record->team1, sizeof(FrontierPokemon) * 3);
    }
}

void sub_0202D4DC(FrontierData *record, int teamIdx, FrontierPokemon *outBuf) {
    if (teamIdx == 0) {
        MI_CpuCopy8(record->team0, outBuf, sizeof(FrontierPokemon) * 3);
    } else {
        MI_CpuCopy8(record->team1, outBuf, sizeof(FrontierPokemon) * 3);
    }
}

u16 sub_0202D4FC(FrontierData *record, BattleTowerSave *save) {
    u16 progressScore, penaltyA, penaltyB, efficiencyBonus, performanceScore;

    progressScore = (save->nextOpponentNum - 1) * 1000;
    penaltyA = save->unk_04 * 10;
    penaltyB = save->unk_03 * 20;

    if (penaltyA + penaltyB > 950) {
        performanceScore = 0;
    } else {
        performanceScore = 950 - (penaltyA + penaltyB);
    }

    if (save->unk_06 > (1000 - 30)) {
        efficiencyBonus = 0;
    } else {
        efficiencyBonus = (1000 - save->unk_06) / 30;
    }

    u16 ratingScore = progressScore + performanceScore + efficiencyBonus;
    record->ratingScore = ratingScore;

    return ratingScore;
}

u16 sub_0202D564(FrontierData *record) {
    return record->ratingScore;
}

u8 sub_0202D568(FrontierData *record) {
    u8 ratingTier = (record->ratingScore) / 1000;
    return ratingTier;
}

u16 sub_0202D57C(FrontierData *record, u16 challengeMode, int op) {
    u16 modeIdx;

    if (challengeMode == BATTLE_TOWER_MODE_5) {
        return 0;
    }

    if (challengeMode == BATTLE_TOWER_MODE_6) {
        modeIdx = 5;
    } else {
        modeIdx = challengeMode;
    }

    switch (op) {
    case 2:
        record->roomNums[modeIdx] = 0;
        break;
    case 3:
        if (record->roomNums[modeIdx] < 65534) {
            record->roomNums[modeIdx] += 1;
        }
        break;
    }

    return record->roomNums[modeIdx];
}

u16 sub_0202D5C4(FrontierData *record, u16 challengeMode, u16 roomNum) {
    u16 modeIdx;

    if (challengeMode == 5) {
        return 0;
    }

    if (challengeMode == 6) {
        modeIdx = 5;
    } else {
        modeIdx = challengeMode;
    }

    record->roomNums[modeIdx] = roomNum;
    return record->roomNums[modeIdx];
}

BOOL sub_0202D5DC(FrontierData *record, u16 bitIdx, int op) {
    u16 mask = 1;

    if (bitIdx >= 16) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    for (u16 i = 0; i < bitIdx; i++) {
        mask <<= 1;
    }

    switch (op) {
    case 2:
        mask = mask ^ 0xFFFF;
        record->flags &= mask;
        break;
    case 1:
        record->flags |= mask;
        break;
    case 0:
        return (record->flags >> bitIdx) & 0x1;
    }

    return 0;
}

void sub_0202D638(FrontierData *record, u32 rngState) {
    record->rngState = rngState;
}

u32 sub_0202D63C(FrontierData *record) {
    return record->rngState;
}
