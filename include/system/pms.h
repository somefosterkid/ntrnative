// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/pms.h, the lines in use so far.
// pokebw2's func_0200ef90 and func_0200efa8 are Black's func_0200e6fc and func_0200e714.
#ifndef BWPORT_SYSTEM_PMS_H
#define BWPORT_SYSTEM_PMS_H

// The phrases made of a sentence and words, and the parameter of overlay 110's phrase select, of ARM9 main. Names
// from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "system/pms_data.h"
#include "system/pmsi_param.h"

// The C-Gear's phrases, which the save keeps
void *getCGearDataBlkAddress(SaveControl *save);
void func_0200e6fc(void *cgear, u32 index, PMSData *sentence);
void func_0200e714(void *cgear, u32 index, const PMSData *sentence);

// Overlay 110, the phrase select
extern const GameProcFunctions PMS_INPUT_PROC_FUNCTIONS;

// pokebw2's CGearPhrases_Init
void func_0200e6b4(PMSData *phrases);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CGearPhrases_GetBlock
// The same block, through the C-Gear's own accessor
void *func_0200e6e8(SaveControl *save);

#endif // BWPORT_SYSTEM_PMS_H
