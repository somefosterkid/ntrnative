// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukanlist.h: the lines in use so far.
#ifndef BWPORT_APP_ZUKANLIST_H
#define BWPORT_APP_ZUKANLIST_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; matching code confirms the offsets of gameData, pokedex and result in Black
// What the list is given, 0x14 bytes
struct ZukanListParam {
    GameData *gameData;
    PokeDexSave *pokedex;
    // Whether the list is a search's
    u16 searched;
    u16 count;
    u16 *list;
    int result;
};

// Black 2's; twinned code confirms the values of ZUKAN_LIST_RESULT_DETAIL in Black
// What the list returns in ZukanListParam's result: the return button, the exit button, a change of Pokédex that makes
// the list again, a species chosen, the search and the habitat list
enum {
    ZUKAN_LIST_RESULT_BACK,
    ZUKAN_LIST_RESULT_CLOSE,
    ZUKAN_LIST_RESULT_RELOAD,
    ZUKAN_LIST_RESULT_DETAIL,
    ZUKAN_LIST_RESULT_SEARCH,
    ZUKAN_LIST_RESULT_HABITAT,
};

#endif // BWPORT_APP_ZUKANLIST_H
