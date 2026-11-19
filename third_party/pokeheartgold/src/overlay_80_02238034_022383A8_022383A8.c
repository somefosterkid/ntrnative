#include "global.h"

#include "party.h"
#include "overlay_80_battle_arcade.h"

void ov80_02238384(BattleArcade *, Pokemon *);

void ov80_022383A8(struct BattleArcade *arcade, Party *party, Pokemon *mon);

void ov80_022383A8(struct BattleArcade *arcade, Party *party, Pokemon *mon) {
    ov80_02238384(arcade, mon);
    Party_AddMon(party, mon);
}
