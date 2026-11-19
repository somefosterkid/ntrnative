#include "global.h"

#include "save.h"
#include "sprite.h"
#include "overlay_80_battle_arcade.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleArcadeSave;
struct FieldBattleDTO;
struct Party;
void ov80_0222A6B8(u8 numMons, u16 trainerID, u16 trainerID2, u16 *monSetIDs, FrontierPokemon *resultMons, u8 *ivs, u32 *personalities, u8 isMultiplayerChallenge);
typedef struct BattleArcadeSave BattleArcadeSave;
u8 BattleArcade_GetOpponentMonCount(u8 challengeType, BOOL includeBothOpponents);
BOOL BattleArcade_MultiplayerCheck(u8 challengeType);
void ov80_022383C0(BattleArcade *battleArcade);

void BattleArcade_SetPartyAfterBattle(BattleArcade *arcade);

void BattleArcade_SetPartyAfterBattle(BattleArcade *arcade) {
    ov80_0222A6B8(BattleArcade_GetOpponentMonCount(arcade->challengeType, 1), arcade->trainerIDs[arcade->currentBattle], arcade->trainerIDs[arcade->currentBattle + 7], arcade->monSetIDs, arcade->opponentMons, arcade->opponentMonIVs, arcade->opponentMonPersonalities, BattleArcade_MultiplayerCheck(arcade->challengeType));

    ov80_022383C0(arcade);
}
