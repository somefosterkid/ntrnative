#include "global.h"

#include "overlay_01_map_load_manager.h"

#include "field/map_prop_animation.h"

#include "error_handling.h"
#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"

struct AreaDataManager;

struct MapPropManager;

enum Quadrant {
    QUADRANT_TOP_LEFT,
    QUADRANT_TOP_RIGHT,
    QUADRANT_BOTTOM_LEFT,
    QUADRANT_BOTTOM_RIGHT,
    QUADRANT_INVALID
};

enum Direction {
    DIRECTION_INVALID,
    DIRECTION_EAST,
    DIRECTION_SOUTH,
    DIRECTION_WEST,
    DIRECTION_NORTH
};

void ov01_021F55F4(const u8 firstResetLoadedMap, const u8 secondResetLoadedMap, const u8 firstKeptLoadedMap, const u8 secondKeptLoadedMap, MapLoadManager *mapLoadManager);

void ov01_021F54AC(const int firstMapMatrixIndex, const int secondMapMatrixIndex, const u8 firstQuadrantToLoad, const u8 secondQuadrantToLoad, const u8 trackedTargetDirection, MapLoadManager *mapLoadManager);

BOOL ov01_021F4704(const int lhsX, const int rhsX, const int mapMatrixWidth);

BOOL ov01_021F4728(const int lhsY, const int rhsY, const int mapMatrixWidth);

void ov01_021F562C(const int trackedTargetMapMatrixIndex, const u8 trackedTargetDirection, MapLoadManager *mapLoadManager);

