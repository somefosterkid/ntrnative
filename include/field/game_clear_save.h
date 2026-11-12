// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/game_clear_save.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_GAME_CLEAR_SAVE_H
#define BWPORT_FIELD_GAME_CLEAR_SAVE_H

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
typedef struct {
    GameSystem *gsys;
    // Whether the Champion was beaten before: the first clear sets the time of the first clear and a record
    u32 repeat;
} GameClearSaveParam;

#endif // BWPORT_FIELD_GAME_CLEAR_SAVE_H
