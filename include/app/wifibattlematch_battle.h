// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifibattlematch_battle.h: the lines in
// use so far.
#ifndef BWPORT_APP_WIFIBATTLEMATCH_BATTLE_H
#define BWPORT_APP_WIFIBATTLEMATCH_BATTLE_H

#include "types.h"
#include "battle/battle_proc.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// The players of a battle, with what the ranking needs
struct WifiBattleMatchPlayers {
    BattlePlayers base;
    u32 unk50;
    GameRecords *records;
};

// Black 2's, not checked against Black
// What the battle step gets
struct WifiBattleMatchBattleParam {
    GameData *gameData;
    BtlSetup *setup;
    WifiBattleMatchPlayers *players;
    u32 result;
};

#endif // BWPORT_APP_WIFIBATTLEMATCH_BATTLE_H
