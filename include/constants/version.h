// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/constants/version.h, the lines in use so
// far.
#ifndef BWPORT_CONSTANTS_VERSION_H
#define BWPORT_CONSTANTS_VERSION_H

// The game versions
#define VERSION_WHITE 20
#define VERSION_BLACK 21

// This game's version: configure.py defines WHITE for White's build, as pokebw2 defines BLACK2 or WHITE2. Code that
// differs between the versions tests GAME_VERSION == VERSION_WHITE; the PC port defines neither, so it is Black
#ifdef WHITE
#define GAME_VERSION VERSION_WHITE
#else
#define GAME_VERSION VERSION_BLACK
#endif

// A version's bit in a mask of versions (a beacon's targetVersions, the flags of the Intrude table): White 1, Black 2.
// Not 1 << GAME_VERSION, which other code uses (mystery_net.c) and which puts White at bit 20
#define VERSION_FLAG(version) (1 << ((version) - VERSION_WHITE))
#define GAME_VERSION_FLAG VERSION_FLAG(GAME_VERSION)

// Black 2's: Black's code compares only with VERSION_WHITE and VERSION_BLACK (the_end.c, trainer_card.c)
#define VERSION_WHITE2 22
#define VERSION_BLACK2 23

// Black 2's, not checked against Black
// Not from swan: the earlier games a Pokémon can come from, as PKM_PARAM_ORIGIN_GAME holds them
#define VERSION_SAPPHIRE 1

// Black 2's, not checked against Black
#define VERSION_RUBY 2
#define VERSION_EMERALD 3
#define VERSION_FIRERED 4
#define VERSION_LEAFGREEN 5
#define VERSION_HEARTGOLD 7
#define VERSION_SOULSILVER 8
#define VERSION_DIAMOND 10
#define VERSION_PEARL 11
#define VERSION_PLATINUM 12
#define VERSION_COLOSSEUM 15

#endif // BWPORT_CONSTANTS_VERSION_H
