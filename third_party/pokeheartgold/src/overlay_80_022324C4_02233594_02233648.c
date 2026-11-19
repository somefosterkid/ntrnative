#include "global.h"

#include "save.h"
#include "overlay_80_battle_castle.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_save.h"

struct BattleCastleSave;

struct FieldBattleDTO;

struct Party;


u16 ov80_02233648(BattleCastle *castle);

u16 ov80_02233648(BattleCastle *castle) {
    u8 bp;
    static const u8 sBPPerRoundSolo[((49 / 7) + 1) + 1] = { 0, 3, 3, 4, 4, 5, 5, 7, 7 };
    static const u8 sBPPerRoundWifi[((49 / 7) + 1) + 1] = { 0, 8, 9, 11, 12, 14, 15, 18, 18 };

    u16 currentRound = castle->currentRound;

    if (castle->challengeType == 0 || castle->challengeType == 1) {
        if (currentRound >= ((49 / 7) + 1)) {
            bp = sBPPerRoundSolo[((49 / 7) + 1)];
        } else {
            bp = sBPPerRoundSolo[currentRound];
        }
    } else {
        if (currentRound >= ((49 / 7) + 1)) {
            bp = sBPPerRoundWifi[((49 / 7) + 1)];
        } else {
            bp = sBPPerRoundWifi[currentRound];
        }
    }

    if (castle->challengeType == 0) {
        if (castle->currentStreak == 21 || castle->currentStreak == 49) {
            bp = 20;
        }
    }

    return bp;
}
