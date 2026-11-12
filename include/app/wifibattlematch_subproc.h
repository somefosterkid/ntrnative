// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifibattlematch_subproc.h: the lines in
// use so far.
#ifndef BWPORT_APP_WIFIBATTLEMATCH_SUBPROC_H
#define BWPORT_APP_WIFIBATTLEMATCH_SUBPROC_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of regulation, gameData and result in Black
struct WifiBattleMatchListParam {
    Regulation *regulation;
    PokeParty *party;
    u16 *otherName;
    u8 otherGender;
    PokeParty *otherParty;
    GameData *gameData;
    u32 unk18;
    // Gets the Pokémon picked
    PokeParty *selected;
    // 0 once the team is picked, 1 or 3 if the Wi-Fi connection was lost, 2 if the network failed
    u32 result;
};

// How the parties are exchanged
enum {
    WIFIBATTLEMATCH_EXCHANGE_WIFI,
    WIFIBATTLEMATCH_EXCHANGE_IRC,
};

// Black 2's; twinned code confirms the offsets of mode, result, party and otherParty in Black
struct WifiBattleMatchExchangeParam {
    // Points to overlay 290's parameters, which start with the GameData
    GameData **gameData;
    // A WIFIBATTLEMATCH_EXCHANGE_*
    u32 mode;
    // 0 once the teams are exchanged, 1 or 3 if the Wi-Fi connection was lost, 2 if the network failed
    u32 result;
    // The team picked, to send
    PokeParty *party;
    // Gets the other player's team
    PokeParty *otherParty;
    void *unk14;
    void *unk18;
};

#endif // BWPORT_APP_WIFIBATTLEMATCH_SUBPROC_H
