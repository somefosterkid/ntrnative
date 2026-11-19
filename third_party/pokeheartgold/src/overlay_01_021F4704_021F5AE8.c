#include "global.h"

#include "error_handling.h"
#include "overlay_01_021F4704.h"

enum Direction {
    DIRECTION_INVALID,
    DIRECTION_EAST,
    DIRECTION_SOUTH,
    DIRECTION_WEST,
    DIRECTION_NORTH
};

u8 ov01_021F5AE8(const u8 direction, const int quadrant) {
    switch (direction) {
    case DIRECTION_NORTH:
        return (quadrant)-2;

    case DIRECTION_SOUTH:
        return (quadrant) + 2;

    case DIRECTION_WEST:
        return (quadrant)-1;

    case DIRECTION_EAST:
        return (quadrant) + 1;
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    return direction; /* r0 on DS = direction, the first argument */
#endif
}

void ov01_021F5B24(const u8 direction, const int quadrant, u8 *oppositeQuadrant) {
    u8 nextQuadrant = ov01_021F5AE8(direction, quadrant);
    u8 nextOppositeQuadrant;

    if (quadrant < nextQuadrant) {
        if (nextQuadrant - quadrant == 1) {

            nextOppositeQuadrant = ((*oppositeQuadrant) - 1);
        } else if (nextQuadrant - quadrant == 2) {

            nextOppositeQuadrant = ((*oppositeQuadrant) - 2);
        } else {
            ((0) ? (void)0 : GF_AssertFail());
        }
    } else if (quadrant > nextQuadrant) {
        if (quadrant - nextQuadrant == 1) {

            nextOppositeQuadrant = ((*oppositeQuadrant) + 1);
        } else if (quadrant - nextQuadrant == 2) {

            nextOppositeQuadrant = ((*oppositeQuadrant) + 2);
        } else {
            ((0) ? (void)0 : GF_AssertFail());
        }
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }

    (*oppositeQuadrant) = nextOppositeQuadrant;
}

u8 ov01_021F5B88(const u8 direction) {
    switch (direction) {
    case DIRECTION_NORTH:
        return DIRECTION_SOUTH;

    case DIRECTION_SOUTH:
        return DIRECTION_NORTH;

    case DIRECTION_EAST:
        return DIRECTION_WEST;

    case DIRECTION_WEST:
        return DIRECTION_EAST;

    default:
        ((0) ? (void)0 : GF_AssertFail());
        return DIRECTION_INVALID;
    }
}
