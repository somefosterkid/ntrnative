#include "global.h"

BOOL sub_02025C98(const NNSG2dCellData *cellData, const MtxFx32 *objectCoords, const NNSG2dViewRect *viewRect);

BOOL sub_02025C98(const NNSG2dCellData *cellData, const MtxFx32 *objectCoords, const NNSG2dViewRect *viewRect) {
    const NNSG2dCellBoundingRectS16 *boundingRect = NNS_G2dGetCellBoundingRect(cellData);
    const fx32 boundingRadius = NNS_G2dGetCellBoundingSphereR(cellData);
    const fx32 originX = objectCoords->_20 - viewRect->posTopLeft.x;
    const fx32 originY = objectCoords->_21 - viewRect->posTopLeft.y;

    fx32 minY, maxY;
    fx32 minX, maxX;
    fx32 tmp;

    if (NNS_G2dCellHasBR(cellData) == 1) {
        minY = boundingRect->minY << 12;
        maxY = boundingRect->maxY << 12;
        minX = boundingRect->minX << 12;
        maxX = boundingRect->maxX << 12;
    } else {
        minY = -boundingRadius << 12;
        maxY = boundingRadius << 12;
        minX = -boundingRadius << 12;
        maxX = boundingRadius << 12;
    }

    minY = FX_MulInline(minY, objectCoords->_01) + FX_MulInline(minY, objectCoords->_11) + originY;
    maxY = FX_MulInline(maxY, objectCoords->_01) + FX_MulInline(maxY, objectCoords->_11) + originY;
    minX = FX_MulInline(minX, objectCoords->_00) + FX_MulInline(minX, objectCoords->_10) + originX;
    maxX = FX_MulInline(maxX, objectCoords->_00) + FX_MulInline(maxX, objectCoords->_10) + originX;

    if (maxY < minY) {
        tmp = maxY;
        maxY = minY;
        minY = tmp;
    }

    if (maxX < minX) {
        tmp = maxX;
        maxX = minX;
        minX = tmp;
    }

    return maxY > 0 && minY < viewRect->sizeView.y && maxX > 0 && minX < viewRect->sizeView.x;
}
