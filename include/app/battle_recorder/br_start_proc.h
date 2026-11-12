// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_start_proc.h: the
// lines in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_START_PROC_H
#define BWPORT_APP_BATTLE_RECORDER_BR_START_PROC_H

#include "types.h"

// Black 2's; twinned code confirms the values of BR_START_MODE_OPEN and BR_START_MODE_CLOSE in Black
// What the screen does, in BrStartProcParam's mode
enum {
    // The Battle Recorder starts up, then the menu opens
    BR_START_MODE_OPEN,
    // The Battle Recorder closes
    BR_START_MODE_CLOSE,
    // The menu opens at once, after a battle video has played
    BR_START_MODE_RETURN,
};

#endif // BWPORT_APP_BATTLE_RECORDER_BR_START_PROC_H
