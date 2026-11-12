// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/boot_screens.h: the lines in use so far.
#ifndef BWPORT_APP_BOOT_SCREENS_H
#define BWPORT_APP_BOOT_SCREENS_H

#include "types.h"
#include "gfl/proc.h"

// Black 2's, not checked against Black
struct BootScreensParam {
    // TRUE shows only the copyright notice
    BOOL skipLogos;
    // Black's static (boot_proc.c's) is 8 bytes: the .bss runs to heapsys.c's; no code of Black reads this word
    u32 unk4;
};

// pokebw2's BOOT_SCREENS_PROC_FUNCTIONS
extern const GameProcFunctions data_ov088_021bd8e8;

#endif // BWPORT_APP_BOOT_SCREENS_H
