#ifndef POKEBW2_SYSTEM_INFOWIN_H
#define POKEBW2_SYSTEM_INFOWIN_H

#include "types.h"
#include "struct_decls.h"

// The information bar along the top of a screen (infowin.c): the clock, the wireless and Wi-Fi icons, the signal
// strength and the battery. The names are ours

// Loads the bar's graphics into the BG's palette and characters, and draws it every VBlank from then on. gameData
// may be NULL, for a bar that shows no Wi-Fi state
void InfoWin_Init(u8 bg, u8 palette, GameData *gameData, HeapID heapId);
// Reads the clock, the battery and the network, and marks the parts that changed to be drawn
void InfoWin_Update(void);
void InfoWin_Exit(void);

#endif // POKEBW2_SYSTEM_INFOWIN_H
