#include "global.h"
#include "pokemon_types_def.h"
#include "overlay_80_battle_castle.h"

struct BattleCastle;
struct Party;
struct Pokemon;
BOOL MapObject_SetHeldMovement(Party *party, Pokemon *mon);
void PlayerAvatar_SetUnk8(BattleCastle *battleCastle, Pokemon *mon);

void sub_0205DA1C(BattleCastle *battleCastle, Party *party, Pokemon *mon);

void sub_0205DA1C(BattleCastle *battleCastle, Party *party, Pokemon *mon) {
    PlayerAvatar_SetUnk8(battleCastle, mon);
    MapObject_SetHeldMovement(party, mon);
}
