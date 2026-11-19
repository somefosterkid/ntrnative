#include "global.h"

#include "party.h"
#include "overlay_80_battle_castle.h"

void ov80_02237DF4(BattleCastle *, Pokemon *);

void ov80_02237E18(struct BattleCastle *castle, Party *party, Pokemon *mon);

void ov80_02237E18(struct BattleCastle *castle, Party *party, Pokemon *mon) {
    ov80_02237DF4(castle, mon);
    Party_AddMon(party, mon);
}
