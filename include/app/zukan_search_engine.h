// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_search_engine.h: the lines in use
// so far.
#ifndef BWPORT_APP_ZUKAN_SEARCH_ENGINE_H
#define BWPORT_APP_ZUKAN_SEARCH_ENGINE_H

#include "types.h"
#include "app/zukan_common.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct ZukanSearchEngine ZukanSearchEngine;

// pokebw2's ZukanSearchEngine_Create
ZukanSearchEngine *func_ov212_021c9428(PokeDexSave *pokedex, const ZukanSearchCriteria *criteria, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZukanSearchEngine_Step
// Runs a step of the search, returning TRUE once it is done, and then the species found and their count
BOOL func_ov212_021c9468(ZukanSearchEngine *engine, HeapID heapId, u16 *count, u16 **list);
// pokebw2's ZukanSearchEngine_Free
void func_ov212_021c9460(ZukanSearchEngine *engine);

#endif // BWPORT_APP_ZUKAN_SEARCH_ENGINE_H
