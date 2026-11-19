#include "global.h"

#include "overlay_01_021F4704.h"

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

BOOL ov01_021F4728(const int lhsY, const int rhsY, const int mapMatrixWidth);

u8 ov01_021F635C(const int mapMatrixIndex, const u8 sLotoId, const MapLoadManager *mapLoadManager);

u8 ov01_021F635C(const int mapMatrixIndex, const u8 sLotoId, const MapLoadManager *mapLoadManager) {
    u8 mapMatrixIndexQuadrant;

    if (mapLoadManager->currentMatrixIndex == mapMatrixIndex) {
        return mapLoadManager->currentMapSlot;
    }

    ((mapMatrixIndex != -1) ? (void)0 : GF_AssertFail());

    switch (mapLoadManager->currentMapSlot) {
    case QUADRANT_TOP_LEFT:
        if (mapMatrixIndex - mapLoadManager->currentMatrixIndex == mapLoadManager->width) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) + 2);
        } else if (mapMatrixIndex - mapLoadManager->currentMatrixIndex == 1 && ov01_021F4728(mapLoadManager->currentMatrixIndex, mapMatrixIndex, mapLoadManager->width)) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) + 1);
        } else if (mapMatrixIndex - mapLoadManager->currentMatrixIndex == mapLoadManager->width + 1 && !ov01_021F4728(mapLoadManager->currentMatrixIndex, mapMatrixIndex, mapLoadManager->width)) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) + 3);
        } else {
            mapMatrixIndexQuadrant = QUADRANT_INVALID;
        }

        break;

    case QUADRANT_TOP_RIGHT:
        if (mapMatrixIndex - mapLoadManager->currentMatrixIndex == mapLoadManager->width) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) + 2);
        } else if (mapLoadManager->currentMatrixIndex - mapMatrixIndex == 1 && ov01_021F4728(mapLoadManager->currentMatrixIndex, mapMatrixIndex, mapLoadManager->width)) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) - 1);
        } else if (mapMatrixIndex - mapLoadManager->currentMatrixIndex == mapLoadManager->width - 1 && !ov01_021F4728(mapLoadManager->currentMatrixIndex, mapMatrixIndex, mapLoadManager->width)) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) + 1);
        } else {
            mapMatrixIndexQuadrant = QUADRANT_INVALID;
        }

        break;

    case QUADRANT_BOTTOM_LEFT:
        if (mapLoadManager->currentMatrixIndex - mapMatrixIndex == mapLoadManager->width) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) - 2);
        } else if (mapLoadManager->currentMatrixIndex - mapMatrixIndex == mapLoadManager->width - 1 && !ov01_021F4728(mapLoadManager->currentMatrixIndex, mapMatrixIndex, mapLoadManager->width)) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) - 1);
        } else if (mapMatrixIndex - mapLoadManager->currentMatrixIndex == 1 && ov01_021F4728(mapLoadManager->currentMatrixIndex, mapMatrixIndex, mapLoadManager->width)) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) + 1);
        } else {
            mapMatrixIndexQuadrant = QUADRANT_INVALID;
        }

        break;

    case QUADRANT_BOTTOM_RIGHT:
        if (mapLoadManager->currentMatrixIndex - mapMatrixIndex == mapLoadManager->width) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) - 2);
        } else if (mapLoadManager->currentMatrixIndex - mapMatrixIndex == mapLoadManager->width + 1 && !ov01_021F4728(mapLoadManager->currentMatrixIndex, mapMatrixIndex, mapLoadManager->width)) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) - 3);
        } else if (mapLoadManager->currentMatrixIndex - mapMatrixIndex == 1 && ov01_021F4728(mapLoadManager->currentMatrixIndex, mapMatrixIndex, mapLoadManager->width)) {
            mapMatrixIndexQuadrant = ((mapLoadManager->currentMapSlot) - 1);
        } else {
            mapMatrixIndexQuadrant = QUADRANT_INVALID;
        }

        break;

    default:
        mapMatrixIndexQuadrant = QUADRANT_INVALID;
    }

    return mapMatrixIndexQuadrant;
}

s32 ov01_021F652C(MapLoadManager *mapLoadManager, u32 index) {
    if (index >= 4U) {
        GF_AssertFail();
    }
    return mapLoadManager->maps[index]->matrixIndex;
}
