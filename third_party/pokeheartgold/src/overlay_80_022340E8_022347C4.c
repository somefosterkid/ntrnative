#include "global.h"

#include "save.h"
#include "sprite.h"
#include "overlay_80_battle_arcade.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleArcadeSave;
struct FieldBattleDTO;
struct Party;
typedef struct BattleArcadeSave BattleArcadeSave;
void ov80_02234588(BattleArcade *arcade, u8 saveType);

void ov80_022347C4(BattleArcade *arcade);

void ov80_022347C4(BattleArcade *arcade) {
    arcade->saveStreak = 1;

    if (arcade->currentRound < ((49 / 7) + 1)) {
        arcade->currentRound++;
    }

    arcade->currentBattle = 0;
    ov80_02234588(arcade, 0);
}
