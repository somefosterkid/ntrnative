#include "global.h"

struct MapMatrix;

typedef struct MapMatrix MapMatrix;

u8 MapMatrix_GetMapAltitude(const MapMatrix *mapMatrix, const int param1, const int x, const int y, const int width);

u8 MapMatrix_GetWidth(const MapMatrix *mapMatrix);

u8 MapMatrix_GetMatrixId(const MapMatrix *mapMatrix);

void ov01_021F5FB8(const int mapMatrixIndex, const int mapMatrixWidth, const MapMatrix *mapMatrix, VecFx32 *position);

void ov01_021F5FB8(const int mapMatrixIndex, const int mapMatrixWidth, const MapMatrix *mapMatrix, VecFx32 *position) {
    position->x = (32 / 2) * (16 * ((fx32)0x0000000000001000L));
    position->z = (32 / 2) * (16 * ((fx32)0x0000000000001000L));

    if (mapMatrixIndex == -1) {
        return;
    }

    u16 mapMatrixX = mapMatrixIndex % mapMatrixWidth;
    u16 mapMatrixZ = mapMatrixIndex / mapMatrixWidth;

    int mapMatrixWidth2 = MapMatrix_GetWidth(mapMatrix);
    u16 mapMatrixID = MapMatrix_GetMatrixId(mapMatrix);
    int altitude = MapMatrix_GetMapAltitude(mapMatrix, mapMatrixID, mapMatrixX, mapMatrixZ, mapMatrixWidth2);
    position->y = altitude * ((16 * ((fx32)0x0000000000001000L)) / 2);

    position->x += mapMatrixX * 32 * (16 * ((fx32)0x0000000000001000L));
    position->z += mapMatrixZ * 32 * (16 * ((fx32)0x0000000000001000L));
}
