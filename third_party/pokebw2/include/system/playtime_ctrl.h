#ifndef POKEBW2_SYSTEM_PLAYTIME_CTRL_H
#define POKEBW2_SYSTEM_PLAYTIME_CTRL_H

#include "types.h"
#include "struct_decls.h"

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// The play time count (playtime_ctrl.c, a guessed name): while it runs, the seconds since it started are added to the
// save's play time

void GameSystemTimer_Disable(void);
void GameSystemTimer_Start(void);
// Adds the seconds that passed since the last call to the play time
void GameSystemTimer_Update(GameData *gameData);

#endif // POKEBW2_SYSTEM_PLAYTIME_CTRL_H
