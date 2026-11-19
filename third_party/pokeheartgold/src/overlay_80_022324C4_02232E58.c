#include "global.h"

#include "overlay_80_022324C4.h"
#include "save.h"
#include "overlay_80_battle_castle.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_save.h"

struct BattleCastleSave;
struct FieldBattleDTO;
struct Party;
u16 ov80_02232E64(BattleCastle *castle);

u8 ov80_02232E58(struct BattleCastle *castle) {
    castle->currentBattle = (u8)(castle->currentBattle + 1);
    return castle->currentBattle;
}

u16 ov80_02232E64(BattleCastle *castle) {
    return castle->currentBattle;
}
