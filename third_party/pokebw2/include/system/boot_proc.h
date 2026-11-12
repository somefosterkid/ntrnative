#ifndef POKEBW2_SYSTEM_BOOT_PROC_H
#define POKEBW2_SYSTEM_BOOT_PROC_H

#include "types.h"
#include "gfl/proc.h"

// The first process of the game, which runs the boot screens, the demos and the title screen in turn, and again
// when the title screen times out. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
extern const GameProcFunctions EVENT_TITLE_FUNCS;

#endif // POKEBW2_SYSTEM_BOOT_PROC_H
