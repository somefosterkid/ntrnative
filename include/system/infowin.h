// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/infowin.h: the lines in use so far.
#ifndef BWPORT_SYSTEM_INFOWIN_H
#define BWPORT_SYSTEM_INFOWIN_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's InfoWin_Init
// Loads the bar's graphics into the BG's palette and characters, and draws it every VBlank from then on. gameData
// may be NULL, for a bar that shows no Wi-Fi state
void func_02024a20(u8 bg, u8 palette, GameData *gameData, HeapID heapId);
// pokebw2's InfoWin_Exit
void func_02024dcc(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's InfoWin_Update
// Reads the OS_GetTick, the battery and the network, and marks the parts that changed to be drawn
void func_02024abc(void);

#endif // BWPORT_SYSTEM_INFOWIN_H
