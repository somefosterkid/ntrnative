#include "global.h"

#include "save.h"
#include "seal_case.h"
#include "sprite.h"
#include "overlay_80_battle_arcade.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleArcadeSave;

struct FieldBattleDTO;


typedef u16 charcode_t;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

typedef struct BattleArcadeSave BattleArcadeSave;

u8 BattleArcade_GetMonCount(u8 challengeType, BOOL includePartnerMons);

int ov80_02234894(BattleArcade *battleArcade, Party *playersParty, Party *partnersParty, u8 partySize);

extern const struct {
    u8 turns;
    u8 score;
} ov80_0223BE9E[5];

int BattleArcade_GetWonBattlePoints(BattleArcade *battleArcade, Party *playersParty, Party *partnersParty, int totalTurnsElapsed);

Pokemon *Party_GetMonByIndex(const Party *party, int slot);
u32 GetMonData(Pokemon *mon, int param, void *dest);
BOOL BattleArcade_MultiplayerCheck(u8 challengeType);
extern const u8 ov80_0223BE90[(((3) >= (2 * 2)) ? (3) : (2 * 2)) + 1];
extern const u8 ov80_0223BE88[(((3) >= (2 * 2)) ? (3) : (2 * 2)) + 1];

int BattleArcade_GetWonBattlePoints(BattleArcade *battleArcade, Party *playersParty, Party *partnersParty, int totalTurnsElapsed) {
    int performance = 0;
    u8 partySize = BattleArcade_GetMonCount(battleArcade->challengeType, 0);

    performance += ov80_02234894(battleArcade, playersParty, partnersParty, partySize);

    for (int i = 0; i < ((int)(sizeof(ov80_0223BE9E) / sizeof(*(ov80_0223BE9E)))); i++) {
        if (totalTurnsElapsed < ov80_0223BE9E[i].turns) {
            performance += ov80_0223BE9E[i].score;
            break;
        }
    }

    return performance;
}

int ov80_02234894(BattleArcade *battleArcade, Party *playersParty, Party *partnersParty, u8 partySize) {
    int performance = 0;
    u8 faintedMons = 0;
    u8 monsWithStatusCondition = 0;

    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(playersParty, i);

        if (GetMonData(mon, MON_DATA_SPECIES_EXISTS, 0L) == 0) {
            continue;
        }

        if (GetMonData(mon, MON_DATA_HP, 0L) == 0) {
            faintedMons++;
        }

        if (GetMonData(mon, MON_DATA_STATUS, 0L) != 0) {
            monsWithStatusCondition++;
        }
    }

    if (BattleArcade_MultiplayerCheck(battleArcade->challengeType) == 1) {
        for (int i = 0; i < partySize; i++) {
            Pokemon *mon = Party_GetMonByIndex(partnersParty, i);

            if (GetMonData(mon, MON_DATA_SPECIES_EXISTS, 0L) == 0) {
                continue;
            }

            if (GetMonData(mon, MON_DATA_HP, 0L) == 0) {
                faintedMons++;
            }

            if (GetMonData(mon, MON_DATA_STATUS, 0L) != 0) {
                monsWithStatusCondition++;
            }
        }
    }

    performance += ov80_0223BE90[monsWithStatusCondition];
    performance += ov80_0223BE88[faintedMons];

    return performance;
}
