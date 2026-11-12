// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_btl_demo.h: the lines in use so
// far.
#ifndef BWPORT_APP_COMM_BTL_DEMO_H
#define BWPORT_APP_COMM_BTL_DEMO_H

#include "types.h"
#include "battle/battle_proc.h"
#include "struct_decls.h"

// The screen's heap and its archive of graphics. Our names, not swan's
#define HEAPID_COMM_BTL_DEMO 0x74

// Black's, three more than Black 2's 178 (comm_btl_demo.c's code)
#define ARCID_COMM_BTL_DEMO 181

// Black 2's; twinned code confirms the values of COMM_BTL_DEMO_TYPE_SINGLE_START, COMM_BTL_DEMO_TYPE_SINGLE_END,
// COMM_BTL_DEMO_TYPE_MULTI_START and COMM_BTL_DEMO_TYPE_MULTI_END in Black
// What the screen shows, BattlePlayers' unk44: the introduction or the result, of a battle of two or of four players
enum {
    COMM_BTL_DEMO_TYPE_NONE,
    COMM_BTL_DEMO_TYPE_SINGLE_START,
    COMM_BTL_DEMO_TYPE_SINGLE_END,
    COMM_BTL_DEMO_TYPE_MULTI_START,
    COMM_BTL_DEMO_TYPE_MULTI_END,
};

// BattlePlayers' result, for the first player's side
enum {
    COMM_BTL_DEMO_RESULT_WIN,
    COMM_BTL_DEMO_RESULT_LOSE,
    COMM_BTL_DEMO_RESULT_DRAW,
};

// What the result screen shows a Pokémon as, a BattlePlayer's order[]
enum {
    COMM_BTL_DEMO_POKE_HEALTHY,
    COMM_BTL_DEMO_POKE_SICK,
    COMM_BTL_DEMO_POKE_FAINTED,
    COMM_BTL_DEMO_POKE_NONE,
};

// What the screen is called with
struct CommBtlDemoParam {
    BattlePlayers battle;
    // Set for the Wi-Fi competition, which has its own background and sounds
    BOOL competition;
    // Where the result screen counts the battle and its result, or NULL
    GameRecords *records;
};

// The screen's proc functions (overlay 213)
extern const GameProcFunctions data_ov213_021bb0b0;

#endif // BWPORT_APP_COMM_BTL_DEMO_H
