#include "global.h"

#include "overlay_80_02237A70.h"
#include "save.h"
#include "seal_case.h"
#include "overlay_80_battle_castle.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_save.h"

struct BattleCastleSave;
struct FieldBattleDTO;
struct Party;
void ov80_0222A6B8(u8 numMons, u16 trainerID, u16 trainerID2, u16 *monSetIDs, FrontierPokemon *resultMons, u8 *ivs, u32 *personalities, u8 isMultiplayerChallenge);
u8 ov80_02237B58(u8 challengeType, BOOL includeBothOpponents);
void ov80_02237E30(BattleCastle *battleCastle);
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
int Party_GetCount(const Party *party);
Pokemon *Party_GetMonByIndex(const Party *party, int slot);
u32 GetMonData(Pokemon *mon, int param, void *dest);
void ov80_02237D9C(Party *party);
u16 sub_0203769C(void);

void ov80_02232F08(BattleCastle *castle);
void ov80_02232F60(BattleCastle *castle);
int ov80_02233020(BattleCastle *castle);

void ov80_02232F08(BattleCastle *castle) {
    ov80_0222A6B8(ov80_02237B58(castle->challengeType, 1), castle->trainerIDs[castle->currentBattle], castle->trainerIDs[castle->currentBattle + 7], castle->monSetIDs, castle->opponentMons, castle->opponentMonIVs, castle->opponentMonPersonalities, ov80_02237D8C(castle->challengeType));

    ov80_02237E30(castle);
}

void ov80_02232F60(BattleCastle *castle) {
    u8 partyOffset, playerPartySize;

    if (sub_0203769C() == 0) {
        partyOffset = 0;
    } else {
        partyOffset = 2;
    }

    playerPartySize = ov80_02237B24(castle->challengeType, 0);
    (void)(Party_GetCount(castle->playersParty));

    for (int i = partyOffset; i < (playerPartySize + partyOffset); i++) {
        Pokemon *mon = Party_GetMonByIndex(castle->playersParty, i);

        castle->startingPP[i - partyOffset][0] = GetMonData(mon, MON_DATA_MOVE1_PP, 0L);
        castle->startingPP[i - partyOffset][1] = GetMonData(mon, MON_DATA_MOVE2_PP, 0L);
        castle->startingPP[i - partyOffset][2] = GetMonData(mon, MON_DATA_MOVE3_PP, 0L);
        castle->startingPP[i - partyOffset][3] = GetMonData(mon, MON_DATA_MOVE4_PP, 0L);
    }

    ov80_02237D9C(castle->playersParty);

    for (int i = 0; i < 2 * 2; i++) {
        castle->appIdentityUnlocked[i] = 0;
        castle->levelAdjustments[i] = 0;
        castle->appStatsUnlocked[i] = 0;
        castle->appMovesUnlocked[i] = 0;
    }
}

int ov80_02233020(BattleCastle *castle) {
    int i, j;
    u8 cpFactors[20];
    int earnedCP = 0;

    for (i = 0; i < 20; i++) {
        cpFactors[i] = 0;
    }

    castle->monFainted = 0;
    int remainingPP = 0;

    u8 partyOffset;
    if (sub_0203769C() == 0) {
        partyOffset = 0;
    } else {
        partyOffset = 2;
    }

    u8 playerPartySize = ov80_02237B24(castle->challengeType, 0);
    u8 opponentPartySize = ov80_02237B58(castle->challengeType, 1);

    for (i = partyOffset; i < playerPartySize + partyOffset; i++) {
        Pokemon *mon = Party_GetMonByIndex(castle->playersParty, i);

        if (GetMonData(mon, MON_DATA_SPECIES_EXISTS, 0L) == 0) {
            continue;
        }

        u32 currentHP = GetMonData(mon, MON_DATA_HP, 0L);
        u32 maxHP = GetMonData(mon, MON_DATA_MAX_HP, 0L);

        if (currentHP > 0) {
            cpFactors[0]++;

            if (currentHP == maxHP) {
                cpFactors[1]++;
            } else {
                if (currentHP >= maxHP / 2) {
                    cpFactors[2]++;
                } else {
                    cpFactors[3]++;
                }
            }

            if (GetMonData(mon, MON_DATA_STATUS, 0L) == 0) {
                cpFactors[4]++;
            }
        } else {
            castle->monFainted = 1;
        }

        remainingPP += GetMonData(mon, MON_DATA_MOVE1_PP, 0L);
        remainingPP += GetMonData(mon, MON_DATA_MOVE2_PP, 0L);
        remainingPP += GetMonData(mon, MON_DATA_MOVE3_PP, 0L);
        remainingPP += GetMonData(mon, MON_DATA_MOVE4_PP, 0L);
    }

    for (i = 0; i < opponentPartySize; i++) {
        if (castle->levelAdjustments[i] == 1) {
            cpFactors[8]++;
        }
    }

    int startingPP = 0;

    for (i = 0; i < playerPartySize; i++) {
        for (j = 0; j < 4; j++) {
            startingPP += castle->startingPP[i][j];
        }
    }

    if (startingPP - remainingPP <= 5) {
        cpFactors[5]++;
    } else if (startingPP - remainingPP <= 10) {
        cpFactors[6]++;
    } else if (startingPP - remainingPP <= 15) {
        cpFactors[7]++;
    }

    earnedCP += cpFactors[0] * 3;
    earnedCP += cpFactors[1] * 3;
    earnedCP += cpFactors[2] * 2;
    earnedCP += cpFactors[3] * 1;
    earnedCP += cpFactors[4] * 1;
    earnedCP += cpFactors[5] * 8;
    earnedCP += cpFactors[6] * 6;
    earnedCP += cpFactors[7] * 4;
    earnedCP += cpFactors[8] * 7;

    if (earnedCP <= 0) {
        earnedCP = 1;
    }

    return earnedCP;
}
