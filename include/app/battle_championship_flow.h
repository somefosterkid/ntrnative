// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_championship_flow.h: the lines in
// use so far.
#ifndef BWPORT_APP_BATTLE_CHAMPIONSHIP_FLOW_H
#define BWPORT_APP_BATTLE_CHAMPIONSHIP_FLOW_H

#include "types.h"
#include "struct_decls.h"

typedef struct {
    GameData *gameData;
    // A BATTLE_CHAMPIONSHIP_FLOW_MODE_*
    u32 mode;
    // A BATTLE_CHAMPIONSHIP_FLOW_RESULT_*
    u32 result;
    LiveBattleMatchPlayerData *playerData;
    LiveBattleMatchPlayerData *partnerData;
    LiveBattleMatchResult *matchResult;
} BattleChampionshipFlowParam;

// What the process starts with: the menu, or the reward, or one of the ways into the live competition (the modes
// of livebattlematch_flow.c, in the order 0, 2, 3, 1)
enum {
    BATTLE_CHAMPIONSHIP_FLOW_MODE_MENU,
    BATTLE_CHAMPIONSHIP_FLOW_MODE_REWARD,
    BATTLE_CHAMPIONSHIP_FLOW_MODE_MENU_2,
    BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_JOIN,
    BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_AFTER_MATCH,
    BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_QUIT,
    BATTLE_CHAMPIONSHIP_FLOW_MODE_LIVE_MENU,
};

// What the process ends with: 0 if the player left the menu and 1 if they joined the Wi-Fi competition. The live
// competition's flow gives 2, 3 or 4, which the process passes on
enum {
    BATTLE_CHAMPIONSHIP_FLOW_RESULT_EXIT,
    BATTLE_CHAMPIONSHIP_FLOW_RESULT_WIFI,
};

#endif // BWPORT_APP_BATTLE_CHAMPIONSHIP_FLOW_H
