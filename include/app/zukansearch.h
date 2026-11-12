// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukansearch.h: the lines in use so far.
#ifndef BWPORT_APP_ZUKANSEARCH_H
#define BWPORT_APP_ZUKANSEARCH_H

#include "types.h"
#include "app/zukan_common.h"
#include "struct_decls.h"

// Black 2's; matching code confirms the offsets of gameData, pokedex, criteria, result and count in Black
// What the search is given, 0x18 bytes: the criteria it edits, and the species it found
struct ZukanSearchParam {
    GameData *gameData;
    PokeDexSave *pokedex;
    ZukanSearchCriteria *criteria;
    int result;
    u16 *list;
    u16 count;
};

// What the search returns in ZukanSearchParam's result
#define ZUKAN_SEARCH_RESULT_FOUND 0

#define ZUKAN_SEARCH_RESULT_CANCEL 1
#define ZUKAN_SEARCH_RESULT_CLOSE 2

#endif // BWPORT_APP_ZUKANSEARCH_H
