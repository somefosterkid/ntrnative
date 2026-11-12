// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/pokewood.h: the lines in use so far.
#ifndef BWPORT_SAVE_POKEWOOD_H
#define BWPORT_SAVE_POKEWOOD_H

#include "types.h"
#include "save/battle_rec.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// BtlSetup fields that only Pokéstar Studios' block keeps
struct PokewoodBattleInfo {
    u32 unkDF;
    u32 unk124;
    u8 unk110[0x14];
};

// Black 2's, not checked against Black
// The setup of a Pokéstar Studios battle, which getPokewoodBlock returns.
struct PokewoodBlock {
    BattleRecSetup setup;
    PokewoodBattleInfo info;
    u32 dataSize;
    u8 data[0x164];
    BattleRecParty party;
    BattleRecClient client;
    u16 trainerClass;
    u8 unk128;
    u8 unkE0;
    u8 unk4A4[8];
    u32 unk130;
};

// Black 2's, not checked against Black
// The number of movies. Each of the save's flag lists has a flag for each
#define POKEWOOD_MOVIE_COUNT 40

#endif // BWPORT_SAVE_POKEWOOD_H
