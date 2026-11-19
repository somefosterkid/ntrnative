#include "global.h"

int ov01_021F5A04(const u32 tileIndex, const int mapMatrixWidth, const int mapMatrixWidthTiles);

int ov01_021F5A04(const u32 tileIndex, const int mapMatrixWidth, const int mapMatrixWidthTiles) {
    u32 tileX = tileIndex % mapMatrixWidthTiles;
    u32 tileZ = tileIndex / mapMatrixWidthTiles;
    u32 mapMatrixX = tileX / 32;
    u32 mapMatrixZ = tileZ / 32;

    return mapMatrixX + mapMatrixZ * mapMatrixWidth;
}
