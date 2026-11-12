// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/battle_select.h.
#ifndef BWPORT_BATTLE_BATTLE_SELECT_H
#define BWPORT_BATTLE_BATTLE_SELECT_H

#include "types.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// Overlay 134 (battle_select.c, a guessed name): the Wi-Fi Club's selection of the team for a battle, which runs
// overlay 91's party list and overlay 131's summary screen, then sends the team picked to the other player. The
// field names are pokebw2's

// What the Wi-Fi Club's event fills in
typedef struct {
    Regulation *regulation;
    PokeParty *party;
    u16 *otherName;
    u8 otherGender;
    PokeParty *otherParty;
    GameData *gameData;
    u8 unk18;
    // Gets the Pokémon picked
    PokeParty *unk1C;
    // Where each player's team picked arrives, by net ID
    PokeParty *parties[2];
    // 1 if the selection ended because the connection was lost
    u32 result;
} BattleSelectParam;

// pokebw2's BATTLE_SELECT_PROC_FUNCTIONS
extern const GameProcFunctions data_ov134_021dcca4;

#endif // BWPORT_BATTLE_BATTLE_SELECT_H
