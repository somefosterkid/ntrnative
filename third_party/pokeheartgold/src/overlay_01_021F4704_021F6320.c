#include "global.h"

#include "field/map_load_manager.h"

#include "field_types_def.h"


enum Quadrant {
    QUADRANT_TOP_LEFT,
    QUADRANT_TOP_RIGHT,
    QUADRANT_BOTTOM_LEFT,
    QUADRANT_BOTTOM_RIGHT,
    QUADRANT_INVALID
};
u8 ov01_021F6328(const u32 tileIndex, const int mapMatrixWidthTiles);

u8 ov01_021F6320(MapLoadManager *mapLoadManager) {
    return *(u8 *)((u8 *)mapLoadManager + 0xAC);
}

u8 ov01_021F6328(const u32 tileIndex, const int mapMatrixWidthTiles) {
    u8 mapQuadrant;

    int tileX = tileIndex % 32;
    int tileZ = tileIndex / mapMatrixWidthTiles;
    int tileZMod = tileZ % 32;

    if (tileX < 32 / 2) {
        if (tileZMod < 32 / 2) {
            mapQuadrant = QUADRANT_TOP_LEFT;
        } else {
            mapQuadrant = QUADRANT_BOTTOM_LEFT;
        }
    } else {
        if (tileZMod < 32 / 2) {
            mapQuadrant = QUADRANT_TOP_RIGHT;
        } else {
            mapQuadrant = QUADRANT_BOTTOM_RIGHT;
        }
    }

    return mapQuadrant;
}
