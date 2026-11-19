#include "global.h"

#include "error_handling.h"

enum Quadrant {
    QUADRANT_TOP_LEFT,
    QUADRANT_TOP_RIGHT,
    QUADRANT_BOTTOM_LEFT,
    QUADRANT_BOTTOM_RIGHT,
    QUADRANT_INVALID
};

u8 ov01_021F5A54(const u8 quadrant);

enum Direction {
    DIRECTION_INVALID,
    DIRECTION_EAST,
    DIRECTION_SOUTH,
    DIRECTION_WEST,
    DIRECTION_NORTH
};
u32 ov01_021F5A80(const u8 trackedTargetDirection, const int mapMatrixHeight, const int mapMatrixWidthTiles, const int trackedTargetTileIndex);

u8 ov01_021F5A54(const u8 quadrant) {
    if (quadrant == QUADRANT_TOP_LEFT) {
        return QUADRANT_BOTTOM_RIGHT;
    } else if (quadrant == QUADRANT_TOP_RIGHT) {
        return QUADRANT_BOTTOM_LEFT;
    } else if (quadrant == QUADRANT_BOTTOM_LEFT) {
        return QUADRANT_TOP_RIGHT;
    } else if (quadrant == QUADRANT_BOTTOM_RIGHT) {
        return QUADRANT_TOP_LEFT;
    } else {
        GF_ASSERT(FALSE);
    }

    return QUADRANT_TOP_LEFT;
}

u32 ov01_021F5A80(const u8 trackedTargetDirection, const int mapMatrixHeight, const int mapMatrixWidthTiles, const int trackedTargetTileIndex) {
    u32 nextTileIndex;
    int nextTileIndexCandidate;

    switch (trackedTargetDirection) {
    case DIRECTION_EAST:
        nextTileIndexCandidate = (trackedTargetTileIndex % mapMatrixWidthTiles) + 1;

        if (nextTileIndexCandidate >= mapMatrixWidthTiles) {
            return trackedTargetTileIndex;
        }

        nextTileIndex = trackedTargetTileIndex + 1;
        break;

    case DIRECTION_SOUTH:
        nextTileIndexCandidate = trackedTargetTileIndex + mapMatrixWidthTiles;

        if (nextTileIndexCandidate >= mapMatrixHeight * 32 * mapMatrixWidthTiles) {
            return trackedTargetTileIndex;
        }

        nextTileIndex = trackedTargetTileIndex + mapMatrixWidthTiles;
        break;

    case DIRECTION_WEST:
        nextTileIndexCandidate = (trackedTargetTileIndex % mapMatrixWidthTiles) - 1;

        if (nextTileIndexCandidate < 0) {
            return trackedTargetTileIndex;
        }

        nextTileIndex = trackedTargetTileIndex - 1;
        break;

    case DIRECTION_NORTH:
        nextTileIndexCandidate = trackedTargetTileIndex - mapMatrixWidthTiles;

        if (nextTileIndexCandidate < 0) {
            return trackedTargetTileIndex;
        }

        nextTileIndex = trackedTargetTileIndex - mapMatrixWidthTiles;
        break;

    default:
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    return nextTileIndex;
}
