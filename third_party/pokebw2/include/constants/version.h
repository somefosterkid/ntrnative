#ifndef POKEBW2_CONSTANTS_VERSION_H
#define POKEBW2_CONSTANTS_VERSION_H

// Not from swan: the earlier games a Pokémon can come from, as PKM_PARAM_ORIGIN_GAME holds them
#define VERSION_SAPPHIRE 1
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

// The game versions, which getGameVersion returns
#define VERSION_WHITE 20
#define VERSION_BLACK 21
#define VERSION_WHITE2 22
#define VERSION_BLACK2 23

// This game's version
#ifdef BLACK2
#define GAME_VERSION VERSION_BLACK2
#else
#define GAME_VERSION VERSION_WHITE2
#endif

#endif // POKEBW2_CONSTANTS_VERSION_H
