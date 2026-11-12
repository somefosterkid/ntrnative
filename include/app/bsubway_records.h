// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/bsubway_records.h: the lines in use so
// far.
#ifndef BWPORT_APP_BSUBWAY_RECORDS_H
#define BWPORT_APP_BSUBWAY_RECORDS_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/proc.h"

// Black 2's; twinned code confirms the offsets of rank and trainNumber in Black
struct BSubwayRecordsParam {
    GameData *gameData;
    // The rank and the train number that the messages show, from the Battle Subway's save
    u32 rank;
    u32 trainNumber;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BSUBWAY_RECORDS_PROC_FUNCTIONS
extern const GameProcFunctions data_ov069_021ba650;

#endif // BWPORT_APP_BSUBWAY_RECORDS_H