void ov01_021F562C(const int trackedTargetMapMatrixIndex, const u8 trackedTargetDirection, MapLoadManager *mapLoadManager) {
    int firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad;
    int firstQuadrantToLoad, secondQuadrantToLoad;

    switch (trackedTargetDirection) {
    case DIRECTION_NORTH:
        ov01_021F55F4(QUADRANT_BOTTOM_LEFT, QUADRANT_BOTTOM_RIGHT, QUADRANT_TOP_LEFT, QUADRANT_TOP_RIGHT, mapLoadManager);

        if (mapLoadManager->quadrant == QUADRANT_BOTTOM_LEFT) {
            firstMapMatrixIndexToLoad = trackedTargetMapMatrixIndex - mapLoadManager->width;
            secondMapMatrixIndexToLoad = trackedTargetMapMatrixIndex - mapLoadManager->width - 1;

            firstQuadrantToLoad = QUADRANT_TOP_RIGHT;
            secondQuadrantToLoad = QUADRANT_TOP_LEFT;

            if (firstMapMatrixIndexToLoad < 0) {
                firstMapMatrixIndexToLoad = -1;
                secondMapMatrixIndexToLoad = -1;
            } else if (secondMapMatrixIndexToLoad < 0 || !ov01_021F4728(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, mapLoadManager->width)) {
                secondMapMatrixIndexToLoad = -1;
            }
        } else if (mapLoadManager->quadrant == QUADRANT_BOTTOM_RIGHT) {
            firstMapMatrixIndexToLoad = trackedTargetMapMatrixIndex - mapLoadManager->width;
            secondMapMatrixIndexToLoad = trackedTargetMapMatrixIndex - mapLoadManager->width + 1;

            firstQuadrantToLoad = QUADRANT_TOP_LEFT;
            secondQuadrantToLoad = QUADRANT_TOP_RIGHT;

            if (firstMapMatrixIndexToLoad < 0) {
                firstMapMatrixIndexToLoad = -1;
                secondMapMatrixIndexToLoad = -1;
            } else if (!ov01_021F4728(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, mapLoadManager->width)) {
                secondMapMatrixIndexToLoad = -1;
            }
        } else {
            ((0) ? (void)0 : GF_AssertFail());
        }

        ov01_021F54AC(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, firstQuadrantToLoad, secondQuadrantToLoad, trackedTargetDirection, mapLoadManager);
        break;

    case DIRECTION_WEST:
        ov01_021F55F4(QUADRANT_TOP_RIGHT, QUADRANT_BOTTOM_RIGHT, QUADRANT_TOP_LEFT, QUADRANT_BOTTOM_LEFT, mapLoadManager);

        if (mapLoadManager->quadrant == QUADRANT_TOP_RIGHT) {
            firstMapMatrixIndexToLoad = trackedTargetMapMatrixIndex - 1;
            secondMapMatrixIndexToLoad = trackedTargetMapMatrixIndex - mapLoadManager->width - 1;

            firstQuadrantToLoad = QUADRANT_BOTTOM_LEFT;
            secondQuadrantToLoad = QUADRANT_TOP_LEFT;

            if (firstMapMatrixIndexToLoad < 0 || !ov01_021F4728(firstMapMatrixIndexToLoad, trackedTargetMapMatrixIndex, mapLoadManager->width)) {
                firstMapMatrixIndexToLoad = -1;
                secondMapMatrixIndexToLoad = -1;
            }

            if (secondMapMatrixIndexToLoad < 0 || !ov01_021F4704(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, mapLoadManager->width)) {
                secondMapMatrixIndexToLoad = -1;
            }
        } else if (mapLoadManager->quadrant == QUADRANT_BOTTOM_RIGHT) {
            firstMapMatrixIndexToLoad = trackedTargetMapMatrixIndex - 1;
            secondMapMatrixIndexToLoad = trackedTargetMapMatrixIndex + mapLoadManager->width - 1;

            firstQuadrantToLoad = QUADRANT_TOP_LEFT;
            secondQuadrantToLoad = QUADRANT_BOTTOM_LEFT;

            if (firstMapMatrixIndexToLoad < 0 || !ov01_021F4728(firstMapMatrixIndexToLoad, trackedTargetMapMatrixIndex, mapLoadManager->width)) {
                firstMapMatrixIndexToLoad = -1;
                secondMapMatrixIndexToLoad = -1;
            }

            if (mapLoadManager->width * mapLoadManager->height <= secondMapMatrixIndexToLoad || !ov01_021F4704(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, mapLoadManager->width)) {
                secondMapMatrixIndexToLoad = -1;
            }
        } else {
            ((0) ? (void)0 : GF_AssertFail());
        }

        ov01_021F54AC(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, firstQuadrantToLoad, secondQuadrantToLoad, trackedTargetDirection, mapLoadManager);
        break;

    case DIRECTION_EAST:
        ov01_021F55F4(QUADRANT_TOP_LEFT, QUADRANT_BOTTOM_LEFT, QUADRANT_TOP_RIGHT, QUADRANT_BOTTOM_RIGHT, mapLoadManager);

        if (mapLoadManager->quadrant == QUADRANT_TOP_LEFT) {
            firstMapMatrixIndexToLoad = trackedTargetMapMatrixIndex + 1;
            secondMapMatrixIndexToLoad = trackedTargetMapMatrixIndex - mapLoadManager->width + 1;

            firstQuadrantToLoad = QUADRANT_BOTTOM_RIGHT;
            secondQuadrantToLoad = QUADRANT_TOP_RIGHT;

            if (mapLoadManager->width * mapLoadManager->height <= firstMapMatrixIndexToLoad || !ov01_021F4728(firstMapMatrixIndexToLoad, trackedTargetMapMatrixIndex, mapLoadManager->width)) {
                firstMapMatrixIndexToLoad = -1;
                secondMapMatrixIndexToLoad = -1;
            }

            if (mapLoadManager->width * mapLoadManager->height <= secondMapMatrixIndexToLoad || !ov01_021F4704(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, mapLoadManager->width)) {
                secondMapMatrixIndexToLoad = -1;
            }
        } else if (mapLoadManager->quadrant == QUADRANT_BOTTOM_LEFT) {
            firstMapMatrixIndexToLoad = trackedTargetMapMatrixIndex + 1;
            secondMapMatrixIndexToLoad = trackedTargetMapMatrixIndex + mapLoadManager->width + 1;

            firstQuadrantToLoad = QUADRANT_TOP_RIGHT;
            secondQuadrantToLoad = QUADRANT_BOTTOM_RIGHT;

            if (mapLoadManager->width * mapLoadManager->height <= firstMapMatrixIndexToLoad || !ov01_021F4728(firstMapMatrixIndexToLoad, trackedTargetMapMatrixIndex, mapLoadManager->width)) {
                firstMapMatrixIndexToLoad = -1;
                secondMapMatrixIndexToLoad = -1;
            }

            if (secondMapMatrixIndexToLoad < 0 || !ov01_021F4704(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, mapLoadManager->width)) {
                secondMapMatrixIndexToLoad = -1;
            }
        }

        ov01_021F54AC(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, firstQuadrantToLoad, secondQuadrantToLoad, trackedTargetDirection, mapLoadManager);
        break;

    case DIRECTION_SOUTH:
        ov01_021F55F4(QUADRANT_TOP_LEFT, QUADRANT_TOP_RIGHT, QUADRANT_BOTTOM_LEFT, QUADRANT_BOTTOM_RIGHT, mapLoadManager);

        if (mapLoadManager->quadrant == QUADRANT_TOP_LEFT) {
            firstMapMatrixIndexToLoad = trackedTargetMapMatrixIndex + mapLoadManager->width;
            secondMapMatrixIndexToLoad = trackedTargetMapMatrixIndex + mapLoadManager->width - 1;

            firstQuadrantToLoad = QUADRANT_BOTTOM_RIGHT;
            secondQuadrantToLoad = QUADRANT_BOTTOM_LEFT;

            if (mapLoadManager->width * mapLoadManager->height <= firstMapMatrixIndexToLoad) {
                firstMapMatrixIndexToLoad = -1;
                secondMapMatrixIndexToLoad = -1;
            } else if (!ov01_021F4728(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, mapLoadManager->width)) {
                secondMapMatrixIndexToLoad = -1;
            }
        } else if (mapLoadManager->quadrant == QUADRANT_TOP_RIGHT) {
            firstMapMatrixIndexToLoad = trackedTargetMapMatrixIndex + mapLoadManager->width;
            secondMapMatrixIndexToLoad = trackedTargetMapMatrixIndex + mapLoadManager->width + 1;

            firstQuadrantToLoad = QUADRANT_BOTTOM_LEFT;
            secondQuadrantToLoad = QUADRANT_BOTTOM_RIGHT;

            if (mapLoadManager->width * mapLoadManager->height <= firstMapMatrixIndexToLoad) {
                firstMapMatrixIndexToLoad = -1;
                secondMapMatrixIndexToLoad = -1;
            } else if (mapLoadManager->width * mapLoadManager->height <= secondMapMatrixIndexToLoad || !ov01_021F4728(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, mapLoadManager->width)) {
                secondMapMatrixIndexToLoad = -1;
            }
        } else {
            ((0) ? (void)0 : GF_AssertFail());
        }

        ov01_021F54AC(firstMapMatrixIndexToLoad, secondMapMatrixIndexToLoad, firstQuadrantToLoad, secondQuadrantToLoad, trackedTargetDirection, mapLoadManager);
        break;

    default:
        ((0) ? (void)0 : GF_AssertFail());
    }
}
