#include "global.h"

s32 ov01_021F5A28(s32 param_1, s32 param_2, s32 param_3);

int ov01_021F5A30(const u32 tileIndex, const int mapMatrixWidth, const int mapMatrixWidthTiles);

s32 ov01_021F5A28(s32 param_1, s32 param_2, s32 param_3) {
    return param_1 + param_2 * param_3;
}

int ov01_021F5A30(const u32 tileIndex, const int mapMatrixWidth, const int mapMatrixWidthTiles) {
    u32 tileX = tileIndex % mapMatrixWidthTiles;
    u32 tileZ = tileIndex / mapMatrixWidthTiles;
    u32 mapMatrixX = tileX / 32;
    u32 mapMatrixZ = tileZ / 32;

    return mapMatrixX + mapMatrixZ * mapMatrixWidth;
}
