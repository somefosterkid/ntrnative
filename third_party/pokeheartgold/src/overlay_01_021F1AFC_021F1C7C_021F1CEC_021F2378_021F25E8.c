#include "global.h"

#include "metatile_behavior.h"

BOOL MetatileBehavior_IsRockClimbInDirection(u32 metatileBehavior, int facingDir);

BOOL MetatileBehavior_IsRockClimbInDirection(u32 metatileBehavior, int facingDir) {
    switch (facingDir) {
    case DIR_NORTH:
    case DIR_SOUTH:
        if (MetatileBehavior_IsRockClimbNorthSouth(metatileBehavior) == 1) {
            return TRUE;
        }
        break;
    case DIR_WEST:
    case DIR_EAST:
        if (MetatileBehavior_IsRockClimbEastWest(metatileBehavior) == 1) {
            return TRUE;
        }
        break;
    }

    return FALSE;
}
