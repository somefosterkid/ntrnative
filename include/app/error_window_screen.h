// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/error_window_screen.h.
#ifndef BWPORT_APP_ERROR_WINDOW_SCREEN_H
#define BWPORT_APP_ERROR_WINDOW_SCREEN_H

#include "types.h"

// An empty error window on the main screen, shown until the game is reset (error_window_screen.c, pokebw2's name for
// the file). Nothing in the game loads the overlay

// Shows the window and never returns (pokebw2's ErrorWindowScreen_Show)
void func_ov109_021bb8e0(void);

#endif // BWPORT_APP_ERROR_WINDOW_SCREEN_H
