// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/save_table.h: the lines in use so far.
#ifndef BWPORT_SAVE_SAVE_TABLE_H
#define BWPORT_SAVE_SAVE_TABLE_H

#include "types.h"
#include "gfl/backup_system.h"
#include "save/save_control.h"

// save_table.c's (pokebw2's, a file of data only). Black's extra saves end at the Hall of Fame
extern const SaveDataParams sv_extra_blk_data[SAVE_EXTRA_ID_HALL_OF_FAME + 1];
// pokebw2's save_block_data
extern const SaveDataParams data_0209d938;

#endif // BWPORT_SAVE_SAVE_TABLE_H
