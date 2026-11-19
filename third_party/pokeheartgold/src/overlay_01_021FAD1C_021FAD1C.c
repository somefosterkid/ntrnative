#include "global.h"

typedef struct BDHCPoint {
    fx32 x;
    fx32 z;
} BDHCPoint;
typedef struct BDHCPlate {
    u16 firstPointIndex;
    u16 secondPointIndex;
    u16 normalIndex;
    u16 constantIndex;
} BDHCPlate;
typedef struct BDHCStrip {
    fx32 scanline;
    u16 accessListElementCount;
    u16 accessListStartIndex;
} BDHCStrip;
typedef struct BDHC {
    BDHCPlate *plates;
    fx32 *constants;
    BDHCStrip *strips;
    u16 *accessList;
    BDHCPoint *points;
    VecFx32 *normals;
    BOOL loaded;
    int stripsCount;
} BDHC;
typedef struct {
    fx32 val;
    int dummy04;
    int dummy08;
} BDHCCandidateObjectHeight;

BOOL ov01_021FAD1C(const BDHCPoint *boundingBoxFirstPoint, const BDHCPoint *boundingBoxSecondPoint, const BDHCPoint *point);
void ov01_021FAD6C(const BDHC *bdhc, u16 plateIndex, BDHCPoint *platePoints);
void ov01_021FAD9C(const BDHC *bdhc, u16 plateIndex, VecFx32 *normal);
void ov01_021FADBC(const BDHC *bdhc, u16 plateIndex, fx32 *constant);
void ov01_021FADD4(BDHCCandidateObjectHeight *candidateObjectHeights);
BOOL ov01_021FADEC(const BDHCStrip *strips, const u16 stripsCount, const fx32 scanline, u16 *stripIndex);

BOOL ov01_021FAD1C(const BDHCPoint *boundingBoxFirstPoint, const BDHCPoint *boundingBoxSecondPoint, const BDHCPoint *point) {
    const fx32 *boundingBoxLeft, *boundingBoxRight, *boundingBoxUp, *boundingBoxDown;

    if (boundingBoxFirstPoint->x <= boundingBoxSecondPoint->x) {
        boundingBoxLeft = &boundingBoxFirstPoint->x;
        boundingBoxRight = &boundingBoxSecondPoint->x;
    } else {
        boundingBoxLeft = &boundingBoxSecondPoint->x;
        boundingBoxRight = &boundingBoxFirstPoint->x;
    }

    if (boundingBoxFirstPoint->z <= boundingBoxSecondPoint->z) {
        boundingBoxUp = &boundingBoxFirstPoint->z;
        boundingBoxDown = &boundingBoxSecondPoint->z;
    } else {
        boundingBoxUp = &boundingBoxSecondPoint->z;
        boundingBoxDown = &boundingBoxFirstPoint->z;
    }

    if (((*boundingBoxLeft <= point->x) && (point->x <= *boundingBoxRight)) && ((*boundingBoxUp <= point->z) && (point->z <= *boundingBoxDown))) {
        return 1;
    }

    return 0;
}

void ov01_021FAD6C(const BDHC *bdhc, u16 plateIndex, BDHCPoint *platePoints) {
    platePoints[0] = bdhc->points[bdhc->plates[plateIndex].firstPointIndex];
    platePoints[1] = bdhc->points[bdhc->plates[plateIndex].secondPointIndex];
}

void ov01_021FAD9C(const BDHC *bdhc, u16 plateIndex, VecFx32 *normal) {
    *normal = bdhc->normals[bdhc->plates[plateIndex].normalIndex];
}

void ov01_021FADBC(const BDHC *bdhc, u16 plateIndex, fx32 *constant) {
    *constant = bdhc->constants[bdhc->plates[plateIndex].constantIndex];
}

void ov01_021FADD4(BDHCCandidateObjectHeight *candidateObjectHeights) {
    for (int i = 0; i < 10; i++) {
        candidateObjectHeights[i].val = 0;
        candidateObjectHeights[i].dummy04 = -1;
        candidateObjectHeights[i].dummy08 = -1;
    }
}

BOOL ov01_021FADEC(const BDHCStrip *strips, const u16 stripsCount, const fx32 scanline, u16 *stripIndex) {
    if (stripsCount == 0) {
        return 0;
    } else if (stripsCount == 1) {
        *stripIndex = 0;
        return 1;
    }

    int low = 0;
    int high = stripsCount - 1;
    u32 mid = high / 2;

    do {
        if (strips[mid].scanline > scanline) {
            if (high - 1 > low) {
                high = mid;
                mid = (low + high) / 2;
            } else {
                *stripIndex = mid;
                return 1;
            }
        } else {
            if (low + 1 < high) {
                low = mid;
                mid = (low + high) / 2;
            } else {
                *stripIndex = mid + 1;
                return 1;
            }
        }
    } while (1);

    return 0;
}
