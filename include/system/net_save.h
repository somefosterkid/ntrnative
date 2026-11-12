// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/net_save.h: the lines in use so far.
#ifndef BWPORT_SYSTEM_NET_SAVE_H
#define BWPORT_SYSTEM_NET_SAVE_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Starts the save
NetSave *func_0201088c(HeapID heapId, GameData *gameData);
// Whether the save has ended
BOOL func_02010904(NetSave *save);

// Starts the save, calling func with work each frame while the save is written
NetSave *func_020108c4(HeapID heapId, GameData *gameData, void (*func)(void *work), void *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_02010934(NetSave *save);

#endif // BWPORT_SYSTEM_NET_SAVE_H
