#include "global.h"

#include "frontier/overlay_80_02238034.h"

int ov80_02238034(u8 challengeType, int currentRound, int trainerOffset);

extern void ov80_022380A0(u8 challengeType, int currentRound, u16 trainerIDs[], u8 numTrainers);

void ov80_022380A0(u8 challengeType, int currentRound, u16 trainerIDs[], u8 numTrainers) {
    int i = 0;
    do {
        trainerIDs[i] = ov80_02238034(challengeType, currentRound, i);

        int j;
        for (j = 0; j < i; j++) {
            if (trainerIDs[j] == trainerIDs[i]) {
                break;
            }
        }

        if (j != i) {
            continue;
        }

        i++;
    } while (i < numTrainers);
}

u8 BattleArcade_GetMonCount(u8 challengeType, BOOL includePartnersMons) {
    switch (challengeType) {
    case 0:
    case 1:
        return 3;
    case 2:
    case 3:
        if (includePartnersMons == 0) {
            return 2;
        } else {
            return 2 * 2;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
    return 3;
}

u8 BattleArcade_GetOpponentMonCount(u8 challengeType, BOOL includePartnersMons) {
    switch (challengeType) {
    case 0:
    case 1:
        return 3;
    case 2:
    case 3:
        if (includePartnersMons == 0) {
            return 2;
        } else {
            return 2 * 2;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
    return 3;
}
