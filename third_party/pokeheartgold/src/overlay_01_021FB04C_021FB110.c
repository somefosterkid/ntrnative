#include "global.h"

#include "filesystem.h"

struct BDHCPlate;
struct BDHCPoint;
struct BDHCStrip;
typedef struct BDHCPlate BDHCPlate;
typedef struct BDHCPoint BDHCPoint;
struct BDHCPoint {
    fx32 x;
    fx32 z;
};
typedef struct BDHCStrip BDHCStrip;
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
    int pointsCount;
    int normalsCount;
    int constantsCount;
    int platesCount;
    int stripsCount;
    int accessListCount;
} BDHCHeader;
struct BDHCPlate {
    u16 firstPointIndex;
    u16 secondPointIndex;
    u16 normalIndex;
    u16 constantIndex;
};
struct BDHCStrip {
    fx32 scanline;
    u16 accessListElementCount;
    u16 accessListStartIndex;
};

void ov01_021FB110(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);
void ov01_021FB120(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);
void ov01_021FB134(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);
void ov01_021FB144(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);
void ov01_021FB154(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);
void ov01_021FB164(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB110(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader) {
    NARC_ReadFile(narc, sizeof(BDHCPoint) * bdhcHeader->pointsCount, bdhc->points);
}

void ov01_021FB120(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader) {
    NARC_ReadFile(narc, sizeof(VecFx32) * bdhcHeader->normalsCount, bdhc->normals);
}

void ov01_021FB134(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader) {
    NARC_ReadFile(narc, sizeof(fx32) * bdhcHeader->constantsCount, bdhc->constants);
}

void ov01_021FB144(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader) {
    NARC_ReadFile(narc, sizeof(BDHCPlate) * bdhcHeader->platesCount, bdhc->plates);
}

void ov01_021FB154(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader) {
    NARC_ReadFile(narc, sizeof(BDHCStrip) * bdhcHeader->stripsCount, bdhc->strips);
}

void ov01_021FB164(NARC *narc, BDHC *bdhc, const BDHCHeader *bdhcHeader) {
    NARC_ReadFile(narc, sizeof(u16) * bdhcHeader->accessListCount, bdhc->accessList);
}
