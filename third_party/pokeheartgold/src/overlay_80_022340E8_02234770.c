#include "global.h"

#include "save.h"
#include "overlay_80_battle_arcade.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct FieldBattleDTO;
struct Party;

u16 ov80_02234770(BattleArcade *arcade);

u16 ov80_02234770(BattleArcade *arcade) {
    return arcade->currentBattle;
}
