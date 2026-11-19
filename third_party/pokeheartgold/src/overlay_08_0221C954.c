#include "global.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

s32 ov08_0221D840(void *, s8);

s32 ov08_0221C954(BattleParty *battleParty);

s32 ov08_0221C954(BattleParty *battleParty) {
    if (battleParty->displayingContestStats == FALSE) {
        ov08_0221D840(battleParty, 6);
    } else {
        ov08_0221D840(battleParty, 8);
    }
    return 0x13;
}

s32 ov08_022213C8(BattleParty *arg0);

s32 ov08_0221C978(BattleParty *battleParty);

s32 ov08_0221C978(BattleParty *battleParty) {
    ov08_022213C8(battleParty);
    if (battleParty->displayingContestStats == FALSE) {
        ov08_0221D840(battleParty, 7);
    } else {
        ov08_0221D840(battleParty, 9);
    }
    return 0x14;
}
