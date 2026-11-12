// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/field_dir.c.
#include "types.h"
#include "constants/directions.h"
#include "field/zone.h"

// The rail's direction keys for the directions of the actors.
// The file's name is a guess, from what it does: the ROM has no string for it. The function's name is swan's

// The directions on the rails, clockwise from up (pokebw2's field_rail.h; Black's field_rail.c has them too)
enum {
    RAIL_DIR_NONE,
    RAIL_DIR_UP,
    RAIL_DIR_RIGHT,
    RAIL_DIR_DOWN,
    RAIL_DIR_LEFT,
};

static const u8 DIR_TO_RAIL_DIR[] = {
    [DIR_UP] = RAIL_DIR_UP,
    [DIR_DOWN] = RAIL_DIR_DOWN,
    [DIR_LEFT] = RAIL_DIR_LEFT,
    [DIR_RIGHT] = RAIL_DIR_RIGHT,
};

u32 ConvDirToRailDir(u32 direction) {
    return DIR_TO_RAIL_DIR[direction];
}
